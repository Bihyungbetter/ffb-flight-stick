#include "hid_joystick.h"

#include <Arduino.h>

#if __has_include(<Joystick.h>)
#include <Joystick.h>
#define FFB_HAVE_STM32_JOYSTICK 1
#endif

namespace ffb {

void HidJoystick::begin() {
#ifdef FFB_HAVE_STM32_JOYSTICK
    Joystick.begin();
#endif
    Serial1.begin(115200);  // debug tap -- see class comment in hid_joystick.h
}

void HidJoystick::updateAxes(float pitchNorm, float rollNorm) {
    pitch_ = constrain(pitchNorm, -1.0f, 1.0f);
    roll_ = constrain(rollNorm, -1.0f, 1.0f);
}

void HidJoystick::update() {
#ifdef FFB_HAVE_STM32_JOYSTICK
    Joystick.X(static_cast<int16_t>(roll_ * 32767));
    Joystick.Y(static_cast<int16_t>(pitch_ * 32767));
    Joystick.send_now();
#endif
    static uint32_t last = 0;
    uint32_t now = millis();
    if (now - last > 50) {  // 20 Hz, plenty for a debug readout
        Serial1.print("pitch=");
        Serial1.print(pitch_, 3);
        Serial1.print(" roll=");
        Serial1.println(roll_, 3);
        last = now;
    }
}

}  // namespace ffb
