#pragma once

const int CHANNEL_LEFT = 1;
const int CHANNEL_RIGHT = 2;
const int CHANNEL_TOP = 3;

class Led {
public:
    Led(int channel, bool stateDefault)
    void setState(bool state);
    int getState() const;

private:
    int channel;
    bool stateDefault;
    bool currentState;
}