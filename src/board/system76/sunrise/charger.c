// SPDX-License-Identifier: GPL-3.0-only
//
// ISL9241
// https://www.renesas.com/us/en/document/sds/isl9241-short-form-datasheet

#include <board/battery.h>
#include <board/smbus.h>
#include <common/debug.h>
#include <common/macro.h>

// Registers
#define REG_CHARGE_CURRENT 0x14
#define REG_CHARGE_VOLTAGE 0x15
#define REG_ADAPTER_CURRENT 0x3F

// XXX: Assumption: ac_last is initialized high.
static bool charger_enabled = false;

int16_t battery_charger_disable(void) {
    int16_t res = 0;

    if (!charger_enabled) return 0;

    // Disable charge current
    res = smbus_write(CHARGER_ADDRESS, REG_CHARGE_CURRENT, 0);
    if (res < 0) return res;

    // Disable charge voltage
    res = smbus_write(CHARGER_ADDRESS, REG_CHARGE_VOLTAGE, 0);
    if (res < 0) return res;

    // Set input current in mA
    //TODO: needed when charging disabled?
    res = smbus_write(CHARGER_ADDRESS, REG_ADAPTER_CURRENT, CHARGER_INPUT_CURRENT);
    if (res < 0) return res;

    DEBUG("Charger disabled\n");
    charger_enabled = false;
    return 0;
}

int16_t battery_charger_enable(void) {
    int16_t res = 0;

    if (charger_enabled) return 0;

    res = battery_charger_disable();
    if (res < 0) return res;

    // Set charge current in mA
    res = smbus_write(CHARGER_ADDRESS, REG_CHARGE_CURRENT, CHARGER_CHARGE_CURRENT);
    if (res < 0) return res;

    // Set charge voltage in mV
    res = smbus_write(CHARGER_ADDRESS, REG_CHARGE_VOLTAGE, CHARGER_CHARGE_VOLTAGE);
    if (res < 0) return res;

    // Set input current in mA
    res = smbus_write(CHARGER_ADDRESS, REG_ADAPTER_CURRENT, CHARGER_INPUT_CURRENT);
    if (res < 0) return res;

    DEBUG("Charger enabled\n");
    charger_enabled = true;
    return 0;
}

void battery_charger_event(void) {
    // Avoid watchdog timeout
    if (charger_enabled) {
        // Set charge voltage in mV
        smbus_write(CHARGER_ADDRESS, REG_CHARGE_VOLTAGE, CHARGER_CHARGE_VOLTAGE);
    }
}

void battery_debug(void) {
    uint16_t data = 0;
    int16_t res = 0;

    #define command(N, A, V) { \
        DEBUG("  " #N ": "); \
        res = smbus_read(A, V, &data); \
        if (res < 0) { \
            DEBUG("ERROR %04X\n", -res); \
        } else { \
            DEBUG("%04X\n", data); \
        } \
    }

    DEBUG("Battery:\n");
    command(Temperature, BATTERY_ADDRESS, 0x08);
    command(Voltage, BATTERY_ADDRESS, 0x09);
    command(Current, BATTERY_ADDRESS, 0x0A);
    command(Charge, BATTERY_ADDRESS, 0x0D);
    command(Status, BATTERY_ADDRESS, 0x16);

    DEBUG("Charger (isl9241):\n");
    command(ChargeCurrent, CHARGER_ADDRESS, REG_CHARGE_CURRENT);
    command(ChargeVoltage, CHARGER_ADDRESS, REG_CHARGE_VOLTAGE);
    command(AdapterCurrent, CHARGER_ADDRESS, REG_ADAPTER_CURRENT);

    #undef command
}
