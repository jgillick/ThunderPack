CFILES = main.c \
         ws2812b.c \
        ../common/thunderpack.c

# Include the math libs in linking (for 'ciel' and 'floor' functions)
LDLIBS = -lm

# Device
LIB_TARGET = stm32/f4
DEVICE = STM32F411CE

# Makefiles
COMMON_DIR = ../common
OPENCM3_DIR = ${COMMON_DIR}/libopencm3
include ../common/rules.mk
