#include "motors.h"
#include <lgpio.h>
#include <algorithm>
#include <iostream>

static int g_speed = SPEED_DEFAULT;
static int g_chip  = -1;

static float toDuty(int pct) {
    return (float)std::clamp(pct, 0, 100);
}

// Moteur GAUCHE (circuit B) — PWM sur pins FWD/BWD, EN à 1 si on bouge
static void setLeft(float fwd, float bwd) {
    lgTxPwm(g_chip, PIN_B_FWD, PWM_FREQ, fwd, 0, 0);
    lgTxPwm(g_chip, PIN_B_BWD, PWM_FREQ, bwd, 0, 0);
    lgGpioWrite(g_chip, PIN_B_EN, (fwd > 0 || bwd > 0) ? 1 : 0);
}

// Moteur DROIT (circuit A)
static void setRight(float fwd, float bwd) {
    lgTxPwm(g_chip, PIN_A_FWD, PWM_FREQ, fwd, 0, 0);
    lgTxPwm(g_chip, PIN_A_BWD, PWM_FREQ, bwd, 0, 0);
    lgGpioWrite(g_chip, PIN_A_EN, (fwd > 0 || bwd > 0) ? 1 : 0);
}

bool Motors::init() {
    g_chip = lgGpiochipOpen(GPIO_CHIP);
    if (g_chip < 0) {
        std::cerr << "[MOTORS] Erreur ouverture gpiochip0\n";
        return false;
    }
    // Toutes les broches en sortie
    for (int pin : {PIN_A_FWD, PIN_A_BWD, PIN_B_FWD, PIN_B_BWD})
        lgGpioClaimOutput(g_chip, 0, pin, 0);
    for (int pin : {PIN_A_EN, PIN_B_EN})
        lgGpioClaimOutput(g_chip, 0, pin, 0);

    Motors::stop();
    std::cout << "[MOTORS] OK\n";
    return true;
}

void Motors::cleanup() {
    Motors::stop();
    if (g_chip >= 0) lgGpiochipClose(g_chip);
}

void Motors::forward(int speed) {
    float d = toDuty(speed);
    setLeft(d, 0);
    setRight(d, 0);
}

void Motors::backward(int speed) {
    float d = toDuty(speed);
    setLeft(0, d);
    setRight(0, d);
}

void Motors::stop() {
    setLeft(0, 0);
    setRight(0, 0);
}

int  Motors::getSpeed() {return g_speed;}