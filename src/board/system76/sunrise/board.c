// SPDX-License-Identifier: GPL-3.0-only

#include <stdint.h>

#include <board/board.h>
#include <common/debug.h>
#include <common/macro.h>

volatile uint8_t __xdata __at(0x0F15) LPCCFG;
volatile uint8_t __xdata __at(0x0F55) LPCCFG3;
volatile uint8_t __xdata __at(0x140E) EXTIOW;
volatile uint8_t __xdata __at(0x1439) ECPF;
volatile uint8_t __xdata __at(0x143A) EXTIO1W;

void board_init(void) {
    // Enable port 0x80
    LPCCFG |= BIT(4);
    // Enable port 0x81
    LPCCFG3 |= BIT(0);
}

void board_event(void) {
    // While port 0x80 or 0x81 have data
    while (ECPF & (BIT(0) | BIT(1))) {
        // Print port 0x80 and 0x81 data
        DEBUG("POST %02X%02X\n", EXTIO1W, EXTIOW);
        // Clear port 0x80 and 0x81 pending data flags
        ECPF = BIT(0) | BIT(1);
    }
}
