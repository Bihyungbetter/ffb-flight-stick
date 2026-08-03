# FFB Flight Stick

A 2-axis **active force-feedback flight stick** built around a **differential gimbal** mechanism, driven by [SimpleFOC](https://github.com/simplefoc/Arduino-FOC).

Both motors drive both axes through a differential — pitch is the sum of the two motors, roll is the difference — instead of a per-axis nested gimbal. No turnkey FFB firmware supports that kind of mixing, so the firmware is built on SimpleFOC with the mixing math written by hand.

## Repo layout

- `CAD/` — CAD / STEP exports of the mechanical assembly
- `PCB/` — no custom PCB in this design (off-the-shelf driver boards)
- `firmware/` — per-motor torque control, differential mixing, and USB joystick output
- `BOM.csv` — full bill of materials

## License

MIT — see [LICENSE](LICENSE).
