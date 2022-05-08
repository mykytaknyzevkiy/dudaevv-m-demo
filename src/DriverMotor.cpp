//
// Created by Nekbakht Zabirov on 07.05.2022.
//

#include "DriverMotor.h"
#include <Servo.h>
#include <Arduino.h>

int pin = 8;
Servo driverMotor;

int minSpeed = 1000;
int maxSpeed = 2000;

void DriverMotor::setup() {
    pinMode(pin, OUTPUT);

    //digitalWrite(pin, HIGH);

    driverMotor.attach(pin, minSpeed, maxSpeed);

    driverMotor.write(maxSpeed);
    delay(2000);
    driverMotor.write(minSpeed);
    delay(2000);

    runSpeed(100);
    delay(2000);
    runSpeed(50);
    delay(2000);
    runSpeed(0);
}

void DriverMotor::runSpeed(int speed) {
    speed = map(speed, 0, 100, minSpeed, maxSpeed);

    driverMotor.write(speed);
}

DriverMotor::DriverMotor() = default;
