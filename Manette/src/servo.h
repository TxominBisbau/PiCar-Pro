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