#pragma once

// !!! PLACEHOLDER PINOUT -- DO NOT TRUST BEFORE CHECKING !!!
//
// These have not been confirmed against ST's B-G431B-ESC1 schematic
// (UM2516) or a verified SimpleFOC community port for this board. Cross-
// check every pin below before flashing: this drives a 6-PWM three-phase
// bridge, and a wrong pin can short the DC bus or destroy the FETs.
//
// Sources to confirm against before trusting this file:
//   - ST UM2516 (B-G431B-ESC1 user manual) pin-mapping table
//   - SimpleFOC's own B-G431B-ESC1 example/port (search "B-G431B-ESC1" in
//     github.com/simplefoc/Arduino-FOC issues/examples)

#define PIN_PWM_UH PA8
#define PIN_PWM_VH PA9
#define PIN_PWM_WH PA10
#define PIN_PWM_UL PB13
#define PIN_PWM_VL PB14
#define PIN_PWM_WL PB15
#define PIN_ENABLE PB6   // gate driver enable
#define PIN_FAULT PB10   // gate driver fault flag -- confirm polarity (active low on most designs)

// Reserved for a future current-mode FOC upgrade. Not read anywhere yet.
#define PIN_CURR_U PB1
#define PIN_CURR_V PB0
#define PIN_CURR_W PC5

#define PIN_ENCODER_CS PA4  // AS5047P SPI chip-select for this motor's encoder

#define UART_RX PA3  // link to stick_controller
#define UART_TX PA2
#define UART_BAUD 921600
