#include "servo.h"
#include "pca9685.h"
#include <cmath>
#include <cstdint>
#include <iostream>


// Conversion angle → ticks PCA9685
int Servo::angleToTicks(float angle) const{
    float pulse_us = minPulse + (maxPulse - minPulse) * angle / 180.0f;
    // période = 1/50Hz = 20 000 µs = 4096 ticks
    return (int)(pulse_us * 4096.0f / 20000.0f);
}

// Servo de direction
Servo::Servo(int channel, float minPulse, float maxPulse, float angleDefault) {
    this->channel = channel;
    this->minPulse = minPulse;
    this->maxPulse = maxPulse;
    this->angleDefault = angleDefault;
    this->currentAngle = angleDefault;

    Servo::center();
}

void Servo::setAngle(float angle){
    if (angle < 0.0f)   angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;
    currentAngle = angle;
    PCA9685::setChannel(channel, angleToTicks(angle));
}

void Servo::center() {setAngle(angleDefault);}

int  Servo::getAngle() const{return currentAngle;}