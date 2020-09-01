library?=../../library
CF="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	+make -s a
	+make -s so
	+make -s program
libboot0:
	+make -s -C $(library)/libboot0 CF=$(CF) none
libboot1:
	+make -s -C $(library)/libboot1 CF=$(CF)
libhard0:
	+make -s -C $(library)/libhard0 CF=$(CF) arm
libhard1:
	+make -s -C $(library)/libhard1 CF=$(CF)
libsoft0:
	+make -s -C $(library)/libsoft0 CF=$(CF) linux
libsoft1:
	+make -s -C $(library)/libsoft1 CF=$(CF)
libuser0:
	+make -s -C $(library)/libuser0 CF=$(CF) linuxglfw
libuser1:
	+make -s -C $(library)/libuser1 CF=$(CF)
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
	-lglfw -lGLEW -lGLU -lGL -lpulse -lpulse-simple -lpthread -lm
	#-lasound -lpthread -lm
clean:
	rm -f *.o *.a *.so
