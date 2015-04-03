binary:
	gcc -c -fPIC -O2 parttable.c
	gcc -c -fPIC -O2 ext.c
	gcc -c -fPIC -O2 fat.c
	gcc -c -fPIC -O2 hfs.c
	gcc -c -fPIC -O2 ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a parttable.o ext.o fat.o hfs.o ntfs.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 parttable.c
	x86_64-elf-gcc -c -fPIC -O2 ext.c
	x86_64-elf-gcc -c -fPIC -O2 fat.c
	x86_64-elf-gcc -c -fPIC -O2 hfs.c
	x86_64-elf-gcc -c -fPIC -O2 ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a parttable.o ext.o fat.o hfs.o ntfs.o
linux:
	gcc -c -fPIC -O2 parttable.c
	gcc -c -fPIC -O2 ext.c
	gcc -c -fPIC -O2 fat.c
	gcc -c -fPIC -O2 hfs.c
	gcc -c -fPIC -O2 ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a parttable.o ext.o fat.o hfs.o ntfs.o
mac:
	clang -c -fPIC -O2 parttable.c
	clang -c -fPIC -O2 ext.c
	clang -c -fPIC -O2 fat.c
	clang -c -fPIC -O2 hfs.c
	clang -c -fPIC -O2 ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a parttable.o ext.o fat.o hfs.o ntfs.o
win:
	gcc -c -O2 parttable.c
	gcc -c -O2 ext.c
	gcc -c -O2 fat.c
	gcc -c -O2 hfs.c
	gcc -c -O2 ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a parttable.o ext.o fat.o hfs.o ntfs.o
clean:
	rm -f *.o *.a
