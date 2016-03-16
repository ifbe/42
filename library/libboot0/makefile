.PHONY:none
none:
	rm -f *.o
	gcc -c -fPIC -O2 none/tx.c
	gcc -c -fPIC -O2 basic.c

.PHONY:arm
arm:
	rm -f *.o
	arm-none-eabi-gcc -c -fPIC -O2 none/tx.c
	arm-none-eabi-gcc -c -fPIC -O2 basic.c

.PHONY:logcat
logcat:
	rm -f *.o
	gcc -c -fPIC -O2 logcat/logcat.c
	gcc -c -fPIC -O2 basic.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	gcc -c -O2 wincmd/wincmd.c
	gcc -c -O2 basic.c

.PHONY:efi
efi:

clean:
	rm -f *.o
