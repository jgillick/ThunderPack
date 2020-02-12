##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
## Copyright (C) 2016 Piotr Esden-Tempski <piotr@esden.net>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

LIB_TARGET = stm32/f4
OPENCM3_DIR ?= $(realpath ./common/libopencm3)

PREFIX		?= arm-none-eabi

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

libopencm3:
ifeq (,$(wildcard $(OPENCM3_DIR)))
	$(info ######## ERROR!!! ########)
	$(info libopencm3 is not initialized.)
	$(info Try running these commands before running make again)
	$(info (you should only need to do this once):)
	$(info * git submodule init)
	$(info * git submodule update)
	$(info * make)
	$(error )
else
	$(Q)$(MAKE) -C $(OPENCM3_DIR) lib TARGETS="$(LIB_TARGET)"
endif

clean:
	$(Q)$(MAKE) -C $(OPENCM3_DIR) clean

.PHONY: libopencm3 clean \
        bin hex srec list images

