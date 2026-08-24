# Wiring

The interconnect harness is defined as text in [`ffb-harness.yml`](ffb-harness.yml)
and rendered with [WireViz](https://github.com/wireviz/WireViz). The YAML is the
source of truth; everything else in this folder is generated from it.

| File | |
|---|---|
| [`ffb-harness.yml`](ffb-harness.yml) | Source. Edit this. |
| [`ffb-harness.svg`](ffb-harness.svg) | Rendered diagram, vector — sharp at any zoom |
| [`ffb-harness.png`](ffb-harness.png) | Same diagram, raster. This is the one embedded in the root README; GitHub's markdown renderer is unreliable with relative SVG. |
| [`ffb-harness.bom.tsv`](ffb-harness.bom.tsv) | Wire and connector BOM, auto-generated |
| [`NOTES.md`](NOTES.md) | Unresolved pins and firmware/BOM discrepancies |

## Designators

The boxes in the diagram are labelled with conventional electronics
abbreviations. Spelled out:

| Tag | Stands for | What it is here |
|---|---|---|
| `DRV0` `DRV1` | **Driver** | The two FOC gate-driver boards, one per motor. Called `DRV` rather than `ESC` because the part is itself named *B-G431B-ESC1* — an `ESC1` designator would be ambiguous with the board model. |
| `MOT0` `MOT1` | **Motor** | The two BLDC gimbal motors driving the differential. |
| `ENC0` `ENC1` | **Encoder** | AS5047P absolute magnetic angle sensors, one per motor. They report rotor angle, which FOC needs to commutate and which the mixing math turns into pitch and roll. |
| `MCU` | **M**icro**c**ontroller **U**nit | The WeAct Blackpill stick controller. Reads both motor nodes, unmixes their angles into pitch/roll, computes the force feedback, and presents the result to the PC as a USB joystick. Runs `firmware/stick_controller/`. |
| `POT` | **Pot**entiometer | A variable resistor — the throttle lever slides its wiper along a resistive track, so lever position can be read as a voltage. Here, the Bourns PDB181-K415K-103B, 10 kΩ linear. |
| `PSU` | **P**ower **S**upply **U**nit | The 12 V DC supply. No specific part is chosen yet — see [`../BUILD.md`](../BUILD.md). |
| `SPLIT` | *(not an abbreviation)* | The power distribution point, or "star point", where the single 12 V rail splits to feed both driver boards. |
| `HOST` | USB **host** | Your PC. In USB terminology the host is the machine in charge and the thing plugged into it is the device — so the PC is the host and the stick is the device. |
| `W_…` | **W**ire | `W` is the conventional designator letter for a cable, so `W_LINK0` is "the cable carrying `link0`". |

The `0`/`1` suffix always means motor node 0 or 1, matching `link0` and `link1`
in `firmware/stick_controller/src/main.cpp`. There is no separate "node 0" and
"node 1" firmware build — both driver boards run identical code, and a node is
only node 0 or node 1 by virtue of which stick-controller UART it's wired to.

## Read NOTES.md before building

The diagram contains pins labelled `TBD-n`. Those are placeholders for
connections that could not be determined from anything in this repo — they are
not guesses, and nothing should be soldered to them. [`NOTES.md`](NOTES.md)
explains what each one is waiting on, and lists five firmware/BOM discrepancies
that writing the harness down exposed. Two of them currently block assembly.

## Rendering

Needs Python and [Graphviz](https://graphviz.org/download/) (the `dot` binary
must be on `PATH`).

```sh
pip install wireviz
wireviz docs/wiring/ffb-harness.yml
```

That writes `.svg`, `.png`, `.html` and `.bom.tsv` next to the YAML. Or:

```sh
./docs/wiring/render.sh
```

Commit the regenerated `.svg`, `.png` and `.bom.tsv` along with the YAML change
so the diagram in the README never drifts from its source. The `.html` output
is gitignored — it's a standalone viewer, useful locally, redundant in the repo.

## The scope of this diagram

It covers connections that leave a board. Signals internal to a module — the
six PWM lines from the G431 to its own gate driver, the current-sense taps, the
enable and fault lines — live in `firmware/motor_node/src/board_pins.h` and are
not harness connections, so they aren't drawn here.
