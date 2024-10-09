#include "WebsocketUtils.h"

void WebsocketUtilsRobonomics::websocketLoop() {
    if (!connectionClosed) {
        webSocket.loop();
    }
}

void WebsocketUtilsRobonomics::websocketSendMessage(String text) {
    webSocket.sendTXT(text);
}

void WebsocketUtilsRobonomics::setupWebsocket() {
    webSocket.beginSSL(WS_SERVER_HOST, WS_SERVER_PORT, "/");
    connectWebscoket();
}

void WebsocketUtilsRobonomics::disconnectWebSocket() {
    // Serial.println("Disconnected from websocket");
    webSocket.disconnect();
    connectionClosed = true;
}

void WebsocketUtilsRobonomics::connectWebscoket() {
    connectionClosed = false;
    // webSocket.onEvent(this->mainWebsocketCallback);
    webSocket.onEvent([&](WStype_t t, uint8_t * p, size_t l) {
        mainWebsocketCallback(t, p, l);
    });
    while (!websocketConnected) {
        websocketLoop();
    }
    setOnTextCallback(& defaultOnTextCallback);
    // setOnTextCallback(this->defaultOnTextCallback);
}
void WebsocketUtilsRobonomics::setOnTextCallback(OnTextWebsocketCallback callback) {
    onTextCallback = callback;
}

void WebsocketUtilsRobonomics::mainWebsocketCallback(WStype_t type, uint8_t *payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            // Serial.printf("[WSc] Disconnected!\n");
            websocketConnected = false;
            break;
        case WStype_CONNECTED: {
            // Serial.printf("[WSc] Connected to url: %s\n", payload);
            websocketConnected = true;
        }
            break;
        case WStype_TEXT:
            // Serial.printf("[WSc] RESPONSE: %s\n", (char *)payload);
            onTextCallback(payload);
            break;
    }
}

void defaultOnTextCallback(uint8_t *payload) {
    // Serial.println("Default on text callback");
}