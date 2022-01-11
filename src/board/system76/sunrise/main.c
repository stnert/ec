// SPDX-License-Identifier: GPL-3.0-only

#include <stdint.h>
#include <stdio.h>

#include <arch/arch.h>
#include <common/debug.h>
#include <common/macro.h>
#include <common/version.h>
#include <ec/gpio.h>
#include <ec/sfr.h>
#include <ec/wdt.h>
//TODO #include <ec/ec.h>

// GPIO definitions
Gpio GPIO_KBC_PWR_ON = GPIO(0x7E);
Gpio GPIO_TXD = GPIO(0x16);

#define INTERRUPT_DUMMY(N) \
    void interrupt_ ## N (void) __interrupt(N) { \
        printf("INT" #N "\n"); \
    }

INTERRUPT_DUMMY(0) // 0x03 - External 0
void timer_0(void) __interrupt(1); // timer_0 is in time.c
INTERRUPT_DUMMY(2) // 0x13 - External 1
INTERRUPT_DUMMY(3) // 0x1B - Timer 1
INTERRUPT_DUMMY(4) // 0x23 - Serial
INTERRUPT_DUMMY(5) // Not used
INTERRUPT_DUMMY(6) // Not used
INTERRUPT_DUMMY(7) // Not used
INTERRUPT_DUMMY(8) // 0x43 - WDT / TMR0-2 / GWG
INTERRUPT_DUMMY(9) // 0x4B - EC ExtCMD / LPC Mailbox, OWM
INTERRUPT_DUMMY(10) // 0x53 - PS/2
INTERRUPT_DUMMY(11) // 0x5B - KBC
INTERRUPT_DUMMY(12) // 0x63 - IKB
INTERRUPT_DUMMY(13) // 0x6B - LPC Legacy I/O
INTERRUPT_DUMMY(14) // 0x73 - EC
INTERRUPT_DUMMY(15) // 0x7B - ESB / power lost
INTERRUPT_DUMMY(16) // 0x83 - FAN0 / eSPI
INTERRUPT_DUMMY(17) // 0x8B - FAN1 / I2CS0-1
INTERRUPT_DUMMY(18) // 0x93 - FSMBus0-5
INTERRUPT_DUMMY(19) // 0x9B - CIR / I2CD
INTERRUPT_DUMMY(20) // 0xA3 - GPT0
INTERRUPT_DUMMY(21) // 0xAB - GPT1
INTERRUPT_DUMMY(22) // 0xB3 - GPT2
INTERRUPT_DUMMY(23) // 0xBB - GPT3
INTERRUPT_DUMMY(24) // 0xC3 - LPC Extended I/O / PECI
INTERRUPT_DUMMY(25) // 0xCB - GPIO0x
INTERRUPT_DUMMY(26) // 0xD3 - GPIO1x
INTERRUPT_DUMMY(27) // 0xDB - GPIO2x
INTERRUPT_DUMMY(28) // 0xE3 - GPIO3x
INTERRUPT_DUMMY(29) // 0xEB - GPIO4x / GPIO6x
INTERRUPT_DUMMY(30) // 0xF3 - GPIO5x / GPIO7x
INTERRUPT_DUMMY(31) // 0xFB - ADC

uint8_t main_cycle = 0;

void wdt_init(void) {
    // Disable WDT
    WDTCFG = 0xC8;
    WDTPF = BIT(1) | BIT(0);
    WDTM0902 = 0;
}

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
    wdt_init();
    arch_init();

    //TODO ec_init();

    // Can happen in any order
    //TODO kbc_init();
    serial_init();

    // Must happen last
    //board_init();
}

#define DUMP(X) printf(#X " = %02X\n", X)

volatile uint8_t __xdata __at(0x1428) PFCSR;

volatile uint8_t __xdata __at(0x1444) PB8SCR;

volatile uint8_t __xdata __at(0x2800) PA0REG;
volatile uint8_t __xdata __at(0x2801) PA1REG;
volatile uint8_t __xdata __at(0x2802) PA2REG;

void main(void) {
    init();

    printf("\nSystem76 EC\n");

    for (main_cycle = 0; ; main_cycle++) {
        printf("CYCLE %d\n", main_cycle);
    }
}
