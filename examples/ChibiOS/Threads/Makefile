# Source files to compile
CPPSRC = LEDController.cpp \
         ProgramController.cpp \
         main.cpp

# Reserve some flash memory to act as EEPROM with a custom linker script
LDSCRIPT = ./STM32F411CEUx_FLASH_NVM.ld

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

###############################################################################
# STM32 Libraries
STM32F4_HAL = ../../STM32Cube/shared/Drivers/STM32F4xx_HAL_Driver
CMSIS =  ../../STM32Cube/shared/Drivers/CMSIS/Device/ST/STM32L4xx

UDEFS = -DSTM32F411xE -DUSE_HAL_DRIVER

CSRC += $(STM32F4_HAL)/Src/stm32f4xx_hal.c \
		$(STM32F4_HAL)/Src/stm32f4xx_hal_flash.c \
        $(STM32F4_HAL)/Src/stm32f4xx_hal_flash_ex.c

ALLINC += $(STM32F4_HAL)/Inc \
          $(CMSIS)/Include \
          $(CMSIS)/Device/ST/STM32F4xx/Include/


##############################################################################

include ../common/Makefile