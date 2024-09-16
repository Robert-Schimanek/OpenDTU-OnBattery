// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <ESPAsyncWebServer.h>
#include <TaskSchedulerDeclarations.h>
#include <ArduinoJson.h>
#include <HoyweiConfiguration.h>

class WebApiPowerMeterClass {
public:
    WebApiPowerMeterClass(PowerMeterSource source, const std::string& baseUrl);

    void init(AsyncWebServer& server, Scheduler& scheduler);
    void updateSettings();

private:
    PowerMeterSource _source;
    std::string _baseUrl;

    void onStatus(AsyncWebServerRequest* request);
    void onAdminGet(AsyncWebServerRequest* request);
    void onAdminPost(AsyncWebServerRequest* request);
    void onTestHttpJsonRequest(AsyncWebServerRequest* request);
    void onTestHttpSmlRequest(AsyncWebServerRequest* request);

    AsyncWebServer* _server;
};
