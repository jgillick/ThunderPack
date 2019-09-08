RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC
include $(RULESPATH)/rules.mk

# Flash to the device using dfu-util
flash-dfu: all
	@echo Flashing via DFU
	dfu-util -d 0483:df11 -a 0 -s 0x08000000:leave -D ./build/program.bin