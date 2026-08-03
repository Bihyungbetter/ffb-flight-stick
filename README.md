# FFB Flight Stick

A 2-axis **active force-feedback flight stick** built around a **differential gimbal** mechanism, driven by [SimpleFOC](https://github.com/simplefoc/Arduino-FOC).

This repo is an open-source dump of the current design state: bill of materials, the mechanical/CAD source, the control architecture, and the software references the firmware will be built from. It is a work in progress.

## Architecture

Both motors drive **both** axes through a differential:

- **Pitch** = sum of the two motors
- **Roll** = difference of the two motors

This is a deliberate design choice (committed to differential, *not* a per-axis nested gimbal). The trade-off: no turnkey FFB firmware (FFBeast, OpenFFBoard) supports differential mixing, so the firmware is built on **SimpleFOC** (a library, not closed firmware) with the mixing math living in user code.

### Differential mixing

```
Torque out:   tau_M0 = (F_pitch + F_roll) / 2
              tau_M1 = (F_pitch - F_roll) / 2

Angle in:     theta_pitch = (theta_M0 + theta_M1) / 2
              theta_roll  = (theta_M0 - theta_M1) / 2
```

Signs and scale depend on the final gear arrangement and are TBD.

## Build order

1. One motor, torque control under SimpleFOC
2. Two motors, independent
3. Add the differential mixing matrix
4. Add USB HID-PID force feedback last (the long pole)

## Known risks

- **Bevel-gear backlash at stick center** — mechanical, not software-fixable. Anti-backlash gears strongly preferred.
- **Coupled calibration** — pitch and roll cannot be tuned fully independently because both motors drive both axes.
- **Stall heat** — limit current (~15 A to start). The N42SH high-temp magnets help.

## Repo layout

- `BOM.csv` — full bill of materials (confirmed + to-be-selected parts, with links)
- `docs/SOFTWARE.md` — software stack and reference projects
- `CAD/` — CAD / STEP exports (see note below)
- `PCB/` — no custom PCB in this design; see `PCB/README.md`
- `firmware/` — SimpleFOC per-motor torque control + differential mixing + USB joystick output; HID-PID FFB is not implemented yet (see `firmware/README.md`)

## CAD

The master assembly lives in Onshape:
https://cad.onshape.com/documents/8b9e32ec69005829dc7feba2/w/32ad78aac4ca843da7f737c5/e/9393deb6d1ad6c6f61d9f366

`CAD/Flight_Sim_Master_Assembly.step` is the current exported snapshot. Re-export from Onshape and replace it whenever the assembly changes — see `CAD/README.md`.

## Status / what's built vs. designed

This is still a work in progress — nothing below has been assembled or run on real hardware yet:

- **Designed, not yet built:** the mechanical assembly (`CAD/`), the BOM (`BOM.csv`).
- **Written, not yet bench-tested:** the firmware (`firmware/`) — per-motor SimpleFOC torque control, differential mixing, UART link between the master controller and the two motor nodes, and USB joystick output.
- **Not started:** USB HID-PID force feedback (a sim driving real force effects into the stick) — see `firmware/README.md` "What's not done yet".

Build photos, a wiring/PCB screenshot, and a demo video will go here once there's a physical unit to point a camera at.

## License

MIT — see [LICENSE](LICENSE).
