# PCB

This project doesn't use a custom PCB. Motor control and sensing are done with off-the-shelf boards:

- 2x **SimpleFOC Shield v2.0.4** driver boards (one per motor) — see `BOM.csv`.
- Encoder and MCU boards are also off-the-shelf modules; see `BOM.csv` for the full list.

Folder kept for layout consistency with the required CAD / PCB / firmware structure. If a custom breakout or interconnect board gets designed later (e.g. to tidy up the wiring between the two shields, the encoders, and the Blackpill (see `docs/WIRING.md`)), its KiCad source will go here.
