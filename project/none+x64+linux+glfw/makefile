project?=$(shell pwd)
library?=../../library
CC=gcc
CF="-c -O2 -fPIC -fno-stack-protector -Wno-address-of-packed-member -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	+make -s exe
	+make -s lib
	+make -s so
	cp a.out ../..
.PHONY:libboot0
libboot0:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) none
.PHONY:libboot1
libboot1:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF)
.PHONY:libhard0
libhard0:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) linuxx64
.PHONY:libhard1
libhard1:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF)
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) linux
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF)
.PHONY:libuser0
libuser0:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) linuxglfw
.PHONY:libuser1
libuser1:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF)
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
	-lglfw3 -lGLEW -lGLU -lGL \
	-lpulse -lpulse-simple \
	-lpthread -lX11 -ldl -lm
clean:
	rm -f *.o *.a *.so
	rm -f */*.o
	make -C $(library) clean
