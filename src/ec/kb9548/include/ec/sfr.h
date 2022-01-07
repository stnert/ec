// SPDX-License-Identifier: GPL-3.0-only

#ifndef _EC_SFR_H
#define _EC_SFR_H

#include <8051.h>

// Serial port control register 2 - high byte of 16-bit counter for baud rate,
// based on 16 MHz clock
__sfr __at (0x9A) SCON2;

// Serial port control register 3 - low byte of 16-bit counter for baud rate,
// based on 16 MHz clock
__sfr __at (0x9B) SCON3;

// Serial port control register 4 - serial port mode 0 baud-rate setting
__sfr __at (0x9C) SCON4;
#define SCON4_8_MHZ 0b00
#define SCON4_4_MHZ 0b01
#define SCON4_2_MHZ 0b10
#define SCON4_1_MHZ 0b11

#endif // _EC_SFR_H
