#cd ____coderoot____
#ln -s ____fullpathofme____ ____linkname____

#source ____build/envsetup.sh____
#lunch ____devicename____
#mmm ____linkname____

#adb root
#adb remount
#adb push coderoot/out/target/product/devicename/system/bin/42 /system/bin/




#0.head
LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)




#1.boot0
LOCAL_SRC_FILES := \
	library/libboot0/none/tx.c \
	library/libboot0/basic.c

#1.boot1
LOCAL_SRC_FILES += \
	library/libboot1/debug.c

#2.hard0
ifeq ($(TARGET_ARCH),x86)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/x86/x86.fp.c \
	library/libhard0/cpu/x86/x86.lock.c \
	library/libhard0/cpu/x86/x86.port.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/x64/x64.fp.c \
	library/libhard0/cpu/x64/x64.lock.c \
	library/libhard0/cpu/x64/x64.port.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/arm/arm.fp.c \
	library/libhard0/cpu/arm/arm.lock.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/arm64/arm64.fp.c \
	library/libhard0/cpu/arm64/arm64.lock.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/mips/mips.fp.c \
	library/libhard0/cpu/mips/mips.lock.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/mips64/mips64.fp.c \
	library/libhard0/cpu/mips64/mips64.lock.c \
	library/libhard0/driver.c
endif

#2.hard1
LOCAL_SRC_FILES += \
	library/libhard1/body.c

#3.soft0
LOCAL_SRC_FILES += \
	library/libsoft0/android/android.flow.sound.c \
	library/libsoft0/android/android.flow.vision.c \
	library/libsoft0/linux/linux.flow.client.c \
	library/libsoft0/linux/linux.flow.server.c \
	library/libsoft0/linux/linux.memory.file.c \
	library/libsoft0/linux/linux.memory.physical.c \
	library/libsoft0/linux/linux.memory.virtual.c \
	library/libsoft0/linux/linux.system.folder.c \
	library/libsoft0/linux/linux.system.process.c \
	library/libsoft0/linux/linux.system.random.c \
	library/libsoft0/linux/linux.system.signal.c \
	library/libsoft0/linux/linux.system.time.c \
	library/libsoft0/linux/linux.wire.bt.c \
	library/libsoft0/linux/linux.wire.gpio.c \
	library/libsoft0/linux/linux.wire.i2c.c \
	library/libsoft0/linux/linux.wire.spi.c \
	library/libsoft0/linux/linux.wire.uart.c \
	library/libsoft0/linux/linux.wire.usb.c \
	library/libsoft0/linux/linux.wire.wifi.c \
	library/libsoft0/system.c

#3.soft1
LOCAL_SRC_FILES += \
	library/libsoft1/libbio/bio.c \
	library/libsoft1/libchem/chem.c \
	library/libsoft1/libflow/libmotion/motion.c \
	library/libsoft1/libflow/libnet/0.raw/net.eth.c \
	library/libsoft1/libflow/libnet/0.raw/net.ppp.c \
	library/libsoft1/libflow/libnet/0.raw/net.slip.c \
	library/libsoft1/libflow/libnet/0.raw/net.wlan.c \
	library/libsoft1/libflow/libnet/1.net/net.arp.c \
	library/libsoft1/libflow/libnet/1.net/net.icmp.c \
	library/libsoft1/libflow/libnet/1.net/net.ipv4.c \
	library/libsoft1/libflow/libnet/1.net/net.ipv6.c \
	library/libsoft1/libflow/libnet/2.tran/net.quic.c \
	library/libsoft1/libflow/libnet/2.tran/net.ssh.c \
	library/libsoft1/libflow/libnet/2.tran/net.ssl.c \
	library/libsoft1/libflow/libnet/2.tran/net.tcp.c \
	library/libsoft1/libflow/libnet/2.tran/net.tls.c \
	library/libsoft1/libflow/libnet/2.tran/net.udp.c \
	library/libsoft1/libflow/libnet/3.app/net.chat.c \
	library/libsoft1/libflow/libnet/3.app/net.ftp.c \
	library/libsoft1/libflow/libnet/3.app/net.http.c \
	library/libsoft1/libflow/libnet/3.app/net.tftp.c \
	library/libsoft1/libflow/libnet/3.app/net.ws.c \
	library/libsoft1/libflow/libnet/manager/net.client.c \
	library/libsoft1/libflow/libnet/manager/net.server.c \
	library/libsoft1/libflow/libnet/manager/net.c \
	library/libsoft1/libflow/libsound/sound.c \
	library/libsoft1/libflow/libvision/vision.c \
	library/libsoft1/libflow/flow.c \
	library/libsoft1/libmath/math.bigint.c \
	library/libsoft1/libmath/math.calc.c \
	library/libsoft1/libmath/math.dct.c \
	library/libsoft1/libmath/math.fft.c \
	library/libsoft1/libmath/math.notation.c \
	library/libsoft1/libmath/math.prime.c \
	library/libsoft1/libmath/math.c \
	library/libsoft1/libmem/libfile/file.c \
	library/libsoft1/libmem/libfs/compress/compress.7z.c \
	library/libsoft1/libmem/libfs/compress/compress.cpio.c \
	library/libsoft1/libmem/libfs/compress/compress.gz.c \
	library/libsoft1/libmem/libfs/compress/compress.tar.c \
	library/libsoft1/libmem/libfs/compress/compress.zip.c \
	library/libsoft1/libmem/libfs/diskimage/img.raw.c \
	library/libsoft1/libmem/libfs/diskimage/img.vhd.c \
	library/libsoft1/libmem/libfs/diskimage/img.vmdk.c \
	library/libsoft1/libmem/libfs/executable/exec.elf.c \
	library/libsoft1/libmem/libfs/executable/exec.macho.c \
	library/libsoft1/libmem/libfs/executable/exec.pe.c \
	library/libsoft1/libmem/libfs/filesystem/fs.ext.c \
	library/libsoft1/libmem/libfs/filesystem/fs.fat.c \
	library/libsoft1/libmem/libfs/filesystem/fs.hfs.c \
	library/libsoft1/libmem/libfs/filesystem/fs.ntfs.c \
	library/libsoft1/libmem/libfs/parttable/pt.apple.c \
	library/libsoft1/libmem/libfs/parttable/pt.gpt.c \
	library/libsoft1/libmem/libfs/parttable/pt.mbr.c \
	library/libsoft1/libmem/libfs/fs.c \
	library/libsoft1/libmem/libsec/crypt/crypt.aes.c \
	library/libsoft1/libmem/libsec/crypt/crypt.base64.c \
	library/libsoft1/libmem/libsec/crypt/crypt.des.c \
	library/libsoft1/libmem/libsec/crypt/crypt.rsa.c \
	library/libsoft1/libmem/libsec/hash/hash.crc8.c \
	library/libsoft1/libmem/libsec/hash/hash.crc16.c \
	library/libsoft1/libmem/libsec/hash/hash.crc32.c \
	library/libsoft1/libmem/libsec/hash/hash.md5.c \
	library/libsoft1/libmem/libsec/hash/hash.sha1.c \
	library/libsoft1/libmem/libsec/hash/hash.string.c \
	library/libsoft1/libmem/libsec/qrcode.c \
	library/libsoft1/libmem/libsec/sec.c \
	library/libsoft1/libmem/libtext/conv/conv.arg.c \
	library/libsoft1/libmem/libtext/conv/conv.ascii.c \
	library/libsoft1/libmem/libtext/conv/conv.bigint.c \
	library/libsoft1/libmem/libtext/conv/conv.file.c \
	library/libsoft1/libmem/libtext/conv/conv.net.c \
	library/libsoft1/libmem/libtext/conv/conv.num.c \
	library/libsoft1/libmem/libtext/conv/conv.utf8.c \
	library/libsoft1/libmem/libtext/diff/diff.cmp.c \
	library/libsoft1/libmem/libtext/diff/diff.diff.c \
	library/libsoft1/libmem/libtext/diff/diff.wild.c \
	library/libsoft1/libmem/libtext/find/find.bm.c \
	library/libsoft1/libmem/libtext/find/find.kmp.c \
	library/libsoft1/libmem/libtext/find/find.rough.c \
	library/libsoft1/libmem/libtext/move/move.copy.c \
	library/libsoft1/libmem/libtext/move/move.swap.c \
	library/libsoft1/libmem/libtext/sort/sort.gnome.c \
	library/libsoft1/libmem/libtext/sort/sort.quick.c \
	library/libsoft1/libmem/libtext/text.c \
	library/libsoft1/libmem/memory.c \
	library/libsoft1/libphys/physics.electrical.c \
	library/libsoft1/libphys/physics.mechanics.c \
	library/libsoft1/libphys/phys.c \
	library/libsoft1/libsys/system.folder.c \
	library/libsoft1/libsys/system.process.c \
	library/libsoft1/libsys/system.setting.c \
	library/libsoft1/libsys/system.c \
	library/libsoft1/libwire/wire.bt.c \
	library/libsoft1/libwire/wire.i2c.c \
	library/libsoft1/libwire/wire.ir.c \
	library/libsoft1/libwire/wire.nfc.c \
	library/libsoft1/libwire/wire.pci.c \
	library/libsoft1/libwire/wire.spi.c \
	library/libsoft1/libwire/wire.uart.c \
	library/libsoft1/libwire/wire.usb.c \
	library/libsoft1/libwire/wire.wifi.c \
	library/libsoft1/libwire/wire.zigbee.c \
	library/libsoft1/libwire/wire.c \
	library/libsoft1/artery.c

#4.ui0
LOCAL_SRC_FILES += \
	library/libui0/linux/linux.cli.c \
	library/libui0/display.c

#4.ui1
LOCAL_SRC_FILES += \
	library/libui1/draw/draw.background.c \
	library/libui1/draw/draw.shape.c \
	library/libui1/draw/draw.ascii.c \
	library/libui1/draw/draw.unicode.c \
	library/libui1/game/game.2048.c \
	library/libui1/game/game.ooxx.c \
	library/libui1/game/game.snake.c \
	library/libui1/game/game.sudoku.c \
	library/libui1/game/game.tetris.c \
	library/libui1/game/game.weiqi.c \
	library/libui1/game/game.xiangqi.c \
	library/libui1/menu/menu.center.c \
	library/libui1/menu/menu.roster.c \
	library/libui1/menu/menu.virtkbd.c \
	library/libui1/test/test.color.c \
	library/libui1/test/test.doodle.c \
	library/libui1/test/test.font.c \
	library/libui1/test/test.pure.c \
	library/libui1/tool/tool.calculator.c \
	library/libui1/tool/tool.circuit.c \
	library/libui1/tool/tool.console.c \
	library/libui1/tool/tool.control.c \
	library/libui1/tool/tool.hex.c \
	library/libui1/tool/tool.qrcode.c \
	library/libui1/tool/tool.sketchpad.c \
	library/libui1/tool/tool.spectrum.c \
	library/libui1/tool/tool.tree.c \
	library/libui1/character.c

#1.visitor0
LOCAL_SRC_FILES += \
	library/visitor0/app/world.c \
	library/visitor0/app/event.c

#1.visitor1
LOCAL_SRC_FILES += \
	library/visitor1/cli.c




#2
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

LOCAL_SHARED_LIBRARIES += libc
LOCAL_LDFLAGS := -Wl,--hash-style=sysv

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := 42
include $(BUILD_EXECUTABLE)