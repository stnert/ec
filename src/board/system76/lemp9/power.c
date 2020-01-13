#include <arch/delay.h>
#include <board/power.h>
#include <board/pnp.h>
#include <common/debug.h>
#include <ec/gpio.h>

// Platform does not currently support Deep Sx
#define DEEP_SX 0

extern uint8_t main_cycle;

extern struct Gpio __code PCH_DPWROK_EC;
extern struct Gpio __code PCH_PWROK_EC;
extern struct Gpio __code LED_PWR;
extern struct Gpio __code ALL_SYS_PWRGD;
extern struct Gpio __code PM_PWROK;
extern struct Gpio __code PWR_SW_N;
extern struct Gpio __code BUF_PLT_RST_N;
extern struct Gpio __code PWR_BTN_N;
extern struct Gpio __code SUSWARN_N;
extern struct Gpio __code EC_EN;
extern struct Gpio __code VA_EC_EN;
extern struct Gpio __code DD_ON;
extern struct Gpio __code EC_RSMRST_N;
extern struct Gpio __code AC_PRESENT;
extern struct Gpio __code SUSC_N_PCH;
extern struct Gpio __code VR_ON;
extern struct Gpio __code SUSB_N_PCH;
extern struct Gpio __code SLP_SUS_N;
extern struct Gpio __code SUS_PWR_ACK;

// VccRTC stable (55%) to RTCRST# high
#define tPCH01 delay_ms(9)
// VccDSW stable (95%) to RSMRST# high
#define tPCH02 delay_ms(10)
// VccPrimary stable (95%) to RSMRST# high
#define tPCH03 delay_ms(10)
// VccRTC stable (90%) to start of VccDSW voltage ramp
#define tPCH04 delay_ms(9)
// RTCRST# high to DSW_PWROK
#define tPCH05 delay_us(1)
// VccDSW 3.3 stable to VccPrimary 1.05V
#define tPCH06 delay_us(200)
// DSW_PWROK high to RSMRST# high
#define tPCH07 delay_ms(0)
// SLP_S3# de-assertion to PCH_PWROK assertion
#define tPCH08 delay_ms(1)
// SLP_A# high when ASW rails are stable (95%)
#define tPCH09 delay_ms(2, 4, 8, 16) //TODO
// PCH_PWROK low to VCCIO dropping 5%
#define tPCH10 delay_ns(400)
// SLP_SUS# asserting to VccPRIM dropping 5%
#define tPCH11 delay_ns(100)
// RSMRST# asserting to VccPRIM dropping 5%
#define tPCH12 delay_ns(400)
// DSW_PWROK falling to any of VccDSW, VccPRIM dropping 5%
#define tPCH14 delay_ns(400)
// De-assertion of RSMRST# to de-assertion of ESPI_RESET#
#if DEEP_SX
    #define tPCH18 delay_us(90)
#else
    #define tPCH18 delay_ms(95)
#endif
// DSW_PWROK assertion to SLP_SUS# de-assertion
#define tPCH32 delay_ms(95)
// RSMRST# de-assertion to SUSPWRDNACK valid
#define tPLT01 delay_ms(200)

// Enable deep sleep well power
void power_on_ds5() {
    DEBUG("%02X: power_on_ds5\n", main_cycle);

#if DEEP_SX
    // See Figure 12-18 in Whiskey Lake Platform Design Guide
    // | VCCRTC | RTCRST# | VCCDSW_3P3 | DSW_PWROK |
    // | tPCH01---------- |            |           |
    // | tPCH04----------------------- |           |
    // |        | tPCH05-------------------------- |
    // |        |         | tPCH02---------------- |

    // tPCH01 and tPCH02 combined make the longest delay
    tPCH01;
    tPCH02;

    // Deep sleep well is a-ok
    gpio_set(PCH_DPWROK_EC, true);
    // Wait for deep sleep well to propogate
    tPCH32;
#else // DEEP_SX
    // See Figure 12-19 in Whiskey Lake Platform Design Guide
    // | VCCRTC | RTCRST# | VccPRIM |
    // | tPCH01---------- |         |
    // | tPCH04-------------------- |

    // tPCH04 is the ideal delay
    tPCH04;
#endif // DEEP_SX
}

// Enable S5 power
void power_on_s5() {
    DEBUG("%02X: power_on_s5\n", main_cycle);

#if DEEP_SX
    // See Figure 12-18 in Whiskey Lake Platform Design Guide

    // TODO
#else // DEEP_SX
    // See Figure 12-19 in Whiskey Lake Platform Design Guide
    // TODO - signal timing graph
    // See Figure 12-25 in Whiskey Lake Platform Design Guide
    // TODO - rail timing graph

    // Enable VCCPRIM_* planes - must be enabled prior to USB power in order to
    // avoid leakage
    gpio_set(&VA_EC_EN, true);
    tPCH06;

    // Enable VDD5
    gpio_set(&DD_ON, true);

    // De-assert SUS_ACK# - TODO is this needed on non-dsx?
    gpio_set(&SUS_PWR_ACK, true);
    tPCH03;

    // Assert DSW_PWROK
    gpio_set(&PCH_DPWROK_EC, true);

    // De-assert RSMRST#
    gpio_set(&EC_RSMRST_N, true);

    // Wait for PCH stability
    tPCH18;

    // Allow processor to control SUSB# and SUSC#
    gpio_set(&EC_EN, true);

    // Wait for SUSPWRDNACK validity
    tPLT01;

    // Extra wait - TODO remove
    delay_ms(200);
#endif // DEEP_SX
}

void power_off_s5() {
    DEBUG("%02X: power_off_s5\n", main_cycle);

#if DEEP_SX
    // TODO
#else // DEEP_SX
    // De-assert SYS_PWROK
    gpio_set(&PCH_PWROK_EC, false);

    // De-assert PCH_PWROK
    gpio_set(&PM_PWROK, false);

    // Block processor from controlling SUSB# and SUSC#
    gpio_set(&EC_EN, false);

    // De-assert RSMRST#
    gpio_set(&EC_RSMRST_N, false);

    // Disable VDD5
    gpio_set(&DD_ON, false);
    tPCH12;

    // Disable VCCPRIM_* planes
    gpio_set(&VA_EC_EN, false);

    // De-assert DSW_PWROK
    gpio_set(&PCH_DPWROK_EC, false);
    tPCH14;
#endif // DEEP_SX
}

enum PowerState {
    POWER_STATE_DEFAULT,
    POWER_STATE_DS5,
    POWER_STATE_S5,
    POWER_STATE_DS3,
    POWER_STATE_S3,
    POWER_STATE_S0,
};

void power_event(void) {
    static enum PowerState state = POWER_STATE_DEFAULT;

    // Always switch to ds5 if EC is running
    if (state == POWER_STATE_DEFAULT) {
        power_on_ds5();
        state = POWER_STATE_DS5;
    }

    // Read power switch state
    static bool ps_last = true;
    bool ps_new = gpio_get(&PWR_SW_N);
    if (!ps_new && ps_last) {
        // Ensure press is not spurious
        delay_ms(10);
        if (gpio_get(&PWR_SW_N) != ps_new) {
            DEBUG("%02X: Spurious press\n", main_cycle);
            ps_new = ps_last;
        } else {
            DEBUG("%02X: Power switch press\n", main_cycle);

            // Enable S5 power if necessary, before sending PWR_BTN
            if (state == POWER_STATE_DS5) {
                power_on_s5();
                state = POWER_STATE_S5;
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
    gpio_set(&PWR_BTN_N, ps_new);

#if DEEP_SX
    //TODO
#else // DEEP_SX
    //TODO: set power state as necessary

    // If system power is good
    static bool pg_last = false;
    bool pg_new = gpio_get(&ALL_SYS_PWRGD);
    if (pg_new && !pg_last) {
        DEBUG("%02X: ALL_SYS_PWRGD asserted\n", main_cycle);

        //TODO: tPLT04;

        // Allow H_VR_READY to set PCH_PWROK
        gpio_set(&PM_PWROK, true);

        // OEM defined delay from ALL_SYS_PWRGD to SYS_PWROK - TODO
        delay_ms(10);

        // Assert SYS_PWROK, system can finally perform PLT_RST# and boot
        gpio_set(&PCH_PWROK_EC, true);
    } else if(!pg_new && pg_last) {
        DEBUG("%02X: ALL_SYS_PWRGD de-asserted\n", main_cycle);

        // De-assert SYS_PWROK
        gpio_set(&PCH_PWROK_EC, false);

        // De-assert PCH_PWROK
        gpio_set(&PM_PWROK, false);
    }
    pg_last = pg_new;

    static bool rst_last = false;
    bool rst_new = gpio_get(&BUF_PLT_RST_N);
    #if LEVEL >= LEVEL_DEBUG
        if (!rst_new && rst_last) {
            DEBUG("%02X: PLT_RST# asserted\n", main_cycle);
        } else
    #endif
    if(rst_new && !rst_last) {
        DEBUG("%02X: PLT_RST# de-asserted\n", main_cycle);

        // LPC was just reset, enable PNP devices
        pnp_enable();
        //TODO: reset KBC and touchpad states
    }
    rst_last = rst_new;

    #if LEVEL >= LEVEL_DEBUG
        static bool s3_last = false;
        bool s3_new = gpio_get(&SUSB_N_PCH);
        if (!s3_new && s3_last) {
            DEBUG("%02X: SLP_S3# asserted\n", main_cycle);
        } else if(s3_new && !s3_last) {
            DEBUG("%02X: SLP_S3# de-asserted\n", main_cycle);
        }
        s3_last = s3_new;

        static bool s4_last = false;
        bool s4_new = gpio_get(&SUSC_N_PCH);
        if (!s4_new && s4_last) {
            DEBUG("%02X: SLP_S4# asserted\n", main_cycle);
        } else if(s4_new && !s4_last) {
            DEBUG("%02X: SLP_S4# de-asserted\n", main_cycle);
        }
        s4_last = s4_new;

        static bool sus_last = false;
        bool sus_new = gpio_get(&SLP_SUS_N);
        if (!sus_new && sus_last) {
            DEBUG("%02X: SLP_SUS# asserted\n", main_cycle);
        } else if (sus_new && !sus_last) {
            DEBUG("%02X: SLP_SUS# de-asserted\n", main_cycle);
        }
        sus_last = sus_new;
    #endif

    // EC must keep VccPRIM powered if SUSPWRDNACK is de-asserted low or system
    // state is S3
    static bool ack_last = false;
    bool ack_new = gpio_get(&SUSWARN_N);
    if (ack_new && !ack_last) {
        DEBUG("%02X: SUSPWRDNACK asserted\n", main_cycle);

        if (gpio_get(&SUSC_N_PCH)) {
            DEBUG("%02X: entering S3 state\n", main_cycle);
        } else if (state == POWER_STATE_S5) {
            power_off_s5();
            state = POWER_STATE_DS5;
        }
    }
    #if LEVEL >= LEVEL_DEBUG
        else if (!ack_new && ack_last) {
            DEBUG("%02X: SUSPWRDNACK de-asserted\n", main_cycle);
        }
    #endif
    ack_last = ack_new;

#endif // DEEP_SX

    switch (state) {
        case POWER_STATE_DEFAULT:
        case POWER_STATE_DS5:
        case POWER_STATE_S5:
        case POWER_STATE_DS3:
        case POWER_STATE_S3:
            gpio_set(&LED_PWR, false);
            break;
        case POWER_STATE_S0:
            gpio_set(&LED_PWR, true);
            break;
    }
}