binary:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 background.c
	gcc -c -fPIC -O2 holyshit.c
	gcc -c -fPIC -O2 printmemory.c
	rm -f libsoft.a
	ar crs libsoft.a anscii.o background.o holyshit.o printmemory.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 anscii.c
	x86_64-elf-gcc -c -fPIC -O2 background.c
	x86_64-elf-gcc -c -fPIC -O2 holyshit.c
	x86_64-elf-gcc -c -fPIC -O2 printmemory.c
	rm -f libsoft.a
	ar crs libsoft.a anscii.o background.o holyshit.o printmemory.o
linux:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 background.c
	gcc -c -fPIC -O2 holyshit.c
	gcc -c -fPIC -O2 printmemory.c
	rm -f libsoft.a
	ar crs libsoft.a anscii.o background.o holyshit.o printmemory.o
mac:
	clang -c -fPIC -O2 anscii.c
	clang -c -fPIC -O2 background.c
	clang -c -fPIC -O2 holyshit.c
	clang -c -fPIC -O2 printmemory.c
	rm -f libsoft.a
	ar crs libsoft.a anscii.o background.o holyshit.o printmemory.o
win:
	gcc -c -O2 anscii.c
	gcc -c -O2 background.c
	gcc -c -O2 holyshit.c
	gcc -c -O2 printmemory.c
	rm -f libsoft.a
	ar crs libsoft.a anscii.o background.o holyshit.o printmemory.o
clean:
	rm -f *.o *.a
