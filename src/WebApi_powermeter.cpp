// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2022-2024 Thomas Basler and others
 */
#include "WebApi_powermeter.h"
#include "VeDirectFrameHandler.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include <HoyweiConfiguration.h>
#include "MqttHandleVedirectHass.h"
#include "MqttHandleHass.h"
#include "MqttSettings.h"
#include "PowerLimiter.h"
#include <PowerMeters.h>
#include "PowerMeterHttpJson.h"
#include "PowerMeterHttpSml.h"
#include "WebApi.h"
#include "helper.h"


WebApiPowerMeterClass::WebApiPowerMeterClass(const std::string& name, const std::string& baseUrl)
    : _name(name), _baseUrl(baseUrl)
{
}

void WebApiPowerMeterClass::init(AsyncWebServer& server, Scheduler& scheduler)
{
    using std::placeholders::_1;

    _server = &server;

    _server->on((_baseUrl + "/status").c_str(), HTTP_GET, std::bind(&WebApiPowerMeterClass::onStatus, this, _1));
    _server->on((_baseUrl + "/config").c_str(), HTTP_GET, std::bind(&WebApiPowerMeterClass::onAdminGet, this, _1));
    _server->on((_baseUrl + "/config").c_str(), HTTP_POST, std::bind(&WebApiPowerMeterClass::onAdminPost, this, _1));
    _server->on((_baseUrl + "/testhttpjsonrequest").c_str(), HTTP_POST, std::bind(&WebApiPowerMeterClass::onTestHttpJsonRequest, this, _1));
    _server->on((_baseUrl + "/testhttpsmlrequest").c_str(), HTTP_POST, std::bind(&WebApiPowerMeterClass::onTestHttpSmlRequest, this, _1));

}

void WebApiPowerMeterClass::updateSettings() {
    if (_name == "PowerMeter") {
        PowerMeter.updateSettings();
    } else if (_name == "PowerMeterInverter") {
        PowerMeterInverter.updateSettings();
    } else if (_name == "PowerMeterCharger") {
        PowerMeterCharger.updateSettings();
    } else if (_name == "PowerMeterSolar") {
        PowerMeterSolar.updateSettings();
    } else {
        Serial.println("Error: Unknown PowerMeter type");
    }
}

void WebApiPowerMeterClass::onStatus(AsyncWebServerRequest* request)
{
    if (!WebApi.checkCredentialsReadonly(request)) {
        return;
    }

    AsyncJsonResponse* response = new AsyncJsonResponse();
    auto& root = response->getRoot();
    auto const& pmcfg = Configuration.getByName(_name);

    root["enabled"] = pmcfg.Enabled;
    root["verbose_logging"] = pmcfg.VerboseLogging;
    root["source"] = pmcfg.Source;

    auto mqtt = root["mqtt"].to<JsonObject>();
    Configuration.serializePowerMeterMqttConfig(pmcfg.Mqtt, mqtt);

    auto serialSdm = root["serial_sdm"].to<JsonObject>();
    Configuration.serializePowerMeterSerialSdmConfig(pmcfg.SerialSdm, serialSdm);

    auto httpJson = root["http_json"].to<JsonObject>();
    Configuration.serializePowerMeterHttpJsonConfig(pmcfg.HttpJson, httpJson);

    auto httpSml = root["http_sml"].to<JsonObject>();
    Configuration.serializePowerMeterHttpSmlConfig(pmcfg.HttpSml, httpSml);

    WebApi.sendJsonResponse(request, response, __FUNCTION__, __LINE__);
}

void WebApiPowerMeterClass::onAdminGet(AsyncWebServerRequest* request)
{
    if (!WebApi.checkCredentials(request)) {
        return;
    }

    this->onStatus(request);
}

void WebApiPowerMeterClass::onAdminPost(AsyncWebServerRequest* request)
{
    if (!WebApi.checkCredentials(request)) {
        return;
    }

    AsyncJsonResponse* response = new AsyncJsonResponse();
    JsonDocument root;
    if (!WebApi.parseRequestData(request, response, root)) {
        return;
    }

    auto& retMsg = response->getRoot();

    if (!(root.containsKey("enabled") && root.containsKey("source"))) {
        retMsg["message"] = "Values are missing!";
        response->setLength();
        request->send(response);
        return;
    }

    auto checkHttpConfig = [&](JsonObject const& cfg) -> bool {
        if (!cfg.containsKey("url")
                || (!cfg["url"].as<String>().startsWith("http://")
                    && !cfg["url"].as<String>().startsWith("https://"))) {
            retMsg["message"] = "URL must either start with http:// or https://!";
            response->setLength();
            request->send(response);
            return false;
        }

        if ((cfg["auth_type"].as<uint8_t>() != HttpRequestConfig::Auth::None)
                && (cfg["username"].as<String>().length() == 0 || cfg["password"].as<String>().length() == 0)) {
            retMsg["message"] = "Username or password must not be empty!";
            response->setLength();
            request->send(response);
            return false;
        }

        if (!cfg.containsKey("timeout")
                || cfg["timeout"].as<uint16_t>() <= 0) {
            retMsg["message"] = "Timeout must be greater than 0 ms!";
            response->setLength();
            request->send(response);
            return false;
        }

        return true;
    };

    if (static_cast<PowerMeterProvider::Type>(root["source"].as<uint8_t>()) == PowerMeterProvider::Type::HTTP_JSON) {
        JsonObject httpJson = root["http_json"];
        JsonArray valueConfigs = httpJson["values"];
        for (uint8_t i = 0; i < valueConfigs.size(); i++) {
            JsonObject valueConfig = valueConfigs[i].as<JsonObject>();

            if (i > 0 && !valueConfig["enabled"].as<bool>()) {
                continue;
            }

            if (i == 0 || httpJson["individual_requests"].as<bool>()) {
                if (!checkHttpConfig(valueConfig["http_request"].as<JsonObject>())) {
                    return;
                }
            }

            if (!valueConfig.containsKey("json_path")
                    || valueConfig["json_path"].as<String>().length() == 0) {
                retMsg["message"] = "Json path must not be empty!";
                response->setLength();
                request->send(response);
                return;
            }
        }
    }

    if (static_cast<PowerMeterProvider::Type>(root["source"].as<uint8_t>()) == PowerMeterProvider::Type::HTTP_SML) {
        JsonObject httpSml = root["http_sml"];
        if (!checkHttpConfig(httpSml["http_request"].as<JsonObject>())) {
            return;
        }
    }

    auto& pmcfg = Configuration.getByName(_name);
    pmcfg.Enabled = root["enabled"].as<bool>();
    pmcfg.VerboseLogging = root["verbose_logging"].as<bool>();
    pmcfg.Source = root["source"].as<uint8_t>();

    Configuration.deserializePowerMeterMqttConfig(root["mqtt"].as<JsonObject>(),
            pmcfg.Mqtt);

    Configuration.deserializePowerMeterSerialSdmConfig(root["serial_sdm"].as<JsonObject>(),
            pmcfg.SerialSdm);

    Configuration.deserializePowerMeterHttpJsonConfig(root["http_json"].as<JsonObject>(),
            pmcfg.HttpJson);

    Configuration.deserializePowerMeterHttpSmlConfig(root["http_sml"].as<JsonObject>(),
            pmcfg.HttpSml);

    WebApi.writeConfig(retMsg);

    WebApi.sendJsonResponse(request, response, __FUNCTION__, __LINE__);

    updateSettings();
}

void WebApiPowerMeterClass::onTestHttpJsonRequest(AsyncWebServerRequest* request)
{
    if (!WebApi.checkCredentials(request)) {
        return;
    }

    AsyncJsonResponse* asyncJsonResponse = new AsyncJsonResponse();
    JsonDocument root;
    if (!WebApi.parseRequestData(request, asyncJsonResponse, root)) {
        return;
    }

    auto& retMsg = asyncJsonResponse->getRoot();

    char response[256];

    auto powerMeterConfig = std::make_unique<PowerMeterHttpJsonConfig>();
    Configuration.deserializePowerMeterHttpJsonConfig(root["http_json"].as<JsonObject>(),
            *powerMeterConfig);
    auto upMeter = std::make_unique<PowerMeterHttpJson>(*powerMeterConfig);
    upMeter->init();
    auto res = upMeter->poll();
    using values_t = PowerMeterHttpJson::power_values_t;
    if (std::holds_alternative<values_t>(res)) {
        retMsg["type"] = "success";
        auto vals = std::get<values_t>(res);
        auto pos = snprintf(response, sizeof(response), "Result: %5.2fW", vals[0]);
        for (size_t i = 1; i < vals.size(); ++i) {
            if (!powerMeterConfig->Values[i].Enabled) { continue; }
            pos += snprintf(response + pos, sizeof(response) - pos, ", %5.2fW", vals[i]);
        }
        snprintf(response + pos, sizeof(response) - pos, ", Total: %5.2f", upMeter->getPowerTotal());
    } else {
        snprintf(response, sizeof(response), "%s", std::get<String>(res).c_str());
    }

    retMsg["message"] = response;
    asyncJsonResponse->setLength();
    request->send(asyncJsonResponse);
}

void WebApiPowerMeterClass::onTestHttpSmlRequest(AsyncWebServerRequest* request)
{
    if (!WebApi.checkCredentials(request)) {
        return;
    }

    AsyncJsonResponse* asyncJsonResponse = new AsyncJsonResponse();
    JsonDocument root;
    if (!WebApi.parseRequestData(request, asyncJsonResponse, root)) {
        return;
    }

    auto& retMsg = asyncJsonResponse->getRoot();

    char response[256];

    auto powerMeterConfig = std::make_unique<PowerMeterHttpSmlConfig>();
    Configuration.deserializePowerMeterHttpSmlConfig(root["http_sml"].as<JsonObject>(),
            *powerMeterConfig);
    auto upMeter = std::make_unique<PowerMeterHttpSml>(*powerMeterConfig);
    upMeter->init();
    auto res = upMeter->poll();
    if (res.isEmpty()) {
        retMsg["type"] = "success";
        snprintf(response, sizeof(response), "Result: %5.2fW", upMeter->getPowerTotal());
    } else {
        snprintf(response, sizeof(response), "%s", res.c_str());
    }

    retMsg["message"] = response;
    asyncJsonResponse->setLength();
    request->send(asyncJsonResponse);
}
