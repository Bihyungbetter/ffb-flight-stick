# Building this project

Split into two halves on purpose: what someone can actually do with this repo
today, and what they'd still have to ask you for. The second half is the
honest answer to "could a stranger rebuild this from the repo alone?" — right
now, no, and the gaps are listed so they can be closed.

---

# Part 1 — Reproducible from this repo today

## Toolchain

[PlatformIO](https://platformio.org/) is the only build dependency; it fetches
the STM32 toolchain and SimpleFOC itself.

```sh
pip install platformio
```

An ST-LINK is needed to flash the driver boards. The Blackpill flashes over
USB DFU (hold BOOT0, tap NRST) or via ST-LINK on its SWD pads.

## Build and test

The differential mixing math has host-side unit tests that need no hardware —
run these first, they're the fastest check that a checkout is sound:

```sh
cd firmware/stick_controller
pio test -e native
```

Five tests in `test/test_mixing/test_mixing.cpp`: pure pitch, pure roll, the
two inverse cases, and a power-conservation check that the forward and inverse
transforms are consistent.

> **On how this was verified.** All five tests pass, but they were confirmed by
> compiling `src/mixing.cpp`, `test/test_mixing/test_mixing.cpp` and Unity
> directly with g++ and running that binary. `pio test -e native` builds and
> links correctly, but a local Windows Application Control policy blocked it
> from executing the binary it had just produced (`WinError 4551`). That
> restriction is a property of one machine, not of this repo, so the command
> above is the right one to use — it just hasn't been observed end-to-end here.

```sh
# Stick controller
cd firmware/stick_controller && pio run -e blackpill_f411ce -t upload

# Motor node — identical firmware, flashed to BOTH driver boards
cd firmware/motor_node && pio run -e b_g431b_esc1 -t upload
```

The two motor nodes run byte-identical firmware. Nothing distinguishes them
except which stick-controller UART each is wired to, so there's no "node 0
build" and "node 1 build" to keep straight.

## Wiring

See [`wiring/`](wiring/) — the harness is a WireViz diagram generated from
[`wiring/ffb-harness.yml`](wiring/ffb-harness.yml).

**Read [`wiring/NOTES.md`](wiring/NOTES.md) first.** Two firmware problems
currently block assembly (motor node 1 is wired to pins that don't exist on
the F411's package, and the node 0 link collides with the debug serial), and
nine connector pin numbers are still unresolved.

## How the differential works

From `firmware/stick_controller/src/mixing.cpp` — this is the part with no
turnkey equivalent, so it's worth stating plainly:

```
Forward  (axis torque -> motor torque)     Inverse  (motor angle -> axis angle)
  m0 = (pitch + roll) / 2                    pitch = (m0 + m1) / 2
  m1 = (pitch - roll) / 2                    roll  = (m0 - m1) / 2
```

Common-mode motion is pitch, differential motion is roll. Both motors
contribute to both axes, which is why per-axis FFB firmware can't drive this
mechanism.

## Protocol

`firmware/common/protocol.h`, shared by both sides. 921600 baud, `0xA5` frame
start, XOR checksum over all preceding bytes.

| Direction | Payload |
|---|---|
| Stick → node | `float target_torque` |
| Node → stick | `float angle_rad`, `float velocity_rad_s`, `uint8_t fault` |

Nodes emit telemetry every 1000 µs. If either link drops a frame, the stick
controller commands zero torque to both motors that cycle
(`stick_controller/src/main.cpp`) — the stick goes limp rather than
unpredictable, which is the right failure direction.

## Constants you will want to touch

| Constant | Value | Where | What it does |
|---|---|---|---|
| `driver.voltage_power_supply` | 12 | `motor_node/src/main.cpp` | Must match your actual supply |
| `motor.voltage_limit` | 4.0 | `motor_node/src/main.cpp` | Caps torque. **Raise slowly.** |
| `BLDCMotor(7)` | 7 | `motor_node/src/main.cpp` | Pole pairs — motor-specific |
| `kSpringK` | 0.6 | `stick_controller/src/main.cpp` | Centring spring strength |
| `kDamperB` | 0.02 | `stick_controller/src/main.cpp` | Damping |
| `kAxisRangeRad` | 0.6 | `stick_controller/src/main.cpp` | Full-scale axis travel, in **motor** radians |

`kAxisRangeRad` is applied to the unmixed *motor* angle, before any mechanical
reduction. The stick deflection it corresponds to therefore depends on the
drivetrain ratio — see gap #3 below, which is currently ambiguous.

## Suggested bring-up order

Derived from what each `setup()` does, not from a tested procedure:

1. `pio test -e native` — mixing math, no hardware.
2. One motor node on the bench, no belt, no linkage. `motor.initFOC()` runs a
   pole-pair and sensor-alignment sweep at power-on; the shaft will move. Give
   it room.
3. Confirm the encoder reads a sane absolute angle before enabling torque.
4. Second node, same checks.
5. Both nodes on the stick controller. Verify telemetry arrives from both —
   if either link is silent, `main.cpp` holds both motors at zero torque, so
   a dead link presents as a completely limp stick, not a one-sided one.
6. Only then couple the drivetrain and raise `motor.voltage_limit`.

Two motors geared into one differential can fight each other hard. Keep the
voltage limit low until the sign conventions are confirmed.

---

# Part 2 — What's missing before someone else could build this

Everything below is a gap, not a defect. Items marked *(STEP)* were read out
of the product names inside `CAD/Flight_Sim_Master_Assembly.step` and are
**unconfirmed** — CAD assemblies routinely contain placeholder parts that were
never swapped out, so treat these as "appears in the model", not "is required".

## 1. No printable geometry

`CAD/` holds a single 21 MB STEP of the whole master assembly. There are no
STL or 3MF exports, so a builder has to open a 21 MB CAD file and work out
which bodies are printed parts. *(STEP)* shows generically-named solids —
`Base`, `Part 1` through `Part 5` — with nothing indicating which are printed,
which are stock, or how they orient on a bed.

Also absent: material, layer height, infill, wall count, support strategy.
The README mentions a table clamp and a throttle quadrant; neither maps to an
identifiable file.

## 2. The drivetrain is not in the BOM

`BOM.csv` has motors, shafting, belts, bearings, inserts, electronics. It has
none of the gearing that makes this a differential. Seen in *(STEP)* but
missing from the BOM:

| Part *(STEP)* | Notes |
|---|---|
| `Spiral Bevel 1.7M 40T 45deg` | The differential itself. Appears multiple times. |
| `Helical 1.5M 35T` | 1.5 module, 35 tooth |
| `Helical 1.5M 15T` | 1.5 module, 15 tooth |
| `16T Pulley Adapter` | The only pulley-ish part in the model |
| `2106 Series 8mm REX E-Clip` | Shaft retention |
| `6656K238_Steel Ball Bearing` | McMaster PN. Not the bearings in `BOM.csv`. |
| `5972K279_Steel Ball Bearing` | McMaster PN. Not the bearings in `BOM.csv`. |

Instance counts in the STEP are placements, not quantities — nested
subassemblies inflate them — so quantities need to come from you, not from the
file. No source or price is recorded for any of it.

## 3. The reduction ratio is ambiguous

The README says the motors drive the differential through **3:1 HTD belt
reductions**. The CAD contains a **35T/15T helical pair** *(STEP)*, which is
2.33:1, plus a `16T Pulley Adapter`. `BOM.csv` lists 5 mm HTD belts but no
pulleys.

These are two different mechanisms and they don't agree. Possibilities: the
helical pair superseded the belt drive, they're separate stages, or one is a
leftover from an earlier revision. **Only you know which.** It matters beyond
documentation — the ratio sets what `kAxisRangeRad` means in terms of actual
stick throw.

## 4. Consumables and fasteners

Not in `BOM.csv`, all needed:

- **12 V power supply** — the firmware assumes 12 V and two GM5208-24s under
  FFB load is not a trivial draw. No current rating is specified anywhere.
- **Wire and connectors** — the harness now specifies gauges and conductor
  counts (`docs/wiring/ffb-harness.bom.tsv`), but no connector series is
  chosen for the driver boards' J3/J8.
- **M4 screws** to pair with the M4 heat-set inserts that *are* listed.
- **Diametric magnets** for the AS5047P encoders. These sometimes ship with
  the breakout — worth confirming and noting either way, since the sensor is
  useless without one correctly positioned.
- **Belt length / tooth count.** `BOM.csv` says "5mm HTD Timing Belt (9mm
  width)" with no length, which isn't orderable as written.
- **USB cable**, filament.

## 5. Motor part number disagreement

`BOM.csv` says **GM5208-24**. The CAD contains a part named
**`GB54-2 Gimbal Motor`** *(STEP)*. Probably a stand-in model, but if the
mounting pattern in the CAD is the GB54-2's, printed parts won't fit a
GM5208-24. Worth a look at the actual bolt circle.

Separately, `BOM.csv` lists a **SimpleFOC Shield V2.0.4** while the firmware
and `PCB/README.md` both specify the **B-G431B-ESC1** — see
[`wiring/NOTES.md`](wiring/NOTES.md) #5.

---

## Closing the gap

Roughly in order of how much it unblocks a stranger:

- [ ] Resolve #3 — belt or helical? Everything mechanical depends on it.
- [ ] Add the drivetrain parts to `BOM.csv` with sources, quantities, prices.
- [ ] Export printed parts as STL/3MF and record print settings.
- [ ] Fix the driver-board line in `BOM.csv` (`NOTES.md` #5).
- [ ] Add PSU, fasteners, wire, magnets, belt length.
- [ ] Fix the two blocking firmware issues (`NOTES.md` #1, #2).
- [ ] Resolve the nine `TBD` connector pins from UM2516.
- [ ] Write the mechanical assembly order once the above settles.
