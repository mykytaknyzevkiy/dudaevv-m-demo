//
// Created by Nekbakht Zabirov on 07.05.2022.
//

#include "Wings.h"
#include <Arduino.h>
#include <Servo.h>

Servo leftWing;
Servo rightWing;

void Wings::setup() {
    int leftWingPin = 17;
    int rightWingPin = 10;

    pinMode(leftWingPin, OUTPUT);
    pinMode(rightWingPin, OUTPUT);

    leftWing.attach(leftWingPin);
    rightWing.attach(rightWingPin);

    delay(1000);

    moveRight(0);
    moveLeft(0);

    delay(1000);

    moveLeft(180);
    moveRight(180);
}

void Wings::moveLeft(int angle) {
    leftWing.write(angle);
}

void Wings::moveRight(int angle) {
    rightWing.write(angle);
}

Wings::Wings() = default;
