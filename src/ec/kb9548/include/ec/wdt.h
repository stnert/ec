// SPDX-License-Identifier: GPL-3.0-only

#ifndef _EC_WDT_H
#define _EC_WDT_H

#include <stdint.h>

void wdt_disable(void);

volatile uint8_t __xdata __at(0x0E00) WDTCFG;

volatile uint8_t __xdata __at(0x0E01) WDTPF;

volatile uint8_t __xdata __at(0x0E02) WDTM0902;

volatile uint8_t __xdata __at(0x0E0A) CLK32CR;

volatile uint8_t __xdata __at(0x0E11) WDTCMR;

#endif // _EC_WDT_H
