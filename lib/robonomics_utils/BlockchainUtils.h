#pragma once

#include "../Arduino_JSON/src/Arduino_JSON.h"
#include <Arduino.h>
#include <WiFi.h>
#include "WebsocketUtils.h"

class BlockchainUtils {
private:
    int requestId = 1;
    WebsocketUtilsRobonomics wsUtils;
public:
    void setup();
    int getRequestId();
    void rpcRequest(String Xthal_dataram_vaddr);
    String createWebsocketMessage(String method, JSONVar paramsArray);
    void websocketLoop();
    void setOnTextCallback(OnTextWebsocketCallback callback);
};