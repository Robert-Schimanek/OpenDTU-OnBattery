// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <HoyweiConfiguration.h>
#include <memory>
#include <vector>
#include <utility>
#include <string>
#include <HTTPClient.h>
#include <WiFiClient.h>

using up_http_client_t = std::unique_ptr<HTTPClient>;
using sp_wifi_client_t = std::shared_ptr<WiFiClient>;

class HttpRequestResult {

    friend class HoyweiHttpGetterClass;

public:
    HttpRequestResult(bool success,
            up_http_client_t upHttpClient = nullptr,
            sp_wifi_client_t spWiFiClient = nullptr)
        : _success(success)
        , _upHttpClient(std::move(upHttpClient))
        , _spWiFiClient(std::move(spWiFiClient)) { }

    ~HttpRequestResult() {
        // the wifi client *must* die *after* the http client, as the http
        // client uses the wifi client in its destructor.
        if (_upHttpClient) { _upHttpClient->end(); }
        _upHttpClient = nullptr;
        _spWiFiClient = nullptr;
    }

    HttpRequestResult(HttpRequestResult const&) = delete;
    HttpRequestResult(HttpRequestResult&&) = delete;
    HttpRequestResult& operator=(HttpRequestResult const&) = delete;
    HttpRequestResult& operator=(HttpRequestResult&&) = delete;

    operator bool() const { return _success; }

    Stream* getStream() {
        if(!_upHttpClient) { return nullptr; }
        return _upHttpClient->getStreamPtr();
    }

private:
    bool _success;
    up_http_client_t _upHttpClient;
    sp_wifi_client_t _spWiFiClient;
};

class HttpGetter {

    friend class HoyweiHttpGetterClass;

public:
    explicit HttpGetter(HttpRequestConfig const& cfg)
        : _config(cfg) { }

    bool init();
    void addHeader(char const* key, char const* value);
    HttpRequestResult performGetRequest();

    char const* getErrorText() const { return _errBuffer; }

private:
    String getAuthDigest(String const& authReq, unsigned int counter);
    HttpRequestConfig const& _config;

    template<typename... Args>
    void logError(char const* format, Args... args);
    char _errBuffer[256];

    bool _useHttps;
    String _host;
    String _uri;
    uint16_t _port;

    sp_wifi_client_t _spWiFiClient; // reused for multiple HTTP requests

    std::vector<std::pair<std::string, std::string>> _additionalHeaders;
};
