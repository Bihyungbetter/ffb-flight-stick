# Wiring

![Wiring diagram](wiring.svg)

Everything runs off one Blackpill. It drives both SimpleFOC shields, reads both AS5047P encoders on one SPI bus, and reads the throttle pot on an analog pin. The shields are Arduino shaped, so they don't plug into the Blackpill. I wire them with jumpers to the header pins listed below.

## Shield solder pads

Each shield picks its pins with solder pads. The two boards are set differently so they can still be stacked if you want.

| Pad | Shield #1 (motor M0) | Shield #2 (motor M1) |
|---|---|---|
| PWM A | D10 | D9 |
| PWM B | D5 | D3 |
| PWM C | D6 | D11 |
| Enable | D8 | D7 |
| LM7808 regulator | leave open | leave open |
| Current sense A / B | not used | not used |

## Blackpill to shields

| Signal | Shield #1 pin | Blackpill | Shield #2 pin | Blackpill |
|---|---|---|---|---|
| PWM A | D10 | PA8 (TIM1_CH1) | D9 | PB6 (TIM4_CH1) |
| PWM B | D5 | PA9 (TIM1_CH2) | D3 | PB7 (TIM4_CH2) |
| PWM C | D6 | PA10 (TIM1_CH3) | D11 | PB8 (TIM4_CH3) |
| Enable | D8 | PB12 | D7 | PB13 |
| Ground | GND | GND | GND | GND |

M0 gets its PWM from TIM1 and M1 gets its PWM from TIM4. Each motor needs all three of its phases on the same timer.

## Encoders (SPI1)

| AS5047P pin | Encoder #0 (M0) | Encoder #1 (M1) |
|---|---|---|
| CSn | PA4 | PB10 |
| CLK | PA5 (shared) | PA5 (shared) |
| MISO | PA6 (shared) | PA6 (shared) |
| MOSI | PA7 (shared) | PA7 (shared) |
| VDD | 3V3 | 3V3 |
| GND | GND | GND |

The encoder boards have to run at 3.3 V. On the AS5047P that means VDD is tied to VDD3V3, so check the jumper on your board. Each motor gets a diametric magnet glued to the back of the shaft, centered over the chip.

## Throttle pot (Bourns PDB181, 10k linear)

| Pot pin | Goes to |
|---|---|
| 1 (end) | GND |
| 2 (wiper) | PB1 (ADC1_IN9) |
| 3 (end) | 3V3 |

If the throttle reads backwards, swap pins 1 and 3 (or flip it in firmware).

## Power

| From | To |
|---|---|
| 12 V supply + | SW1, then TB_PWR + on both shields |
| 12 V supply − | TB_PWR − on both shields |
| PC USB | Blackpill USB-C (power and HID) |

The 12 V supply isn't in the BOM yet. Anything 12 V at 3 A or more works. The shields take 12 to 24 V, but the firmware assumes 12 V (`driver.voltage_power_supply = 12`).

SW1 is a switch on the 12 V line. The stick can push back hard, so keep it where you can reach it.

The Blackpill gets power from USB and nothing else. Don't connect a shield's VIN or 5V pin to it. If the regulator pad on a shield is bridged, VIN sits at 8 V and will cook the Blackpill.

All the grounds are tied together: the supply, both shields, the Blackpill, the encoders and the pot.

## Other notes

- Motor phase order doesn't matter. `initFOC()` figures out the direction and the electrical zero on startup.
- Current sense (A0 to A3 on the shields) isn't wired because the torque loop runs in voltage mode. If I add it later, the shield's ADC range pad has to be set to 3.3 V, since the Blackpill's analog pins can't take 5 V. PA1, PA2, PA3 and PB0 are free for it.
- Keep the SPI wires short, under about 20 cm, and twist each one with a ground.
- Pins I stayed away from: PA0 (KEY button), PA11/PA12 (USB), PA13/PA14 (SWD), PB2 (BOOT1) and PC13 (LED). PB11 doesn't exist on this chip.

## Firmware

The code in `firmware/` is still written for the old setup with two B-G431B-ESC1 boards. There, each board ran its own FOC loop and talked to the Blackpill over UART. The shields don't have a microcontroller, so the Blackpill has to run both motors itself, with two `BLDCDriver3PWM` and two `MagneticSensorSPI` on the pins above. That port hasn't been done yet.
