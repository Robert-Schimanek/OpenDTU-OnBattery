/* Author: Robert Schimanek
 */

#include "PowerMeter.h"
#include "PowerLimiter.h"
#include "Control.h"
#include "Configuration.h"
#include "MessageOutput.h"
#include <QuickPID.h>

#include <ctime>
#include <cmath>

ControlClass Control;

ControlClass::ControlClass()
    : myPID(&_input, &_output, &_setPoint)
{
}

void ControlClass::init(Scheduler& scheduler)
{
    //scheduler.addTask(_loopTask);
    //_loopTask.setCallback(std::bind(&ControlClass::loop, this));
    //_loopTask.setIterations(TASK_FOREVER);
    //_loopTask.enable();

    _lastControlUpdate = 0;
    _controldiff = 2000;
    _setPoint = 0;


    //apply PID gains
    myPID.SetTunings(_Kp, _Ki, _Kd);

    myPID.SetMode(QuickPID::Control::automatic);

    myPID.SetOutputLimits(_Min, _Max);

}

void ControlClass::loop()
{
    return;

    CONFIG_T const& config = Configuration.get();
    _verboseLogging = config.PowerMeter.VerboseLogging;

    // Make sure power limiter is enabled
    if (!config.PowerLimiter.Enabled) {
        return;
    }

    // Make sure power meter is enabled
    if (!config.PowerMeter.Enabled) {
        return;
    }

    if (millis() < (_lastControlUpdate + _controldiff)) {
        return;
    }

    if (_verboseLogging) {
        MessageOutput.printf("ControlClass: loop \r\n");
    }




    // auto inverter = static_cast<float>(PowerMeter.getPowerInverter());


    // MessageOutput.printf("ControlClass: Household consumption %f Watt\r\n", _input);
    // MessageOutput.printf("ControlClass: Inverter prduction %f Watt\r\n", inverter);
    // MessageOutput.printf("ControlClass: Output of PID %f Watt\r\n", _output);


    _lastControlUpdate = millis();


}

float ControlClass::compute()
{
    _input = static_cast<float>(PowerMeter.getPowerTotal());

    myPID.Compute();

    return _output;
}
