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
