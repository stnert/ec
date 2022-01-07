// SPDX-License-Identifier: GPL-3.0-only

#include <stdint.h>
#include <stdio.h>

#ifdef SERIAL_DEBUG
    #include <mcs51/8051.h>
#endif

int putchar(int c) {
    uint8_t byte = (uint8_t)c;

#ifdef SERIAL_DEBUG
    SBUF = byte;
    while (!TI) {}
    TI = 0;
#endif

    return (int)byte;
}
