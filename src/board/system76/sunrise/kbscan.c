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
    static uint8_t outputs[12] = { 0 };

    for (int output_i = 0; output_i < ARRAY_SIZE(outputs); output_i++) {
        gpio_set_output(&KSO[output_i], true);

        delay_ms(20);

        uint8_t inputs = ~KSI_IN;

        gpio_set_output(&KSO[output_i], false);

        if (inputs != outputs[output_i]) {
            for (int input_i = 0; input_i < 8; input_i++) {
                bool old = outputs[output_i] & BIT(input_i);
                bool new = inputs & BIT(input_i);
                if (old != new) {
                    uint16_t key = KEYMAP[0][output_i][input_i];
                    if (new) {
                        DEBUG("%d, %d press 0x%x\n", output_i, input_i, key);
                    } else {
                        DEBUG("%d, %d release 0x%x\n", output_i, input_i, key);
                    }
                }
            }
            outputs[output_i] = inputs;
        }
    }
}
