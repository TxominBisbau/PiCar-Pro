#pragma once

// Moteur DROIT — circuit A
const int PIN_A_EN  = 4;
const int PIN_A_FWD = 26;
const int PIN_A_BWD = 21;
// Moteur GAUCHE — circuit B
const int PIN_B_EN  = 17;
const int PIN_B_FWD = 27;
const int PIN_B_BWD = 18;

const int   GPIO_CHIP     = 0;
const float PWM_FREQ      = 100;
const int   SPEED_DEFAULT = 60;
const int   SPEED_MIN     = 20;
const int   SPEED_MAX     = 100;

namespace Motors {
    bool init();
    void cleanup();
    void forward(int speed);
    void backward(int speed);
    void stop();
    int getSpeed();
}