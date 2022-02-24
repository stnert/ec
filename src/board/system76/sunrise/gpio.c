// SPDX-License-Identifier: GPL-3.0-only

#include <board/gpio.h>

//TODO: use ADP_DET (0x15) instead?
Gpio BARREL_ADP_DET_N = GPIO(0x79);
Gpio BAT_AMBERLED = GPIO(0x19);
Gpio BAT_WHITELED = GPIO(0x0F);
Gpio FCH_PWRGD = GPIO(0x0D);
Gpio KBC_PWR_ON = GPIO(0x7E);
Gpio LED_PWRSTBY = GPIO(0x54);
Gpio LID_SW_N = GPIO(0x7C);
Gpio LPC_RST_N = GPIO(0x05);
Gpio PWRBTN_N = GPIO(0x7B);
Gpio PWR_BTN_OUT_N = GPIO(0x6B);
Gpio RSMRST_N_KBC = GPIO(0x56);
Gpio SLP_S3_N = GPIO(0x52);
Gpio SLP_S5_N = GPIO(0x48);

void gpio_init(void) {
    //TODO: AOODC = BIT(2) | BIT(1) | BIT(0);

    // Set GPIO drive
    GPIODC00 = 0;
    GPIODC08 = BIT(7);
    GPIODC10 = BIT(0);
    GPIODC18 = BIT(2) | BIT(1);
    GPIODC20 = 0;
    GPIODC28 = 0;
    GPIODC30 = 0;
    GPIODC38 = 0;
    GPIODC40 = 0;
    GPIODC48 = 0;
    GPIODC50 = BIT(4) | BIT(3);
    GPIODC58 = 0;
    GPIODC60 = 0;
    GPIODC68 = 0;
    GPIODC70 = 0;
    GPIODC78 = 0;

    // Set GPIO function
    GPIOFS00 = 0;
    GPIOFS08 = BIT(6);
    GPIOFS10 = BIT(6) | BIT(2) | BIT(1) | BIT(0);
    GPIOFS18 = BIT(5);
    GPIOFS20 = BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0);
    GPIOFS28 = BIT(3) | BIT(2) | BIT(1) | BIT(0);
    GPIOFS30 = 0;
    GPIOFS38 = 0;
    GPIOFS40 = BIT(7) | BIT(6) | BIT(5) | BIT(4);
    GPIOFS48 = BIT(5) | BIT(4) | BIT(3) | BIT(2);
    GPIOFS50 = 0;
    GPIOFS58 = 0;
    GPIOFS60 = 0;
    GPIOFS68 = 0;
    GPIOFS70 = 0;
    GPIOFS78 = 0;

    // Set GPIO open-drain
    GPIOOD00 = 0;
    GPIOOD08 = 0;
    GPIOOD10 = 0;
    GPIOOD18 = 0;
    GPIOOD20 = 0;
    GPIOOD28 = 0;
    GPIOOD30 = 0;
    GPIOOD38 = 0;
    GPIOOD40 = 0;
    GPIOOD48 = 0;
    GPIOOD50 = 0;
    GPIOOD58 = 0;
    GPIOOD60 = 0;
    GPIOOD68 = 0;
    GPIOOD70 = 0;
    GPIOOD78 = 0;

    // Set GPIO data
    GPIOD00 = 0;
    GPIOD08 = BIT(4) | BIT(2);
    GPIOD10 = 0;
    GPIOD18 = 0;
    GPIOD20 = 0;
    GPIOD28 = 0;
    GPIOD30 = 0;
    GPIOD38 = 0;
    GPIOD40 = 0;
    GPIOD48 = BIT(6);
    GPIOD50 = 0;
    GPIOD58 = BIT(6);
    GPIOD60 = BIT(7);
    GPIOD68 = 0;
    GPIOD70 = 0;
    GPIOD78 = BIT(7);

    // Set GPIO output enable
    GPIOOE00 = 0;
    GPIOOE08 = BIT(7) | BIT(5) | BIT(4) | BIT(2) | BIT(0);
    GPIOOE10 = BIT(3) | BIT(1);
    GPIOOE18 = BIT(2) | BIT(1);
    GPIOOE20 = 0;
    GPIOOE28 = BIT(6) | BIT(5) | BIT(4);
    GPIOOE30 = 0;
    GPIOOE38 = BIT(6) | BIT(4);
    GPIOOE40 = BIT(3);
    GPIOOE48 = BIT(6);
    GPIOOE50 = BIT(6) | BIT(4) | BIT(3);
    GPIOOE58 = BIT(6) | BIT(5);
    GPIOOE60 = BIT(7);
    GPIOOE68 = BIT(3) | BIT(0);
    GPIOOE70 = 0;
    GPIOOE78 = BIT(7) | BIT(6) | BIT(5);

    // Set GPIO pull-up
    GPIOPU00 = 0;
    GPIOPU08 = 0;
    GPIOPU10 = 0;
    GPIOPU18 = 0;
    GPIOPU20 = 0;
    GPIOPU28 = 0;
    GPIOPU30 = BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0);
    GPIOPU38 = 0;
    GPIOPU40 = 0;
    GPIOPU48 = 0;
    GPIOPU50 = 0;
    GPIOPU58 = 0;
    GPIOPU60 = 0;
    GPIOPU68 = 0;
    GPIOPU70 = 0;
    GPIOPU78 = 0;

    // Set GPIO low voltage mode
    GPIOLV00 = 0;
    GPIOLV08 = 0;
    GPIOLV10 = 0;
    GPIOLV18 = 0;
    GPIOLV20 = 0;
    GPIOLV28 = 0;
    GPIOLV30 = 0;
    GPIOLV38 = 0;
    GPIOLV40 = 0;
    GPIOLV48 = 0;
    GPIOLV50 = 0;
    GPIOLV58 = 0;
    GPIOLV60 = 0;
    GPIOLV68 = 0;
    GPIOLV70 = 0;
    GPIOLV78 = 0;

    // Set GPIO input enable
    GPIOIE00 = BIT(5);
    GPIOIE08 = BIT(3);
    GPIOIE10 = BIT(7) | BIT(5) | BIT(4);
    GPIOIE18 = BIT(5);
    GPIOIE20 = 0;
    GPIOIE28 = BIT(7);
    GPIOIE30 = BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0);
    GPIOIE38 = BIT(3) | BIT(2) | BIT(1) | BIT(0);
    GPIOIE40 = BIT(7) | BIT(6) | BIT(5) | BIT(4);
    GPIOIE48 = BIT(7) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(0);
    GPIOIE50 = BIT(7) | BIT(2) | BIT(0);
    GPIOIE58 = 0;
    GPIOIE60 = BIT(3);
    GPIOIE68 = BIT(1);
    GPIOIE70 = 0;
    GPIOIE78 = BIT(5) | BIT(4) | BIT(3) | BIT(1);
}
