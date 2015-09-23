binary:
	gcc -c -fPIC -O2 calc.c
	gcc -c -fPIC -O2 cmpsb.c
	gcc -c -fPIC -O2 convert.c
	gcc -c -fPIC -O2 holyshit.c
	gcc -c -fPIC -O2 memory.c
	gcc -c -fPIC -O2 math.c
	rm -f libsoft.a
	ar crs libsoft.a calc.o convert.o cmpsb.o holyshit.o memory.o math.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 calc.c
	x86_64-elf-gcc -c -fPIC -O2 cmpsb.c
	x86_64-elf-gcc -c -fPIC -O2 convert.c
	x86_64-elf-gcc -c -fPIC -O2 holyshit.c
	x86_64-elf-gcc -c -fPIC -O2 memory.c
	x86_64-elf-gcc -c -fPIC -O2 math.c
	rm -f libsoft.a
	ar crs libsoft.a calc.o convert.o cmpsb.o holyshit.o memory.o math.o
linux:
	gcc -c -fPIC -O2 calc.c
	gcc -c -fPIC -O2 cmpsb.c
	gcc -c -fPIC -O2 convert.c
	gcc -c -fPIC -O2 holyshit.c
	gcc -c -fPIC -O2 memory.c
	gcc -c -fPIC -O2 math.c
	rm -f libsoft.a
	ar crs libsoft.a calc.o convert.o cmpsb.o holyshit.o memory.o math.o
mac:
	clang -c -fPIC -O2 calc.c
	clang -c -fPIC -O2 cmpsb.c
	clang -c -fPIC -O2 convert.c
	clang -c -fPIC -O2 holyshit.c
	clang -c -fPIC -O2 memory.c
	clang -c -fPIC -O2 math.c
	rm -f libsoft.a
	ar crs libsoft.a calc.o convert.o cmpsb.o holyshit.o memory.o math.o
win:
	gcc -c -O2 calc.c
	gcc -c -O2 cmpsb.c
	gcc -c -O2 convert.c
	gcc -c -O2 holyshit.c
	gcc -c -O2 memory.c
	gcc -c -O2 math.c
	rm -f libsoft.a
	ar crs libsoft.a calc.o convert.o cmpsb.o holyshit.o memory.o math.o
clean:
	rm -f *.o *.a
