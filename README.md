# FFB Flight Stick

A 2-axis **active force-feedback flight stick** built around a **differential gimbal** mechanism, driven by [SimpleFOC](https://github.com/simplefoc/Arduino-FOC).

Both motors drive both axes through a differential — pitch is the sum of the two motors, roll is the difference — instead of a per-axis nested gimbal. No turnkey FFB firmware supports that kind of mixing, so the firmware is built on SimpleFOC with the mixing math written by hand.

Two brushless motors drive a bevel differential through 3:1 HTD belt reductions. Each motor carries an absolute magnetic encoder; stick pitch and roll are derived from the sum and difference of the two motor angles, and force feedback is applied by commanding opposing or common-mode torque. There also includes a clamp on the flight stick to secure the stick to a table. 
It also includes a throttle quadrant that uses a single potentiometer to measure lever position.

Here is inspiration that I took. https://www.youtube.com/shorts/7KhpVU0RoAo 

## Repo layout

- `CAD/` — CAD / STEP exports of the mechanical assembly
- `PCB/` — no custom PCB in this design (off-the-shelf driver boards)
- `firmware/` — per-motor torque control, differential mixing, and USB joystick output
- `BOM.csv` — full bill of materials

## Pictures
<img width="3296" height="2547" alt="Flight Sim Master Assembly (4)" src="https://github.com/user-attachments/assets/09d1983d-284f-40d0-b0bd-5abb75de9913" />
<img width="3296" height="2547" alt="Flight Sim Master Assembly (3)" src="https://github.com/user-attachments/assets/6021f4fc-b0fd-4586-b654-d1e5381bf1ac" />


## License

MIT — see [LICENSE](LICENSE).
