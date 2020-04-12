CFILES = main.c \
        ../common/thunderpack.c

# To reserve space for memory, we need to provide our own linker file
LDSCRIPT = ./STM32F411CEUx_FLASH_NVM.ld

# Device
LIB_TARGET = stm32/f4
DEVICE = STM32F411CE

# Makefiles
COMMON_DIR = ../common
OPENCM3_DIR = ${COMMON_DIR}/libopencm3
include ../common/rules.mk
