
// SPDX-License-Identifier: GPL-3.0-only

#include <stdint.h>
#include <stdio.h>

#include <arch/arch.h>
#include <common/debug.h>
#include <common/macro.h>
#include <common/version.h>
#include <ec/gpio.h>
#include <ec/sfr.h>
//TODO #include <ec/ec.h>

void external_0(void) __interrupt(0) {}
// timer_0 is in time.c
void timer_0(void) __interrupt(1);
void external_1(void) __interrupt(2) {}
void timer_1(void) __interrupt(3) {}
void serial(void) __interrupt(4) {}
void timer_2(void) __interrupt(5) {}

uint8_t main_cycle = 0;

void serial_init(void) {
    // Set serial mode to 1, disable recieve function
    SCON = 0x40;

    // Set baud to 9600 using timer 1 in 8-bit reload mode
    TMOD |= 0x20;
    TH1 = 0xF7;
    TR1 = 1;

    // Enable GPIO16 alt mode (TXD)
    GPIOFS10 |= BIT(6);

    // Enable GPIO16 output
    GPIOOE10 |= BIT(6);
}

void init(void) {
    // Must happen first
    arch_init();
    //TODO ec_init();

    // Can happen in any order
    //TODO kbc_init();

    // Must happen last
    //TODO board_init();
}

void main(void) {
    //TODO init();

    // Disable interrupts
    EA = 0;

    //serial_init();

    //printf("\nSystem76 EC\n");

    GPIOOE10 |= BIT(6);

    for (main_cycle = 0; ; main_cycle++) {
        //putchar('\n');

        if (main_cycle == 0) {
            GPIOD10 |= BIT(6);
        }

        if (main_cycle == 128) {
            GPIOD10 &= ~BIT(6);
        }
    }
}
