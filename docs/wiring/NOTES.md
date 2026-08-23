# Wiring notes and open questions

Writing down the harness in [`ffb-harness.yml`](ffb-harness.yml) forced every
inter-board connection to be named explicitly. Five of them could not be
resolved from anything in this repo. They are listed here rather than guessed
at in the diagram, because a harness drawing that invents plausible pin numbers
is worse than one that admits it doesn't know them.

Nothing below is a rendering problem — the diagram builds. These are questions
about the hardware.

---

## 1. `link1` is wired to pins that don't exist — blocking

`firmware/stick_controller/src/main.cpp:10`

```cpp
UartLink link1(PC10, PC11, 921600);
```

The BOM specifies the **STM32F411CEU6**, which is a **UFQFPN48** package. That
package brings out only `PC13`, `PC14` and `PC15` from port C. `PC10` and
`PC11` are not bonded out — they exist on the 64-pin and larger F411 variants
only. (`PC10`/`PC11` are the USART3 pins on those bigger packages, which is
presumably where the numbers came from.)

So motor node 1 currently has nowhere to land. `link1` needs remapping to a
UART that physically exists on the board before this harness can be built.
`MCU:TBD-A` / `MCU:TBD-B` in the YAML are holding that spot.

**Needs a decision:** which UART to move `link1` to. See #2, which constrains
the answer.

## 2. `link0` and the debug serial are the same peripheral — blocking

`firmware/stick_controller/src/main.cpp:9` declares:

```cpp
UartLink link0(PA9, PA10, 921600);
```

`PA9`/`PA10` are USART1. But `firmware/stick_controller/src/hid_joystick.cpp:16`
also does:

```cpp
Serial1.begin(115200);
```

`Serial1` is that same USART1. Two owners, two baud rates, one peripheral —
whichever `begin()` runs last wins, and the human-readable `pitch=... roll=...`
debug print at `hid_joystick.cpp:33-36` is being transmitted into motor node 0's
command parser. The parser will drop it (no `0xA5` frame start, and the
checksum backs that up), but node 0's telemetry is simultaneously being fed to
whatever is reading the debug port.

Between #1 and #2 the controller needs three UARTs — two links plus debug — and
currently has one assigned twice. Worth allocating all three together.

**Also, likely argument swap.** `UartLink`'s constructor is
`UartLink(rxPin, txPin, baud)` (`uart_link.h:16`, `uart_link.cpp:7`), so
`link0(PA9, PA10, ...)` requests RX on PA9 and TX on PA10. On the F411 the
alternate-function mapping is the other way round: PA9 is USART1_**TX** and
PA10 is USART1_**RX**. The STM32 Arduino core resolves the peripheral by
looking the pin up in `PinMap_UART_RX`, and PA9 isn't in it. The arguments look
reversed — `link1` has the same pattern. The YAML draws the corrected
orientation (`PA10` = RX, `PA9` = TX).

## 3. The ESC1 pin map may collide with the board's analog front end

`firmware/motor_node/src/board_pins.h:16-19` assigns:

```c
#define PIN_ENCODER_CS PA4
#define UART_RX PA3
#define UART_TX PA2
```

ST's documentation for the B-G431B-ESC1 describes `PA2` as `OP1_OUT`, `PA3` as
`Curr_fdbk1_OPAmp-` and `PA4` as `BEMF1` — all tied to the on-board current
sense and back-EMF circuitry, not free GPIO. If that holds, none of these three
can be used as assigned, and the encoder plus the whole telemetry link need
different pins.

**This one is not confirmed.** It comes from ST's summary documentation, not
from a pinout table read end to end. Two attempts to retrieve UM2516 failed
(one host 410'd, one had an expired certificate). Before doing anything about
it, check **UM2516, Table 4, "Main board STM32G431CB pinout for motor
control"** against `board_pins.h`, and confirm which signals are actually
exposed on **J3** (PWM/UART/BEC) and **J8** (motor sensor). Those are the same
tables that resolve `ESC0:TBD-1` through `TBD-9`.

Worth noting that the 6-PWM pins in `board_pins.h` (`PA8`/`PA9`/`PA10` high,
`PB13`/`PB14`/`PB15` low) do match the standard TIM1 complementary mapping, so
the pin map is not wholesale wrong — just possibly wrong in these three places.

## 4. The throttle potentiometer isn't read by any firmware

`BOM.csv` line 9 lists the Bourns PDB181-K415K-103B as "Throttle position
sensor", and the README describes a throttle quadrant. No firmware reads it:
there is no `analogRead`, no ADC pin in `board_pins.h`, and `HidJoystick`
exposes only `updateAxes(pitchNorm, rollNorm)` — two axes, no throttle.

The harness therefore has a pot with a defined 3V3/GND connection and an
undefined wiper destination (`MCU:TBD-C`). Picking a free ADC pin is easy; it
should be chosen at the same time as #1 and #2 so the pin budget is allocated
once.

Adding the axis means extending `HidJoystick` with a Z or slider axis and
adding the HID report descriptor entry — a real change, not just a pin
assignment.

## 5. `BOM.csv` and the firmware disagree on the driver board

`BOM.csv` line 8 lists:

> `"SimpleFOC Shield V2.0.4","2","18.99"`

But `firmware/motor_node/platformio.ini:3` builds for `board = b_g431b_esc1`,
`board_pins.h` uses the ESC1's 6-PWM pin map, and `PCB/README.md` states the
design uses "2x ST **B-G431B-ESC1** FOC gate-driver boards".

Three sources say ESC1, one says SimpleFOC Shield. The diagram follows the
firmware, since that's the version with actual pin numbers behind it. These are
not interchangeable — different MCU (G431 vs. whatever host the shield stacks
on), different pinout, different current sensing. `BOM.csv` also then has no
line item for the MCU that a SimpleFOC Shield would need.

If ESC1 is correct, `BOM.csv` needs updating — both the part and the price.

---

## Resolving these

Items #1, #2 and #4 are all pin-budget questions on the same MCU and are best
settled in one pass. #3 and the `TBD-1..9` designators are all answered by one
document (UM2516). #5 is a one-line BOM correction once someone confirms which
board is on the bench.

When a TBD is resolved, replace the designator in `ffb-harness.yml`, re-render,
and delete the corresponding note here.
