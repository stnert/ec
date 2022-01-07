# SPDX-License-Identifier: GPL-3.0-only

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
CONSOLE_BAUD?=9600

EC=kb9548

$(BUILD)/ec.pad: $(BUILD)/ec.rom
	cp $< $@
	truncate -s 163840 $@

console_external:
	sudo tio -b $(CONSOLE_BAUD) -m INLCRNL -t /dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller-if00-port0

flash_external: $(BUILD)/ec.pad
	sudo /home/jeremy/Projects/flashrom/flashrom -p ch341a_spi -c "KB9548 (EDI)" -V -w $<
