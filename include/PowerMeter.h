// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <HoyweiPowerMeterProvider.h>
#include <TaskSchedulerDeclarations.h>
#include <memory>
#include <mutex>
#include <map>

class PowerMeterClass {

friend class HoyweiPowerMeterClass;

public:
    PowerMeterClass(PowerMeterSource source) : _source(source) {}

    void init(Scheduler& scheduler);

    void updateSettings();

    float getPowerTotal() const;
    uint32_t getLastUpdate() const;
    bool isDataValid() const;

private:
    void loop();

    Task _loopTask;

    PowerMeterSource _source;
    bool _verboseLogging = true;
    uint32_t _lastPowerMeterCheck;
    // Used in Power limiter for safety check
    uint32_t _lastPowerMeterUpdate;

    std::map<String, float*> _mqttSubscriptions;

    mutable std::mutex _mutex;
    std::unique_ptr<PowerMeterProvider> _upProvider = nullptr;
};
