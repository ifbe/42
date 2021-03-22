library?=../../library
CF="-c -O2 -fno-stack-protector -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s exe
	make -s lib
	cp a.out ../..
libboot0:
	make -s -C $(library)/libboot0 CC="clang" CF=$(CF) none
libboot1:
	make -s -C $(library)/libboot1 CC="clang" CF=$(CF)
libhard0:
	make -s -C $(library)/libhard0 CC="clang" CF=$(CF) macx64
libhard1:
	make -s -C $(library)/libhard1 CC="clang" CF=$(CF)
libsoft0:
	make -s -C $(library)/libsoft0 CC="clang" CF=$(CF) mac
libsoft1:
	make -s -C $(library)/libsoft1 CC="clang" CF=$(CF)
libuser0:
	make -s -C $(library)/libuser0 CC="clang" CF=$(CF) mactui
libuser1:
	make -s -C $(library)/libuser1 CC="clang" CF=$(CF)
lib:
	rm -f libboot0/entry.o
	libtool -static -o 42.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o 2>/dev/null
dylib:
	libtool -dynamic -macosx_version_min 10.15 -o 42.dylib \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lm -lc -framework AudioToolbox
exe:
	clang \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lm -framework AudioToolbox
clean:
	rm -f *.exe *.out *.o *.a *.dylib
	make -C $(library) clean
