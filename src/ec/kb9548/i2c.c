// SPDX-License-Identifier: GPL-3.0-only

#include <stdbool.h>

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
