// SPDX-License-Identifier: GPL-3.0-only

#ifndef _BOARD_KBSCAN_H
#define _BOARD_KBSCAN_H

#include <stdbool.h>

#include <board/keymap.h>

void kbscan_init(void);
void kbscan_event(void);

#endif // _BOARD_KBSCAN_H
