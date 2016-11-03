elf64-x86-64:
	objcopy -I binary -O elf64-x86-64 -B i386 unicode.bin unicode.o
pei-x86-64:
	objcopy -I binary -O pei-x86-64 -B i386 unicode.bin unicode.o
clean:
	rm -f uac.res
	rm -f *.o
