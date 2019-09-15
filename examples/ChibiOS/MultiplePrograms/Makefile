# Source files to compile
ALLCPPSRC = LEDController.cpp \
            ProgramController.cpp \
            main.cpp

# Required for internal EEPROM HAL APIs
ALLCSRC +=  ../shared/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal.c \
            ../shared/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ramfunc.c \
            ../shared/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash.c \
            ../shared/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ex.c \
            ../shared/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cortex.c

# Build options
include ../shared/options.mk

# Build rules
include ../shared/rules.mk