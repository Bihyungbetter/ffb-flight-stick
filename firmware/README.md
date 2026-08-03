# Firmware (WIP)

SimpleFOC + USB HID-PID force-feedback firmware for the differential FFB flight stick. Not written yet — this folder is a placeholder.

## Target hardware

- **MCU:** STM32 (Blackpill F411 or Nucleo) — needs an FPU.
- **Drivers:** 2× BLDC gate-driver boards (B-G431B-ESC1 for prototyping, or DRV8302-class for the final build).
- **Encoders:** 2× AS5047P over SPI in absolute mode.
- **Motors:** 2× Flipsky 5065 270KV.

## Stack

- [SimpleFOC](https://github.com/simplefoc/Arduino-FOC) for per-motor FOC torque control.
- A user-written differential mixing layer (see ../docs/SOFTWARE.md).
- A USB HID-PID FFB descriptor + effect engine (the long pole — see ../docs/SOFTWARE.md).

## Build order

1. One motor, torque control under SimpleFOC.
2. Two motors, independent.
3. Add the differential mixing matrix.
4. Add USB HID-PID force feedback last.

## Current-limit note

Start around 15 A to manage stall heat. The N42SH high-temp magnets give some thermal margin.
