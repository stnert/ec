// SPDX-License-Identifier: GPL-3.0-only

#ifndef _BOARD_GPIO_H
#define _BOARD_GPIO_H

#include <ec/gpio.h>

extern Gpio BARREL_ADP_DET_N;
extern Gpio BAT_AMBERLED;
extern Gpio BAT_WHITELED;
extern Gpio FCH_PWRGD;
extern Gpio KBC_PWR_ON;
extern Gpio LED_PWRSTBY;
extern Gpio LID_SW_N;
extern Gpio LPC_RST_N;
extern Gpio PWRBTN_N;
extern Gpio PWR_BTN_OUT_N;
extern Gpio RSMRST_N_KBC;
extern Gpio SLP_S3_N;
extern Gpio SLP_S5_N;

void gpio_init(void);

#endif // _BOARD_GPIO_H
