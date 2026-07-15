#include "led.h"
#include <cmath>
#include <cstdint>
#include <iostream>

Led::Led(int channel, bool stateDefault) {
    this->channel = channel;
    this->stateDefault = stateDefault;
    this->currentState = currentState;
}

void Led::setState(float state){
    currentState = state;
    ::setChannel(channel, state);
}

int Led::getState() const (return currentState;)