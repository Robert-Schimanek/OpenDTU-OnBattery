// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "Configuration.h"
#include <Arduino.h>
#include <memory>
#include <functional>
#include <optional>
#include <TaskSchedulerDeclarations.h>
#include <QuickPID.h>

class ControlClass {
public:
    enum class Status : unsigned {
        Initializing,
        DisabledByConfig,
        PowerMeterPending,
        Stable,
    };

    void init(Scheduler& scheduler);
    ControlClass(); // Constructor declaration
    float compute();

private:
    void loop();

    Task _loopTask;
    Status _lastStatus = Status::Initializing;
    bool _verboseLogging = true;

    uint32_t _lastControlUpdate;
    uint32_t _controldiff;


    float _setPoint = 0.0;
    float _input = 0.0;
    float _output = 0.0;

    float _Kp = 0.5;
    float _Ki = 0.01;
    float _Kd = 1.00;

    float _Min = -1500;
    float _Max = 1500;



    QuickPID myPID; // Declare without parameters


};

extern ControlClass Control;
