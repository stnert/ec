// SPDX-License-Identifier: GPL-3.0-only

#ifndef _EC_GPIO_H
#define _EC_GPIO_H

#include <common/macro.h>

#include <stdbool.h>
#include <stdint.h>

typedef const struct _Gpio {
    uint8_t index;
    uint8_t value;
} __code Gpio;

#define GPIO(NUMBER) { \
    .index = (NUMBER >> 3) & 0xF, \
    .value = BIT(NUMBER & 0x7), \
}

bool gpio_get(Gpio * gpio) __reentrant;
void gpio_set(Gpio * gpio, bool value) __reentrant;

void gpio_set_function(Gpio * gpio, bool value) __reentrant;
void gpio_set_output(Gpio * gpio, bool value) __reentrant;

volatile uint8_t __xdata __at(0x0000) GPIOFS[16];

volatile uint8_t __xdata __at(0x0010) GPIOOE[16];

volatile uint8_t __xdata __at(0x0020) GPIOD[16];

volatile uint8_t __xdata __at(0x0030) GPIOIN[16];

volatile uint8_t __xdata __at(0x007C) GPIORPSC;

#endif // _EC_GPIO_H
