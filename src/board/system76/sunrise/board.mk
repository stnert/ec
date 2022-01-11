# SPDX-License-Identifier: GPL-3.0-only

EC=kb9548

# Set log level
# 0 - NONE
# 1 - ERROR
# 2 - WARN
# 3 - INFO
# 4 - DEBUG
# 5 - TRACE
CFLAGS+=-DLEVEL=4

# Enable serial debugging
CFLAGS+=-DSERIAL_DEBUG
CONSOLE_BAUD?=131072

# Set battery I2C bus
CFLAGS+=-DI2C_SMBUS=I2C_1

# Set smart charger parameters
CFLAGS+=\
	-DCHARGER_CHARGE_CURRENT=1536 \
	-DCHARGER_CHARGE_VOLTAGE=13200 \
	-DCHARGER_INPUT_CURRENT=3330

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
