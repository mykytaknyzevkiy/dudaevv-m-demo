//
// Created by Nekbakht Zabirov on 08.05.2022.
//

#include "MyServer.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#ifdef ESP32
#elif defined(ESP8266
#endif

AsyncWebSocket ws("/ws");

void notifyClients() {
    ws.textAll("hey");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;

    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        if (strcmp((char*)data, "moveLeft=") == 0) {
            Serial.println("on moveLeft");
            notifyClients();
        }
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void MyServer::setup() {
    Serial.begin(115200);

    WiFi.softAP("esp-captive");

    ws.onEvent(onEvent);
}

void MyServer::run(OnLeftWingMove onLeftWingMove,
                   OnRightWingMove onRightWingMove,
                   OnSpeedUp onSpeedUp) {
    ws.cleanupClients();
}


