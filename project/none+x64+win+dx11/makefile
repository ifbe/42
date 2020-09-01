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
	make -s -C $(library)/libuser0 CF=$(CF) windx11
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
	-ld3d11 -ld3dcompiler -lgdi32 -lole32 -lstrmiids -lxinput -lwinmm -lws2_32
exe:
	windres --input-format=rc -O coff -i $(datafile)/winres/42.rc -o $(datafile)/winres/42.res
	g++ -mwindows $(datafile)/winres/42.res \
	$(library)/libboot0/*.o $(library)/libboot1/*.o \
	$(library)/libhard0/*.o $(library)/libhard1/*.o \
	$(library)/libsoft0/*.o $(library)/libsoft1/*.o \
	$(library)/libuser0/*.o $(library)/libuser1/*.o \
	-ld3d11 -ld3dcompiler -lgdi32 -lole32 -lstrmiids -lxinput -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.so
	make -C $(library) clean
