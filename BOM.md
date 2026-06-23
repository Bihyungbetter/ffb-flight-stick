# Bill of Materials — FFB Flight Stick

Prices are rough estimates (each), for planning only. Verify against live listings before buying.

## Confirmed parts

| Part | Qty | Spec / Notes | Est. each | Source |
|------|-----|--------------|-----------|--------|
| Flipsky 5065 270KV sensored BLDC | 2 | Keyed 8mm shaft, N42SH high-temp magnets, JST-ZH 6-pin Hall connector, 4 Nm max torque | \$64 | [flipsky.net](https://flipsky.net/products/bldc-motor) |
| AS5047P magnetic encoder | 2 | SPI **absolute** mode (true startup zero — critical for differential). Needs 6×2.5mm diametric magnet, non-ferromagnetic holder, 0.5–3mm air gap, centered within 0.5mm | \$8 | [mouser](https://www.mouser.com/c/?q=AS5047P) |
| 6×2.5mm diametric magnet | 2 | Diametrically magnetized, paired with AS5047P | \$2 | [digikey](https://www.digikey.com) |
| HTD-5M 15T pinion | 2 | ~4:1 reduction (270KV too high for direct-drive FFB torque) | \$6 | [sdp-si](https://www.sdp-si.com) |
| HTD-5M 60T pulley | 2 | Output pulley (4:1 with 15T pinion) | \$12 | [sdp-si](https://www.sdp-si.com) |
| HTD-5M timing belt | 2 | 15mm width to match pulleys | \$5 | [sdp-si](https://www.sdp-si.com) |
| 6804 bearing (20×27×4) | 12 | Thin-section deep-groove | \$1.50 | — |
| 6805 bearing (25×37×7) | 2 | Deep-groove | \$2.50 | — |
| PSU 24V ~150–250W | 1 | Bench/enclosed supply | \$35 | [meanwell](https://www.meanwell.com) |

## Hardware to be selected

| Part | Qty | Spec / Notes | Est. each | Status |
|------|-----|--------------|-----------|--------|
| STM32 Blackpill F411 | 1 | F411 has **FPU** + enough flash. F103 NOT sufficient (no FPU). Alt: Nucleo F411/F446. | \$8 | To buy — verify |
| B-G431B-ESC1 (STM32G431) | 2 | Easy SimpleFOC start, integrated current sense. **Caveat:** lower current headroom; community reports fried units under sustained high current. Good for prototyping the 5065s at limited current. | \$40 | Option A — buy 1 to test |
| DRV8302-class gate driver board | 2 | ~15–25A continuous; better for sustained 5065 torque. Pair with the STM32. Preferred for final build. | \$25 | Option B — final |
| Anti-backlash miter gear pair (45° shaft, 1:1) | 2 pairs | Differential 90° shaft angle, 1:1 = 45° miter. Anti-backlash strongly preferred (lash at center hurts FFB feel). PIC Design N1-1-AB / N2-1-AB, or SDP/SI angular miter. | \$60 | To buy — verify bore |
| JST-ZH 6-pin Hall cable | 2 | To match motor Hall connector | \$2 | To buy |

## Notes

- The originally-considered MKS XDrive (ODrive firmware) is no longer needed under SimpleFOC.
- 270KV is too high for useful direct-drive FFB torque, hence the ~4:1 belt reduction.
