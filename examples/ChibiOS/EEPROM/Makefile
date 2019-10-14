# Source files to compile
CSRC = main.c

# STM32 HAL Drivers for EEPROM
STM32L0_HAL = ../../STM32Cube/shared/Drivers/STM32L0xx_HAL_Driver
CMSIS =  ../../STM32Cube/shared/Drivers/CMSIS/Device/ST/STM32L0xx

UDEFS = -DSTM32L073xx -DUSE_HAL_DRIVER

CSRC += $(STM32L0_HAL)/Src/stm32l0xx_hal.c \
		$(STM32L0_HAL)/Src/stm32l0xx_hal_flash.c \
        $(STM32L0_HAL)/Src/stm32l0xx_hal_flash_ex.c

ALLINC += $(STM32L0_HAL)/Inc \
          $(CMSIS)/Include \
          $(CMSIS)/Device/ST/STM32L0xx/Include/

# Silence GCC 5 warnings
UDEFS += -DPORT_IGNORE_GCC_VERSION_CHECK

#
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
#(should contain halconf.h, mcuconf.h, chconf.h & stm32l0xx_hal_conf)
CONFDIR = ../common/conf

######

include ../common/Makefile