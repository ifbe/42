library?=../../library
res?=../../res
all:
	nasm 42.s -f win64 -o 42.o
	gcc 42.o 42.a -lgdi32 -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.exe
