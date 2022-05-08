#include <DriverMotor.h>
#include <Wings.h>
#include <Arduino.h>

DriverMotor motorSpeed = DriverMotor();
Wings wings = Wings();

void setup() {
    wings.setup();
    motorSpeed.setup();
}

void loop() {
}

