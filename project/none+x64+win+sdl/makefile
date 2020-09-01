library?=../../library
datafile?=../../datafile
CF="-c -O2 -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s exe
	make -s lib
	cp a.exe ../..
libboot0:
	make -s -C $(library)/libboot0 CF=$(CF) wincmd
libboot1:
	make -s -C $(library)/libboot1 CF=$(CF)
libhard0:
	make -s -C $(library)/libhard0 CF=$(CF) winx64
libhard1:
	make -s -C $(library)/libhard1 CF=$(CF)
libsoft0:
	make -s -C $(library)/libsoft0 CF=$(CF) win
libsoft1:
	make -s -C $(library)/libsoft1 CF=$(CF)
libuser0:
	make -s -C $(library)/libuser0 CF=$(CF) winsdl
libuser1:
	make -s -C $(library)/libuser1 CF=$(CF)
lib:
	rm -f $(library)/libboot0/entry.o
	ar crs 42.a \
	$(library)/libboot0/*.o $(library)/libboot1/*.o \
	$(library)/libhard0/*.o $(library)/libhard1/*.o \
	$(library)/libsoft0/*.o $(library)/libsoft1/*.o \
	$(library)/libuser0/*.o $(library)/libuser1/*.o
	g++ -shared -o 42.dll \
	$(library)/libboot0/*.o $(library)/libboot1/*.o \
	$(library)/libhard0/*.o $(library)/libhard1/*.o \
	$(library)/libsoft0/*.o $(library)/libsoft1/*.o \
	$(library)/libuser0/*.o $(library)/libuser1/*.o \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid -lws2_32 -lstrmiids -lxinput
exe:
	windres --input-format=rc -O coff -i $(datafile)/42.rc -o $(datafile)/42.res
	g++ -mwindows $(datafile)/winres/42.res \
	$(library)/libboot0/*.o $(library)/libboot1/*.o \
	$(library)/libhard0/*.o $(library)/libhard1/*.o \
	$(library)/libsoft0/*.o $(library)/libsoft1/*.o \
	$(library)/libuser0/*.o $(library)/libuser1/*.o \
	-Wl,--no-undefined -static -static-libgcc \
	-lmingw32 -lSDL2main -lSDL2 -lsetupapi -lhid \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid -lws2_32 -lstrmiids -lxinput
clean:
	rm -f *.o *.a *.so
