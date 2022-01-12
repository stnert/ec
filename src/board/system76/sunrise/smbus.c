// SPDX-License-Identifier: GPL-3.0-only

#include <board/smbus.h>
#include <common/macro.h>
#include <ec/i2c.h>

void smbus_init(void) {
    // Enable SMBUS controller
    *(I2C_SMBUS.cfg) = BIT(5) | BIT(0);

    // Set frequency to 100 kHz
    *(I2C_SMBUS.ofh) = 0x09;
    *(I2C_SMBUS.ofl) = 0x09;
}

int16_t smbus_read(uint8_t address, uint8_t command, uint16_t * data) {
    return i2c_get(&I2C_SMBUS, address, command, (uint8_t *)data, 2);
}

int16_t smbus_write(uint8_t address, uint8_t command, uint16_t data) {
    return i2c_set(&I2C_SMBUS, address, command, (uint8_t *)&data, 2);
}
