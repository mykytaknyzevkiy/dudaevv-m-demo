//
// Created by Nekbakht Zabirov on 08.05.2022.
//

#include "MyServer.h"
#include <Arduino.h>
#include "wifimanager.h"

// Create a instance of the WifiManager
WIFIMANAGER WifiManager;

void onRequest(AsyncWebServerRequest *request);

void MyServer::setup() {
    Serial.begin(9600);

    // Load well known Wifi AP credentials from NVS
    WifiManager.startBackgroundTask();

    // We do need the Webserver to attach our RESTful API
    AsyncWebServer webServer(80);
    // Attach our API to the Webserver
    WifiManager.attachWebServer(&webServer);
    // Run the Webserver
    webServer.begin();

    WifiManager.runSoftAP();

    &webServer.on("test", onRequest);
}

void MyServer::run() {
    WifiManager.runSoftAP();
}


void onRequest(AsyncWebServerRequest *request) {
    Serial.println("on test post");
}


