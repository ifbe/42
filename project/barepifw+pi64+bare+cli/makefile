library?=../../library
AS=aarch64-elf-as
AF="-c"
CC=aarch64-elf-gcc
CF="-c -O2 -I../lib1 -fPIC -ffreestanding -fno-stack-protector"
LD=aarch64-elf-ld
LF=-T link.ld -o 42.elf

all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	mv $(library)/libboot0/entry.o .
	$(LD) $(LF) entry.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o
	aarch64-elf-objcopy 42.elf -O binary kernel8.img
libboot0:
	make -s -C $(library)/libboot0 AS=$(AS) CC=$(CC) CF=$(CF) barerpi
libboot1:
	make -s -C $(library)/libboot1 AS=$(AS) CC=$(CC) CF=$(CF)
libhard0:
	make -s -C $(library)/libhard0 AS=$(AS) CC=$(CC) CF=$(CF) pi64bare
libhard1:
	make -s -C $(library)/libhard1 AS=$(AS) CC=$(CC) CF=$(CF)
libsoft0:
	make -s -C $(library)/libsoft0 AS=$(AS) CC=$(CC) CF=$(CF) bare
libsoft1:
	make -s -C $(library)/libsoft1 AS=$(AS) CC=$(CC) CF=$(CF)
libuser0:
	make -s -C $(library)/libuser0 AS=$(AS) CC=$(CC) CF=$(CF) pibarecli
libuser1:
	make -s -C $(library)/libuser1 AS=$(AS) CC=$(CC) CF=$(CF)
rpi3:
	qemu-system-aarch64 -M raspi3 -serial null -serial stdio -drive file=../../../live/live.img,if=sd,format=raw -kernel kernel8.img
clean:
	rm -f *.o *.out *.bin *.img
