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
bool gpio_is_set(Gpio * gpio) __reentrant;
void gpio_set(Gpio * gpio, bool value) __reentrant;

void gpio_set_function(Gpio * gpio, bool value) __reentrant;
void gpio_set_output(Gpio * gpio, bool value) __reentrant;

// GPIO function select
volatile uint8_t __xdata __at(0x0000) GPIOFS[16];
volatile uint8_t __xdata __at(0x0000) GPIOFS00;
volatile uint8_t __xdata __at(0x0001) GPIOFS08;
volatile uint8_t __xdata __at(0x0002) GPIOFS10;
volatile uint8_t __xdata __at(0x0003) GPIOFS18;
volatile uint8_t __xdata __at(0x0004) GPIOFS20;
volatile uint8_t __xdata __at(0x0005) GPIOFS28;
volatile uint8_t __xdata __at(0x0006) GPIOFS30;
volatile uint8_t __xdata __at(0x0007) GPIOFS38;
volatile uint8_t __xdata __at(0x0008) GPIOFS40;
volatile uint8_t __xdata __at(0x0009) GPIOFS48;
volatile uint8_t __xdata __at(0x000A) GPIOFS50;
volatile uint8_t __xdata __at(0x000B) GPIOFS58;
volatile uint8_t __xdata __at(0x000C) GPIOFS60;
volatile uint8_t __xdata __at(0x000D) GPIOFS68;
volatile uint8_t __xdata __at(0x000E) GPIOFS70;
volatile uint8_t __xdata __at(0x000F) GPIOFS78;

// GPIO output enable
volatile uint8_t __xdata __at(0x0010) GPIOOE[16];
volatile uint8_t __xdata __at(0x0010) GPIOOE00;
volatile uint8_t __xdata __at(0x0011) GPIOOE08;
volatile uint8_t __xdata __at(0x0012) GPIOOE10;
volatile uint8_t __xdata __at(0x0013) GPIOOE18;
volatile uint8_t __xdata __at(0x0014) GPIOOE20;
volatile uint8_t __xdata __at(0x0015) GPIOOE28;
volatile uint8_t __xdata __at(0x0016) GPIOOE30;
volatile uint8_t __xdata __at(0x0017) GPIOOE38;
volatile uint8_t __xdata __at(0x0018) GPIOOE40;
volatile uint8_t __xdata __at(0x0019) GPIOOE48;
volatile uint8_t __xdata __at(0x001A) GPIOOE50;
volatile uint8_t __xdata __at(0x001B) GPIOOE58;
volatile uint8_t __xdata __at(0x001C) GPIOOE60;
volatile uint8_t __xdata __at(0x001D) GPIOOE68;
volatile uint8_t __xdata __at(0x001E) GPIOOE70;
volatile uint8_t __xdata __at(0x001F) GPIOOE78;

// GPIO output data
volatile uint8_t __xdata __at(0x0020) GPIOD[16];
volatile uint8_t __xdata __at(0x0020) GPIOD00;
volatile uint8_t __xdata __at(0x0021) GPIOD08;
volatile uint8_t __xdata __at(0x0022) GPIOD10;
volatile uint8_t __xdata __at(0x0023) GPIOD18;
volatile uint8_t __xdata __at(0x0024) GPIOD20;
volatile uint8_t __xdata __at(0x0025) GPIOD28;
volatile uint8_t __xdata __at(0x0026) GPIOD30;
volatile uint8_t __xdata __at(0x0027) GPIOD38;
volatile uint8_t __xdata __at(0x0028) GPIOD40;
volatile uint8_t __xdata __at(0x0029) GPIOD48;
volatile uint8_t __xdata __at(0x002A) GPIOD50;
volatile uint8_t __xdata __at(0x002B) GPIOD58;
volatile uint8_t __xdata __at(0x002C) GPIOD60;
volatile uint8_t __xdata __at(0x002D) GPIOD68;
volatile uint8_t __xdata __at(0x002E) GPIOD70;
volatile uint8_t __xdata __at(0x002F) GPIOD78;

// GPIO input data
volatile uint8_t __xdata __at(0x0030) GPIOIN[16];
volatile uint8_t __xdata __at(0x0030) GPIOIN00;
volatile uint8_t __xdata __at(0x0031) GPIOIN08;
volatile uint8_t __xdata __at(0x0032) GPIOIN10;
volatile uint8_t __xdata __at(0x0033) GPIOIN18;
volatile uint8_t __xdata __at(0x0034) GPIOIN20;
volatile uint8_t __xdata __at(0x0035) GPIOIN28;
volatile uint8_t __xdata __at(0x0036) GPIOIN30;
volatile uint8_t __xdata __at(0x0037) GPIOIN38;
volatile uint8_t __xdata __at(0x0038) GPIOIN40;
volatile uint8_t __xdata __at(0x0039) GPIOIN48;
volatile uint8_t __xdata __at(0x003A) GPIOIN50;
volatile uint8_t __xdata __at(0x003B) GPIOIN58;
volatile uint8_t __xdata __at(0x003C) GPIOIN60;
volatile uint8_t __xdata __at(0x003D) GPIOIN68;
volatile uint8_t __xdata __at(0x003E) GPIOIN70;
volatile uint8_t __xdata __at(0x003F) GPIOIN78;

// GPIO pull-up
volatile uint8_t __xdata __at(0x0040) GPIOPU[16];
volatile uint8_t __xdata __at(0x0040) GPIOPU00;
volatile uint8_t __xdata __at(0x0041) GPIOPU08;
volatile uint8_t __xdata __at(0x0042) GPIOPU10;
volatile uint8_t __xdata __at(0x0043) GPIOPU18;
volatile uint8_t __xdata __at(0x0044) GPIOPU20;
volatile uint8_t __xdata __at(0x0045) GPIOPU28;
volatile uint8_t __xdata __at(0x0046) GPIOPU30;
volatile uint8_t __xdata __at(0x0047) GPIOPU38;
volatile uint8_t __xdata __at(0x0048) GPIOPU40;
volatile uint8_t __xdata __at(0x0049) GPIOPU48;
volatile uint8_t __xdata __at(0x004A) GPIOPU50;
volatile uint8_t __xdata __at(0x004B) GPIOPU58;
volatile uint8_t __xdata __at(0x004C) GPIOPU60;
volatile uint8_t __xdata __at(0x004D) GPIOPU68;
volatile uint8_t __xdata __at(0x004E) GPIOPU70;
volatile uint8_t __xdata __at(0x004F) GPIOPU78;

// GPIO open-drain
volatile uint8_t __xdata __at(0x0050) GPIOOD[16];
volatile uint8_t __xdata __at(0x0050) GPIOOD00;
volatile uint8_t __xdata __at(0x0051) GPIOOD08;
volatile uint8_t __xdata __at(0x0052) GPIOOD10;
volatile uint8_t __xdata __at(0x0053) GPIOOD18;
volatile uint8_t __xdata __at(0x0054) GPIOOD20;
volatile uint8_t __xdata __at(0x0055) GPIOOD28;
volatile uint8_t __xdata __at(0x0056) GPIOOD30;
volatile uint8_t __xdata __at(0x0057) GPIOOD38;
volatile uint8_t __xdata __at(0x0058) GPIOOD40;
volatile uint8_t __xdata __at(0x0059) GPIOOD48;
volatile uint8_t __xdata __at(0x005A) GPIOOD50;
volatile uint8_t __xdata __at(0x005B) GPIOOD58;
volatile uint8_t __xdata __at(0x005C) GPIOOD60;
volatile uint8_t __xdata __at(0x005D) GPIOOD68;
volatile uint8_t __xdata __at(0x005E) GPIOOD70;
volatile uint8_t __xdata __at(0x005F) GPIOOD78;

// GPIO input enable
volatile uint8_t __xdata __at(0x0060) GPIOIE[16];
volatile uint8_t __xdata __at(0x0060) GPIOIE00;
volatile uint8_t __xdata __at(0x0061) GPIOIE08;
volatile uint8_t __xdata __at(0x0062) GPIOIE10;
volatile uint8_t __xdata __at(0x0063) GPIOIE18;
volatile uint8_t __xdata __at(0x0064) GPIOIE20;
volatile uint8_t __xdata __at(0x0065) GPIOIE28;
volatile uint8_t __xdata __at(0x0066) GPIOIE30;
volatile uint8_t __xdata __at(0x0067) GPIOIE38;
volatile uint8_t __xdata __at(0x0068) GPIOIE40;
volatile uint8_t __xdata __at(0x0069) GPIOIE48;
volatile uint8_t __xdata __at(0x006A) GPIOIE50;
volatile uint8_t __xdata __at(0x006B) GPIOIE58;
volatile uint8_t __xdata __at(0x006C) GPIOIE60;
volatile uint8_t __xdata __at(0x006D) GPIOIE68;
volatile uint8_t __xdata __at(0x006E) GPIOIE70;
volatile uint8_t __xdata __at(0x006F) GPIOIE78;

// GPIO low voltage mode
volatile uint8_t __xdata __at(0x0080) GPIOLV[16];
volatile uint8_t __xdata __at(0x0080) GPIOLV00;
volatile uint8_t __xdata __at(0x0081) GPIOLV08;
volatile uint8_t __xdata __at(0x0082) GPIOLV10;
volatile uint8_t __xdata __at(0x0083) GPIOLV18;
volatile uint8_t __xdata __at(0x0084) GPIOLV20;
volatile uint8_t __xdata __at(0x0085) GPIOLV28;
volatile uint8_t __xdata __at(0x0086) GPIOLV30;
volatile uint8_t __xdata __at(0x0087) GPIOLV38;
volatile uint8_t __xdata __at(0x0088) GPIOLV40;
volatile uint8_t __xdata __at(0x0089) GPIOLV48;
volatile uint8_t __xdata __at(0x008A) GPIOLV50;
volatile uint8_t __xdata __at(0x008B) GPIOLV58;
volatile uint8_t __xdata __at(0x008C) GPIOLV60;
volatile uint8_t __xdata __at(0x008D) GPIOLV68;
volatile uint8_t __xdata __at(0x008E) GPIOLV70;
volatile uint8_t __xdata __at(0x008F) GPIOLV78;

// GPIO driving control
volatile uint8_t __xdata __at(0x0090) GPIODC[16];
volatile uint8_t __xdata __at(0x0090) GPIODC00;
volatile uint8_t __xdata __at(0x0091) GPIODC08;
volatile uint8_t __xdata __at(0x0092) GPIODC10;
volatile uint8_t __xdata __at(0x0093) GPIODC18;
volatile uint8_t __xdata __at(0x0094) GPIODC20;
volatile uint8_t __xdata __at(0x0095) GPIODC28;
volatile uint8_t __xdata __at(0x0096) GPIODC30;
volatile uint8_t __xdata __at(0x0097) GPIODC38;
volatile uint8_t __xdata __at(0x0098) GPIODC40;
volatile uint8_t __xdata __at(0x0099) GPIODC48;
volatile uint8_t __xdata __at(0x009A) GPIODC50;
volatile uint8_t __xdata __at(0x009B) GPIODC58;
volatile uint8_t __xdata __at(0x009C) GPIODC60;
volatile uint8_t __xdata __at(0x009D) GPIODC68;
volatile uint8_t __xdata __at(0x009E) GPIODC70;
volatile uint8_t __xdata __at(0x009F) GPIODC78;

#endif // _EC_GPIO_H
