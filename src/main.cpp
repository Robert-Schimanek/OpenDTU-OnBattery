// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2022-2024 Thomas Basler and others
 */
#include <HoyweiConfiguration.h>
#include "Datastore.h"
#include "Display_Graphic.h"
#include "InverterSettings.h"
#include "Led_Single.h"
#include "MessageOutput.h"
#include "SerialPortManager.h"
#include "SPIPortManager.h"
#include "VictronMppt.h"
#include <HoyweiBattery.h>
#include <HoyweiHuaweiCan.h>
#include "MqttHandleDtu.h"
#include "MqttHandleHass.h"
#include "MqttHandleVedirectHass.h"
#include <HoyweiMqttHandleBatteryHass.h>
#include "MqttHandleInverter.h"
#include "MqttHandleInverterTotal.h"
#include "MqttHandleVedirect.h"
#include <HoyweiMqttHandleHuawei.h>
#include <HoyweiMqttHandlePowerLimiter.h>
#include <HoyweiMqttHandlePowerLimiterHass.h>
#include "MqttSettings.h"
#include "NetworkSettings.h"
#include "NtpSettings.h"
#include "HoyweiPinMapping.h"
#include "Scheduler.h"
#include "SunPosition.h"
#include "Utils.h"
#include "HoyweiWebApi.h"
#include "PowerMeter.h"
#include <PowerMeters.h>
#include <PowerLimiters.h>
#include "defaults.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <TaskScheduler.h>
#include <esp_heap_caps.h>
#include <PriceProvider.h>

void setup()
{
    // Move all dynamic allocations >512byte to psram (if available)
    heap_caps_malloc_extmem_enable(512);

    // Initialize serial output
    Serial.begin(SERIAL_BAUDRATE);
#if ARDUINO_USB_CDC_ON_BOOT
    Serial.setTxTimeoutMs(0);
    delay(200);
#else
    while (!Serial)
        yield();
#endif
    MessageOutput.init(scheduler);
    MessageOutput.println();
    MessageOutput.println("Starting OpenDTU");

    // Initialize file system
    MessageOutput.print("Initialize FS... ");
    if (!LittleFS.begin(false)) { // Do not format if mount failed
        MessageOutput.print("failed... trying to format...");
        if (!LittleFS.begin(true)) {
            MessageOutput.print("success");
        } else {
            MessageOutput.print("failed");
        }
    } else {
        MessageOutput.println("done");
    }

    // Read configuration values
    MessageOutput.print("Reading configuration... ");
    if (!Configuration.read()) {
        MessageOutput.print("initializing... ");
        Configuration.init();
        if (Configuration.write()) {
            MessageOutput.print("written... ");
        } else {
            MessageOutput.print("failed... ");
        }
    }
    Configuration.readExtended();
    //if (Configuration.get().Cfg.Version != CONFIG_VERSION) {
    //    MessageOutput.print("migrated... ");
    //    Configuration.migrate();
    //}

    auto& config = Configuration.get();
    MessageOutput.println("done");

    // Load PinMapping
    MessageOutput.print("Reading PinMapping... ");
    if (PinMapping.init(String(Configuration.get().Dev_PinMapping))) {
        MessageOutput.print("found valid mapping ");
    } else {
        MessageOutput.print("using default config ");
    }
    const auto& pin = PinMapping.get();
    MessageOutput.println("done");

    // Initialize PortManagers
    SerialPortManager.init();
    SPIPortManager.init();

    // Initialize WiFi
    MessageOutput.print("Initialize Network... ");
    NetworkSettings.init(scheduler);
    MessageOutput.println("done");
    NetworkSettings.applyConfig();

    // Initialize NTP
    MessageOutput.print("Initialize NTP... ");
    NtpSettings.init();
    MessageOutput.println("done");

    // Initialize SunPosition
    MessageOutput.print("Initialize SunPosition... ");
    SunPosition.init(scheduler);
    MessageOutput.println("done");

    // Initialize MqTT
    MessageOutput.print("Initialize MqTT... ");
    MqttSettings.init();
    MqttHandleDtu.init(scheduler);
    MqttHandleInverter.init(scheduler);
    MqttHandleInverterTotal.init(scheduler);
    MqttHandleVedirect.init(scheduler);
    MqttHandleHass.init(scheduler);
    MqttHandleVedirectHass.init(scheduler);
    MqttHandleBatteryHass.init(scheduler);
    MqttHandleHuawei.init(scheduler);
    MqttHandlePowerLimiter.init(scheduler);
    MqttHandlePowerLimiterHass.init(scheduler);
    MessageOutput.println("done");

    // Initialize WebApi
    MessageOutput.print("Initialize WebApi... ");
    WebApi.init(scheduler);
    MessageOutput.println("done");

    // Initialize Display
    MessageOutput.print("Initialize Display... ");
    Display.init(
        scheduler,
        static_cast<DisplayType_t>(pin.display_type),
        pin.display_data,
        pin.display_clk,
        pin.display_cs,
        pin.display_reset);
    Display.setDiagramMode(static_cast<DiagramMode_t>(config.Display.Diagram.Mode));
    Display.setOrientation(config.Display.Rotation);
    Display.enablePowerSafe = config.Display.PowerSafe;
    Display.enableScreensaver = config.Display.ScreenSaver;
    Display.setContrast(config.Display.Contrast);
    Display.setLanguage(config.Display.Language);
    Display.setStartupDisplay();
    MessageOutput.println("done");

    // Initialize Single LEDs
    MessageOutput.print("Initialize LEDs... ");
    LedSingle.init(scheduler);
    MessageOutput.println("done");

    // Check for default DTU serial
    MessageOutput.print("Check for default DTU serial... ");
    if (config.Dtu.Serial == DTU_SERIAL) {
        MessageOutput.print("generate serial based on ESP chip id: ");
        const uint64_t dtuId = Utils::generateDtuSerial();
        MessageOutput.printf("%0x%08x... ",
            ((uint32_t)((dtuId >> 32) & 0xFFFFFFFF)),
            ((uint32_t)(dtuId & 0xFFFFFFFF)));
        config.Dtu.Serial = dtuId;
        Configuration.writeExtended();
    }
    MessageOutput.println("done");
    MessageOutput.println("done");

    InverterSettings.init(scheduler);

    Datastore.init(scheduler);

    VictronMppt.init(scheduler);

    PowerMeter.init(scheduler);

    PowerMeterInverter.init(scheduler);

    PowerMeterCharger.init(scheduler);

    PowerMeterSolar.init(scheduler);

    PowerLimiter.init(scheduler);

    // Initialize Huawei AC-charger PSU / CAN bus
    MessageOutput.println("Initialize Huawei AC charger interface... ");
    if (PinMapping.isValidHuaweiConfig()) {
        MessageOutput.printf("Huawei AC-charger miso = %d, mosi = %d, clk = %d, irq = %d, cs = %d, power_pin = %d\r\n", pin.huawei_miso, pin.huawei_mosi, pin.huawei_clk, pin.huawei_irq, pin.huawei_cs, pin.huawei_power);
        HuaweiCan.init(scheduler, pin.huawei_miso, pin.huawei_mosi, pin.huawei_clk, pin.huawei_irq, pin.huawei_cs, pin.huawei_power);
        MessageOutput.println("done");
    } else {
        MessageOutput.println("Invalid pin config");
    }

    Battery.init(scheduler);

    OneMoreThing.init(scheduler); // If you want to test one more thing you must uncomment this
}

void loop()
{
    scheduler.execute();
}
