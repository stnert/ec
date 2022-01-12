// SPDX-License-Identifier: GPL-3.0-only

#include <stdbool.h>

#include <common/macro.h>
#include <ec/i2c.h>

struct I2C __code I2C_0 = I2C(0);
struct I2C __code I2C_1 = I2C(1);
struct I2C __code I2C_2 = I2C(2);
struct I2C __code I2C_3 = I2C(3);
struct I2C __code I2C_4 = I2C(4);
struct I2C __code I2C_5 = I2C(5);

void i2c_reset(struct I2C * i2c, bool kill) {
    //TODO: implement
    i2c = i2c;
    kill = kill;
}

int16_t i2c_start(struct I2C * i2c, uint8_t addr, bool read) __reentrant {
    *(i2c->adr) = (addr << 1) | read;

    return 0;
}

void i2c_stop(struct I2C * i2c) {
    //TODO: implement
    i2c = i2c;
}

static int16_t i2c_transaction(struct I2C * i2c, uint8_t * data, uint16_t length, bool read) {
    uint16_t i;

    //TODO: allow writes of any size
    if (length > 32) {
        return -0x2000;
    }

    // Copy data to buffer if writing
    if (!read) {
        for (i = 0; i < length; i++) {
            i2c->dat[i] = data[i];
        }
    }

    // Set length
    *(i2c->cnt) = (uint8_t)length;

    // Set stop bit
    //TODO: don't always stop, not all devices support this
    *(i2c->frt) = BIT(0);

    // Initiate flexible protocol
    *(i2c->prtcl) = 0x7F;

    // Wait for completion
    while (!((*(i2c->pf)) & BIT(0))) {}

    // Get status and clear complete flag
    *(i2c->pf) = BIT(0);

    // Return error, if found
    if ((*(i2c->sts)) & 0x1F) {
        return -(int16_t)((*(i2c->sts)) & 0x1F);
    }

    // Copy data from buffer if reading
    if (read) {
        for (i = 0; i < length; i++) {
            data[i] = i2c->dat[i];
        }
    }

    return length;
}

int16_t i2c_read(struct I2C * i2c, uint8_t * data, uint16_t length) __reentrant {
    return i2c_transaction(i2c, data, length, true);
}

int16_t i2c_write(struct I2C * i2c, uint8_t * data, uint16_t length) __reentrant {
    return i2c_transaction(i2c, data, length, false);
}
