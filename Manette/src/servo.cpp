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
class Servo::Servo(int channel, float minPulse, float maxPulse, float angleDefault) {
    Servo::center();
    channel = 0;
    minPulse = 0;
    maxPulse = 180;
    angleDefault = 90;
    std::cout << "[SERVO] OK (canal " << channel << ", prescale=" << ")\n";

    if (g_fd >= 0) close(g_fd);

    return 1;
}

void Servo::setAngle(float angle) const{
    if (angle < 0.0f)   angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;
    currentAngle = Servo::angleToTicks(angle);
    PCA9685::setChannel(channel, angleToTicks(angle));
}

void Servo::center() {setAngle(angleDefault);}

int  Servo::getAngle() {return g_angle;}