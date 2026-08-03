# Firmware

Two independent PlatformIO projects, plus the binary protocol shared between them:

- `common/protocol.h` — UART framing shared by both projects below.
- `motor_node/` — flash onto **each** B-G431B-ESC1 board (one per motor). Runs SimpleFOC in voltage-mode torque control and talks to `stick_controller` over a dedicated UART.
- `stick_controller/` — flash onto the Blackpill F411. Reads both motor nodes' telemetry, un-mixes it into pitch/roll, runs a basic centering-spring force model, mixes the resulting torques back down to the two motors, and exposes pitch/roll to the PC as a standard USB joystick.

## Target hardware

- **MCU:** STM32 Blackpill F411 (`stick_controller`) — needs an FPU.
- **Drivers:** 2× ST B-G431B-ESC1 FOC gate-driver boards, one per motor, each running `motor_node`.
- **Encoders:** 2× AS5047P over SPI in absolute mode, one per motor node.
- **Motors:** 2× T-Motor GB54-2 gimbal motors (see `../BOM.csv`).

## Wiring

| Signal | motor_node 0 | motor_node 1 | stick_controller |
|---|---|---|---|
| Link UART | to stick_controller UART A | to stick_controller UART B | UART A + UART B (2 separate hardware UARTs) |
| Encoder | AS5047P #0, SPI, on this node | AS5047P #1, SPI, on this node | — |
| Motor phases | T-Motor GB54-2 #0 | T-Motor GB54-2 #1 | — |
| USB | — | — | to PC (joystick) |

Exact pins are in `motor_node/src/board_pins.h` and the `UartLink` constructors in `stick_controller/src/main.cpp`. **Both are explicitly marked placeholders** — check them against the real wiring harness and ST's B-G431B-ESC1 pinout (UM2516) before flashing. This drives a three-phase bridge; a wrong pin can short the DC bus.

## Stack

- [SimpleFOC](https://github.com/simplefoc/Arduino-FOC) for per-motor FOC torque control, running on each `motor_node`.
- The differential mixing layer (`stick_controller/src/mixing.h/.cpp`, see also `../docs/SOFTWARE.md`).
- A USB HID-PID FFB descriptor + effect engine — **not yet built**, see "What's not done yet" below.

## Build order (bring-up, matches the top-level README)

1. Flash `motor_node` onto **one** ESC1 board, motor free-spinning and *not* bolted to the gimbal. Confirm `initFOC()` completes and the shaft responds to a manually-set `target_torque`.
2. Flash the same firmware onto the second ESC1 board and repeat, independently.
3. Flash `stick_controller`, wire up both UART links and both encoders. Tap `pitch`/`roll` over `Serial1` (see `HidJoystick`) and confirm they track physical pitch/roll with the correct sign *before* enabling torque.
4. Only once 1–3 are confirmed on the bench: bolt to the gimbal, start with very soft spring/damper gains (`kSpringK`, `kDamperB` in `stick_controller/src/main.cpp`), and increase gradually.

## Current-limit note

Start around 15 A to manage stall heat (N42SH high-temp magnets give some thermal margin) — see the top-level README's "Known risks". `motor_node` currently limits via `motor.voltage_limit` (volts), since it runs voltage-mode torque control; a true amp-based limit needs the current-mode FOC upgrade below.

## What's not done yet

- **USB HID-PID** (the "long pole" from `../docs/SOFTWARE.md`) — a sim driving real force-feedback effects into this stick over USB. Not implemented. `stick_controller` currently only runs a fixed spring-to-center effect computed locally, and reports pitch/roll to the PC as ordinary joystick axes — no in-game effect (spring/damper/constant-force from a sim) reaches the motors yet. Porting the effect math from `YukMingLaw/ArduinoJoystickWithFFBLibrary` (see `../docs/SOFTWARE.md`) onto an STM32 USB HID stack is the next real step.
- **Current-mode FOC** (`TorqueControlType::foc_current`) — `motor_node` runs voltage-mode torque control for now, which doesn't need current sense wired up. `board_pins.h` reserves the current-sense ADC pins for when someone wires up `LowsideCurrentSense` with the real shunt/gain values from the ESC1 schematic — voltage mode is coarser but is the right place to start bring-up.
- **STM32 USB Joystick API** — `hid_joystick.cpp` assumes STM32duino's `<Joystick.h>` API; it's guarded with `__has_include` and always falls back to a `Serial1` printout of pitch/roll if that header isn't present in your installed core version, so the rest of the firmware stays testable either way.

None of this has run on real hardware yet — everything above is written to the documented API surface of SimpleFOC / STM32duino, not verified against the physical build.

## Building

Each of `motor_node/` and `stick_controller/` is its own PlatformIO project:

```
cd firmware/motor_node && pio run -t upload
cd firmware/stick_controller && pio run -t upload
```

## Testing the mixing math without hardware

```
cd firmware/stick_controller && pio test -e native
```

Runs `test/test_mixing/test_mixing.cpp` against `src/mixing.cpp` on the host, no board required. Useful for changing the differential mixing formula (e.g. once the real gear ratio is known) without needing the bench setup.
