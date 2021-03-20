project?=$(shell pwd)
library?=../../library
CC=clang
CF="-c -O2 -fno-stack-protector -Wno-address-of-packed-member -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s exe
	make -s lib
	cp a.out ../..
libboot0:
	mkdir -p boot0
	make -s -C $(library)/libboot0 DIR=$(project)/boot0 CC=$(CC) CF=$(CF) none
libboot1:
	mkdir -p boot1
	make -s -C $(library)/libboot1 DIR=$(project)/boot1 CC=$(CC) CF=$(CF)
libhard0:
	mkdir -p hard0
	make -s -C $(library)/libhard0 DIR=$(project)/hard0 CC=$(CC) CF=$(CF) macx64
libhard1:
	mkdir -p hard1
	make -s -C $(library)/libhard1 DIR=$(project)/hard1 CC=$(CC) CF=$(CF)
libsoft0:
	mkdir -p soft0
	make -s -C $(library)/libsoft0 DIR=$(project)/soft0 CC=$(CC) CF=$(CF) mac
libsoft1:
	mkdir -p soft1
	make -s -C $(library)/libsoft1 DIR=$(project)/soft1 CC=$(CC) CF=$(CF)
libuser0:
	mkdir -p user0
	make -s -C $(library)/libuser0 DIR=$(project)/user0 CC=$(CC) CF=$(CF) macglfw
libuser1:
	mkdir -p user1
	make -s -C $(library)/libuser1 DIR=$(project)/user1 CC=$(CC) CF=$(CF)
lib:
	rm -f $(library)/libboot0/entry.o
	libtool -static -o 42.a \
	$(library)/libboot0/*.o boot1/*.o \
	$(library)/libhard0/*.o hard1/*.o \
	$(library)/libsoft0/*.o soft1/*.o \
	$(library)/libuser0/*.o user1/*.o 2>/dev/null
	#libtool -dynamic -macosx_version_min 11.0 -o 42.dylib \
	$(library)/libboot0/*.o $(library)/libboot1/*.o \
	$(library)/libhard0/*.o $(library)/libhard1/*.o \
	$(library)/libsoft0/*.o $(library)/libsoft1/*.o \
	$(library)/libuser0/*.o $(library)/libuser1/*.o \
	-lGLEW -lglfw -lc -lm -framework OpenGL -framework AudioToolbox
exe:
	clang \
	$(library)/libboot0/*.o boot1/*.o \
	$(library)/libhard0/*.o hard1/*.o \
	$(library)/libsoft0/*.o soft1/*.o \
	$(library)/libuser0/*.o user1/*.o \
	-lGLEW -lglfw -lm -framework OpenGL -framework AudioToolbox
clean:
	rm -f */*.o
	rm -f *.exe *.out *.o *.a *.dylib
	make -C $(library) clean
