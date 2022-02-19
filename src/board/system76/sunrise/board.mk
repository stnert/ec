# SPDX-License-Identifier: GPL-3.0-only

EC=kb9548

KEYBOARD_DIR=src/keyboard/system76/sunrise
include $(KEYBOARD_DIR)/keyboard.mk

# Set log level
# 0 - NONE
# 1 - ERROR
# 2 - WARN
# 3 - INFO
# 4 - DEBUG
# 5 - TRACE
CFLAGS+=-DLEVEL=5

# Enable serial debugging
CFLAGS+=-DSERIAL_DEBUG
CONSOLE_BAUD?=131072

# AMD CPU
CFLAGS+=-DHAVE_AMD_CPU=1

# Set CPU I2C bus
CFLAGS+=-DI2C_CPU=I2C_2

# Set battery I2C bus
CFLAGS+=-DI2C_SMBUS=I2C_1

# Set smart charger parameters
CFLAGS+=\
	-DCHARGER_CHARGE_CURRENT=1536 \
	-DCHARGER_CHARGE_VOLTAGE=13200 \
	-DCHARGER_INPUT_CURRENT=3330

# Set battery charging thresholds
BATTERY_START_THRESHOLD?=0
BATTERY_END_THRESHOLD?=100

CFLAGS+=\
	-DBATTERY_START_THRESHOLD=$(BATTERY_START_THRESHOLD) \
	-DBATTERY_END_THRESHOLD=$(BATTERY_END_THRESHOLD)

$(BUILD)/ec.pad: $(BUILD)/ec.rom
	cp $< $@
	truncate -s 163840 $@

console_external:
	sudo tio -b $(CONSOLE_BAUD) -m INLCRNL -t /dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller-if00-port0

flash_external: $(BUILD)/ec.pad
	sudo /home/jeremy/Projects/flashrom/flashrom \
		--programmer ch341a_spi \
		--chip "KB9548 (EDI)" \
		--noverify \
		--write $<
