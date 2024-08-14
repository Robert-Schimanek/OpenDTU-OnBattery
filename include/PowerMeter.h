// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "PowerMeterProvider.h"
#include <TaskSchedulerDeclarations.h>
#include <memory>
#include <mutex>

class PowerMeterClass {
public:
    void init(Scheduler& scheduler);

    void updateSettings();

    float getPowerTotal() const;
    float getPowerInverter() const;
    float getPowerCharger() const;
    uint32_t getLastUpdate() const;
    uint32_t getLastPowerMeterInverterUpdate() const;
    uint32_t getLastPowerMeterChargerUpdate() const;
    bool isDataValid() const;

private:
    void loop();

    Task _loopTask;

    bool _verboseLogging = true;
    uint32_t _lastPowerMeterCheck;
    // Used in Power limiter for safety check
    uint32_t _lastPowerMeterUpdate;
    uint32_t _lastPowerMeterInverterUpdate;
    uint32_t _lastPowerMeterChargerUpdate;

    float _powerMeter1Power = 0.0;
    float _powerMeter2Power = 0.0;
    float _powerMeter3Power = 0.0;
    float _powerMeter1Voltage = 0.0;
    float _powerMeter2Voltage = 0.0;
    float _powerMeter3Voltage = 0.0;
    float _powerMeterImport = 0.0;
    float _powerMeterExport = 0.0;

    float _powerMeterInverterPower = 0.0;

    float _powerMeterChargerPower = 0.0;

    std::map<String, float*> _mqttSubscriptions;

    mutable std::mutex _mutex;
    std::unique_ptr<PowerMeterProvider> _upProvider = nullptr;
};

extern PowerMeterClass PowerMeter;
