// SPDX-License-Identifier: GPL-3.0-only

#include <board/fan.h>
#include <board/peci.h>
#include <board/power.h>
#include <common/debug.h>
#include <common/macro.h>
#include <ec/i2c.h>

// Fan speed is the lowest requested over HEATUP seconds
#ifndef BOARD_HEATUP
    #define BOARD_HEATUP 4
#endif

static uint8_t FAN_HEATUP[BOARD_HEATUP] = { 0 };

// Fan speed is the highest HEATUP speed over COOLDOWN seconds
#ifndef BOARD_COOLDOWN
    #define BOARD_COOLDOWN 10
#endif

static uint8_t FAN_COOLDOWN[BOARD_COOLDOWN] = { 0 };

// Tjunction = 100C for i7-8565U (and probably the same for all WHL-U)
#define T_JUNCTION 100

bool peci_on = false;
int16_t peci_temp = 0;

#define PECI_TEMP(X) ((int16_t)(X))

#define FAN_POINT(T, D) { .temp = PECI_TEMP(T), .duty = PWM_DUTY(D) }

// Fan curve with temperature in degrees C, duty cycle in percent
static struct FanPoint __code FAN_POINTS[] = {
#ifdef BOARD_FAN_POINTS
    BOARD_FAN_POINTS
#else
    FAN_POINT(70, 40),
    FAN_POINT(75, 50),
    FAN_POINT(80, 60),
    FAN_POINT(85, 65),
    FAN_POINT(90, 65)
#endif
};

static struct Fan __code FAN = {
    .points = FAN_POINTS,
    .points_size = ARRAY_SIZE(FAN_POINTS),
    .heatup = FAN_HEATUP,
    .heatup_size = ARRAY_SIZE(FAN_HEATUP),
    .cooldown = FAN_COOLDOWN,
    .cooldown_size = ARRAY_SIZE(FAN_COOLDOWN),
    .interpolate = SMOOTH_FANS != 0,
};

void peci_init(void) {
    // Enable SMBUS controller
    *(I2C_CPU.cfg) = BIT(5) | BIT(0);

    // Set frequency to 100 kHz
    *(I2C_CPU.ofh) = 0x09;
    *(I2C_CPU.ofl) = 0x09;
}

// This uses SB-TSI, in AMD document 40821
uint8_t peci_get_fan_duty(void) {
    uint8_t duty;

    peci_on = power_state == POWER_STATE_S0;

    if (peci_on) {
        // Use SB-TSI if in S0 state
        // We only read the CPU temp high value, since a resolution of 1C is
        // good enough
        int8_t cpu_temp_high;
        int res = i2c_get(&I2C_CPU, 0x4C, 0x01, &cpu_temp_high, 1);
        if (res == 1) {
            // Convert from AMD format
            peci_temp = PECI_TEMP(cpu_temp_high);
            duty = fan_duty(&FAN, peci_temp);
        } else {
            DEBUG("SB-TSI temp error: %d\n", res);
            // Default to 50% if there is an error
            peci_temp = 0;
            duty = PWM_DUTY(50);
        }
    } else {
        // Turn fan off if not in S0 state
        peci_temp = 0;
        duty = PWM_DUTY(0);
    }

    if (peci_on && fan_max) {
        // Override duty if fans are manually set to maximum
        duty = PWM_DUTY(100);
    } else {
        // Apply heatup and cooldown filters to duty
        duty = fan_heatup(&FAN, duty);
        duty = fan_cooldown(&FAN, duty);
    }

    TRACE("SB-TSI temp=%d\n", peci_temp);
    return duty;
}
