.PHONY:none
none:
	rm -f *.o
	gcc -c -fPIC -O2 basic.c
	gcc -c -fPIC -O2 none/serial.rx.c
	gcc -c -fPIC -O2 none/serial.tx.c

.PHONY:arm
arm:
	rm -f *.o
	arm-none-eabi-gcc -c -fPIC -O2 basic.c
	arm-none-eabi-gcc -c -fPIC -O2 none/serial.rx.c
	arm-none-eabi-gcc -c -fPIC -O2 none/serial.tx.c

.PHONY:logcat
logcat:
	rm -f *.o
	gcc -c -fPIC -O2 logcat/logcat.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	gcc -c -O2 wincmd/wincmd.c

.PHONY:efi
efi:
	what

clean:
	rm -f *.o
