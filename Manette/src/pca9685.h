#pragma once

namespace PCA9685 {
    bool init(int i2cBus, int i2cAddr, float freqHz);
    void cleanup();
    void setChannel(int channel, int ticks);
}