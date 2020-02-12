CFILES = main.c

# Device
LIB_TARGET = stm32/f4
DEVICE = STM32F412CGUx

# Makefiles
COMMON_DIR = ../common
OPENCM3_DIR = $(COMMON_DIR)/libopencm3
include $(OPENCM3_DIR)/mk/genlink-config.mk
include $(COMMON_DIR)/rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
