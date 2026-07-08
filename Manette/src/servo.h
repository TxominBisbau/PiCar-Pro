#pragma once

const int   SERVO_I2C_BUS     = 1;      // /dev/i2c-1
const int   SERVO_I2C_ADDR    = 0x40;
const float SERVO_FREQ        = 50.0f;    // Hz

const int CHANNEL_DIRECTION = 0;    // Servo de direction
const int CHANNEL_ROTATION = 1;     // Servo de rotation de la pince
const int CHANNEL_INCINE = 2;       // Servo d'inclinaison de la pince
const int CHANNEL_POSITION = 3;     // Servo de position de la pince
const int CHANNEL_OUVERT = 4;       // Servo d'ouverture de la pince

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