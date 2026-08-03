# PCB

This project doesn't use a custom PCB. Motor control and sensing are done with off-the-shelf boards:

- 2x ST **B-G431B-ESC1** FOC gate-driver boards (one per motor) — see `BOM.csv`.
- Encoder and MCU boards are also off-the-shelf modules; see `BOM.csv` and `firmware/README.md` for the full list and wiring.

Folder kept for layout consistency with the required CAD / PCB / firmware structure. If a custom breakout or interconnect board gets designed later (e.g. to tidy up the wiring between the two ESC1 boards, the encoders, and the Blackpill), its KiCad source will go here.
