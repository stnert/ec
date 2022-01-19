// SPDX-License-Identifier: GPL-3.0-only

#include <board/board.h>
#include <ec/gpio.h>

Gpio FCH_PWRGD = GPIO(0x0D);
void board_init(void) {
    //TODO: figure out how the vendor EC firmware handles this
    gpio_set_output(&FCH_PWRGD, false);
}

void board_event(void) {}
