#include "config.h"

#ifdef ROBONOMICS_USE_HTTP

#include "HTTPRequests.h"
#include <HTTPClient.h>


void HTTPRequests::setup(String host) {
    node_url = "http://" + host + "/rpc/";
}

void HTTPRequests::disconnect() {}

JSONVar HTTPRequests::sendRequest(String message) {
    Serial.print("[HTTP]+POST:\n"); 
    JSONVar response;
    HTTPClient http;
    http.begin(wifi_client, node_url.c_str());
    http.addHeader("Content-Type", "application/json");
    uint32_t httpCode = (uint32_t)http.POST(message);
    Serial.println("sent:");
    Serial.println(message);
    if (httpCode > 0) {
        Serial.printf("[HTTP]+POST code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK) {
            const String& payload = http.getString();
            Serial.println("received:");
            Serial.println(payload);
            response = JSON.parse(payload);
        } else {
            Serial.println("HTTP response is not 200");
        }
    } else {
        Serial.println("HTTP response is 0");
    }
    return response;
}

#endif