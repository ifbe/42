library?=../../library
datafile?=../../datafile
all:
	make -s -C $(library)/init \
	cflag="-c -O2"
	make -s -C $(library)/libboot0 wincmd \
	cflag="-c -O2"
	make -s -C $(library)/libboot1 \
	cflag="-c -O2"
	make -s -C $(library)/libhard0 winx64 \
	cflag="-c -O2"
	make -s -C $(library)/libhard1 \
	cflag="-c -O2"
	make -s -C $(library)/libsoft0 win \
	cflag="-c -O2 -I."
	make -s -C $(library)/libsoft1 \
	cflag="-c -O2 -I."
	make -s -C $(library)/libui0 winsdl \
	cflag="-c -O2 -I."
	make -s -C $(library)/libui1 \
	cflag="-c -O2 -I."
	make -s a
	make -s dll
	make -s program
a:
	ar crs 42.a \
	$(library)/init/startapp.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o
dll:
	g++ -shared -o 42.dll \
	$(library)/init/startapp.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid -lws2_32 -lstrmiids
program:
	windres --input-format=rc -O coff -i $(datafile)/42.rc -o $(datafile)/42.res
	g++ -mwindows \
	$(datafile)/42.res \
	$(library)/init/*.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-Wl,--no-undefined -static -static-libgcc \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid -lws2_32 -lstrmiids
clean:
	rm -f *.o *.a *.so
