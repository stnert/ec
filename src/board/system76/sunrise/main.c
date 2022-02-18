// SPDX-License-Identifier: GPL-3.0-only

#include <arch/arch.h>
#include <arch/delay.h>
#include <arch/time.h>
#include <board/battery.h>
#include <board/board.h>
#include <board/fan.h>
#include <board/gpio.h>
#include <board/kbscan.h>
#include <board/lid.h>
#include <board/peci.h>
#include <board/power.h>
#include <board/smbus.h>
#include <common/debug.h>
#include <common/version.h>
#include <ec/sfr.h>
#include <ec/wdt.h>

#define INTERRUPT_EMPTY(N) \
    void interrupt_ ## N (void) __interrupt(N) {}

INTERRUPT_EMPTY(0) // 0x03 - External 0
void timer_0(void) __interrupt(1); // timer_0 is in time.c
INTERRUPT_EMPTY(2) // 0x13 - External 1
INTERRUPT_EMPTY(3) // 0x1B - Timer 1
INTERRUPT_EMPTY(4) // 0x23 - Serial
INTERRUPT_EMPTY(5) // Not used
INTERRUPT_EMPTY(6) // Not used
INTERRUPT_EMPTY(7) // Not used
INTERRUPT_EMPTY(8) // 0x43 - WDT / TMR0-2 / GWG
INTERRUPT_EMPTY(9) // 0x4B - EC ExtCMD / LPC Mailbox, OWM
INTERRUPT_EMPTY(10) // 0x53 - PS/2
INTERRUPT_EMPTY(11) // 0x5B - KBC
INTERRUPT_EMPTY(12) // 0x63 - IKB
INTERRUPT_EMPTY(13) // 0x6B - LPC Legacy I/O
INTERRUPT_EMPTY(14) // 0x73 - EC
INTERRUPT_EMPTY(15) // 0x7B - ESB / power lost
INTERRUPT_EMPTY(16) // 0x83 - FAN0 / eSPI
INTERRUPT_EMPTY(17) // 0x8B - FAN1 / I2CS0-1
INTERRUPT_EMPTY(18) // 0x93 - FSMBus0-5
INTERRUPT_EMPTY(19) // 0x9B - CIR / I2CD
INTERRUPT_EMPTY(20) // 0xA3 - GPT0
INTERRUPT_EMPTY(21) // 0xAB - GPT1
INTERRUPT_EMPTY(22) // 0xB3 - GPT2
INTERRUPT_EMPTY(23) // 0xBB - GPT3
INTERRUPT_EMPTY(24) // 0xC3 - LPC Extended I/O / PECI
INTERRUPT_EMPTY(25) // 0xCB - GPIO0x
INTERRUPT_EMPTY(26) // 0xD3 - GPIO1x
INTERRUPT_EMPTY(27) // 0xDB - GPIO2x
INTERRUPT_EMPTY(28) // 0xE3 - GPIO3x
INTERRUPT_EMPTY(29) // 0xEB - GPIO4x / GPIO6x
INTERRUPT_EMPTY(30) // 0xF3 - GPIO5x / GPIO7x
INTERRUPT_EMPTY(31) // 0xFB - ADC

uint8_t main_cycle = 0;
const uint16_t battery_interval = 1000;
// update fan speed more frequently for smoother fans
// NOTE: event loop is longer than 100ms and maybe even longer than 250
const uint16_t fan_interval = SMOOTH_FANS != 0 ? 250 : 1000;

void serial_init(void) {
    // Set serial mode to 1, disable recieve function
    SCON = 0x40;

    // Set baud to 131072 Hz using timer 1 in 8-bit reload mode
    TMOD |= 0x20;
    TH1 = 0xF7;
    TR1 = 1;
}

void init(void) {
    // Must happen first
    wdt_disable();
    arch_init();
    gpio_init();

    // Can happen in any order
    fan_init();
    kbscan_init();
    peci_init();
    serial_init();
    smbus_init();

    // Must happen last
    board_init();
}

void main(void) {
    init();

    INFO("\n");

    // Test delay function
    delay_ms(10);

    INFO("System76 EC board '%s', version '%s', time %d\n", board(), version(), time_get());

    uint32_t last_time_battery = 0;
    uint32_t last_time_fan = 0;

    for(main_cycle = 0; ; main_cycle++) {
        switch (main_cycle % 3U) {
            case 0:
                // Handle power states
                power_event();
                break;
            case 1:
                // Scans keyboard and sends keyboard packets
                kbscan_event();
                break;
            case 2:
                // Handle lid close/open
                lid_event();
                break;
        }

        if (main_cycle == 0) {
            uint32_t time = time_get();

            // Only run the following once per interval
            if ((time - last_time_fan) >= fan_interval) {
                last_time_fan = time;

                // Update fan speed
                fan_duty_set(peci_get_fan_duty());
            }

            // Only run the following once per interval
            if ((time - last_time_battery) >= battery_interval) {
                last_time_battery = time;

                // Updates battery status
                battery_event();
            }
        }

        // Board-specific events
        board_event();

        // Checks for keyboard/mouse packets from host
        //TODO: kbc_event(&KBC);
        // Handles ACPI communication
        //TODO: pmc_event(&PMC_1);
        // AP/EC communication over SMFI
        //TODO: smfi_event();
    }
}
