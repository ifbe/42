binary:
	gcc -c -fPIC -O2 cmpsb.c
	gcc -c -fPIC -O2 convert.c
	gcc -c -fPIC -O2 holyshit.c
	gcc -c -fPIC -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a convert.o cmpsb.o holyshit.o memory.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 cmpsb.c
	x86_64-elf-gcc -c -fPIC -O2 convert.c
	x86_64-elf-gcc -c -fPIC -O2 holyshit.c
	x86_64-elf-gcc -c -fPIC -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a convert.o cmpsb.o holyshit.o memory.o
linux:
	gcc -c -fPIC -O2 cmpsb.c
	gcc -c -fPIC -O2 convert.c
	gcc -c -fPIC -O2 holyshit.c
	gcc -c -fPIC -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a convert.o cmpsb.o holyshit.o memory.o
mac:
	clang -c -fPIC -O2 cmpsb.c
	clang -c -fPIC -O2 convert.c
	clang -c -fPIC -O2 holyshit.c
	clang -c -fPIC -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a convert.o cmpsb.o holyshit.o memory.o
win:
	gcc -c -O2 cmpsb.c
	gcc -c -O2 convert.c
	gcc -c -O2 holyshit.c
	gcc -c -O2 memory.c
	rm -f libsoft.a
	ar crs libsoft.a convert.o cmpsb.o holyshit.o memory.o
clean:
	rm -f *.o *.a
