#pragma once

const int   SERVO_I2C_BUS     = 1;      // /dev/i2c-1
const int   SERVO_I2C_ADDR    = 0x40;
const float SERVO_FREQ        = 50.0f;    // Hz

const int CHANNEL_DIRECTION = 0

class Servo {
public:
    Servo(int channel, float minPulse, float maxPulse, float angleDefault);
    void setAngle(float angle);
    void center();
    int  getAngle() const;

private:
    int   channel;
    float minPulse;
    float maxPulse;
    float angleDefault;
    int   currentAngle;

    int angleToTicks(float angle) const;
};