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
library/libhard0/device.c
endif
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
library/libhard0/cpu/x64/x64.fp.c \
library/libhard0/cpu/x64/x64.port.c \
library/libhard0/device.c
endif
ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
library/libhard0/cpu/arm/arm.fp.c \
library/libhard0/device.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += \
library/libhard0/cpu/arm64/arm64.fp.c \
library/libhard0/device.c
endif
ifeq ($(TARGET_ARCH),mips)
LOCAL_SRC_FILES += \
library/libhard0/cpu/mips/mips.fp.c \
library/libhard0/device.c
endif
ifeq ($(TARGET_ARCH),mips64)
LOCAL_SRC_FILES += \
library/libhard0/cpu/mips64/mips64.fp.c \
library/libhard0/device.c
endif

#libhard1
LOCAL_SRC_FILES += \
library/libhard1/driver.c

#libsoft0
LOCAL_SRC_FILES += \
library/libsoft0/android/android.flow.motion.sensor.c \
library/libsoft0/android/android.flow.sound.audiotrack.c \
library/libsoft0/android/android.flow.vision.qcamera2.c \
library/libsoft0/linux/linux.flow.epoll.c \
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
library/libsoft1/libalgo/liblang/assembly/asm.x64.c \
library/libsoft1/libalgo/liblang/assembly/asm.arm.c \
library/libsoft1/libalgo/liblang/program/prog.purec.c \
library/libsoft1/libalgo/liblang/program/prog.cpp.c \
library/libsoft1/libalgo/liblang/lang.c \
library/libsoft1/libalgo/libstr/conv/conv.arg.c \
library/libsoft1/libalgo/libstr/conv/conv.ascii.c \
library/libsoft1/libalgo/libstr/conv/conv.bigint.c \
library/libsoft1/libalgo/libstr/conv/conv.file.c \
library/libsoft1/libalgo/libstr/conv/conv.num.c \
library/libsoft1/libalgo/libstr/conv/conv.url.c \
library/libsoft1/libalgo/libstr/conv/conv.utf8.c \
library/libsoft1/libalgo/libstr/count/count.item.c \
library/libsoft1/libalgo/libstr/count/count.line.c \
library/libsoft1/libalgo/libstr/diff/diff.cmp.c \
library/libsoft1/libalgo/libstr/diff/diff.wild.c \
library/libsoft1/libalgo/libstr/move/move.copy.c \
library/libsoft1/libalgo/libstr/move/move.swap.c \
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
library/libsoft1/libalgo/solve/solve.snake.c \
library/libsoft1/libalgo/solve/solve.sudoku.c \
library/libsoft1/libalgo/solve/solve.tetris.c \
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
library/libsoft1/libfile/audio/audio.ape.c \
library/libsoft1/libfile/audio/audio.mp3.c \
library/libsoft1/libfile/audio/audio.ogg.c \
library/libsoft1/libfile/audio/audio.wav.c \
library/libsoft1/libfile/audio/tone.drum.c \
library/libsoft1/libfile/audio/tone.flute.c \
library/libsoft1/libfile/audio/tone.piano.c \
library/libsoft1/libfile/audio/tone.violin.c \
library/libsoft1/libfile/certificate/cert.pem.c \
library/libsoft1/libfile/certificate/cert.x509.c \
library/libsoft1/libfile/compress/comp.7z.c \
library/libsoft1/libfile/compress/comp.cpio.c \
library/libsoft1/libfile/compress/comp.gz.c \
library/libsoft1/libfile/compress/comp.tar.c \
library/libsoft1/libfile/compress/comp.zip.c \
library/libsoft1/libfile/document/doc.doc.c \
library/libsoft1/libfile/document/doc.docx.c \
library/libsoft1/libfile/document/doc.latex.c \
library/libsoft1/libfile/document/doc.pdf.c \
library/libsoft1/libfile/document/doc.ppt.c \
library/libsoft1/libfile/document/doc.pptx.c \
library/libsoft1/libfile/document/doc.xls.c \
library/libsoft1/libfile/document/doc.xlsx.c \
library/libsoft1/libfile/electronic/ee.brd.c \
library/libsoft1/libfile/electronic/ee.dsn.c \
library/libsoft1/libfile/electronic/ee.pcbdoc.c \
library/libsoft1/libfile/electronic/ee.schdoc.c \
library/libsoft1/libfile/executable/exec.elf.c \
library/libsoft1/libfile/executable/exec.macho.c \
library/libsoft1/libfile/executable/exec.pe.c \
library/libsoft1/libfile/filesystem/fs.ext.c \
library/libsoft1/libfile/filesystem/fs.fat.c \
library/libsoft1/libfile/filesystem/fs.hfs.c \
library/libsoft1/libfile/filesystem/fs.ntfs.c \
library/libsoft1/libfile/font/font.fon.c \
library/libsoft1/libfile/font/font.otf.c \
library/libsoft1/libfile/font/font.ttf.c \
library/libsoft1/libfile/model/model.3mf.c \
library/libsoft1/libfile/model/model.dwg.c \
library/libsoft1/libfile/model/model.stl.c \
library/libsoft1/libfile/parttable/pt.apple.c \
library/libsoft1/libfile/parttable/pt.gpt.c \
library/libsoft1/libfile/parttable/pt.mbr.c \
library/libsoft1/libfile/picture/pic.bmp.c \
library/libsoft1/libfile/picture/pic.flif.c \
library/libsoft1/libfile/picture/pic.heif.c \
library/libsoft1/libfile/picture/pic.jpg.c \
library/libsoft1/libfile/picture/pic.png.c \
library/libsoft1/libfile/picture/pic.webp.c \
library/libsoft1/libfile/vdisk/disk.raw.c \
library/libsoft1/libfile/vdisk/disk.vhd.c \
library/libsoft1/libfile/vdisk/disk.vmdk.c \
library/libsoft1/libfile/video/video.avi.c \
library/libsoft1/libfile/video/video.gif.c \
library/libsoft1/libfile/video/video.mkv.c \
library/libsoft1/libfile/video/video.mp4.c \
library/libsoft1/libfile/video/video.rmvb.c \
library/libsoft1/libfile/file.c \
library/libsoft1/libnet/0.phy/phy.bt.c \
library/libsoft1/libnet/0.phy/phy.eth.c \
library/libsoft1/libnet/0.phy/phy.raw.c \
library/libsoft1/libnet/0.phy/phy.wlan.c \
library/libsoft1/libnet/1.link/link.arp.c \
library/libsoft1/libnet/1.link/link.ipv4.c \
library/libsoft1/libnet/1.link/link.ipv6.c \
library/libsoft1/libnet/1.link/link.ipx.c \
library/libsoft1/libnet/1.link/link.pppoe.c \
library/libsoft1/libnet/1.link/link.slip.c \
library/libsoft1/libnet/2.ip/ip46.icmp.c \
library/libsoft1/libnet/2.ip/ip46.tcp.c \
library/libsoft1/libnet/2.ip/ip46.udp.c \
library/libsoft1/libnet/2.ip/ipx.spx.c \
library/libsoft1/libnet/3.udp/udp.dns.c \
library/libsoft1/libnet/3.udp/udp.hole.c \
library/libsoft1/libnet/3.udp/udp.quic.c \
library/libsoft1/libnet/3.udp/udp.tftp.c \
library/libsoft1/libnet/4.tcp/tcp.dl.c \
library/libsoft1/libnet/4.tcp/tcp.ftp.c \
library/libsoft1/libnet/4.tcp/tcp.http.c \
library/libsoft1/libnet/4.tcp/tcp.proxy.c \
library/libsoft1/libnet/4.tcp/tcp.rdp.c \
library/libsoft1/libnet/4.tcp/tcp.rtmp.c \
library/libsoft1/libnet/4.tcp/tcp.sql.c \
library/libsoft1/libnet/4.tcp/tcp.vnc.c \
library/libsoft1/libnet/4.tcp/tcp.ssh.c \
library/libsoft1/libnet/4.tcp/tcp.tls.c \
library/libsoft1/libnet/4.tcp/tcp.ws.c \
library/libsoft1/libnet/app/app.chat.c \
library/libsoft1/libsci/libbio/bio.chance.c \
library/libsoft1/libsci/libchem/chem.equation.c \
library/libsoft1/libsci/libmath/math.bigint.c \
library/libsoft1/libsci/libmath/math.matrix.c \
library/libsoft1/libsci/libmath/math.notation.c \
library/libsoft1/libsci/libmath/math.prime.c \
library/libsoft1/libsci/libmath/math.quaternion.c \
library/libsoft1/libsci/libmath/math.vector.c \
library/libsoft1/libsci/libphys/phys.elec.c \
library/libsoft1/libsci/libphys/phys.mech.c \
library/libsoft1/artery.c

#libuser0
LOCAL_SRC_FILES += \
library/libuser0/local/android/local.android.nativewindow.c \
library/libuser0/remote/remote.http.c \
library/libuser0/remote/remote.ws.c \
library/libuser0/sound/sound.c \
library/libuser0/video/video.c \
library/libuser0/arena.c

#libuser1
LOCAL_SRC_FILES += \
library/libuser1/test/test.clock.c \
library/libuser1/test/test.codeimg.c \
library/libuser1/test/test.doodle.c \
library/libuser1/test/test.example.c \
library/libuser1/test/test.fractal.c \
library/libuser1/test/test.palette.c \
library/libuser1/game/game.2048.c \
library/libuser1/game/game.chess.c \
library/libuser1/game/game.klotski.c \
library/libuser1/game/game.maze.c \
library/libuser1/game/game.ooxx.c \
library/libuser1/game/game.pegged.c \
library/libuser1/game/game.poker.c \
library/libuser1/game/game.rubik.c \
library/libuser1/game/game.snake.c \
library/libuser1/game/game.sudoku.c \
library/libuser1/game/game.tetris.c \
library/libuser1/game/game.weiqi.c \
library/libuser1/game/game.xiangqi.c \
library/libuser1/tool/tool.browser.c \
library/libuser1/tool/tool.calculator.c \
library/libuser1/tool/tool.camera.c \
library/libuser1/tool/tool.circuit.c \
library/libuser1/tool/tool.editor.c \
library/libuser1/tool/tool.font.c \
library/libuser1/tool/tool.piano.c \
library/libuser1/tool/tool.qrcode.c \
library/libuser1/tool/tool.sketchpad.c \
library/libuser1/tool/tool.spectrum.c \
library/libuser1/tool/tool.stl.c \
library/libuser1/tool/tool.terminal.c \
library/libuser1/hack/hack.algo.c \
library/libuser1/hack/hack.bintree.c \
library/libuser1/hack/hack.bplus.c \
library/libuser1/hack/hack.fs.c \
library/libuser1/hack/hack.graph.c \
library/libuser1/hack/hack.hex.c \
library/libuser1/hack/hack.input.c \
library/libuser1/hack/hack.rawdump.c \
library/libuser1/hack/hack.switch.c \
library/libuser1/lib1d/cli/print.cli.c \
library/libuser1/lib1d/tui/print.tui.c \
library/libuser1/lib1d/json/json.c \
library/libuser1/lib1d/latex/latex.c \
library/libuser1/lib1d/xml/html.c \
library/libuser1/lib1d/xml/xaml.c \
library/libuser1/lib1d/xml/xml.c \
library/libuser1/lib1d/lib1d.c \
library/libuser1/lib2d/draw/draw.ascii.c \
library/libuser1/lib2d/draw/draw.icon.c \
library/libuser1/lib2d/draw/draw.shape.c \
library/libuser1/lib2d/draw/draw.ttf.c \
library/libuser1/lib2d/draw/draw.utf8.c \
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
library/libuser1/lib3d/carve/carve.trigon.c \
library/libuser1/lib3d/carve/carve.other.c \
library/libuser1/lib3d/model/model.magnify.c \
library/libuser1/lib3d/model/model.minify.c \
library/libuser1/lib3d/model/model.rotate.c \
library/libuser1/lib3d/lib3d.c \
library/libuser1/lib4d/help.asset.c \
library/libuser1/lib4d/help.bgfg.c \
library/libuser1/lib4d/help.ctx.c \
library/libuser1/lib4d/help.input.c \
library/libuser1/lib4d/help.login.c \
library/libuser1/lib4d/help.output.c \
library/libuser1/lib4d/help.touch.c \
library/libuser1/lib4d/help.vkbd.c \
library/libuser1/lib4d/lib4d.c \
library/libuser1/actor.c




#2
LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/library/libsoft0 \
$(LOCAL_PATH)/library/libsoft1 \
$(LOCAL_PATH)/library/libuser0 \
$(LOCAL_PATH)/library/libuser1
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
LOCAL_LDLIBS := -lm -llog -ljnigraphics -landroid -lEGL -lGLESv3
LOCAL_MODULE := finalanswer
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
