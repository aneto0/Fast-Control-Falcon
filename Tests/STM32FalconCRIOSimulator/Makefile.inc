LINKER_LD_FILE=STM32F746ZGTx_FLASH.ld

COMPILER = arm-none-eabi-gcc
LINKER = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

CFLAGS =  -DSTM32F746xx -MD -MP -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mlittle-endian -mthumb -mthumb-interwork -fno-builtin -ffunction-sections -fdata-sections -g -Wall
LFLAGS =-mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -falign-functions=16 -lm -lgcc -lc -lnosys -specs=nosys.specs -Wl,--gc-section -g


INCLUDES =-I.
INCLUDES +=-I../../../Inc/ 
INCLUDES += -I../Inc/ 
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Include/ 
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/STM32F7xx_HAL_Driver/Inc/ 
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/STM32F7xx/Include/

C_FILES = $(wildcard *.c)
OBJS := $(C_FILES:%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o : %.c
	mkdir -p $(BUILD_DIR)
	$(COMPILER) -c $(OPTIM) $(INCLUDES) $(CFLAGS) $*.c -o $(BUILD_DIR)/$*.o

$(BUILD_DIR)/%.S : %.s
	mkdir -p $(BUILD_DIR)
	$(COMPILER) -c $(OPTIM) $(INCLUDES) $(CFLAGS) $*.s -o $(BUILD_DIR)/$*.o

$(BUILD_DIR)/%.a : $(OBJS) 
	mkdir -p $(BUILD_DIR)
	touch $@
	rm -f $(BUILD_DIR)/$*.a
	$(LINKER) -Ur $(OBJS) $(LIBRARIES_STATIC) -o $@

$(BUILD_DIR)/%.elf: $(OBJS) 
	$(COMPILER) -T $(LINKER_LD_FILE) $(LFLAGS) $(OBJS) $(LIBRARIES_STATIC) -o $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -Oihex  $< $@

$(BUILD_DIR)/%.lst: $(BUILD_DIR)/%.elf
	$(OBJDUMP) -d $^ > $@

$(BUILD_DIR)/%.sym: $(BUILD_DIR)/%.elf
	$(NM)  $^ | sort > $@

clean: 
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/*.a
	rm -f $(BUILD_DIR)/*.d
	rm -f $(BUILD_DIR)/*.S
	rm -f $(BUILD_DIR)/*.elf

