# Source files to compile
CPPSRC = LEDController.cpp \
         ProgramController.cpp \
         main.cpp

###############################################################################
# Define the environment
#

# ChibiOS lib directory
CHIBIOS = ../common/ChibiOS

# Directory with board.[ch] files
BOARD_DIR = ../common/board

# Conf directory. 
# (should contain halconf.h, mcuconf.h, chconf.h & stm32l0xx_hal_conf)
CONFDIR = ../common/conf

##############################################################################

###############################################################################
# Include STM32 HAL drivers for EEPROM
#
STM32L0_HAL = ../../STM32Cube/shared/Drivers/STM32L0xx_HAL_Driver
CMSIS = ../../STM32Cube/shared/Drivers/CMSIS/Device/ST/STM32L0xx
UDEFS = -DSTM32L073xx -DUSE_HAL_DRIVER -DPORT_IGNORE_GCC_VERSION_CHECK

# Required for readint/writing to internal EEPROM
ALLCSRC +=  $(STM32L0_HAL)/Src/stm32l0xx_hal.c \
            $(STM32L0_HAL)/Src/stm32l0xx_hal_flash.c \
            $(STM32L0_HAL)/Src/stm32l0xx_hal_flash_ex.c

ALLINC += $(STM32L0_HAL)/Inc \
          $(CMSIS)/Include \
          $(CMSIS)/Device/ST/STM32L0xx/Include/

##############################################################################

include ../common/Makefile