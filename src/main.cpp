#include <DriverMotor.h>
#include <Wings.h>
#include <Arduino.h>
// #include <MyServer.h>
#include <Webservo.h>

DriverMotor motorSpeed = DriverMotor();
Wings wings = Wings();

// MyServer myServer = MyServer();
Webservo webservo = Webservo();

void setup() {
    wings.setup();
    motorSpeed.setup();
    webservo.setup();
    //myServer.setup();

}

void loop() {
    // myServer.run();
    webservo.loop();
}
