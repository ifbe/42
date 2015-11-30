binary:
	gcc -c -fPIC -O2 algorithm/stack.c
	gcc -c -fPIC -O2 anscii/anscii.c
	gcc -c -fPIC -O2 calc/calc.c
	gcc -c -fPIC -O2 filesystem/ext.c
	gcc -c -fPIC -O2 filesystem/fat.c
	gcc -c -fPIC -O2 filesystem/hfs.c
	gcc -c -fPIC -O2 filesystem/ntfs.c
	gcc -c -fPIC -O2 partitiontable/gpt.c
	gcc -c -fPIC -O2 partitiontable/mbr.c
	gcc -c -fPIC -O2 master.c
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 algorithm/stack.c
	x86_64-elf-gcc -c -fPIC -O2 anscii/anscii.c
	x86_64-elf-gcc -c -fPIC -O2 calc/calc.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/ext.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/fat.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/hfs.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/ntfs.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/gpt.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/mbr.c
	x86_64-elf-gcc -c -fPIC -O2 master.c
linux:
	gcc -c -fPIC -O2 algorithm/stack.c
	gcc -c -fPIC -O2 anscii/anscii.c
	gcc -c -fPIC -O2 calc/calc.c
	gcc -c -fPIC -O2 filesystem/ext.c
	gcc -c -fPIC -O2 filesystem/fat.c
	gcc -c -fPIC -O2 filesystem/hfs.c
	gcc -c -fPIC -O2 filesystem/ntfs.c
	gcc -c -fPIC -O2 partitiontable/gpt.c
	gcc -c -fPIC -O2 partitiontable/mbr.c
	gcc -c -fPIC -O2 master.c
mac:
	clang -c -fPIC -O2 algorithm/stack.c
	clang -c -fPIC -O2 anscii/anscii.c
	clang -c -fPIC -O2 calc/calc.c
	clang -c -fPIC -O2 filesystem/ext.c
	clang -c -fPIC -O2 filesystem/fat.c
	clang -c -fPIC -O2 filesystem/hfs.c
	clang -c -fPIC -O2 filesystem/ntfs.c
	clang -c -fPIC -O2 partitiontable/gpt.c
	clang -c -fPIC -O2 partitiontable/mbr.c
	clang -c -fPIC -O2 master.c
win:
	gcc -c -O2 algorithm/stack.c
	gcc -c -O2 anscii/anscii.c
	gcc -c -O2 calc/calc.c
	gcc -c -O2 filesystem/ext.c
	gcc -c -O2 filesystem/fat.c
	gcc -c -O2 filesystem/hfs.c
	gcc -c -O2 filesystem/ntfs.c
	gcc -c -O2 partitiontable/gpt.c
	gcc -c -O2 partitiontable/mbr.c
	gcc -c -O2 master.c
clean:
	rm -f *.o *.a
