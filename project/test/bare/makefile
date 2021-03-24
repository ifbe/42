CC=x86_64-elf-gcc
LD=x86_64-elf-ld


all:
	$(CC) -c start.c
	$(CC) -c main.c
	$(LD) -T link.ld start.o main.o
