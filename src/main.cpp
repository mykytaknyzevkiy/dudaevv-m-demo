#include <DriverMotor.h>
#include <Wings.h>
#include <Arduino.h>
#include <MyServer.h>

DriverMotor motorSpeed = DriverMotor();
Wings wings = Wings();

MyServer myServer = MyServer();

void onLeftWingMove(int angle)
{
    wings.moveLeft(angle);
}
void onRightWingMove(int angle)
{
    wings.moveRight(angle);
}
void onSpeedUp(int angle)
{
    // wings.moveLeft(angle);
}

void setup()
{
    wings.setup();
    motorSpeed.setup();
    myServer.setup();
}

void loop()
{
    myServer.run(onLeftWingMove, onRightWingMove, onSpeedUp);
}
