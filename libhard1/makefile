binary:
	gcc -c -fPIC -O2 convert.c
	gcc -c -fPIC -O2 cmpsb.c
	rm -f libhard1.a
	ar crs libhard1.a convert.o cmpsb.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 convert.c
	x86_64-elf-gcc -c -fPIC -O2 cmpsb.c
	rm -f libhard1.a
	ar crs libhard1.a convert.o cmpsb.o
linux:
	gcc -c -fPIC -O2 convert.c
	gcc -c -fPIC -O2 cmpsb.c
	rm -f libhard1.a
	ar crs libhard1.a convert.o cmpsb.o
mac:
	clang -c -fPIC -O2 convert.c
	clang -c -fPIC -O2 cmpsb.c
	rm -f libhard1.a
	ar crs libhard1.a convert.o cmpsb.o
win:
	gcc -c -O2 convert.c
	gcc -c -O2 cmpsb.c
	rm -f libhard1.a
	ar crs libhard1.a convert.o cmpsb.o
clean:
	rm -f *.o *.a
