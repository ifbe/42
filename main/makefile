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
	gcc -c -O2 gui/showreal0.c
	gcc -c -O2 gui/showreal1.c
	gcc -c -O2 gui/showreal2.c
	gcc -c -O2 gui/showreal3.c
	gcc -c -O2 gui/showlogic0.c
	rm -f main.a
	ar crs main.a gui.o console.o disk.o showreal0.o showlogic0.o
clean:
	rm -f *.o *.a
