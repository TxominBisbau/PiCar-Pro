#include "led.h"
#include <lgpio.h>
#include <iostream>

static int g_chip = -1; 

Led::Led(int pin, bool stateDefault) {
    this->pin = pin;
    this->currentState = stateDefault;

    if (g_chip < 0) {
        g_chip = lgGpiochipOpen(0);
        if (g_chip < 0) {
            std::cerr << "[LED] Erreur ouverture gpiochip0\n";
        }
    }

    lgGpioClaimOutput(g_chip, 0, pin, 0);
    setState(stateDefault);
    std::cout << "[LED] OK (pin " << pin << ")\n";
}

void Led::setState(bool state){
    currentState = state;
    lgGpioWrite(g_chip, pin, state ? 1 : 0);
}

int Led::getState() const {return currentState;}