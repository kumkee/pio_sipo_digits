# Makefile based on
# 	https://docs.platformio.org/en/latest/integration/ide/vim.html
# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

ifndef SERIAL_DEV
  ifneq (,$(wildcard /dev/ttyUSB0))
    SERIAL_DEV = /dev/ttyUSB0
  else ifneq (,$(wildcard /dev/ttyACM0))
    SERIAL_DEV = /dev/ttyACM0
  else
    SERIAL_DEV = unknown
  endif
endif

all:
	pio run

upload:
	pio run --target upload

clean:
	pio run --target clean

program:
	pio run --target program

uploadfs:
	pio run --target uploadfs

monitor:
	pio device monitor -p $(SERIAL_DEV) -b 115200

update:
	pio update

compiledb:
	COMPILATIONDB_INCLUDE_TOOLCHAIN=true pio run -t compiledb

mount:
	sudo chgrp dialout $(SERIAL_DEV)
	sudo chmod g+rw $(SERIAL_DEV)
