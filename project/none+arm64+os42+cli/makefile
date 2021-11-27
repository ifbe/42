CC=aarch64-elf-gcc
CF=-c -O2 -fPIC -ffreestanding -fno-stack-protector
LD=aarch64-elf-ld
LF=-T link.ld


all:
	$(CC) $(CF) start.c
	$(CC) $(CF) main.c
	$(LD) $(LF) start.o main.o
	aarch64-elf-objcopy a.out -O binary app.bin
clean:
	rm -f *.o *.out *.bin
