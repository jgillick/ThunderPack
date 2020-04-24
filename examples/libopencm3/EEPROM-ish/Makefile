CFILES = main.c \
        ../common/thunderpack.c

# To reserve space for memory, we need to provide our own linker file
LDSCRIPT = ./STM32F411CEUx_FLASH_NVM.ld

# Device
LIB_TARGET = stm32/f4
OPENCM3_LIB = opencm3_stm32f4
OPENCM3_DEFS = -DSTM32F4
ARCH_FLAGS =  -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Makefiles
COMMON_DIR = ../common
OPENCM3_DIR = ${COMMON_DIR}/libopencm3
include ../common/rules.mk
