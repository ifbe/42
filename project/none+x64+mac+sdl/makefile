library?=../../library
all:
	make -s -C $(library)/libboot0 none \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libboot1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libhard0 x64 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libhard1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libsoft0 mac \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libsoft1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libui0 macsdl \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libui1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s a
	make -s dylib
	make -s program
a:
	ar crs 42.a \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o
dylib:
	libtool -dynamic -macosx_version_min 10.12 \
        -o 42.dylib \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-lSDL2 -lm -lc
program:
	gcc \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-lSDL2 -lm
clean:
	rm -f *.o
