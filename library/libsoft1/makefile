compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) artery.c
	$(compiler) $(cflag) interface/interface.c
	$(compiler) $(cflag) interface/interface.bluetooth.c
	$(compiler) $(cflag) interface/interface.i2c.c
	$(compiler) $(cflag) interface/interface.ir.c
	$(compiler) $(cflag) interface/interface.nfc.c
	$(compiler) $(cflag) interface/interface.pci.c
	$(compiler) $(cflag) interface/interface.spi.c
	$(compiler) $(cflag) interface/interface.uart.c
	$(compiler) $(cflag) interface/interface.usb.c
	$(compiler) $(cflag) interface/interface.wifi.c
	$(compiler) $(cflag) interface/interface.zigbee.c
	$(compiler) $(cflag) memory/memory.c
	$(compiler) $(cflag) memory/prelibation.c
	$(compiler) $(cflag) memory/compress/memory.comp.7z.c
	$(compiler) $(cflag) memory/compress/memory.comp.cpio.c
	$(compiler) $(cflag) memory/compress/memory.comp.gz.c
	$(compiler) $(cflag) memory/compress/memory.comp.tar.c
	$(compiler) $(cflag) memory/compress/memory.comp.zip.c
	$(compiler) $(cflag) memory/crypt/memory.crypt.aes.c
	$(compiler) $(cflag) memory/crypt/memory.crypt.des.c
	$(compiler) $(cflag) memory/crypt/memory.crypt.rsa.c
	$(compiler) $(cflag) memory/executable/memory.exec.elf.c
	$(compiler) $(cflag) memory/executable/memory.exec.macho.c
	$(compiler) $(cflag) memory/executable/memory.exec.pe.c
	$(compiler) $(cflag) memory/filesystem/memory.fs.c
	$(compiler) $(cflag) memory/filesystem/memory.fs.ext.c
	$(compiler) $(cflag) memory/filesystem/memory.fs.fat.c
	$(compiler) $(cflag) memory/filesystem/memory.fs.hfs.c
	$(compiler) $(cflag) memory/filesystem/memory.fs.ntfs.c
	$(compiler) $(cflag) memory/hash/memory.hash.crc8.c
	$(compiler) $(cflag) memory/hash/memory.hash.crc16.c
	$(compiler) $(cflag) memory/hash/memory.hash.crc32.c
	$(compiler) $(cflag) memory/hash/memory.hash.md5.c
	$(compiler) $(cflag) memory/hash/memory.hash.sha256.c
	$(compiler) $(cflag) memory/hash/memory.hash.string.c
	$(compiler) $(cflag) memory/parttable/memory.pt.c
	$(compiler) $(cflag) memory/parttable/memory.pt.apple.c
	$(compiler) $(cflag) memory/parttable/memory.pt.gpt.c
	$(compiler) $(cflag) memory/parttable/memory.pt.mbr.c
	$(compiler) $(cflag) memory/string/memory.string.compare.c
	$(compiler) $(cflag) memory/string/memory.string.convert.c
	$(compiler) $(cflag) memory/string/memory.string.parse.c
	$(compiler) $(cflag) memory/text/memory.text.ascii.c
	$(compiler) $(cflag) memory/text/memory.text.gbk.c
	$(compiler) $(cflag) memory/text/memory.text.unicode.c
	$(compiler) $(cflag) net/net.c
	$(compiler) $(cflag) net/net.arp.c
	$(compiler) $(cflag) net/net.ethernet.c
	$(compiler) $(cflag) net/net.ftp.c
	$(compiler) $(cflag) net/net.http.c
	$(compiler) $(cflag) net/net.icmp.c
	$(compiler) $(cflag) net/net.ipv4.c
	$(compiler) $(cflag) net/net.ipv6.c
	$(compiler) $(cflag) net/net.ppp.c
	$(compiler) $(cflag) net/net.quic.c
	$(compiler) $(cflag) net/net.slip.c
	$(compiler) $(cflag) net/net.ssh.c
	$(compiler) $(cflag) net/net.tcp.c
	$(compiler) $(cflag) net/net.tftp.c
	$(compiler) $(cflag) net/net.udp.c
	$(compiler) $(cflag) special/special.c
	$(compiler) $(cflag) special/math/special.math.calc.c
	$(compiler) $(cflag) special/math/special.math.notation.c
	$(compiler) $(cflag) special/parasite/special.parasite.folder.c
	$(compiler) $(cflag) special/parasite/special.parasite.setting.c
crosscompile:
	make -s target compiler="x86_64-elf-gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
gcc:
	make -s target compiler="gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
mingw:
	make -s target compiler="gcc" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
clang:
	make -s target compiler="clang" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
clean:
	rm -f *.o *.a
