CC=x86_64-elf-gcc
CF=-c -O2 -fPIC
LD=x86_64-elf-ld
LF=-T link.ld


all:
	$(CC) $(CF) start.c
	$(CC) $(CF) ../main.c
	$(LD) $(LF) start.o main.o
clean:
	rm -f *.o *.out *.bin
