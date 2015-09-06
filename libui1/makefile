win:
	gcc -c -O2 0-menu.c
	gcc -c -O2 1-hex.c
	gcc -c -O2 1-console.c
	gcc -c -O2 2-real.c
	gcc -c -O2 2-logic.c
	gcc -c -O2 3-d.c
	gcc -c -O2 3-jiong.c
	rm -f libui1.a
	ar crs libui1.a 0-menu.o 1-hex.o 1-console.o 2-real.o 2-logic.o 3-d.o 3-jiong.o
clean:
	rm -f *.o *.a
