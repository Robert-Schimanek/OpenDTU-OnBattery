// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <HoyweiConfiguration.h>
#include "Battery.h"
#include "BatteryCanReceiver.h"
#include <driver/twai.h>
#include <Arduino.h>

class PylontechCanReceiver : public BatteryCanReceiver {
public:
    bool init(bool verboseLogging) final;
    void onMessage(twai_message_t rx_message) final;

    std::shared_ptr<BatteryStats> getStats() const final { return _stats; }

private:
    void dummyData();

    std::shared_ptr<PylontechBatteryStats> _stats =
        std::make_shared<PylontechBatteryStats>();
};
