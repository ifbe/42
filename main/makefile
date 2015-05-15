.PHONY:win
win:
	gcc -c -O2 cli.c
	rm -f main.a
	ar crs main.a cli.o
.PHONY:gui
gui:
	gcc -c -O2 gui/gui.c
	gcc -c -O2 gui/console.c
	gcc -c -O2 gui/disk.c
	gcc -c -O2 gui/real0.c
	gcc -c -O2 gui/real1.c
	gcc -c -O2 gui/real2.c
	gcc -c -O2 gui/real3.c
	gcc -c -O2 gui/logic0.c
	rm -f main.a
	ar crs main.a gui.o console.o disk.o real0.o logic0.o
clean:
	rm -f *.o *.a
