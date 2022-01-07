// SPDX-License-Identifier: GPL-3.0-only

#ifndef _EC_GPIO_H
#define _EC_GPIO_H

volatile uint8_t __xdata __at(0xFC02) GPIOFS10;

volatile uint8_t __xdata __at(0xFC12) GPIOOE10;

volatile uint8_t __xdata __at(0xFC22) GPIOD10;

#endif // _EC_GPIO_H
