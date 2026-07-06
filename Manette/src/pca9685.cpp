#include "pca9685.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cmath>
#include <cstdint>
#include <iostream>


static const uint8_t REG_MODE1   = 0x00;
static const uint8_t REG_PRESCALE = 0xFE;
static const uint8_t REG_LED0_BASE = 0x06;  // base canal 0 (4 octets par canal)

static int g_fd = -1;

// Lecture / écriture I2C bas niveau
static void writeReg(uint8_t reg, uint8_t val) {
    uint8_t buf[2] = {reg, val};
    write(g_fd, buf, 2);
}

static uint8_t readReg(uint8_t reg) {
    write(g_fd, &reg, 1);
    uint8_t val = 0;
    read(g_fd, &val, 1);
    return val:
}

// Envoi des ticks sur un canal PCA9685
static void setChannel(int channel, int ticks) {
    uint8_t base = REG_LED0_BASE + 4 * channel;
    // ON toujours à 0, OFF = ticks (début de pulse à t=0)
    uint8_t buf[5] = {
        base,
        0x00, 0x00,                              // ON_L, ON_H
        (uint8_t)(ticks & 0xFF),                 // OFF_L
        (uint8_t)((ticks >> 8) & 0x0F)          // OFF_H
    };
    write(g_fd, buf, 5);
}

bool PCA9685::init() {
    char path[16];
    snprintf(path, sizeof(path), "/dev/i2c-%d", i2cBus);
    g_fd = open(path, O_RDWR);
    if (g_fd < 0) {
        std::cerr << "[SERVO] Erreur ouverture " << path << "\n";
        return false;
    }
    if (ioctl(g_fd, I2C_SLAVE, i2cAddr) < 0) {
        std::cerr << "[SERVO] Erreur adresse I2C 0x" << std::hex << i2cAddr << "\n";
        return false;
    }

    // Calcul du prescaler pour 50 Hz
    uint8_t prescale = (uint8_t)(std::round(25000000.0f / (4096.0f * freqHz)) - 1);

    // Mise en veille obligatoire pour changer le prescaler
    uint8_t oldmode = readReg(REG_MODE1);
    writeReg(REG_MODE1, (oldmode & 0x7F) | 0x10);  // bit SLEEP = 1
    writeReg(REG_PRESCALE, prescale);
    writeReg(REG_MODE1, oldmode);                   // réveil
    usleep(5000);                                   // attente oscillateur
    writeReg(REG_MODE1, oldmode | 0x80);            // RESTART
}