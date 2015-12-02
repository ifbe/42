binary:
	gcc -c -fPIC -O2 algorithm/stack.c
	gcc -c -fPIC -O2 anscii/anscii.c
	gcc -c -fPIC -O2 calc/calc.c
	gcc -c -fPIC -O2 filesystem/fs.ext.c
	gcc -c -fPIC -O2 filesystem/fs.fat.c
	gcc -c -fPIC -O2 filesystem/fs.hfs.c
	gcc -c -fPIC -O2 filesystem/fs.ntfs.c
	gcc -c -fPIC -O2 partitiontable/pt.gpt.c
	gcc -c -fPIC -O2 partitiontable/pt.mbr.c
	gcc -c -fPIC -O2 master.c
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 algorithm/stack.c
	x86_64-elf-gcc -c -fPIC -O2 anscii/anscii.c
	x86_64-elf-gcc -c -fPIC -O2 calc/calc.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/fs.ext.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/fs.fat.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/fs.hfs.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/fs.ntfs.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/pt.gpt.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/pt.mbr.c
	x86_64-elf-gcc -c -fPIC -O2 master.c
gcc:
	gcc -c -O2 algorithm/stack.c
	gcc -c -O2 anscii/anscii.c
	gcc -c -O2 calc/calc.c
	gcc -c -O2 filesystem/fs.ext.c
	gcc -c -O2 filesystem/fs.fat.c
	gcc -c -O2 filesystem/fs.hfs.c
	gcc -c -O2 filesystem/fs.ntfs.c
	gcc -c -O2 partitiontable/pt.gpt.c
	gcc -c -O2 partitiontable/pt.mbr.c
	gcc -c -O2 master.c
clang:
	clang -c -O2 algorithm/stack.c
	clang -c -O2 anscii/anscii.c
	clang -c -O2 calc/calc.c
	clang -c -O2 filesystem/fs.ext.c
	clang -c -O2 filesystem/fs.fat.c
	clang -c -O2 filesystem/fs.hfs.c
	clang -c -O2 filesystem/fs.ntfs.c
	clang -c -O2 partitiontable/pt.gpt.c
	clang -c -O2 partitiontable/pt.mbr.c
	clang -c -O2 master.c
clean:
	rm -f *.o *.a
