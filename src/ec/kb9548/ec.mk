# SPDX-License-Identifier: GPL-3.0-only

ARCH=8051

# 64 KB is the max without banking
CODE_SIZE=65536

# SRAM size is 5 KiB, and it is at 0xE800
SRAM_ADDR=0xE800
SRAM_SIZE=5120
