binary:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 anscii.convert.c
	gcc -c -fPIC -O2 math.c
	gcc -c -fPIC -O2 math.convert.c
	gcc -c -fPIC -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a \
	anscii.o anscii.convert.o \
	math.o math.convert.o \
	memory.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 anscii.c
	x86_64-elf-gcc -c -fPIC -O2 anscii.convert.c
	x86_64-elf-gcc -c -fPIC -O2 math.c
	x86_64-elf-gcc -c -fPIC -O2 math.convert.c
	x86_64-elf-gcc -c -fPIC -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a \
	anscii.o anscii.convert.o \
	math.o math.convert.o \
	memory.o
linux:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 anscii.convert.c
	gcc -c -fPIC -O2 math.c
	gcc -c -fPIC -O2 math.convert.c
	gcc -c -fPIC -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a \
	math.o math.convert.o \
	anscii.o anscii.convert.o \
	memory.o
mac:
	clang -c -fPIC -O2 anscii.c
	clang -c -fPIC -O2 anscii.convert.c
	clang -c -fPIC -O2 math.c
	clang -c -fPIC -O2 math.convert.c
	clang -c -fPIC -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a \
	anscii.o anscii.convert.o \
	math.o math.convert.o \
	memory.o
win:
	gcc -c -O2 anscii.c
	gcc -c -O2 anscii.convert.c
	gcc -c -O2 math.c
	gcc -c -O2 math.convert.c
	gcc -c -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a \
	anscii.o anscii.convert.o \
	math.o math.convert.o \
	memory.o
clean:
	rm -f *.o *.a
