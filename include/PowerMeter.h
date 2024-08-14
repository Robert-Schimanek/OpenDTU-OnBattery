// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "PowerMeterProvider.h"
#include <TaskSchedulerDeclarations.h>
#include <memory>
#include <mutex>
#include <map>

class PowerMeterClass {
public:
    PowerMeterClass(const std::string& name) : _name(name) {}

    void init(Scheduler& scheduler);

    void updateSettings();

    float getPowerTotal() const;
    uint32_t getLastUpdate() const;
    bool isDataValid() const;

private:
    void loop();

    Task _loopTask;

    std::string _name;
    bool _verboseLogging = true;
    uint32_t _lastPowerMeterCheck;
    // Used in Power limiter for safety check
    uint32_t _lastPowerMeterUpdate;

    std::map<String, float*> _mqttSubscriptions;

    mutable std::mutex _mutex;
    std::unique_ptr<PowerMeterProvider> _upProvider = nullptr;
};
