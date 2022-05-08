//
// Created by Nekbakht Zabirov on 08.05.2022.
//

#include <string>
#include <stdlib.h>
#include "MyServer.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <iostream>
#ifdef ESP32
#elif defined(ESP8266
#endif
#include "SPIFFS.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

bool ledState = 0;
const int ledPin = 2;
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>html {font-family: Arial, Helvetica, sans-serif;text-align: center;}h1 {font-size: 1.8rem;color: white;}h2{font-size: 1.5rem;font-weight: bold;color: #143642;}.topnav {overflow: hidden;background-color: #143642;}body {margin: 0;}.content {padding: 30px;max-width: 600px;margin: 0 auto;}.card {background-color: #F8F7F9;;box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);padding-top:10px;padding-bottom:20px;}.button {padding: 15px 50px;font-size: 24px;text-align: center;outline: none;color: #fff;background-color: #0f8b8d;border: none;border-radius: 5px;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;-webkit-tap-highlight-color: rgba(0,0,0,0);}.button:active {background-color: #0f8b8d;box-shadow: 2 2px #CDCDCD;transform: translateY(2px);}.state {font-size: 1.5rem;color:#8c8c8c;font-weight: bold;}</style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Output - GPIO 2</h2>
      <p class="state">state: <span id="state">%STATE%</span></p>
      <p><button id="button" class="button">Toggle</button></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle');
  }
</script>
</body>
</html>
)rawliteral";

OnLeftWingMove onLeftWingMove;

std::string SubstringOfCString(const char *cstr,
                               size_t start, size_t length)
{
    return std::string(cstr + start, length);
}

bool isPartOf(char *w1, char *w2)
{
    int i = 0;
    int j = 0;

    while (w1[i] != '\0')
    {
        if (w1[i] == w2[j])
        {
            int init = i;
            while (w1[i] == w2[j] && w2[j] != '\0')
            {
                j++;
                i++;
            }
            if (w2[j] == '\0')
            {
                return true;
            }
            j = 0;
        }
        i++;
    }
    return false;
}

void notifyClients()
{
    ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;

    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        char *n = (char *)data;
        if (isPartOf(n, "moveLeft"))
        {
            std::string newN = SubstringOfCString(n, 9, 12);
            int nn = std::stoi(newN);
            Serial.println("on moveLeft for ");
            Serial.println(nn);
            Serial.println(newN.c_str());
            notifyClients();
            onLeftWingMove(nn);
        }
        if (strcmp((char *)data, "toggle") == 0)
        {
            ledState = !ledState;
            notifyClients();
        }
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
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

String processor(const String &var)
{
    Serial.println(var);
    if (var == "STATE")
    {
        if (ledState)
        {
            return "ON";
        }
        else
        {
            return "OFF";
        }
    }
    return String();
}

void MyServer::setup()
{
    Serial.begin(115200);

    // WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("esp-captive");

    ws.onEvent(onEvent);
    server.addHandler(&ws);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });

    server.begin();
}

void MyServer::run(OnLeftWingMove onLeftWingMoveN,
                   OnRightWingMove onRightWingMove,
                   OnSpeedUp onSpeedUp)
{
    onLeftWingMove = onLeftWingMoveN;
    ws.cleanupClients();
}
