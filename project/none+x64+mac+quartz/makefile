library?=../../library
CF="-c -O2 -fno-stack-protector -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s a
	make -s dylib
	make -s program
	cp a.out ../..
libboot0:
	make -s -C $(library)/libboot0 CC="clang" CF=$(CF) none
libboot1:
	make -s -C $(library)/libboot1 CC="clang" CF=$(CF)
libhard0:
	make -s -C $(library)/libhard0 CC="clang" CF=$(CF) x64
libhard1:
	make -s -C $(library)/libhard1 CC="clang" CF=$(CF)
libsoft0:
	make -s -C $(library)/libsoft0 CC="clang" CF=$(CF) mac
libsoft1:
	make -s -C $(library)/libsoft1 CC="clang" CF=$(CF)
libuser0:
	make -s -C $(library)/libuser0 CC="clang" CF=$(CF) macquartz
libuser1:
	make -s -C $(library)/libuser1 CC="clang" CF=$(CF)
a:
	libtool -static -o 42.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o
dylib:
	libtool -dynamic -o 42.dylib \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o
program:
	clang -framework Foundation \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lm
clean:
	rm -f *.o *.a *.dylib
