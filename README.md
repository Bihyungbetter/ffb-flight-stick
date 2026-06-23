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

- `BOM.md` — full bill of materials (confirmed + to-be-selected parts, with links)
- `docs/SOFTWARE.md` — software stack and reference projects
- `cad/` — CAD / STEP exports (see note below)
- `firmware/` — SimpleFOC + HID-FFB firmware (WIP)

## CAD

The master assembly lives in Onshape:
https://cad.onshape.com/documents/8b9e32ec69005829dc7feba2/w/32ad78aac4ca843da7f737c5/e/9393deb6d1ad6c6f61d9f366

STEP files should be exported from Onshape and dropped into `cad/`.

## License

MIT — see [LICENSE](LICENSE).
