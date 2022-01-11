// SPDX-License-Identifier: GPL-3.0-only

#include <arch/arch.h>
#include <common/debug.h>
#include <common/version.h>
#include <ec/gpio.h>
#include <ec/sfr.h>
#include <ec/wdt.h>

int8_t main_cycle = 0;

// GPIO definitions
Gpio GPIO_KBC_PWR_ON = GPIO(0x7E);
Gpio GPIO_TXD = GPIO(0x16);

void serial_init(void) {
    // Set serial mode to 1, disable recieve function
    SCON = 0x40;

    // Set baud to 131072 Hz using timer 1 in 8-bit reload mode
    TMOD |= 0x20;
    TH1 = 0xF7;
    TR1 = 1;

    // Enable GPIO16 alt mode (TXD)
    gpio_set_function(&GPIO_TXD, true);

    // Enable GPIO16 output
    gpio_set_output(&GPIO_TXD, true);
}

void board_init(void) {
    // Set power enable GPIO
    gpio_set(&GPIO_KBC_PWR_ON, true);
    gpio_set_output(&GPIO_KBC_PWR_ON, true);
}

void init(void) {
    // Must happen first
    wdt_disable();
    arch_init();
    //TODO ec_init();

    // Can happen in any order
    //TODO kbc_init();
    serial_init();

    // Must happen last
    //board_init();
}

void main(void) {
    init();

    INFO("\nSystem76 EC board '%s', version '%s'\n", board(), version());

    for (main_cycle = 0; ; main_cycle++) {
        DEBUG("CYCLE %d\n", main_cycle);
    }
}
