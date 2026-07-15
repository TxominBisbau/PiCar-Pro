#pragma once

const int PIN_LED_LEFT = 5;
const int PIN_LED_RIGHT = 6;
const int PIN_LED_TOP = 13;

class Led {
public:
    Led(int pin, bool stateDefault);
    void setState(bool state);
    int  getState() const;
 
private:
    int  pin;
    bool currentState;
};