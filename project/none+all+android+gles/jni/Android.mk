LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)




#libboot0
LOCAL_SRC_FILES += \
	library/libboot0/startapp.c \
	library/libboot0/basic.c \
	library/libboot0/logcat/logcat.c

#libboot1
LOCAL_SRC_FILES += \
	library/libboot1/main.c \
	library/libboot1/debug.c \
	library/libboot1/say.c \
	library/libboot1/event.c \
	library/libboot1/connect.c

#libhard0
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

#libhard1
LOCAL_SRC_FILES += \
	library/libhard1/body.c

#libsoft0
LOCAL_SRC_FILES += \
	library/libsoft0/android/android.flow.motion.sensor.c \
	library/libsoft0/android/android.flow.sound.audiotrack.c \
	library/libsoft0/android/android.flow.vision.qcamera2.c \
	library/libsoft0/linux/linux.flow.socket.c \
	library/libsoft0/linux/linux.memory.file.c \
	library/libsoft0/linux/linux.memory.folder.c \
	library/libsoft0/linux/linux.memory.managed.c \
	library/libsoft0/linux/linux.system.process.c \
	library/libsoft0/linux/linux.system.random.c \
	library/libsoft0/linux/linux.system.signal.c \
	library/libsoft0/linux/linux.system.time.c \
	library/libsoft0/linux/linux.system.thread.c \
	library/libsoft0/linux/linux.system.watcher.c \
	library/libsoft0/linux/linux.wire.bt.c \
	library/libsoft0/linux/linux.wire.gpio.c \
	library/libsoft0/linux/linux.wire.i2c.c \
	library/libsoft0/linux/linux.wire.spi.c \
	library/libsoft0/linux/linux.wire.uart.c \
	library/libsoft0/linux/linux.wire.usb.c \
	library/libsoft0/linux/linux.wire.wifi.c \
	library/libsoft0/system.c

#libsoft1
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
	library/libsoft1/libmem/libalgo/crypt/crypt.aes.c \
	library/libsoft1/libmem/libalgo/crypt/crypt.des.c \
	library/libsoft1/libmem/libalgo/crypt/crypt.rsa.c \
	library/libsoft1/libmem/libalgo/hash/hash.crc8.c \
	library/libsoft1/libmem/libalgo/hash/hash.crc16.c \
	library/libsoft1/libmem/libalgo/hash/hash.crc32.c \
	library/libsoft1/libmem/libalgo/hash/hash.md5.c \
	library/libsoft1/libmem/libalgo/hash/hash.sha1.c \
	library/libsoft1/libmem/libalgo/hash/hash.sha256.c \
	library/libsoft1/libmem/libalgo/hash/hash.sha512.c \
	library/libsoft1/libmem/libalgo/hash/hash.string.c \
	library/libsoft1/libmem/libalgo/keyexch/keyexch.dh.c \
	library/libsoft1/libmem/libalgo/keyexch/keyexch.ecdhe.c \
	library/libsoft1/libmem/libalgo/search/search.bm.c \
	library/libsoft1/libmem/libalgo/search/search.kmp.c \
	library/libsoft1/libmem/libalgo/search/search.rough.c \
	library/libsoft1/libmem/libalgo/sort/sort.gnome.c \
	library/libsoft1/libmem/libalgo/sort/sort.quick.c \
	library/libsoft1/libmem/libalgo/transform/trfm.cosine.c \
	library/libsoft1/libmem/libalgo/transform/trfm.fourier.c \
	library/libsoft1/libmem/libalgo/transform/trfm.laplace.c \
	library/libsoft1/libmem/libdata/acodec/acodec.aac.c \
	library/libsoft1/libmem/libdata/acodec/acodec.mp3.c \
	library/libsoft1/libmem/libdata/vcodec/vcodec.avc.c \
	library/libsoft1/libmem/libdata/vcodec/vcodec.hevc.c \
	library/libsoft1/libmem/libdata/gcode/gcode.barcode.c \
	library/libsoft1/libmem/libdata/gcode/gcode.qrcode.c \
	library/libsoft1/libmem/libdata/tcode/tcode.base64.c \
	library/libsoft1/libmem/libdata/tcode/tcode.huffman.c \
	library/libsoft1/libmem/libdata/data.c \
	library/libsoft1/libmem/libfmt/av1.audio/audio.ape.c \
	library/libsoft1/libmem/libfmt/av1.audio/audio.mp3.c \
	library/libsoft1/libmem/libfmt/av1.audio/audio.ogg.c \
	library/libsoft1/libmem/libfmt/av1.audio/audio.wav.c \
	library/libsoft1/libmem/libfmt/av1.tone/tone.drum.c \
	library/libsoft1/libmem/libfmt/av1.tone/tone.flute.c \
	library/libsoft1/libmem/libfmt/av1.tone/tone.piano.c \
	library/libsoft1/libmem/libfmt/av1.tone/tone.violin.c \
	library/libsoft1/libmem/libfmt/av2.picture/pic.bmp.c \
	library/libsoft1/libmem/libfmt/av2.picture/pic.flif.c \
	library/libsoft1/libmem/libfmt/av2.picture/pic.heif.c \
	library/libsoft1/libmem/libfmt/av2.picture/pic.jpg.c \
	library/libsoft1/libmem/libfmt/av2.picture/pic.png.c \
	library/libsoft1/libmem/libfmt/av2.picture/pic.webp.c \
	library/libsoft1/libmem/libfmt/av2.video/video.avi.c \
	library/libsoft1/libmem/libfmt/av2.video/video.gif.c \
	library/libsoft1/libmem/libfmt/av2.video/video.mkv.c \
	library/libsoft1/libmem/libfmt/av2.video/video.mp4.c \
	library/libsoft1/libmem/libfmt/av2.video/video.rmvb.c \
	library/libsoft1/libmem/libfmt/av3.model/model.3mf.c \
	library/libsoft1/libmem/libfmt/av3.model/model.dwg.c \
	library/libsoft1/libmem/libfmt/av3.model/model.stl.c \
	library/libsoft1/libmem/libfmt/it.comp/comp.7z.c \
	library/libsoft1/libmem/libfmt/it.comp/comp.cpio.c \
	library/libsoft1/libmem/libfmt/it.comp/comp.gz.c \
	library/libsoft1/libmem/libfmt/it.comp/comp.tar.c \
	library/libsoft1/libmem/libfmt/it.comp/comp.zip.c \
	library/libsoft1/libmem/libfmt/it.exec/exec.elf.c \
	library/libsoft1/libmem/libfmt/it.exec/exec.macho.c \
	library/libsoft1/libmem/libfmt/it.exec/exec.pe.c \
	library/libsoft1/libmem/libfmt/it.fs/fs.ext.c \
	library/libsoft1/libmem/libfmt/it.fs/fs.fat.c \
	library/libsoft1/libmem/libfmt/it.fs/fs.hfs.c \
	library/libsoft1/libmem/libfmt/it.fs/fs.ntfs.c \
	library/libsoft1/libmem/libfmt/it.disk/disk.raw.c \
	library/libsoft1/libmem/libfmt/it.disk/disk.vhd.c \
	library/libsoft1/libmem/libfmt/it.disk/disk.vmdk.c \
	library/libsoft1/libmem/libfmt/it.pt/pt.apple.c \
	library/libsoft1/libmem/libfmt/it.pt/pt.gpt.c \
	library/libsoft1/libmem/libfmt/it.pt/pt.mbr.c \
	library/libsoft1/libmem/libfmt/xx.cert/cert.pem.c \
	library/libsoft1/libmem/libfmt/xx.cert/cert.x509.c \
	library/libsoft1/libmem/libfmt/xx.doc/doc.doc.c \
	library/libsoft1/libmem/libfmt/xx.doc/doc.docx.c \
	library/libsoft1/libmem/libfmt/xx.doc/doc.latex.c \
	library/libsoft1/libmem/libfmt/xx.doc/doc.pdf.c \
	library/libsoft1/libmem/libfmt/xx.doc/doc.ppt.c \
	library/libsoft1/libmem/libfmt/xx.doc/doc.pptx.c \
	library/libsoft1/libmem/libfmt/xx.doc/doc.xls.c \
	library/libsoft1/libmem/libfmt/xx.doc/doc.xlsx.c \
	library/libsoft1/libmem/libfmt/xx.ee/ee.brd.c \
	library/libsoft1/libmem/libfmt/xx.ee/ee.dsn.c \
	library/libsoft1/libmem/libfmt/xx.ee/ee.pcbdoc.c \
	library/libsoft1/libmem/libfmt/xx.ee/ee.schdoc.c \
	library/libsoft1/libmem/libfmt/xx.font/font.fon.c \
	library/libsoft1/libmem/libfmt/xx.font/font.otf.c \
	library/libsoft1/libmem/libfmt/xx.font/font.ttf.c \
	library/libsoft1/libmem/libfmt/file.c \
	library/libsoft1/libmem/libgame/solve.2048.c \
	library/libsoft1/libmem/libgame/solve.chess.c \
	library/libsoft1/libmem/libgame/solve.sudoku.c \
	library/libsoft1/libmem/liblang/assembly/asm.x64.c \
	library/libsoft1/libmem/liblang/assembly/asm.arm64.c \
	library/libsoft1/libmem/liblang/program/prog.purec.c \
	library/libsoft1/libmem/liblang/program/prog.cpp.c \
	library/libsoft1/libmem/liblang/lang.c \
	library/libsoft1/libmem/libstr/conv/conv.arg.c \
	library/libsoft1/libmem/libstr/conv/conv.ascii.c \
	library/libsoft1/libmem/libstr/conv/conv.bigint.c \
	library/libsoft1/libmem/libstr/conv/conv.file.c \
	library/libsoft1/libmem/libstr/conv/conv.num.c \
	library/libsoft1/libmem/libstr/conv/conv.url.c \
	library/libsoft1/libmem/libstr/conv/conv.utf8.c \
	library/libsoft1/libmem/libstr/count/count.item.c \
	library/libsoft1/libmem/libstr/count/count.line.c \
	library/libsoft1/libmem/libstr/diff/diff.cmp.c \
	library/libsoft1/libmem/libstr/diff/diff.diff.c \
	library/libsoft1/libmem/libstr/diff/diff.wild.c \
	library/libsoft1/libmem/libstr/move/move.copy.c \
	library/libsoft1/libmem/libstr/move/move.swap.c \
	library/libsoft1/libmem/libstr/str.c \
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

#libui0
LOCAL_SRC_FILES += \
	library/libui0/android/android.nativewindow.c \
	library/libui0/cross/cross.net.c \
	library/libui0/cross/cross.voice.c \
	library/libui0/arena.c

#libui1
LOCAL_SRC_FILES += \
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
	library/libui1/content/levi/levi.login.c \
	library/libui1/content/levi/levi.navi.c \
	library/libui1/content/levi/levi.joystick.c \
	library/libui1/content/levi/levi.keyboard.c \
	library/libui1/content/levi/levi.c \
	library/libui1/content/content.c \
	library/libui1/helper/helper.term.c \
	library/libui1/helper/helper.win.c \
	library/libui1/helper/helper.c \
	library/libui1/lib1d/html/html.shape.c \
	library/libui1/lib1d/json/json.shape.c \
	library/libui1/lib1d/latex/latex.shape.c \
	library/libui1/lib1d/xaml/xaml.shape.c \
	library/libui1/lib1d/lib1d.c \
	library/libui1/lib2d/draw/draw.ascii.c \
	library/libui1/lib2d/draw/draw.color.c \
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
	library/libui1/actor.c




#2
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/library/libsoft0 $(LOCAL_PATH)/library/libsoft1 \
	$(LOCAL_PATH)/library/libui0 $(LOCAL_PATH)/library/libui1
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
LOCAL_LDLIBS := -lm -llog -ljnigraphics -landroid
LOCAL_MODULE := finalanswer
include $(BUILD_SHARED_LIBRARY)
