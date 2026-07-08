#include "servo.h"
#include "pca9685.H"
#include <cmath>
#include <cstdint>
#include <iostream>


static int g_fd = -1;
static int g_angle = ANGLE_DEFAULT;

// Conversion angle → ticks PCA9685
int Servo::angleToTicks(float angle) {
    float pulse_us = minPulse + (maxPulse - minPulse) * angle / 180.0f;
    // période = 1/50Hz = 20 000 µs = 4096 ticks
    return (int)(pulse_us * 4096.0f / 20000.0f);
}

// Interface publique
bool Servo::init() {
    Servo::center();
    std::cout << "[SERVO] OK (canal " << SERVO_CHANNEL << ", prescale=" << (int)prescale << ")\n";
    return true;
}

void Servo::cleanup() {
    Servo::center();
    if (g_fd >= 0) close(g_fd);
}

void Servo::setAngle(float angle) {
    if (angle < 0.0f)   angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;
    setChannel(SERVO_CHANNEL, angleToTicks(angle));
}

void Servo::center() {setAngle(ANGLE_DEFAULT);}

int  Servo::getAngle() {return g_angle;}