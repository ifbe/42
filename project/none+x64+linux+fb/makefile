project?=$(shell pwd)
library?=../../library
CC=gcc
CF="-c -O2 -fPIC -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	+make -s exe
	+make -s lib
	+make -s so
	cp a.out ../..
libboot0:
	+make -s -C $(library)/libboot0 CF=$(CF) none
libboot1:
	+make -s -C $(library)/libboot1 CF=$(CF)
libhard0:
	+make -s -C $(library)/libhard0 CF=$(CF) linuxx64
libhard1:
	+make -s -C $(library)/libhard1 CF=$(CF)
libsoft0:
	+make -s -C $(library)/libsoft0 CF=$(CF) linux
libsoft1:
	+make -s -C $(library)/libsoft1 CF=$(CF)
libuser0:
	+make -s -C $(library)/libuser0 CF=$(CF) linuxfb
libuser1:
	+make -s -C $(library)/libuser1 CF=$(CF)
lib:
	rm -f libboot0/entry.o
	ar crs 42.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o
so:
	gcc -shared -o 42.so \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o
exe:
	gcc -o a.out \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lpulse -lpulse-simple -lpthread -lm
clean:
	rm -f *.o *.a *.so
	make -C $(library) clean
