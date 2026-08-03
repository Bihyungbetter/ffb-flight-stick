#pragma once

namespace ffb {

// USB HID output to the PC. STM32duino's Arduino core exposes a Joystick_
// class (via <Joystick.h>) on boards built with -DUSBD_USE_HID_COMPOSITE,
// analogous to the standard Arduino Mouse/Keyboard API -- but the exact
// header/class name has moved between STM32duino core versions. If this
// doesn't compile against your installed core, check
// STM32duino/Arduino_Core_STM32's USBDevice/src/USBHID folder for the
// current API and adjust HidJoystick's .cpp accordingly.
//
// Until that's confirmed working end-to-end, this also always mirrors
// pitch/roll out over Serial1, so the rest of the firmware (mixing + UART
// links + spring effect) is independently testable without a PC seeing a
// USB joystick.
//
// This reports position only. It is NOT the USB HID-PID force-feedback
// protocol -- see firmware/README.md "What's not done yet".
class HidJoystick {
public:
    void begin();
    void updateAxes(float pitchNorm, float rollNorm);  // each expected in [-1, 1]
    void update();                                       // call once per loop(), after updateAxes()

private:
    float pitch_ = 0, roll_ = 0;
};

}  // namespace ffb
