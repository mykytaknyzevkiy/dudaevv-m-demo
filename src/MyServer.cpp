//
// Created by Nekbakht Zabirov on 08.05.2022.
//

#include "MyServer.h"
#include <Arduino.h>
#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"

DNSServer dnsServer;
AsyncWebServer server(80);

String valueString = String(5);

class CaptiveRequestHandler : public AsyncWebHandler {
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request){
        //request->addInterestingHeader("ANY");
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("text/html");

        response->println("HTTP/1.1 200 OK");
        response->println("Content-type:text/html");
        response->println("Connection: close");
        response->println();

        // Display the HTML web page
        response->println("<!DOCTYPE html><html>");
        response->println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
        response->println("<link rel=\"icon\" href=\"data:,\">");
        // CSS to style the on/off buttons
        // Feel free to change the background-color and font-size attributes to fit your preferences
        response->println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
        response->println(".slider { width: 300px; }</style>");
        response->println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");

        // Web Page
        response->println("</head><body><h1>ESP32 with Servo</h1>");
        response->println("<p>Position: <span id=\"servoPos\"></span></p>");
        response->println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");

        response->println("<script>var slider = document.getElementById(\"servoSlider\");");
        response->println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
        response->println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
        response->println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
        response->println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");

        response->println("</body></html>");

        request->send(response);

        Serial.begin(115200);
        Serial.println("Test");
        Serial.println(request->getParam("value")->value());

    };
};

void MyServer::setup() {
    //your other setup stuff...
    WiFi.softAP("esp-captive");
    dnsServer.start(53, "*", WiFi.softAPIP());
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
    //more handlers...
    server.begin();
}

void MyServer::run() {
    dnsServer.processNextRequest();
}


void onRequest(AsyncWebServerRequest *request) {
    Serial.println("on test post");
}
