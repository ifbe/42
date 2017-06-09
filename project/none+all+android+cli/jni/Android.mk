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




#0.init
LOCAL_SRC_FILES := \
	library/init/startapp.c \
	library/init/main.c

#1.boot0
LOCAL_SRC_FILES += \
	library/libboot0/none/tx.c \
	library/libboot0/basic.c

#1.boot1
LOCAL_SRC_FILES += \
	library/libboot1/say.c \
	library/libboot1/event.c \
	library/libboot1/debug.c

#2.hard0
ifeq ($(TARGET_ARCH),x86)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/x86/x86.fp.c \
	library/libhard0/cpu/x86/x86.port.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/x64/x64.fp.c \
	library/libhard0/cpu/x64/x64.port.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/arm/arm.fp.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/arm64/arm64.fp.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/mips/mips.fp.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/mips64/mips64.fp.c \
	library/libhard0/driver.c
endif

#2.hard1
LOCAL_SRC_FILES += \
	library/libhard1/body.c

#3.soft0
LOCAL_SRC_FILES += \
	library/libsoft0/android/android.flow.motion.sensor.c \
	library/libsoft0/android/android.flow.sound.audiotrack.c \
	library/libsoft0/android/android.flow.vision.qcamera2.c \
	library/libsoft0/android/android.system.tray.c \
	library/libsoft0/linux/linux.flow.net.socket.c \
	library/libsoft0/linux/linux.memory.file.c \
	library/libsoft0/linux/linux.memory.folder.c \
	library/libsoft0/linux/linux.memory.managed.c \
	library/libsoft0/linux/linux.system.process.c \
	library/libsoft0/linux/linux.system.random.c \
	library/libsoft0/linux/linux.system.signal.c \
	library/libsoft0/linux/linux.system.time.c \
	library/libsoft0/linux/linux.system.thread.c \
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
	library/libsoft1/libbio/bio.chance.c \
	library/libsoft1/libbio/bio.c \
	library/libsoft1/libchem/chem.equation.c \
	library/libsoft1/libchem/chem.c \
	library/libsoft1/libflow/libmotion/motion.c \
	library/libsoft1/libflow/libnet/0.phy/phy.bt.c \
	library/libsoft1/libflow/libnet/0.phy/phy.eth.c \
	library/libsoft1/libflow/libnet/0.phy/phy.wlan.c \
	library/libsoft1/libflow/libnet/1.link/link.arp.c \
	library/libsoft1/libflow/libnet/1.link/link.ipv4.c \
	library/libsoft1/libflow/libnet/1.link/link.ipv6.c \
	library/libsoft1/libflow/libnet/2.ip/ip46.icmp.c \
	library/libsoft1/libflow/libnet/2.ip/ip46.tcp.c \
	library/libsoft1/libflow/libnet/2.ip/ip46.udp.c \
	library/libsoft1/libflow/libnet/3.tran/tcp.ssh.c \
	library/libsoft1/libflow/libnet/3.tran/tcp.tls.c \
	library/libsoft1/libflow/libnet/3.tran/udp.quic.c \
	library/libsoft1/libflow/libnet/app/app.chat.c \
	library/libsoft1/libflow/libnet/app/app.raw.c \
	library/libsoft1/libflow/libnet/app/tcp.dl.c \
	library/libsoft1/libflow/libnet/app/tcp.ftp.c \
	library/libsoft1/libflow/libnet/app/tcp.http.c \
	library/libsoft1/libflow/libnet/app/tcp.proxy.c \
	library/libsoft1/libflow/libnet/app/tcp.rdp.c \
	library/libsoft1/libflow/libnet/app/tcp.rtmp.c \
	library/libsoft1/libflow/libnet/app/tcp.sql.c \
	library/libsoft1/libflow/libnet/app/tcp.vnc.c \
	library/libsoft1/libflow/libnet/app/tcp.ws.c \
	library/libsoft1/libflow/libnet/app/udp.dns.c \
	library/libsoft1/libflow/libnet/app/udp.hole.c \
	library/libsoft1/libflow/libnet/app/udp.tftp.c \
	library/libsoft1/libflow/libnet/net.c \
	library/libsoft1/libflow/libsound/sound.c \
	library/libsoft1/libflow/libvision/vision.c \
	library/libsoft1/libflow/flow.c \
	library/libsoft1/libmath/math.bigint.c \
	library/libsoft1/libmath/math.calc.c \
	library/libsoft1/libmath/math.notation.c \
	library/libsoft1/libmath/math.prime.c \
	library/libsoft1/libmath/math.c \
	library/libsoft1/libmem/libdata/acodec/acodec.aac.c \
	library/libsoft1/libmem/libdata/acodec/acodec.mp3.c \
	library/libsoft1/libmem/libdata/vcodec/vcodec.avc.c \
	library/libsoft1/libmem/libdata/vcodec/vcodec.hevc.c \
	library/libsoft1/libmem/libdata/crypt/crypt.aes.c \
	library/libsoft1/libmem/libdata/crypt/crypt.des.c \
	library/libsoft1/libmem/libdata/crypt/crypt.rsa.c \
	library/libsoft1/libmem/libdata/gcode/gcode.barcode.c \
	library/libsoft1/libmem/libdata/gcode/gcode.qrcode.c \
	library/libsoft1/libmem/libdata/tcode/tcode.base64.c \
	library/libsoft1/libmem/libdata/tcode/tcode.huffman.c \
	library/libsoft1/libmem/libdata/hash/hash.crc8.c \
	library/libsoft1/libmem/libdata/hash/hash.crc16.c \
	library/libsoft1/libmem/libdata/hash/hash.crc32.c \
	library/libsoft1/libmem/libdata/hash/hash.md5.c \
	library/libsoft1/libmem/libdata/hash/hash.sha1.c \
	library/libsoft1/libmem/libdata/hash/hash.sha256.c \
	library/libsoft1/libmem/libdata/hash/hash.sha512.c \
	library/libsoft1/libmem/libdata/hash/hash.string.c \
	library/libsoft1/libmem/libdata/keyex/keyex.dh.c \
	library/libsoft1/libmem/libdata/keyex/keyex.ecdhe.c \
	library/libsoft1/libmem/libdata/search/search.bm.c \
	library/libsoft1/libmem/libdata/search/search.kmp.c \
	library/libsoft1/libmem/libdata/search/search.rough.c \
	library/libsoft1/libmem/libdata/sort/sort.gnome.c \
	library/libsoft1/libmem/libdata/sort/sort.quick.c \
	library/libsoft1/libmem/libdata/trfm/trfm.cosine.c \
	library/libsoft1/libmem/libdata/trfm/trfm.fourier.c \
	library/libsoft1/libmem/libdata/trfm/trfm.laplace.c \
	library/libsoft1/libmem/libdata/data.c \
	library/libsoft1/libmem/libfmt/computer/certificate/cert.pem.c \
	library/libsoft1/libmem/libfmt/computer/certificate/cert.x509.c \
	library/libsoft1/libmem/libfmt/audiovideo/1.tone/tone.drum.c \
	library/libsoft1/libmem/libfmt/audiovideo/1.tone/tone.flute.c \
	library/libsoft1/libmem/libfmt/audiovideo/1.tone/tone.piano.c \
	library/libsoft1/libmem/libfmt/audiovideo/1.tone/tone.violin.c \
	library/libsoft1/libmem/libfmt/audiovideo/2.picture/pic.bmp.c \
	library/libsoft1/libmem/libfmt/audiovideo/2.picture/pic.flif.c \
	library/libsoft1/libmem/libfmt/audiovideo/2.picture/pic.jpg.c \
	library/libsoft1/libmem/libfmt/audiovideo/2.picture/pic.png.c \
	library/libsoft1/libmem/libfmt/audiovideo/2.picture/pic.webp.c \
	library/libsoft1/libmem/libfmt/computer/compress/compress.7z.c \
	library/libsoft1/libmem/libfmt/computer/compress/compress.cpio.c \
	library/libsoft1/libmem/libfmt/computer/compress/compress.gz.c \
	library/libsoft1/libmem/libfmt/computer/compress/compress.tar.c \
	library/libsoft1/libmem/libfmt/computer/compress/compress.zip.c \
	library/libsoft1/libmem/libfmt/computer/diskimage/img.raw.c \
	library/libsoft1/libmem/libfmt/computer/diskimage/img.vhd.c \
	library/libsoft1/libmem/libfmt/computer/diskimage/img.vmdk.c \
	library/libsoft1/libmem/libfmt/computer/executable/exec.elf.c \
	library/libsoft1/libmem/libfmt/computer/executable/exec.macho.c \
	library/libsoft1/libmem/libfmt/computer/executable/exec.pe.c \
	library/libsoft1/libmem/libfmt/computer/filesystem/fs.ext.c \
	library/libsoft1/libmem/libfmt/computer/filesystem/fs.fat.c \
	library/libsoft1/libmem/libfmt/computer/filesystem/fs.hfs.c \
	library/libsoft1/libmem/libfmt/computer/filesystem/fs.ntfs.c \
	library/libsoft1/libmem/libfmt/computer/parttable/pt.apple.c \
	library/libsoft1/libmem/libfmt/computer/parttable/pt.gpt.c \
	library/libsoft1/libmem/libfmt/computer/parttable/pt.mbr.c \
	library/libsoft1/libmem/libfmt/file.c \
	library/libsoft1/libmem/liblang/assembly/asm.x64.c \
	library/libsoft1/libmem/liblang/assembly/asm.arm64.c \
	library/libsoft1/libmem/liblang/program/prog.purec.c \
	library/libsoft1/libmem/liblang/program/prog.cpp.c \
	library/libsoft1/libmem/liblang/lang.c \
	library/libsoft1/libmem/libtext/conv/conv.arg.c \
	library/libsoft1/libmem/libtext/conv/conv.ascii.c \
	library/libsoft1/libmem/libtext/conv/conv.bigint.c \
	library/libsoft1/libmem/libtext/conv/conv.file.c \
	library/libsoft1/libmem/libtext/conv/conv.num.c \
	library/libsoft1/libmem/libtext/conv/conv.url.c \
	library/libsoft1/libmem/libtext/conv/conv.utf8.c \
	library/libsoft1/libmem/libtext/count/count.item.c \
	library/libsoft1/libmem/libtext/count/count.line.c \
	library/libsoft1/libmem/libtext/diff/diff.cmp.c \
	library/libsoft1/libmem/libtext/diff/diff.diff.c \
	library/libsoft1/libmem/libtext/diff/diff.wild.c \
	library/libsoft1/libmem/libtext/move/move.copy.c \
	library/libsoft1/libmem/libtext/move/move.swap.c \
	library/libsoft1/libmem/libtext/text.c \
	library/libsoft1/libmem/memory.c \
	library/libsoft1/libphys/phys.elec.c \
	library/libsoft1/libphys/phys.mech.c \
	library/libsoft1/libphys/phys.c \
	library/libsoft1/libsys/sys.folder.c \
	library/libsoft1/libsys/sys.monitor.c \
	library/libsoft1/libsys/sys.shell.c \
	library/libsoft1/libsys/sys.c \
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
	library/libui0/cross/cross.net.c \
	library/libui0/cross/cross.voice.c \
	library/libui0/display.c

#4.ui1
LOCAL_SRC_FILES += \
	library/libui1/lib1d/term/term.cli.c \
	library/libui1/lib1d/lib1d.c \
	library/libui1/lib2d/draw/draw.ascii.c \
	library/libui1/lib2d/draw/draw.background.c \
	library/libui1/lib2d/draw/draw.icon.c \
	library/libui1/lib2d/draw/draw.shape.c \
	library/libui1/lib2d/draw/draw.ttf.c \
	library/libui1/lib2d/draw/draw.vt100.c \
	library/libui1/lib2d/photo/photo.blur.c \
	library/libui1/lib2d/photo/photo.channel.c \
	library/libui1/lib2d/photo/photo.convert.c \
	library/libui1/lib2d/photo/photo.gray.c \
	library/libui1/lib2d/photo/photo.mosaic.c \
	library/libui1/lib2d/photo/photo.rotate.c \
	library/libui1/lib2d/photo/photo.scale.c \
	library/libui1/lib2d/lib2d.c \
	library/libui1/lib3d/carve/carve.ground.c \
	library/libui1/lib3d/carve/carve.shape.c \
	library/libui1/lib3d/carve/carve.thing.c \
	library/libui1/lib3d/carve/carve.water.c \
	library/libui1/lib3d/model/model.magnify.c \
	library/libui1/lib3d/model/model.minify.c \
	library/libui1/lib3d/lib3d.c \
	library/libui1/external/external.title.c \
	library/libui1/external/external.toast.c \
	library/libui1/external/external.c \
	library/libui1/content/float/float.menu.c \
	library/libui1/content/float/float.vkbd.c \
	library/libui1/content/game/game.2048.c \
	library/libui1/content/game/game.chess.c \
	library/libui1/content/game/game.ooxx.c \
	library/libui1/content/game/game.snake.c \
	library/libui1/content/game/game.sudoku.c \
	library/libui1/content/game/game.tetris.c \
	library/libui1/content/game/game.weiqi.c \
	library/libui1/content/game/game.xiangqi.c \
	library/libui1/content/hack/hack.algo.c \
	library/libui1/content/hack/hack.browse.c \
	library/libui1/content/hack/hack.editor.c \
	library/libui1/content/hack/hack.hex.c \
	library/libui1/content/hack/hack.input.c \
	library/libui1/content/hack/hack.tree.c \
	library/libui1/content/test/test.color.c \
	library/libui1/content/test/test.doodle.c \
	library/libui1/content/test/test.font.c \
	library/libui1/content/test/test.pure.c \
	library/libui1/content/tool/tool.3dmodel.c \
	library/libui1/content/tool/tool.calculator.c \
	library/libui1/content/tool/tool.camera.c \
	library/libui1/content/tool/tool.circuit.c \
	library/libui1/content/tool/tool.qrcode.c \
	library/libui1/content/tool/tool.sketchpad.c \
	library/libui1/content/tool/tool.spectrum.c \
	library/libui1/content/content.c \
	library/libui1/ui.c




#2
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

LOCAL_SHARED_LIBRARIES += libc
LOCAL_LDFLAGS := -Wl,--hash-style=sysv

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := 42
include $(BUILD_EXECUTABLE)
