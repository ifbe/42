LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)




#libboot0
LOCAL_SRC_FILES += \
library/libboot0/logcat/logcat.c

#libboot1
LOCAL_SRC_FILES += \
library/libboot1/main.c \
library/libboot1/stdin.c \
library/libboot1/stdout.c \
library/libboot1/stdev.c \
library/libboot1/stdrel.c

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
library/libsoft0/linux/linux.system.shell.c \
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
library/libsoft1/libalgo/crypt/crypt.aes.c \
library/libsoft1/libalgo/crypt/crypt.des.c \
library/libsoft1/libalgo/crypt/crypt.rsa.c \
library/libsoft1/libalgo/encdec/encdec.base64.c \
library/libsoft1/libalgo/encdec/encdec.qrcode.c \
library/libsoft1/libalgo/graph/graph.forcedirected.c \
library/libsoft1/libalgo/graph/graph.kruskal.c \
library/libsoft1/libalgo/graph/graph.prim.c \
library/libsoft1/libalgo/hash/hash.crc8.c \
library/libsoft1/libalgo/hash/hash.crc16.c \
library/libsoft1/libalgo/hash/hash.crc32.c \
library/libsoft1/libalgo/hash/hash.md5.c \
library/libsoft1/libalgo/hash/hash.sha1.c \
library/libsoft1/libalgo/hash/hash.sha256.c \
library/libsoft1/libalgo/hash/hash.sha512.c \
library/libsoft1/libalgo/hash/hash.string.c \
library/libsoft1/libalgo/keyexch/keyexch.dh.c \
library/libsoft1/libalgo/keyexch/keyexch.ecdhe.c \
library/libsoft1/libalgo/linear/linear.disjoint.c \
library/libsoft1/libalgo/linear/linear.memory.c \
library/libsoft1/libalgo/linear/linear.queue.c \
library/libsoft1/libalgo/linear/linear.stack.c \
library/libsoft1/libalgo/matching/match.bm.c \
library/libsoft1/libalgo/matching/match.kmp.c \
library/libsoft1/libalgo/matching/match.rough.c \
library/libsoft1/libalgo/notation/nota.calc.c \
library/libsoft1/libalgo/notation/nota.postfix.c \
library/libsoft1/libalgo/notation/nota.prefix.c \
library/libsoft1/libalgo/solve/solve.2048.c \
library/libsoft1/libalgo/solve/solve.chess.c \
library/libsoft1/libalgo/solve/solve.maze.c \
library/libsoft1/libalgo/solve/solve.rubik.c \
library/libsoft1/libalgo/solve/solve.sudoku.c \
library/libsoft1/libalgo/solve/solve.xiangqi.c \
library/libsoft1/libalgo/sorting/sort.bubble.c \
library/libsoft1/libalgo/sorting/sort.gnome.c \
library/libsoft1/libalgo/sorting/sort.heap.c \
library/libsoft1/libalgo/sorting/sort.insert.c \
library/libsoft1/libalgo/sorting/sort.merge.c \
library/libsoft1/libalgo/sorting/sort.quick.c \
library/libsoft1/libalgo/sorting/sort.select.c \
library/libsoft1/libalgo/transform/trfm.cosine.c \
library/libsoft1/libalgo/transform/trfm.fourier.c \
library/libsoft1/libalgo/transform/trfm.laplace.c \
library/libsoft1/libalgo/tree/tree.binary.c \
library/libsoft1/libalgo/tree/tree.redblack.c \
library/libsoft1/libalgo/tree/tree.bplus.c \
library/libsoft1/libflow/libmotion/motion.c \
library/libsoft1/libflow/libnet/0.phy/phy.bt.c \
library/libsoft1/libflow/libnet/0.phy/phy.eth.c \
library/libsoft1/libflow/libnet/0.phy/phy.wlan.c \
library/libsoft1/libflow/libnet/1.link/link.arp.c \
library/libsoft1/libflow/libnet/1.link/link.ipv4.c \
library/libsoft1/libflow/libnet/1.link/link.ipv6.c \
library/libsoft1/libflow/libnet/1.link/link.ipx.c \
library/libsoft1/libflow/libnet/1.link/link.pppoe.c \
library/libsoft1/libflow/libnet/1.link/link.slip.c \
library/libsoft1/libflow/libnet/2.ip/ip46.icmp.c \
library/libsoft1/libflow/libnet/2.ip/ip46.tcp.c \
library/libsoft1/libflow/libnet/2.ip/ip46.udp.c \
library/libsoft1/libflow/libnet/2.ip/ipx.spx.c \
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
library/libsoft1/libfmt/audio/audio.ape.c \
library/libsoft1/libfmt/audio/audio.mp3.c \
library/libsoft1/libfmt/audio/audio.ogg.c \
library/libsoft1/libfmt/audio/audio.wav.c \
library/libsoft1/libfmt/audio/tone.drum.c \
library/libsoft1/libfmt/audio/tone.flute.c \
library/libsoft1/libfmt/audio/tone.piano.c \
library/libsoft1/libfmt/audio/tone.violin.c \
library/libsoft1/libfmt/picture/pic.bmp.c \
library/libsoft1/libfmt/picture/pic.flif.c \
library/libsoft1/libfmt/picture/pic.heif.c \
library/libsoft1/libfmt/picture/pic.jpg.c \
library/libsoft1/libfmt/picture/pic.png.c \
library/libsoft1/libfmt/picture/pic.webp.c \
library/libsoft1/libfmt/video/video.avi.c \
library/libsoft1/libfmt/video/video.gif.c \
library/libsoft1/libfmt/video/video.mkv.c \
library/libsoft1/libfmt/video/video.mp4.c \
library/libsoft1/libfmt/video/video.rmvb.c \
library/libsoft1/libfmt/compress/comp.7z.c \
library/libsoft1/libfmt/compress/comp.cpio.c \
library/libsoft1/libfmt/compress/comp.gz.c \
library/libsoft1/libfmt/compress/comp.tar.c \
library/libsoft1/libfmt/compress/comp.zip.c \
library/libsoft1/libfmt/executable/exec.elf.c \
library/libsoft1/libfmt/executable/exec.macho.c \
library/libsoft1/libfmt/executable/exec.pe.c \
library/libsoft1/libfmt/filesystem/fs.ext.c \
library/libsoft1/libfmt/filesystem/fs.fat.c \
library/libsoft1/libfmt/filesystem/fs.hfs.c \
library/libsoft1/libfmt/filesystem/fs.ntfs.c \
library/libsoft1/libfmt/vdisk/disk.raw.c \
library/libsoft1/libfmt/vdisk/disk.vhd.c \
library/libsoft1/libfmt/vdisk/disk.vmdk.c \
library/libsoft1/libfmt/parttable/pt.apple.c \
library/libsoft1/libfmt/parttable/pt.gpt.c \
library/libsoft1/libfmt/parttable/pt.mbr.c \
library/libsoft1/libfmt/certificate/cert.pem.c \
library/libsoft1/libfmt/certificate/cert.x509.c \
library/libsoft1/libfmt/document/doc.doc.c \
library/libsoft1/libfmt/document/doc.docx.c \
library/libsoft1/libfmt/document/doc.latex.c \
library/libsoft1/libfmt/document/doc.pdf.c \
library/libsoft1/libfmt/document/doc.ppt.c \
library/libsoft1/libfmt/document/doc.pptx.c \
library/libsoft1/libfmt/document/doc.xls.c \
library/libsoft1/libfmt/document/doc.xlsx.c \
library/libsoft1/libfmt/electronic/ee.brd.c \
library/libsoft1/libfmt/electronic/ee.dsn.c \
library/libsoft1/libfmt/electronic/ee.pcbdoc.c \
library/libsoft1/libfmt/electronic/ee.schdoc.c \
library/libsoft1/libfmt/font/font.fon.c \
library/libsoft1/libfmt/font/font.otf.c \
library/libsoft1/libfmt/font/font.ttf.c \
library/libsoft1/libfmt/fmt.c \
library/libsoft1/libmem/liblang/assembly/asm.x64.c \
library/libsoft1/libmem/liblang/assembly/asm.arm.c \
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
library/libsoft1/libmem/libstr/diff/diff.wild.c \
library/libsoft1/libmem/libstr/move/move.copy.c \
library/libsoft1/libmem/libstr/move/move.swap.c \
library/libsoft1/libmem/libstr/str.c \
library/libsoft1/libmem/memory.c \
library/libsoft1/libsci/libbio/bio.chance.c \
library/libsoft1/libsci/libbio/bio.c \
library/libsoft1/libsci/libchem/chem.equation.c \
library/libsoft1/libsci/libchem/chem.c \
library/libsoft1/libsci/libmath/math.bigint.c \
library/libsoft1/libsci/libmath/math.matrix.c \
library/libsoft1/libsci/libmath/math.notation.c \
library/libsoft1/libsci/libmath/math.prime.c \
library/libsoft1/libsci/libmath/math.quaternion.c \
library/libsoft1/libsci/libmath/math.vector.c \
library/libsoft1/libsci/libphys/phys.elec.c \
library/libsoft1/libsci/libphys/phys.mech.c \
library/libsoft1/libsci/libphys/phys.c \
library/libsoft1/libwire/wire.bt.c \
library/libsoft1/libwire/wire.gpio.c \
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

#libuser0
LOCAL_SRC_FILES += \
library/libuser0/android/android.nativewindow.c \
library/libuser0/cross/cross.net.c \
library/libuser0/cross/cross.voice.c \
library/libuser0/arena.c

#libuser1
LOCAL_SRC_FILES += \
library/libuser1/0test/test.clock.c \
library/libuser1/0test/test.codeimg.c \
library/libuser1/0test/test.doodle.c \
library/libuser1/0test/test.example.c \
library/libuser1/0test/test.fractal.c \
library/libuser1/0test/test.palette.c \
library/libuser1/1game/game.2048.c \
library/libuser1/1game/game.chess.c \
library/libuser1/1game/game.klotski.c \
library/libuser1/1game/game.maze.c \
library/libuser1/1game/game.ooxx.c \
library/libuser1/1game/game.pegged.c \
library/libuser1/1game/game.rubik.c \
library/libuser1/1game/game.snake.c \
library/libuser1/1game/game.sudoku.c \
library/libuser1/1game/game.tetris.c \
library/libuser1/1game/game.weiqi.c \
library/libuser1/1game/game.xiangqi.c \
library/libuser1/2tool/tool.browser.c \
library/libuser1/2tool/tool.calculator.c \
library/libuser1/2tool/tool.camera.c \
library/libuser1/2tool/tool.circuit.c \
library/libuser1/2tool/tool.editor.c \
library/libuser1/2tool/tool.font.c \
library/libuser1/2tool/tool.qrcode.c \
library/libuser1/2tool/tool.sketchpad.c \
library/libuser1/2tool/tool.spectrum.c \
library/libuser1/2tool/tool.stl.c \
library/libuser1/2tool/tool.terminal.c \
library/libuser1/3hack/hack.algo.c \
library/libuser1/3hack/hack.bintree.c \
library/libuser1/3hack/hack.bplus.c \
library/libuser1/3hack/hack.graph.c \
library/libuser1/3hack/hack.hex.c \
library/libuser1/3hack/hack.input.c \
library/libuser1/lib1d/cli/print.cli.c \
library/libuser1/lib1d/tui/print.tui.c \
library/libuser1/lib1d/html/html.shape.c \
library/libuser1/lib1d/json/json.shape.c \
library/libuser1/lib1d/latex/latex.shape.c \
library/libuser1/lib1d/xaml/xaml.shape.c \
library/libuser1/lib1d/lib1d.c \
library/libuser1/lib2d/draw/draw.ascii.c \
library/libuser1/lib2d/draw/draw.icon.c \
library/libuser1/lib2d/draw/draw.shape.c \
library/libuser1/lib2d/draw/draw.ttf.c \
library/libuser1/lib2d/draw/draw.vt100.c \
library/libuser1/lib2d/photo/photo.blur.c \
library/libuser1/lib2d/photo/photo.channel.c \
library/libuser1/lib2d/photo/photo.convert.c \
library/libuser1/lib2d/photo/photo.gray.c \
library/libuser1/lib2d/photo/photo.mosaic.c \
library/libuser1/lib2d/photo/photo.rotate.c \
library/libuser1/lib2d/photo/photo.scale.c \
library/libuser1/lib2d/lib2d.c \
library/libuser1/lib3d/carve/carve.ascii.c \
library/libuser1/lib3d/carve/carve.point.c \
library/libuser1/lib3d/carve/carve.line.c \
library/libuser1/lib3d/carve/carve.plane.c \
library/libuser1/lib3d/carve/carve.shape.c \
library/libuser1/lib3d/model/model.magnify.c \
library/libuser1/lib3d/model/model.minify.c \
library/libuser1/lib3d/model/model.rotate.c \
library/libuser1/lib3d/lib3d.c \
library/libuser1/lib4d/asset/asset.font.c \
library/libuser1/lib4d/asset/asset.image.c \
library/libuser1/lib4d/asset/asset.sound.c \
library/libuser1/lib4d/asset/asset.texture.c \
library/libuser1/lib4d/help/help.bgfg.c \
library/libuser1/lib4d/help/help.ctx.c \
library/libuser1/lib4d/help/help.input.c \
library/libuser1/lib4d/help/help.login.c \
library/libuser1/lib4d/help/help.win.c \
library/libuser1/lib4d/lib4d.c \
library/libuser1/actor.c




#2
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/library/libsoft0 \
	$(LOCAL_PATH)/library/libsoft1 \
	$(LOCAL_PATH)/library/libuser0 \
	$(LOCAL_PATH)/library/libuser1
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
LOCAL_LDLIBS := -lm -llog -ljnigraphics -landroid
LOCAL_MODULE := finalanswer
include $(BUILD_SHARED_LIBRARY)
