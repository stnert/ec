// SPDX-License-Identifier: GPL-3.0-only

#ifndef _EC_FAN_H
#define _EC_FAN_H

#include <stdint.h>

// Fan configuration
volatile uint8_t __xdata __at(0x0A00) FANCFG0;
volatile uint8_t __xdata __at(0x0B00) FANCFG1;

// Fan control and status
volatile uint8_t __xdata __at(0x0A01) FANSTS0;
volatile uint8_t __xdata __at(0x0B01) FANSTS1;

// Fan tachometer current speed
volatile uint8_t __xdata __at(0x0A02) FANMONH0;
volatile uint8_t __xdata __at(0x0A03) FANMONL0;
volatile uint8_t __xdata __at(0x0B02) FANMONH1;
volatile uint8_t __xdata __at(0x0B03) FANMONL1;

// Fan automatic speed
volatile uint8_t __xdata __at(0x0A04) FANSETH0;
volatile uint8_t __xdata __at(0x0A05) FANSETL0;
volatile uint8_t __xdata __at(0x0B04) FANSETH1;
volatile uint8_t __xdata __at(0x0B05) FANSETL1;

// Fan high period length
volatile uint8_t __xdata __at(0x0A06) FANPWMH0;
volatile uint8_t __xdata __at(0x0A07) FANPWML0;
volatile uint8_t __xdata __at(0x0B06) FANPWMH1;
volatile uint8_t __xdata __at(0x0B07) FANPWML1;

// Fan current high period length
volatile uint8_t __xdata __at(0x0A08) FANCPWMH0;
volatile uint8_t __xdata __at(0x0A09) FANCPWML0;
volatile uint8_t __xdata __at(0x0B08) FANCPWMH1;
volatile uint8_t __xdata __at(0x0B09) FANCPWML1;

// Fan cycle length
volatile uint8_t __xdata __at(0x0A0A) FANPWMCH0;
volatile uint8_t __xdata __at(0x0A0B) FANPWMCL0;
volatile uint8_t __xdata __at(0x0B0A) FANPWMCH1;
volatile uint8_t __xdata __at(0x0B0B) FANPWMCL1;

#endif // _EC_FAN_H
