// SPDX-License-Identifier: GPL-3.0-only

#include <stdbool.h>

#include <common/i2c.h>

struct I2C;

int16_t i2c_start(struct I2C * i2c, uint8_t addr, bool read) __reentrant {
    //TODO: implement
    i2c = i2c;
    addr = addr;
    read = read;
    return -1;
}

void i2c_stop(struct I2C * i2c) {
    //TODO: implement
    i2c = i2c;
}

static int16_t i2c_transaction(struct I2C * i2c, uint8_t * data, uint16_t length, bool read) {
    //TODO: implement
    i2c = i2c;
    data = data;
    length = length;
    read = read;
    return -1;
}

int16_t i2c_read(struct I2C * i2c, uint8_t * data, uint16_t length) __reentrant {
    return i2c_transaction(i2c, data, length, true);
}

int16_t i2c_write(struct I2C * i2c, uint8_t * data, uint16_t length) __reentrant {
    return i2c_transaction(i2c, data, length, false);
}
