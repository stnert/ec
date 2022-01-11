// SPDX-License-Identifier: GPL-3.0-only

#ifndef _EC_I2C_H
#define _EC_I2C_H

#include <common/i2c.h>

struct I2C {
    volatile uint8_t __xdata * cfg;
    volatile uint8_t __xdata * ofh;
    volatile uint8_t __xdata * ofl;
    volatile uint8_t __xdata * pf;
    volatile uint8_t __xdata * prtcl;
    volatile uint8_t __xdata * sts;
    volatile uint8_t __xdata * adr;
    volatile uint8_t __xdata * cmd;
    volatile uint8_t __xdata * dat;
    volatile uint8_t __xdata * cnt;
};

#define I2C(NUMBER) { \
    .cfg = &FSMB ## NUMBER ## CFG, \
    .ofh = &FSMB ## NUMBER ## OFH, \
    .ofl = &FSMB ## NUMBER ## OFL, \
    .pf = &FSMB ## NUMBER ## PF, \
    .prtcl = &FSMB ## NUMBER ## PRTCL, \
    .sts = &FSMB ## NUMBER ## STS, \
    .adr = &FSMB ## NUMBER ## ADR, \
    .cmd = &FSMB ## NUMBER ## CMD, \
    .dat = FSMB ## NUMBER ## DAT, \
    .cnt = FSMB ## NUMBER ## CNT, \
}

extern struct I2C __code I2C_0;
extern struct I2C __code I2C_1;
extern struct I2C __code I2C_2;
extern struct I2C __code I2C_3;
extern struct I2C __code I2C_4;
extern struct I2C __code I2C_5;

void i2c_reset(struct I2C * i2c, bool kill);

// Config
volatile uint8_t __xdata __at(0x1610) FSMB0CFG;
volatile uint8_t __xdata __at(0x1710) FSMB1CFG;
volatile uint8_t __xdata __at(0x2110) FSMB2CFG;
volatile uint8_t __xdata __at(0x2210) FSMB3CFG;
volatile uint8_t __xdata __at(0x2310) FSMB4CFG;
volatile uint8_t __xdata __at(0x2410) FSMB5CFG;

// Operating frequency (high)
volatile uint8_t __xdata __at(0x1613) FSMB0OFH;
volatile uint8_t __xdata __at(0x1713) FSMB1OFH;
volatile uint8_t __xdata __at(0x2113) FSMB2OFH;
volatile uint8_t __xdata __at(0x2213) FSMB3OFH;
volatile uint8_t __xdata __at(0x2313) FSMB4OFH;
volatile uint8_t __xdata __at(0x2413) FSMB5OFH;

// Operating frequency (low)
volatile uint8_t __xdata __at(0x1614) FSMB0OFL;
volatile uint8_t __xdata __at(0x1714) FSMB1OFL;
volatile uint8_t __xdata __at(0x2114) FSMB2OFL;
volatile uint8_t __xdata __at(0x2214) FSMB3OFL;
volatile uint8_t __xdata __at(0x2314) FSMB4OFL;
volatile uint8_t __xdata __at(0x2414) FSMB5OFL;

// Pending flag
volatile uint8_t __xdata __at(0x1616) FSMB0PF;
volatile uint8_t __xdata __at(0x1716) FSMB1PF;
volatile uint8_t __xdata __at(0x2116) FSMB2PF;
volatile uint8_t __xdata __at(0x2216) FSMB3PF;
volatile uint8_t __xdata __at(0x2316) FSMB4PF;
volatile uint8_t __xdata __at(0x2416) FSMB5PF;

// Protocol
volatile uint8_t __xdata __at(0x1618) FSMB0PRTCL;
volatile uint8_t __xdata __at(0x1718) FSMB1PRTCL;
volatile uint8_t __xdata __at(0x2118) FSMB2PRTCL;
volatile uint8_t __xdata __at(0x2218) FSMB3PRTCL;
volatile uint8_t __xdata __at(0x2318) FSMB4PRTCL;
volatile uint8_t __xdata __at(0x2418) FSMB5PRTCL;

// Status
volatile uint8_t __xdata __at(0x1619) FSMB0STS;
volatile uint8_t __xdata __at(0x1719) FSMB1STS;
volatile uint8_t __xdata __at(0x2119) FSMB2STS;
volatile uint8_t __xdata __at(0x2219) FSMB3STS;
volatile uint8_t __xdata __at(0x2319) FSMB4STS;
volatile uint8_t __xdata __at(0x2419) FSMB5STS;

// Address
volatile uint8_t __xdata __at(0x161A) FSMB0ADR;
volatile uint8_t __xdata __at(0x171A) FSMB1ADR;
volatile uint8_t __xdata __at(0x211A) FSMB2ADR;
volatile uint8_t __xdata __at(0x221A) FSMB3ADR;
volatile uint8_t __xdata __at(0x231A) FSMB4ADR;
volatile uint8_t __xdata __at(0x241A) FSMB5ADR;

// Command
volatile uint8_t __xdata __at(0x161B) FSMB0CMD;
volatile uint8_t __xdata __at(0x171B) FSMB1CMD;
volatile uint8_t __xdata __at(0x211B) FSMB2CMD;
volatile uint8_t __xdata __at(0x221B) FSMB3CMD;
volatile uint8_t __xdata __at(0x231B) FSMB4CMD;
volatile uint8_t __xdata __at(0x241B) FSMB5CMD;

// Data
volatile uint8_t __xdata __at(0x161C) FSMB0DAT[32];
volatile uint8_t __xdata __at(0x171C) FSMB1DAT[32];
volatile uint8_t __xdata __at(0x211C) FSMB2DAT[32];
volatile uint8_t __xdata __at(0x221C) FSMB3DAT[32];
volatile uint8_t __xdata __at(0x231C) FSMB4DAT[32];
volatile uint8_t __xdata __at(0x241C) FSMB5DAT[32];

// Count
volatile uint8_t __xdata __at(0x163C) FSMB0CNT;
volatile uint8_t __xdata __at(0x173C) FSMB1CNT;
volatile uint8_t __xdata __at(0x213C) FSMB2CNT;
volatile uint8_t __xdata __at(0x223C) FSMB3CNT;
volatile uint8_t __xdata __at(0x233C) FSMB4CNT;
volatile uint8_t __xdata __at(0x243C) FSMB5CNT;

#endif // _EC_I2C_H
