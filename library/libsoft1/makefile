binary:
	gcc -c -fPIC -O2 calc/calc.c
	gcc -c -fPIC -O2 filesystem/fs.ext.c
	gcc -c -fPIC -O2 filesystem/fs.fat.c
	gcc -c -fPIC -O2 filesystem/fs.hfs.c
	gcc -c -fPIC -O2 filesystem/fs.ntfs.c
	gcc -c -fPIC -O2 partitiontable/pt.gpt.c
	gcc -c -fPIC -O2 partitiontable/pt.mbr.c
	gcc -c -fPIC -O2 text/anscii.c
	gcc -c -fPIC -O2 text/unicode.c
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 servent.c
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 calc/calc.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/fs.ext.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/fs.fat.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/fs.hfs.c
	x86_64-elf-gcc -c -fPIC -O2 filesystem/fs.ntfs.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/pt.gpt.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/pt.mbr.c
	x86_64-elf-gcc -c -fPIC -O2 text/anscii.c
	x86_64-elf-gcc -c -fPIC -O2 text/unicode.c
	x86_64-elf-gcc -c -fPIC -O2 master.c
	x86_64-elf-gcc -c -fPIC -O2 servent.c
gcc:
	gcc -c -O2 calc/calc.c
	gcc -c -O2 filesystem/fs.ext.c
	gcc -c -O2 filesystem/fs.fat.c
	gcc -c -O2 filesystem/fs.hfs.c
	gcc -c -O2 filesystem/fs.ntfs.c
	gcc -c -O2 partitiontable/pt.gpt.c
	gcc -c -O2 partitiontable/pt.mbr.c
	gcc -c -O2 text/anscii.c
	gcc -c -O2 text/unicode.c
	gcc -c -O2 master.c
	gcc -c -O2 servent.c
clang:
	clang -c -O2 calc/calc.c
	clang -c -O2 filesystem/fs.ext.c
	clang -c -O2 filesystem/fs.fat.c
	clang -c -O2 filesystem/fs.hfs.c
	clang -c -O2 filesystem/fs.ntfs.c
	clang -c -O2 partitiontable/pt.gpt.c
	clang -c -O2 partitiontable/pt.mbr.c
	clang -c -O2 text/anscii.c
	clang -c -O2 text/unicode.c
	clang -c -O2 master.c
	clang -c -O2 servent.c
clean:
	rm -f *.o *.a
