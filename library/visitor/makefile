linuxstatic:
	rm -f *.o
	gcc -c -fPIC -O2 temple/temple.abbot.c
	gcc -c -fPIC -O2 temple/temple.affair.c
	gcc -c -fPIC -O2 temple/temple.monk.c
linuxshared:
	rm -f *.o
	gcc -c -fPIC -O2 temple/temple.abbot.c
	gcc -c -fPIC -O2 temple/temple.affair.c
	gcc -c -fPIC -O2 temple/temple.monk.c
winstatic:
	rm -f *.o
	gcc -c -O2 temple/temple.abbot.c
	gcc -c -O2 temple/temple.affair.c
	gcc -c -O2 temple/temple.monk.c
winshared:
	rm -f *.o
	gcc -c -O2 temple/temple.abbot.c
	gcc -c -O2 temple/temple.affair.c
	gcc -c -O2 temple/temple.monk.c
cli:
	rm -f *.o
	gcc -c temple/temple.abbot.c
	gcc -c temple/temple.affair.c
	gcc -c temple/temple.monk.c
	gcc -c traveller/cli.c
gui:
	rm -f *.o
	gcc -c temple/temple.abbot.c
	gcc -c temple/temple.affair.c
	gcc -c temple/temple.monk.c
	gcc -c traveller/gui.c
web:
	rm -f *.o
	gcc -c temple/temple.abbot.c
	gcc -c temple/temple.affair.c
	gcc -c temple/temple.monk.c
	gcc -c traveller/web.c
clean:
	rm -f *.o
