# SPDX-License-Identifier: GPL-3.0-only

LVFS_DEVICE_ID = com.system76.darp5.ec.firmware
LVFS_DEVICE_NAME = "Darter Pro 5"
# TODO
LVFS_DEVICE_UUID = "00000000-0000-0000-0000-000000000000"

EC=it8587e

# Include keyboard
KEYBOARD=15in_102

# Set keyboard LED mechanism
KBLED=darp5
CFLAGS+=-DI2C_KBLED=I2C_1

# Set battery I2C bus
CFLAGS+=-DI2C_SMBUS=I2C_0

# Set touchpad PS2 bus
CFLAGS+=-DPS2_TOUCHPAD=PS2_3

# Set smart charger parameters
CFLAGS+=\
	-DCHARGER_CHARGE_CURRENT=1536 \
	-DCHARGER_CHARGE_VOLTAGE=17600 \
	-DCHARGER_INPUT_CURRENT=3200

# Add system76 common code
include src/board/system76/common/common.mk
