.PHONY:none
none:
	rm -f *.o
	gcc -c -fPIC -O2 none/basic.c
	gcc -c -fPIC -O2 none/serial.rx.c
	gcc -c -fPIC -O2 none/serial.tx.c

.PHONY:arm
arm:
	rm -f *.o
	arm-none-eabi-gcc -c -fPIC -O2 none/basic.c
	arm-none-eabi-gcc -c -fPIC -O2 none/serial.rx.c
	arm-none-eabi-gcc -c -fPIC -O2 none/serial.tx.c

.PHONY:logcat
logcat:
	rm -f *.o
	gcc -c -fPIC -O2 logcat/logcat.c

.PHONY:parasitize
parasitize:
	rm -f *.o
	gcc -c -fPIC -O2 parasitize/parasitize.c

.PHONY:efi
efi:
	what

clean:
	rm -f *.o
