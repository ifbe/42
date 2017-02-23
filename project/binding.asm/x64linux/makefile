library?=../../library
res?=../../res
cli:
	nasm cli.s -f elf64 -o cli.o
	gcc cli.o 42.a -lpulse -lpulse-simple -lpthread
gui:
	nasm gui.s -f elf64 -o gui.o
	gcc gui.o 42.a -lpulse -lpulse-simple -lpthread -lX11
clean:
	rm -f *.o *.a *.exe
