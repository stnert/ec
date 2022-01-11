// SPDX-License-Identifier: GPL-3.0-only

#include <common/macro.h>
#include <ec/wdt.h>

void wdt_disable(void) {
    // Disable WDT
    WDTCFG = 0xC8;

    // Clear pending flags
    WDTPF = BIT(1) | BIT(0);

    // Clear timer bits
    WDTM0902 = 0;
}
