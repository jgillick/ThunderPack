# Source files to compile
CSRC = main.c

# Reserve some flash memory with a custom linker file
LDSCRIPT = ./STM32F411CEUx_FLASH_NVM.ld

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

###############################################################################
# Define the environment
#

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