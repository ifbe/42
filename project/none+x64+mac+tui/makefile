library?=../../library
flag="-c -O2 -fno-stack-protector -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s a
	make -s dylib
	make -s program
	cp a.out ../..
libboot0:
	make -s -C $(library)/libboot0 compiler="clang" cflag=$(flag) none
libboot1:
	make -s -C $(library)/libboot1 compiler="clang" cflag=$(flag)
libhard0:
	make -s -C $(library)/libhard0 compiler="clang" cflag=$(flag) macx64
libhard1:
	make -s -C $(library)/libhard1 compiler="clang" cflag=$(flag)
libsoft0:
	make -s -C $(library)/libsoft0 compiler="clang" cflag=$(flag) mac
libsoft1:
	make -s -C $(library)/libsoft1 compiler="clang" cflag=$(flag)
libuser0:
	make -s -C $(library)/libuser0 compiler="clang" cflag=$(flag) mactui
libuser1:
	make -s -C $(library)/libuser1 compiler="clang" cflag=$(flag)
a:
	libtool -static -o 42.a \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o 2>/dev/null
dylib:
	libtool -dynamic -macosx_version_min 10.15 \
	-o 42.dylib \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-lm -lc -framework AudioToolbox
program:
	clang \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-lm -framework AudioToolbox
clean:
	make -C $(library) clean
	rm -f *.o *.a *.dylib
