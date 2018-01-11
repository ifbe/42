library?=../../library
all:
	make -s -C $(library)/libboot0 none \
	cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
	make -s -C $(library)/libboot1 \
	cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
	make -s -C $(library)/libhard0 arm \
	cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
	make -s -C $(library)/libhard1 \
	cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
	make -s -C $(library)/libsoft0 linux \
	cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
	make -s -C $(library)/libsoft1 \
	cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
	make -s -C $(library)/libuser0 linuxcli \
	cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
	make -s -C $(library)/libuser1 \
	cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
	make -s a
	make -s so
	make -s program
a:
	ar crs 42.a \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o
so:
	gcc -shared -o 42.so \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o
program:
	gcc -o a.out \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-lpulse -lpulse-simple -lpthread -lm
	#-lasound -lpthread -lm
clean:
	rm -f *.o *.a *.so
