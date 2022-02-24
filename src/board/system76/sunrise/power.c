// SPDX-License-Identifier: GPL-3.0-only

#include <arch/delay.h>
#include <arch/time.h>
#include <board/battery.h>
#include <board/board.h>
#include <board/fan.h>
#include <board/gpio.h>
#include <board/lid.h>
#include <board/peci.h>
#include <board/power.h>
#include <common/debug.h>

#define GPIO_SET_DEBUG(G, V) { \
    DEBUG("%s = %s\n", #G, V ? "true" : "false"); \
    gpio_set(&G, V); \
}

extern uint8_t main_cycle;

enum PowerState power_state = POWER_STATE_DEFAULT;

enum PowerState calculate_power_state(void) {
    //TODO: Deep Sx states using SLP_SUS#

    if (gpio_get(&SLP_S3_N)) {
        // S3, S4, and S5 planes powered
        return POWER_STATE_S0;
    }

    if (gpio_get(&SLP_S5_N)) {
        // S4 and S5 planes powered
        return POWER_STATE_S3;
    }

    if (gpio_is_set(&RSMRST_N_KBC)) {
        // S5 plane powered
        return POWER_STATE_S5;
    }

    return POWER_STATE_DS5;
}

void update_power_state(void) {
    enum PowerState new_power_state = calculate_power_state();
    if (power_state != new_power_state) {
        power_state = new_power_state;

    #if LEVEL >= LEVEL_DEBUG
        switch (power_state) {
            case POWER_STATE_DEFAULT:
                DEBUG("POWER_STATE_DEFAULT\n");
                break;
            case POWER_STATE_DS5:
                DEBUG("POWER_STATE_DS5\n");
                break;
            case POWER_STATE_S5:
                DEBUG("POWER_STATE_S5\n");
                break;
            case POWER_STATE_DS3:
                DEBUG("POWER_STATE_DS3\n");
                break;
            case POWER_STATE_S3:
                DEBUG("POWER_STATE_S3\n");
                break;
            case POWER_STATE_S0:
                DEBUG("POWER_STATE_S0\n");
                break;
        }
    #endif
    }
}

// Enable deep sleep well power
void power_on_ds5(void) {
    DEBUG("power_on_ds5\n");

    delay_ms(100);

    update_power_state();
}

// Enable S5 power
void power_on_s5(void) {
    DEBUG("power_on_s5\n");

    // 0ms
    GPIO_SET_DEBUG(KBC_PWR_ON, true);

    delay_ms(3);
    // 3ms
    GPIO_SET_DEBUG(PWR_BTN_OUT_N, true);

    delay_ms(27);
    // 30ms

    delay_ms(80);
    // 110ms
    GPIO_SET_DEBUG(RSMRST_N_KBC, true);

    delay_ms(70);
    // 180ms
    GPIO_SET_DEBUG(PWR_BTN_OUT_N, false);

    delay_ms(120);
    // 300ms
    GPIO_SET_DEBUG(PWR_BTN_OUT_N, true);

    update_power_state();
}

void power_off_s5(void) {
    DEBUG("power_off_s5\n");

    GPIO_SET_DEBUG(RSMRST_N_KBC, false);

    delay_us(1);

    GPIO_SET_DEBUG(KBC_PWR_ON, false);

    update_power_state();
}

// This function is run when the CPU is reset
void power_cpu_reset(void) {
    // Reset fans
    fan_reset();
}

static bool power_button_disabled(void) {
    // Disable power button if lid is closed and AC is disconnected
    return !gpio_get(&LID_SW_N) && gpio_get(&BARREL_ADP_DET_N);
}

void power_event(void) {
    // Always switch to ds5 if EC is running
    if (power_state == POWER_STATE_DEFAULT) {
        power_on_ds5();
    }

    // Check if the adapter line goes low
    static bool ac_send_sci = true;
    static bool ac_last = true;
    bool ac_new = gpio_get(&BARREL_ADP_DET_N);
    if (ac_new != ac_last) {
        DEBUG("Power adapter ");
        if (ac_new) {
            DEBUG("unplugged\n");
            battery_charger_disable();
        } else {
            DEBUG("plugged in\n");
            battery_charger_configure();
        }
        battery_debug();

        // Reset main loop cycle to force reading PECI and battery
        main_cycle = 0;

        // Send SCI to update AC and battery information
        ac_send_sci = true;
    }
    if (ac_send_sci) {
        // Send SCI 0x16 for AC detect event if ACPI OS is loaded
        /*TODO if (acpi_ecos != EC_OS_NONE) */ {
            /*TODO if (pmc_sci(&PMC_1, 0x16)) */ {
                ac_send_sci = false;
            }
        }
    }
    ac_last = ac_new;

    // Configure charger based on charging thresholds when plugged in
    if (!ac_new) {
        battery_charger_configure();
    }

    // Read power switch state
    static bool ps_last = true;
    bool ps_new = gpio_get(&PWRBTN_N);
    if (!ps_new && ps_last) {
        // Ensure press is not spurious
        for (uint8_t i = 100; i != 0; i--) {
            delay_ms(1);
            if (gpio_get(&PWRBTN_N) != ps_new) {
                DEBUG("%02X: Spurious press\n", main_cycle);
                ps_new = ps_last;
                break;
            } else if (power_button_disabled()) {
                // Ignore press when power button disabled
                DEBUG("%02X: Power switch disabled\n", main_cycle);
                ps_new = ps_last;
                break;
            }
        }

        if (ps_new != ps_last) {
            DEBUG("%02X: Power switch press\n", main_cycle);

            // Enable S5 power if necessary, before sending PWR_BTN
            update_power_state();
            if (power_state == POWER_STATE_DS5) {
                power_on_s5();
            }
        }
    }
    #if LEVEL >= LEVEL_DEBUG
        else if (ps_new && !ps_last) {
            DEBUG("%02X: Power switch release\n", main_cycle);
        }
    #endif
    ps_last = ps_new;

    // Send power signal to PCH
    gpio_set(&PWR_BTN_OUT_N, ps_new);

    // Update power state before determining actions
    update_power_state();

    static bool rst_last = false;
    bool rst_new = gpio_get(&LPC_RST_N);
    #if LEVEL >= LEVEL_DEBUG
        if (!rst_new && rst_last) {
            DEBUG("%02X: PLT_RST# asserted\n", main_cycle);
        } else
    #endif
    if(rst_new && !rst_last) {
        DEBUG("%02X: PLT_RST# de-asserted\n", main_cycle);
        power_cpu_reset();
    }
    rst_last = rst_new;

    //TODO: power off S5 when necessary

    static uint32_t last_time = 0;
    uint32_t time = time_get();
    if (power_state == POWER_STATE_S0) {
        // CPU on, power light
        gpio_set(&LED_PWRSTBY, true);
    } else if (power_state == POWER_STATE_S3 || power_state == POWER_STATE_DS3) {
        // Suspended, flashing power light
        if ((time - last_time) >= 1000) {
            gpio_set(&LED_PWRSTBY, !gpio_is_set(&LED_PWRSTBY));
            last_time = time;
        }
    } else {
        // CPU off
        gpio_set(&LED_PWRSTBY, false);
    }

    if (!(battery_status & BATTERY_INITIALIZED)) {
        // No battery connected
        gpio_set(&BAT_AMBERLED, false);
        gpio_set(&BAT_WHITELED, false);
    } else if (ac_new) {
        // Discharging (no AC adapter)
        gpio_set(&BAT_AMBERLED, false);
        gpio_set(&BAT_WHITELED, false);
    } else if (battery_current == 0) {
        // Fully charged
        // TODO: turn off charger
        gpio_set(&BAT_AMBERLED, false);
        gpio_set(&BAT_WHITELED, true);
    } else {
        // Charging
        gpio_set(&BAT_AMBERLED, true);
        gpio_set(&BAT_WHITELED, false);
    }
}
