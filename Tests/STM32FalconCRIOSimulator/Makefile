BUILD_DIR=Build

LIBRARIES_STATIC=$(BUILD_DIR)/Drivers.a
LIBRARIES_STATIC+=$(BUILD_DIR)/CMSIS.a
LIBRARIES_STATIC+=$(BUILD_DIR)/Main.a
LIBRARIES_STATIC+=$(BUILD_DIR)/startup_stm32f746xx.o

all: clean_hex other $(BUILD_DIR)/output.hex

clean_hex:
	rm -f Build/output.*

other:
	make -C Drivers/STM32F7xx_HAL_Driver/Src/
	make -C Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/
	make -C Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/
	make -C Src/

include Makefile.inc

