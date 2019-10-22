library?=../../library
flag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	+make -s a
	+make -s so
	+make -s program
	cp a.out ../..
libboot0:
	+make -s -C $(library)/libboot0 cflag=$(flag) rpi
libboot1:
	+make -s -C $(library)/libboot1 cflag=$(flag)
libhard0:
	+make -s -C $(library)/libhard0 cflag=$(flag) linuxarm
libhard1:
	+make -s -C $(library)/libhard1 cflag=$(flag)
libsoft0:
	+make -s -C $(library)/libsoft0 cflag=$(flag) linux
libsoft1:
	+make -s -C $(library)/libsoft1 cflag=$(flag)
libuser0:
	+make -s -C $(library)/libuser0 cflag=$(flag) linuxcli
libuser1:
	+make -s -C $(library)/libuser1 cflag=$(flag)
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
	-lwiringPi -lpulse -lpulse-simple -lpthread -lm
	#-lasound -lpthread -lm
clean:
	make -C $(library) clean
	rm -f *.o *.a *.so
