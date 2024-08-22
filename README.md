- [Fork of OpenDTU-OnBattery](#fork-of-opendtu-onbattery)
  - [What is this fork of OpenDTU-OnBattery](#what-is-this-fork-of-opendtu-onbattery)
  - [History of the project](#history-of-the-project)
  - [Documentation](#documentation)
  - [Acknowledgment](#acknowledgment)

# Fork of OpenDTU-OnBattery

This is a fork from the OpenDTU-OnBattery project [OpenDTU-OnBattery](https://github.com/helgeerbe/OpenDTU-OnBattery).

## What is this fork of OpenDTU-OnBattery

This fork is focused on creating a slim version of OpenDTU-OnBattery for AC-powered batteries. It supports AC charging and AC discharging, meaning that the development is centered on AC-connected batteries. The philosophy behind this is simple: to achieve modularity and changeability, AC-connected systems are favorable. With the traction on dynamic pricing, future battery systems need more development regarding AC grid connectivity.

## History of the project

The original OpenDTU project was started from [a discussion on
Mikrocontroller.net](https://www.mikrocontroller.net/topic/525778). It was the
goal to replace the original Hoymiles DTU (Telemetry Gateway) to avoid using
Hoymile's cloud. With a lot of reverse engineering the Hoymiles protocol was
decrypted and analyzed.

The original OpenDTU-OnBattery is an extension of OpenDTU to support battery chargers, battery management systems (BMS), and power meters on a single ESP32. With the help of a dynamic power limiter, power production can be adjusted to the actual consumption. In this way, it is possible to come as close as possible to the goal of zero feed-in.

In the summer of 2024, I built my AC-powered battery. I decided to fork OpenDTU and extend it to suit my own needs.

## Documentation

Documentation of OpenDTU-OnBattery extensions can be found in [the project's wiki](https://github.com/helgeerbe/OpenDTU-OnBattery/wiki).

For documentation of OpenDTU core functionality, refer to the original [repo](https://github.com/tbnobody/OpenDTU) and its [documentation](https://opendtu.solar).

## Acknowledgment

A special thank you to the authors of the original [OpenDTU](https://github.com/tbnobody/OpenDTU) and [OpenDTU-OnBattery](https://github.com/helgeerbe/OpenDTU-OnBattery) projects. You are doing a great job!
