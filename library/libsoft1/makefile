compiler ?= gcc
cflag ?= -c -O2
extra ?=
target:
	$(compiler) $(cflag) $(extra) libmemory/libbio/bio.c
	$(compiler) $(cflag) $(extra) libmemory/libchem/chem.c
	$(compiler) $(cflag) $(extra) libmemory/libfile/file.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/compress/compress.7z.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/compress/compress.cpio.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/compress/compress.gz.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/compress/compress.tar.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/compress/compress.zip.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/diskimage/img.raw.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/diskimage/img.vhd.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/diskimage/img.vmdk.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/executable/exec.elf.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/executable/exec.macho.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/executable/exec.pe.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/filesystem/fs.ext.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/filesystem/fs.fat.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/filesystem/fs.hfs.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/filesystem/fs.ntfs.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/parttable/pt.apple.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/parttable/pt.gpt.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/parttable/pt.mbr.c
	$(compiler) $(cflag) $(extra) libmemory/libfs/fs.c
	$(compiler) $(cflag) $(extra) libmemory/libmath/math.calc.c
	$(compiler) $(cflag) $(extra) libmemory/libmath/math.notation.c
	$(compiler) $(cflag) $(extra) libmemory/libmath/math.c
	$(compiler) $(cflag) $(extra) libmemory/libphys/physics.electrical.c
	$(compiler) $(cflag) $(extra) libmemory/libphys/physics.mechanics.c
	$(compiler) $(cflag) $(extra) libmemory/libphys/phys.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/crypt/crypt.aes.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/crypt/crypt.base64.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/crypt/crypt.des.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/crypt/crypt.rsa.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/hash/hash.crc8.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/hash/hash.crc16.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/hash/hash.crc32.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/hash/hash.md5.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/hash/hash.sha1.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/hash/hash.string.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/qrcode.c
	$(compiler) $(cflag) $(extra) libmemory/libsec/sec.c
	$(compiler) $(cflag) $(extra) libmemory/libtext/compare/compare.simple.c
	$(compiler) $(cflag) $(extra) libmemory/libtext/compare/compare.wildcard.c
	$(compiler) $(cflag) $(extra) libmemory/libtext/convert/convert.argument.c
	$(compiler) $(cflag) $(extra) libmemory/libtext/convert/convert.ascii.c
	$(compiler) $(cflag) $(extra) libmemory/libtext/convert/convert.encoding.c
	$(compiler) $(cflag) $(extra) libmemory/libtext/convert/convert.filename.c
	$(compiler) $(cflag) $(extra) libmemory/libtext/convert/convert.network.c
	$(compiler) $(cflag) $(extra) libmemory/libtext/convert/convert.number.c
	$(compiler) $(cflag) $(extra) libmemory/libtext/text.c
	$(compiler) $(cflag) $(extra) libmemory/memory.c
	$(compiler) $(cflag) $(extra) libnet/net.client.c
	$(compiler) $(cflag) $(extra) libnet/net.server.c
	$(compiler) $(cflag) $(extra) libnet/net.arp.c
	$(compiler) $(cflag) $(extra) libnet/net.ethernet.c
	$(compiler) $(cflag) $(extra) libnet/net.ftp.c
	$(compiler) $(cflag) $(extra) libnet/net.http.c
	$(compiler) $(cflag) $(extra) libnet/net.icmp.c
	$(compiler) $(cflag) $(extra) libnet/net.ipv4.c
	$(compiler) $(cflag) $(extra) libnet/net.ipv6.c
	$(compiler) $(cflag) $(extra) libnet/net.ppp.c
	$(compiler) $(cflag) $(extra) libnet/net.quic.c
	$(compiler) $(cflag) $(extra) libnet/net.slip.c
	$(compiler) $(cflag) $(extra) libnet/net.ssh.c
	$(compiler) $(cflag) $(extra) libnet/net.tftp.c
	$(compiler) $(cflag) $(extra) libnet/net.c
	$(compiler) $(cflag) $(extra) libsystem/system.folder.c
	$(compiler) $(cflag) $(extra) libsystem/system.process.c
	$(compiler) $(cflag) $(extra) libsystem/system.thread.c
	$(compiler) $(cflag) $(extra) libsystem/system.setting.c
	$(compiler) $(cflag) $(extra) libsystem/system.c
	$(compiler) $(cflag) $(extra) libwire/wire.bt.c
	$(compiler) $(cflag) $(extra) libwire/wire.gpio.c
	$(compiler) $(cflag) $(extra) libwire/wire.i2c.c
	$(compiler) $(cflag) $(extra) libwire/wire.ir.c
	$(compiler) $(cflag) $(extra) libwire/wire.nfc.c
	$(compiler) $(cflag) $(extra) libwire/wire.pci.c
	$(compiler) $(cflag) $(extra) libwire/wire.spi.c
	$(compiler) $(cflag) $(extra) libwire/wire.uart.c
	$(compiler) $(cflag) $(extra) libwire/wire.usb.c
	$(compiler) $(cflag) $(extra) libwire/wire.wifi.c
	$(compiler) $(cflag) $(extra) libwire/wire.zigbee.c
	$(compiler) $(cflag) $(extra) libwire/wire.c
	$(compiler) $(cflag) $(extra) artery.c
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
