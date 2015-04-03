binary:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 annoying.c
	gcc -c -fPIC -O2 ext.c
	gcc -c -fPIC -O2 fat.c
	gcc -c -fPIC -O2 hfs.c
	gcc -c -fPIC -O2 ntfs.c
	rm -f main.a
	ar crs main.a master.o ext.o fat.o hfs.o ntfs.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 master.c
	x86_64-elf-gcc -c -fPIC -O2 annoying.c
	x86_64-elf-gcc -c -fPIC -O2 ext.c
	x86_64-elf-gcc -c -fPIC -O2 fat.c
	x86_64-elf-gcc -c -fPIC -O2 hfs.c
	x86_64-elf-gcc -c -fPIC -O2 ntfs.c
	rm -f main.a
	ar crs main.a master.o ext.o fat.o hfs.o ntfs.o
linux:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 annoying.c
	gcc -c -fPIC -O2 ext.c
	gcc -c -fPIC -O2 fat.c
	gcc -c -fPIC -O2 hfs.c
	gcc -c -fPIC -O2 ntfs.c
	rm -f main.a
	ar crs main.a master.o annoying.o ext.o fat.o hfs.o ntfs.o
mac:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 annoying.c
	gcc -c -fPIC -O2 ext.c
	gcc -c -fPIC -O2 fat.c
	gcc -c -fPIC -O2 hfs.c
	gcc -c -fPIC -O2 ntfs.c
	rm -f main.a
	ar crs main.a master.o annoying.o ext.o fat.o hfs.o ntfs.o
win:
	gcc -c -O2 ext.c
	gcc -c -O2 fat.c
	gcc -c -O2 hfs.c
	gcc -c -O2 ntfs.c
	rm -f fs.a
	ar crs fs.a ext.o fat.o hfs.o ntfs.o
win-graphic:
	gcc -c -O2 ext.c
	gcc -c -O2 fat.c
	gcc -c -O2 hfs.c
	gcc -c -O2 ntfs.c
	rm -f fs.a
	ar crs fs.a ext.o fat.o hfs.o ntfs.o
clean:
	rm -f *.o *.a
