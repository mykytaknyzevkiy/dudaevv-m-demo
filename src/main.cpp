#include <DriverMotor.h>
#include <Wings.h>
#include <Arduino.h>
#include <MyServer.h>

DriverMotor motorSpeed = DriverMotor();
Wings wings = Wings();

MyServer myServer = MyServer();

void setup() {
    wings.setup();
    motorSpeed.setup();
    myServer.setup();
}

void loop() {
    myServer.run();
}

