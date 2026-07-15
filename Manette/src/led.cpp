#include "led.h"
#include "pca9685.h"
#include <cmath>
#include <cstdint>
#include <iostream>

// Conversion angle → ticks PCA9685
int Led::angleToTicks(bool state) const{
    float pulse_us = 4095*state;
    return (int)(pulse_us);
}

Led::Led(int channel, bool stateDefault) {
    this->channel = channel;
    this->stateDefault = stateDefault;
    this->currentState = stateDefault;
}

void Led::setState(bool state){
    currentState = state;
    PCA9685::setChannel(channel, angleToTicks(state));
}

int Led::getState() const {return currentState;}