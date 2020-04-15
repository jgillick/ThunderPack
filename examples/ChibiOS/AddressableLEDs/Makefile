# Source files to compile
CSRC = main.c \
       ws2812b.c

# Linking math library required by ws2812
USE_LDOPT = -lm

###############################################################################
# Define the environment
#

# Enable DMA service (stm32_dma.c)
UDEFS += -DSTM32_DMA_REQUIRED

# ChibiOS lib directory
CHIBIOS = ../common/ChibiOS

# Directory with board.[ch] files
BOARD_DIR = ../common/board

# Drivers directory, from STM32CubMX
# (should contain "CMSIS" & "STM32L0xx_HAL_Driver" directories)
CUBEMX_DRIVERS = ../common/drivers

# Conf directory.
# should contain halconf.h, mcuconf.h, chconf.h
CONFDIR = ../common/conf

##############################################################################

include ../common/Makefile