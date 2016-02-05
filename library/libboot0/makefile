.PHONY:none
none:
	rm -f *.o
	gcc -c -O2 none/basic.c
	gcc -c -O2 none/serial.rx.c
	gcc -c -O2 none/serial.tx.c

.PHONY:winparasitize
winparasitize:
	rm -f *.o
	gcc -c -O2 winparasitize/winparasitize.c

.PHONY:efi
efi:
	what

.PHONY:arm
arm:
	arm-none-eabi-gcc -v

clean:
	rm -f *.o
