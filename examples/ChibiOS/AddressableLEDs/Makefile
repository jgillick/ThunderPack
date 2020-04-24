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

###############################################################################
# STM32 Libraries
STM32F4_HAL = ../../STM32Cube/shared/Drivers/STM32F4xx_HAL_Driver
CMSIS =  ../../STM32Cube/shared/Drivers/CMSIS/Device/ST/STM32L4xx

UDEFS = -DSTM32F411xE -DUSE_HAL_DRIVER

CSRC += $(STM32F4_HAL)/Src/stm32f4xx_hal.c \
        $(STM32F4_HAL)/Src/stm32f4xx_hal_flash.c \
        $(STM32F4_HAL)/Src/stm32f4xx_hal_flash_ex.c \
        $(STM32F4_HAL)/Src/stm32f4xx_hal_tim.c \
        $(STM32F4_HAL)/Src/stm32f4xx_hal_tim_ex.c \
        $(STM32F4_HAL)/Src/stm32f4xx_hal_dma.c \
        $(STM32F4_HAL)/Src/stm32f4xx_hal_gpio.c \
        $(STM32F4_HAL)/Src/stm32f4xx_hal_cortex.c

ALLINC += $(STM32F4_HAL)/Inc \
          $(CMSIS)/Include \
          $(CMSIS)/Device/ST/STM32F4xx/Include/

##############################################################################

include ../common/Makefile