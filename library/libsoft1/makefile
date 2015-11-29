binary:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 anscii/anscii.c
	gcc -c -fPIC -O2 anscii/anscii.convert.c
	gcc -c -fPIC -O2 calc/calc.c
	gcc -c -fPIC -O2 calc/stack.c
	gcc -c -fPIC -O2 partitiontable/gpt.c
	gcc -c -fPIC -O2 partitiontable/mbr.c
	gcc -c -fPIC -O2 ext.c
	gcc -c -fPIC -O2 fat.c
	gcc -c -fPIC -O2 hfs.c
	gcc -c -fPIC -O2 ntfs.c
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 master.c
	x86_64-elf-gcc -c -fPIC -O2 anscii/anscii.c
	x86_64-elf-gcc -c -fPIC -O2 anscii/anscii.convert.c
	x86_64-elf-gcc -c -fPIC -O2 calc/calc.c
	x86_64-elf-gcc -c -fPIC -O2 calc/stack.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/gpt.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/mbr.c
	x86_64-elf-gcc -c -fPIC -O2 ext.c
	x86_64-elf-gcc -c -fPIC -O2 fat.c
	x86_64-elf-gcc -c -fPIC -O2 hfs.c
	x86_64-elf-gcc -c -fPIC -O2 ntfs.c
linux:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 anscii/anscii.c
	gcc -c -fPIC -O2 anscii/anscii.convert.c
	gcc -c -fPIC -O2 calc/calc.c
	gcc -c -fPIC -O2 calc/stack.c
	gcc -c -fPIC -O2 partitiontable/gpt.c
	gcc -c -fPIC -O2 partitiontable/mbr.c
	gcc -c -fPIC -O2 filesystem/ext.c
	gcc -c -fPIC -O2 filesystem/fat.c
	gcc -c -fPIC -O2 filesystem/hfs.c
	gcc -c -fPIC -O2 filesystem/ntfs.c
mac:
	clang -c -fPIC -O2 master.c
	clang -c -fPIC -O2 anscii/anscii.c
	clang -c -fPIC -O2 anscii/anscii.convert.c
	clang -c -fPIC -O2 calc/calc.c
	clang -c -fPIC -O2 calc/stack.c
	clang -c -fPIC -O2 partitiontable/gpt.c
	clang -c -fPIC -O2 partitiontable/mbr.c
	clang -c -fPIC -O2 filesystem/ext.c
	clang -c -fPIC -O2 filesystem/fat.c
	clang -c -fPIC -O2 filesystem/hfs.c
	clang -c -fPIC -O2 filesystem/ntfs.c
win:
	gcc -c -O2 master.c
	gcc -c -O2 anscii/anscii.c
	gcc -c -O2 anscii/anscii.convert.c
	gcc -c -O2 calc/calc.c
	gcc -c -O2 calc/stack.c
	gcc -c -O2 partitiontable/gpt.c
	gcc -c -O2 partitiontable/mbr.c
	gcc -c -O2 filesystem/ext.c
	gcc -c -O2 filesystem/fat.c
	gcc -c -O2 filesystem/hfs.c
	gcc -c -O2 filesystem/ntfs.c
clean:
	rm -f *.o *.a
