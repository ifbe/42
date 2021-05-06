project=$(shell pwd)
library=../../library
PROGRAM=test
CC=arm-none-eabi-gcc
CF=-mthumb -mcpu=cortex-m3
LD=arm-none-eabi-ld
LF=-fno-exceptions -ffunction-sections -fdata-sections -nostartfiles -Wl,--gc-sections,-Tlink.ld

all:libboot0
	$(LD) $(LF) -o $(PROGRAM).elf libboot0/*
	arm-none-eabi-objcopy -O ihex $(PROGRAM).elf $(PROGRAM).hex
	arm-none-eabi-objcopy -O binary $(PROGRAM).elf $(PROGRAM).bin
	arm-none-eabi-readelf -a $(PROGRAM).elf > $(PROGRAM).info_elf
	arm-none-eabi-size -d -B -t $(PROGRAM).elf > $(PROGRAM).info_size
	arm-none-eabi-objdump -S $(PROGRAM).elf > $(PROGRAM).info_code
	arm-none-eabi-nm -t d -S --size-sort -s $(PROGRAM).elf > $(PROGRAM).info_symbol
flash:
	sudo stm32flash -w $(PROGRAM).bin -v -g 0x00 /dev/ttyUSB0
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) stm32
clean:
	rm -rf lib*/*.o
	rm -f *.o *.EFI
	make -C $(library) clean
