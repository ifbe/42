library?=../../library
res?=../../res
cli:
	nasm cli.s -f win64 -o cli.o
	gcc cli.o 42.a -lwinmm -lws2_32
gui:
	nasm gui.s -f win64 -o gui.o
	gcc gui.o 42.a -lgdi32 -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.exe
