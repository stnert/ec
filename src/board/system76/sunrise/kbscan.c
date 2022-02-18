// SPDX-License-Identifier: GPL-3.0-only

#include <arch/delay.h>
#include <board/kbscan.h>
#include <common/debug.h>
#include <common/macro.h>
#include <ec/gpio.h>

#define KSI_IN GPIOIN30

Gpio KSO[12] = {
    GPIO(0x20),
    GPIO(0x21),
    GPIO(0x22),
    GPIO(0x23),
    GPIO(0x24),
    GPIO(0x25),
    GPIO(0x26),
    GPIO(0x27),
    GPIO(0x28),
    GPIO(0x29),
    GPIO(0x2A),
    GPIO(0x2B),
};

void kbscan_init(void) {
    for (int i = 0; i < ARRAY_SIZE(KSO); i++) {
        gpio_set_function(&KSO[i], false);
        gpio_set_output(&KSO[i], false);
        gpio_set(&KSO[i], false);
    }
}

void kbscan_event(void) {
    static uint8_t rows[12] = { 0 };

    for (int row_i = 0; row_i < ARRAY_SIZE(rows); row_i++) {
        gpio_set_output(&KSO[row_i], true);

        delay_ms(20);

        uint8_t row = ~KSI_IN;

        gpio_set_output(&KSO[row_i], false);

        if (row != rows[row_i]) {
            for (int col_i = 0; col_i < 8; col_i++) {
                bool old = rows[row_i] & BIT(col_i);
                bool new = row & BIT(col_i);
                if (old != new) {
                    if (new) {
                        DEBUG("%d, %d press\n", row_i, col_i);
                    } else {
                        DEBUG("%d, %d release\n", row_i, col_i);
                    }
                }
            }
            rows[row_i] = row;
        }
    }
}
