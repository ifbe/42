library?=../../library
res?=../../res
all:
	make -s -C $(library)/libboot0 none
	make -s -C $(library)/libboot1 clang
	make -s -C $(library)/libhard0 x64
	make -s -C $(library)/libhard1 clang
	make -s -C $(library)/libsoft0 mac
	make -s -C $(library)/libsoft1 clang
	make -s -C $(library)/libui0 maccli
	make -s -C $(library)/libui1 clang
	make -s -C $(library)/visitor cli
	clang \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor/*.o
static:
	make -s -C $(library)/libboot0 none
	make -s -C $(library)/libboot1 clang
	make -s -C $(library)/libhard0 x64
	make -s -C $(library)/libhard1 clang
	make -s -C $(library)/libsoft0 mac
	make -s -C $(library)/libsoft1 clang
	make -s -C $(library)/libui0 maccli
	make -s -C $(library)/libui1 clang
	make -s -C $(library)/visitor cli
	libtool -static -o library.a \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor/*.o
shared:
	make -s -C $(library)/libboot0 none
	make -s -C $(library)/libboot1 clang
	make -s -C $(library)/libhard0 x64
	make -s -C $(library)/libhard1 clang
	make -s -C $(library)/libsoft0 mac
	make -s -C $(library)/libsoft1 clang
	make -s -C $(library)/libui0 maccli
	make -s -C $(library)/libui1 clang
	make -s -C $(library)/visitor winshared
	libtool -dynamic -o library.dylib \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor/temple.*.o
clean:
	rm -f *.o
