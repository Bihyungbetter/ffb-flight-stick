# Bill of Materials (BOM)

Force-Feedback Flight Stick — generated from the Onshape "Flight Sim Master Assembly".

| # | Item | Qty | Source | Unit Price | Link | Notes |
|---|------|-----|--------|-----------|------|-------|
| 1 | T-Motor GB54-2 Gimbal Motor (KV26, 3-6S brushless) | 2 | T-Motor | $74.90 | https://store.tmotor.com/product/gb54-2-gimbal-type.html | FFB drive motors (one per axis) |
| 2 | Spiral Bevel Gear, 1.7M 40T 45 deg | 3 | 3D printed | n/a | (printed) | Bevel differential gears |
| 3 | goBILDA 3415 HTD Pulley (14mm Bore, 48T) 3415-0014-0048 | 2 | goBILDA | $12.99 | https://www.gobilda.com/3415-series-5mm-htd-pitch-hub-mount-timing-belt-pulley-14mm-bore-48-tooth/ | Driven pulley |
| 4 | goBILDA 3417 HTD Pinion (8mm REX Bore, 16T) 3417-4008-0016 | 2 | goBILDA | $9.99 | https://www.gobilda.com/3417-series-5mm-htd-pitch-set-screw-pinion-timing-belt-pulley-8mm-rex-bore-16-tooth/ | Motor pinion pulley |
| 5 | goBILDA 2106 Series 8mm REX Stainless Shaft | 2 | goBILDA | from $3.69 | https://www.gobilda.com/stainless-steel-rex-shafting/ | Pick length to match build |
| 6 | goBILDA 5mm HTD Timing Belt (9mm width) | 2 | goBILDA | $4.79-$6.99 | https://www.gobilda.com/5mm-htd-timing-belts/ | Pick length for center distance |
| 7 | goBILDA 1611 Flanged Ball Bearing (8mm REX ID x 14mm OD) | 1 pk | goBILDA | $5.99 / 2-pack | https://www.gobilda.com/1611-series-flanged-ball-bearing-8mm-rex-id-x-14mm-od-5mm-thickness-2-pack/ | Shaft support bearings |
| 8 | Large axis bearing (verify OD/ID from CAD) | 2 | Amazon | TBD | https://www.amazon.com/s?k=thin+section+turntable+ball+bearing | Confirm size against CAD |
| 9 | McMaster 97164A116 Heat-Set Insert, M4 x 0.7mm | 1 pk | McMaster-Carr | $8.77 / 25-pack | https://www.mcmaster.com/97164A116/ | For 3D-printed parts |
| 10 | STM32 B-G431B-ESC1 FOC Motor Driver Board | 2 | DigiKey | $39.53 | https://www.digikey.com/en/products/detail/stmicroelectronics/B-G431B-ESC1/10321670 | One FOC driver per motor |
| 11 | Bourns PDB181-K415K-103B Potentiometer (10k, linear) | 1 | DigiKey | $1.81 | https://www.digikey.com/en/products/detail/bourns-inc/PDB181-K415K-103B/3820293 | Throttle position sensor |

**Estimated total (excl. item 8 TBD, qty-adjusted): ~$310.73**

The GB54-2 is a brushless gimbal motor, so it needs a FOC driver with current sensing (the B-G431B-ESC1 above), one per motor.
