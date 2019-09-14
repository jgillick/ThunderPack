# Source files to compile
CPPSRC = LEDController.cpp \
         ProgramController.cpp \
         main.cpp

#
# Define the environment
#

# ChibiOS lib directory
CHIBIOS = ../common/ChibiOS

# Directory with board.[ch] files
BOARD_DIR = ../common/board

# Drivers directory, from STM32CubMX
# (should contain "CMSIS" & "STM32L0xx_HAL_Driver" directories)
# CUBEMX_DRIVERS = ../common/Drivers

# HAL driver folder root 
# (should have an "Inc" directory inside of it)
HAL_DRIVER = ../common/STM32L0xx_HAL_Driver

# Path to CMSIS directory 
# (should contain "Device" & "Include" directories)
CMSIS = ../common/CMSIS

# Conf directory. 
#(should contain halconf.h, mcuconf.h, chconf.h & stm32l0xx_hal_conf)
CONFDIR = ../common/conf

######

#
# Dependencies
#

# Required for readint/writing to internal EEPROM
ALLCSRC +=  $(BOARD_DIR)/board.c \
            $(HAL_DRIVER)/Src/stm32l0xx_hal.c \
            $(HAL_DRIVER)/Src/stm32l0xx_hal_flash_ramfunc.c \
            $(HAL_DRIVER)/Src/stm32l0xx_hal_flash.c \
            $(HAL_DRIVER)/Src/stm32l0xx_hal_flash_ex.c \
            $(HAL_DRIVER)/Src/stm32l0xx_hal_cortex.c

include ../common/Makefile