library?=../../library
res?=../../res
all:
	nasm 42.s -f win64 -o 42.o
	gcc 42.o 42.a -lpulse -lpulse-simple -lpthread
clean:
	rm -f *.o *.a *.exe
