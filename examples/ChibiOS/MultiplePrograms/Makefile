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

# Drivers directory, from STM32CubMX
# (should contain "CMSIS" & "STM32L0xx_HAL_Driver" directories)
CUBEMX_DRIVERS = ../common/Drivers

# HAL driver folder root 

# Conf directory. 
#(should contain halconf.h, mcuconf.h, chconf.h & stm32l0xx_hal_conf)
CONFDIR = ../common/conf

##############################################################################

###############################################################################
# Dependencies
#
STM32L0_HAL = $(CUBEMX_DRIVERS)/STM32L0xx_HAL_Driver
CMSIS = $(CUBEMX_DRIVERS)/CMSIS

# Required for readint/writing to internal EEPROM
ALLCSRC +=  $(STM32L0_HAL)/Src/stm32l0xx_hal.c \
            $(STM32L0_HAL)/Src/stm32l0xx_hal_flash_ramfunc.c \
            $(STM32L0_HAL)/Src/stm32l0xx_hal_flash.c \
            $(STM32L0_HAL)/Src/stm32l0xx_hal_flash_ex.c \
            $(STM32L0_HAL)/Src/stm32l0xx_hal_cortex.c

ALLINC += $(STM32L0_HAL)/Inc \
          $(CMSIS)/Include \
          $(CMSIS)/Device/ST/STM32L0xx/Include/

##############################################################################

include ../common/Makefile