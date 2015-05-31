.PHONY:win
win:
	gcc -c -O2 cli.c
	rm -f main.a
	ar crs main.a cli.o
.PHONY:gui
gui:
	gcc -c -O2 gui/gui.c
	gcc -c -O2 gui/0-disk.c
	gcc -c -O2 gui/1-real.c
	gcc -c -O2 gui/2-logic.c
	gcc -c -O2 gui/3-hex.c
	gcc -c -O2 gui/4-console.c
	gcc -c -O2 gui/5-overview.c
	rm -f main.a
	ar crs main.a gui.o 0-disk.o 1-real.o 2-logic.o 3-hex.o 4-console.o 5-overview.o
clean:
	rm -f *.o *.a
