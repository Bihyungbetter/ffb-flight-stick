# Software stack & references

The firmware is built on **SimpleFOC** (a library, not closed firmware) so the differential mixing math can live in user code. There is no turnkey FFB firmware that supports differential mixing, so this is assembled from the references below.

## Core

- **[SimpleFOC / Arduino-FOC](https://github.com/simplefoc/Arduino-FOC)** — per-motor field-oriented torque control. The differential mixing matrix lives in user code on top of this.

## Reference projects (to copy from)

- **[MackaJunest/Ez-Force-Feedback](https://github.com/MackaJunest/Ez-Force-Feedback)** — SimpleFOC-based DIY FFB joystick. Closest conceptual match. Note it uses ESP32 + AS5600 + Bluetooth, so it is a *conceptual* reference; this build targets STM32 + AS5047P (SPI) + USB HID.
- **[YukMingLaw/ArduinoJoystickWithFFBLibrary](https://github.com/YukMingLaw/ArduinoJoystickWithFFBLibrary)** — multi-axis HID FFB library with a clean `Gains` / `EffectParams` API (spring, damper, inertia, friction, constant, ramp, periodic). **Caveat:** written for ATmega32U4 (AVR) — needs porting to STM32.
- **[vsulako/AFFBWheel](https://github.com/vsulako/AFFBWheel)** — FFB wheel project; has an open discussion thread on integrating SimpleFOC.
- **[NicoHood/HID](https://github.com/NicoHood/HID)** — reference for the USB HID **PID** descriptor so the sim recognizes the device as an FFB controller.

## The long pole — HID-PID FFB library for STM32

The hardest remaining piece is a **maintained, SimpleFOC-compatible HID-PID USB FFB library for the chosen STM32** (F411). I could not confirm a clearly-maintained drop-in for STM32 — search `stm32-hid-ffb` forks and the [hid-ffb topic](https://github.com/topics/hid-ffb). The realistic path is porting the FFB-effect math from the YukMingLaw library onto an STM32 USB HID stack (TinyUSB or the STM32 Arduino core USB).

## Differential mixing

```
Torque out:   tau_M0 = (F_pitch + F_roll) / 2
              tau_M1 = (F_pitch - F_roll) / 2

Angle in:     theta_pitch = (theta_M0 + theta_M1) / 2
              theta_roll  = (theta_M0 - theta_M1) / 2
```

Signs and scale depend on the final gear arrangement (TBD).

## Suggested build order

1. One motor — torque control under SimpleFOC.
2. Two motors — independent.
3. Add the differential mixing matrix.
4. Add USB HID-PID force feedback last.
