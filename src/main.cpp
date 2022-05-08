#include <DriverMotor.h>
#include <Wings.h>
#include <Arduino.h>
#include <MyServer.h>

DriverMotor motorSpeed = DriverMotor();
Wings wings = Wings();

void setup() {
    wings.setup();
    motorSpeed.setup();
    MyServer().setup();
}

void loop() {
}

