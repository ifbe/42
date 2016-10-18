compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) artery.c
	$(compiler) $(cflag) interface/interface.c
	$(compiler) $(cflag) interface/interface.bluetooth.c
	$(compiler) $(cflag) interface/interface.gpio.c
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
	$(compiler) $(cflag) memory/datasystem/compress/comp.7z.c
	$(compiler) $(cflag) memory/datasystem/compress/comp.cpio.c
	$(compiler) $(cflag) memory/datasystem/compress/comp.gz.c
	$(compiler) $(cflag) memory/datasystem/compress/comp.tar.c
	$(compiler) $(cflag) memory/datasystem/compress/comp.zip.c
	$(compiler) $(cflag) memory/datasystem/executable/exec.elf.c
	$(compiler) $(cflag) memory/datasystem/executable/exec.macho.c
	$(compiler) $(cflag) memory/datasystem/executable/exec.pe.c
	$(compiler) $(cflag) memory/datasystem/filesystem/fs.ext.c
	$(compiler) $(cflag) memory/datasystem/filesystem/fs.fat.c
	$(compiler) $(cflag) memory/datasystem/filesystem/fs.hfs.c
	$(compiler) $(cflag) memory/datasystem/filesystem/fs.ntfs.c
	$(compiler) $(cflag) memory/datasystem/parttable/pt.apple.c
	$(compiler) $(cflag) memory/datasystem/parttable/pt.gpt.c
	$(compiler) $(cflag) memory/datasystem/parttable/pt.mbr.c
	$(compiler) $(cflag) memory/sec/qrcode.c
	$(compiler) $(cflag) memory/sec/crypt/crypt.aes.c
	$(compiler) $(cflag) memory/sec/crypt/crypt.base64.c
	$(compiler) $(cflag) memory/sec/crypt/crypt.des.c
	$(compiler) $(cflag) memory/sec/crypt/crypt.rsa.c
	$(compiler) $(cflag) memory/sec/hash/hash.crc8.c
	$(compiler) $(cflag) memory/sec/hash/hash.crc16.c
	$(compiler) $(cflag) memory/sec/hash/hash.crc32.c
	$(compiler) $(cflag) memory/sec/hash/hash.md5.c
	$(compiler) $(cflag) memory/sec/hash/hash.sha1.c
	$(compiler) $(cflag) memory/sec/hash/hash.string.c
	$(compiler) $(cflag) memory/text/compare/compare.simple.c
	$(compiler) $(cflag) memory/text/compare/compare.wildcard.c
	$(compiler) $(cflag) memory/text/convert/convert.argument.c
	$(compiler) $(cflag) memory/text/convert/convert.ascii.c
	$(compiler) $(cflag) memory/text/convert/convert.encoding.c
	$(compiler) $(cflag) memory/text/convert/convert.filename.c
	$(compiler) $(cflag) memory/text/convert/convert.network.c
	$(compiler) $(cflag) memory/text/convert/convert.number.c
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
	$(compiler) $(cflag) special/math/math.calc.c
	$(compiler) $(cflag) special/math/math.notation.c
	$(compiler) $(cflag) special/physics/physics.electrical.c
	$(compiler) $(cflag) special/physics/physics.mechanics.c
	$(compiler) $(cflag) special/system/system.folder.c
	$(compiler) $(cflag) special/system/system.process.c
	$(compiler) $(cflag) special/system/system.thread.c
	$(compiler) $(cflag) special/system/system.setting.c
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
