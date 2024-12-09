#pragma once

#include "config.h"

#ifdef ROBONOMICS_USE_HTTP

#include <WiFi.h>
#include <Arduino_JSON.h>

class HTTPRequests {
    private:
        WiFiClient wifi_client;
        String node_url;
    public:
        void setup(String host);
        void disconnect();
        JSONVar sendRequest(String message);
};

#endif