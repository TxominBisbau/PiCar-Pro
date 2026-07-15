#pragma once

const int   SERVO_I2C_BUS     = 1;      // /dev/i2c-1
const int   SERVO_I2C_ADDR    = 0x40;

const int CHANNEL_LEFT = 5;
const int CHANNEL_RIGHT = 6;
const int CHANNEL_TOP = 13;

class Led {
public:
    Led(int channel, bool stateDefault);
    void setState(bool state);
    int getState() const;

private:
    int channel;
    bool stateDefault;
    bool currentState;

    int angleToTicks(bool state) const;
};