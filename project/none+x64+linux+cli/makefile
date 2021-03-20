project?=$(shell pwd)
library?=../../library
CC=gcc
CF="-c -O2 -fno-stack-protector -Wno-address-of-packed-member -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	+make -s exe
	+make -s lib
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
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) linuxcli
.PHONY:libuser1
libuser1:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF)
lib:
	rm -f $(library)/libboot0/entry.o
	ar crs 42.a \
	$(library)/libboot0/*.o libboot1/*.o \
	$(library)/libhard0/*.o libhard1/*.o \
	$(library)/libsoft0/*.o libsoft1/*.o \
	$(library)/libuser0/*.o libuser1/*.o
	gcc -shared -o 42.so \
	$(library)/libboot0/*.o libboot1/*.o \
	$(library)/libhard0/*.o libhard1/*.o \
	$(library)/libsoft0/*.o libsoft1/*.o \
	$(library)/libuser0/*.o libuser1/*.o
exe:
	gcc -o a.out \
	$(library)/libboot0/*.o libboot1/*.o \
	$(library)/libhard0/*.o libhard1/*.o \
	$(library)/libsoft0/*.o libsoft1/*.o \
	$(library)/libuser0/*.o libuser1/*.o \
	-lpulse -lpulse-simple -lpthread -lm
	#-lasound -lpthread
clean:
	rm -f *.o *.a *.so
	make -C $(library) clean
