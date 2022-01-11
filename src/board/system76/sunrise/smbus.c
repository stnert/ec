// SPDX-License-Identifier: GPL-3.0-only

#include <board/smbus.h>
#include <common/macro.h>
#include <ec/i2c.h>

void smbus_init(void) {
    // Enable SMBUS controller
    *(I2C_SMBUS.cfg) = BIT(5) | BIT(0);

    // Set frequency to 400 kHz
    *(I2C_SMBUS.ofh) = 0x01;
    *(I2C_SMBUS.ofl) = 0x02;
}

int16_t smbus_read(uint8_t address, uint8_t command, uint16_t * data) {
    // Issue command
    *(I2C_SMBUS.adr) = (address << 1) | 1;
    *(I2C_SMBUS.cmd) = command;
    *(I2C_SMBUS.prtcl) = 0x09;

    // Wait for completion
    while (!((*(I2C_SMBUS.pf)) & BIT(0))) {}

    // Get status and clear complete flag
    uint8_t sts = *(I2C_SMBUS.sts);
    *(I2C_SMBUS.pf) = BIT(0);

    // Return error, if found
    if (sts & 0x1F) {
        return -(int16_t)(sts & 0x1F);
    }

    // Read data
    //TODO: do before clearing complete flag?
    *data = (uint16_t)(I2C_SMBUS.dat[0]) |
           ((uint16_t)(I2C_SMBUS.dat[1]) << 8);

    // Return byte count
    return 2;
}

int16_t smbus_write(uint8_t address, uint8_t command, uint16_t data) {
    // Issue command
    *(I2C_SMBUS.adr) = (address << 1) | 0;
    *(I2C_SMBUS.cmd) = command;
    I2C_SMBUS.dat[0] = (uint8_t)data;
    I2C_SMBUS.dat[1] = (uint8_t)(data >> 8);
    *(I2C_SMBUS.prtcl) = 0x08;

    // Wait for completion
    while (!((*(I2C_SMBUS.pf)) & BIT(0))) {}

    // Get status and clear complete flag
    uint8_t sts = *(I2C_SMBUS.sts);
    *(I2C_SMBUS.pf) = BIT(0);

    // Return error, if found
    if (sts & 0x1F) {
        return -(int16_t)(sts & 0x1F);
    }

    // Return byte count
    return 2;
}
