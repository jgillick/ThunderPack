#
# STM32 Standard Peripheral Library and other shared files
# used to compile the examples.
#

# Get the path to this directory
THIS_FILE := $(lastword $(MAKEFILE_LIST))
SHARED_DIR := $(realpath $(dir $(abspath $(THIS_FILE))))

# Config directory
ifeq ($(CONFDIR),)
  CONFDIR = $(SHARED_DIR)/conf
endif

# Includes
ALLINC += $(SHARED_DIR) \
          $(CONFDIR) \
					$(SHARED_DIR)/STM32L0xx_HAL_Driver/Inc \
					$(SHARED_DIR)/CMSIS/Device/ST/STM32L0xx/Include \
					$(SHARED_DIR)/CMSIS/Include

# C Sources
ALLCSRC +=  $(SHARED_DIR)/board.c \
            $(SHARED_DIR)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal.c \
            $(SHARED_DIR)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ramfunc.c \
            $(SHARED_DIR)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash.c \
            $(SHARED_DIR)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ex.c \
            $(SHARED_DIR)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cortex.c