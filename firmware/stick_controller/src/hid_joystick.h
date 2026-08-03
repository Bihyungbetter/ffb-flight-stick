#pragma once

namespace ffb {

class HidJoystick {
public:
    void begin();
    void updateAxes(float pitchNorm, float rollNorm);
    void update();

private:
    float pitch_ = 0, roll_ = 0;
};

}
