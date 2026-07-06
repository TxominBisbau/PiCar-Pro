#pragma once

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

/*
// PCA9685 — Servo de direction
const int   SERVO_I2C_BUS     = 1;      // /dev/i2c-1
const int   SERVO_I2C_ADDR    = 0x40;
const int   SERVO_CHANNEL     = 0;

const float SERVO_MIN_PULSE   = 500.0f;   // µs → 0°
const float SERVO_MAX_PULSE   = 2400.0f;  // µs → 180°
const float SERVO_FREQ        = 50.0f;    // Hz

const float ANGLE_DEFAULT     = 90.0f;  // tout droit

namespace Servo {
    bool init();
    void cleanup();
    void setAngle(float angle);   // 0.0 - 180.0°
    void center();
    int getAngle();
}
*/