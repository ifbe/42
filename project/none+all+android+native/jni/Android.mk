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
library/libsoft1/libchem/chem.equation.c \
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
library/libsoft1/libmath/math.bigint.c \
library/libsoft1/libmath/math.matrix.c \
library/libsoft1/libmath/math.notation.c \
library/libsoft1/libmath/math.prime.c \
library/libsoft1/libmath/math.quaternion.c \
library/libsoft1/libmath/math.vector.c \
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
library/libsoft1/libphys/phys.elec.c \
library/libsoft1/libphys/phys.mech.c \
library/libsoft1/artery.c

#libuser0
LOCAL_SRC_FILES += \
library/libuser0/osandroid/android.native.c \
library/libuser0/osandroid/android.tray.c \
library/libuser0/osandroid/android.cam.c \
library/libuser0/osandroid/android.mic.c \
library/libuser0/bydummy/dummy.func.c \
library/libuser0/bydummy/dummy.html.c \
library/libuser0/bydummy/dummy.json.c \
library/libuser0/bydummy/dummy.rgba.c \
library/libuser0/bydummy/dummy.tty.c \
library/libuser0/bydummy/dummy.vbo.c \
library/libuser0/bydummy/dummy.vpin.c \
library/libuser0/bydummy/dummy.ws.c \
library/libuser0/bydummy/dummy.xml.c \
library/libuser0/arena.c

#libuser1
LOCAL_SRC_FILES += \
library/libuser1/lib1d/lib1d.c \
library/libuser1/lib1d/cli/print.cli.c \
library/libuser1/lib1d/tui/print.tui.c \
library/libuser1/lib1d/json/json.c \
library/libuser1/lib1d/latex/latex.c \
library/libuser1/lib1d/xml/html.c \
library/libuser1/lib1d/xml/xaml.c \
library/libuser1/lib1d/xml/xml.c \
library/libuser1/lib2d/lib2d.c \
library/libuser1/lib2d/draw/draw.line.c \
library/libuser1/lib2d/draw/draw.solid.c \
library/libuser1/lib2d/draw/draw.shape.c \
library/libuser1/lib2d/draw/draw.ascii.c \
library/libuser1/lib2d/draw/draw.utf8.c \
library/libuser1/lib2d/draw/draw.vt100.c \
library/libuser1/lib2d/photo/photo.blur.c \
library/libuser1/lib2d/photo/photo.channel.c \
library/libuser1/lib2d/photo/photo.convert.c \
library/libuser1/lib2d/photo/photo.gray.c \
library/libuser1/lib2d/photo/photo.mosaic.c \
library/libuser1/lib2d/photo/photo.rotate.c \
library/libuser1/lib2d/photo/photo.scale.c \
library/libuser1/lib3d/lib3d.c \
library/libuser1/lib3d/carve/carve.ascii.c \
library/libuser1/lib3d/carve/carve.point.c \
library/libuser1/lib3d/carve/carve.line.c \
library/libuser1/lib3d/carve/carve.trigon.c \
library/libuser1/lib3d/carve/carve.other.c \
library/libuser1/lib3d/carve/carve2d.ascii.c \
library/libuser1/lib3d/carve/carve2d.point.c \
library/libuser1/lib3d/carve/carve2d.line.c \
library/libuser1/lib3d/carve/carve2d.trigon.c \
library/libuser1/lib3d/carve/carve2d.other.c \
library/libuser1/lib3d/camera/camera.matrix.c \
library/libuser1/lib3d/camera/camera.rotate.c \
library/libuser1/lib3d/intersect/intersect.obb.c \
library/libuser1/lib3d/intersect/intersect.ray.c \
library/libuser1/lib3d/intersect/intersect.rect.c \
library/libuser1/lib3d/intersect/intersect.sphere.c \
library/libuser1/lib3d/model/model.magnify.c \
library/libuser1/lib3d/model/model.minify.c \
library/libuser1/lib3d/model/model.rotate.c \
library/libuser1/lib4d/lib4d.c \
library/libuser1/lib4d/asset.file.c \
library/libuser1/lib4d/asset.file.image.c \
library/libuser1/lib4d/asset.file.model.c \
library/libuser1/lib4d/asset.file.sound.c \
library/libuser1/lib4d/asset.func.c \
library/libuser1/lib4d/prepost.c \
library/libuser1/libback/back.c \
library/libuser1/libback/back.gif.c \
library/libuser1/libback/back.jpg.c \
library/libuser1/libfore/fore.c \
library/libuser1/libfore/mode0.testland.c \
library/libuser1/libfore/mode1.console.c \
library/libuser1/libfore/mode2.overview.c \
library/libuser1/libfore/mode3.detail.c \
library/libuser1/libfore/mode4.editor.c \
library/libuser1/libfore/mode4.editor.camera.c \
library/libuser1/libfore/mode4.editor.target.c \
library/libuser1/libfore/mode5.player.c \
library/libuser1/libfore/mode6.unknown.c \
library/libuser1/libfore/mode7.oneonone.c \
library/libuser1/libfore/menu.list.c \
library/libuser1/libfore/menu.nav.c \
library/libuser1/libfore/menu.tag.c \
library/libuser1/libtemp/popup.c \
library/libuser1/libvkbd/vkbd.c \
library/libuser1/libvkbd/vkbd.joystick.c \
library/libuser1/libvkbd/vkbd.keyboard.c \
library/libuser1/thealgo/algo.algo.c \
library/libuser1/thealgo/algo.bintree.c \
library/libuser1/thealgo/algo.bplus.c \
library/libuser1/thealgo/algo.graph.c \
library/libuser1/thegame/card/game.mahjong.c \
library/libuser1/thegame/card/game.poker.c \
library/libuser1/thegame/chess/game.chess.c \
library/libuser1/thegame/chess/game.pegged.c \
library/libuser1/thegame/chess/game.weiqi.c \
library/libuser1/thegame/chess/game.xiangqi.c \
library/libuser1/thegame/classic/game.klotski.c \
library/libuser1/thegame/classic/game.snake.c \
library/libuser1/thegame/classic/game.tetris.c \
library/libuser1/thegame/game.2048.c \
library/libuser1/thegame/game.maze.c \
library/libuser1/thegame/game.ooxx.c \
library/libuser1/thegame/game.rubik.c \
library/libuser1/thegame/game.sudoku.c \
library/libuser1/thehack/hack.fs.c \
library/libuser1/thehack/hack.joystick.c \
library/libuser1/thehack/hack.keyboard.c \
library/libuser1/thehack/hack.rawdump.c \
library/libuser1/thehack/hack.switch.c \
library/libuser1/theitem/item.clock.c \
library/libuser1/theitem/item.earth.c \
library/libuser1/theitem/item.house.c \
library/libuser1/theitem/item.human.c \
library/libuser1/theitem/item.light.c \
library/libuser1/theitem/item.mirror.c \
library/libuser1/theitem/item.model.c \
library/libuser1/theitem/item.ocean.c \
library/libuser1/theitem/item.piano.c \
library/libuser1/theitem/item.picture.c \
library/libuser1/theitem/item.portal.c \
library/libuser1/theitem/item.skydome.c \
library/libuser1/theitem/item.terrian.c \
library/libuser1/theitem/item.tree.c \
library/libuser1/theitem/item.water.c \
library/libuser1/thetest/test.codeimg.c \
library/libuser1/thetest/test.doodle.c \
library/libuser1/thetest/test.example.c \
library/libuser1/thetest/test.fractal.c \
library/libuser1/thetest/test.palette.c \
library/libuser1/thetest/test.planet.c \
library/libuser1/thetool/tool.browser.c \
library/libuser1/thetool/tool.calculator.c \
library/libuser1/thetool/tool.camera.c \
library/libuser1/thetool/tool.camera.ci.c \
library/libuser1/thetool/tool.camera.si.c \
library/libuser1/thetool/tool.circuit.c \
library/libuser1/thetool/tool.editor.c \
library/libuser1/thetool/tool.font.c \
library/libuser1/thetool/tool.hex.c \
library/libuser1/thetool/tool.qrcode.c \
library/libuser1/thetool/tool.sketchpad.c \
library/libuser1/thetool/tool.spectrum.c \
library/libuser1/thetool/tool.terminal.c \
library/libuser1/thetool/tool.terminal.ci.c \
library/libuser1/thetool/tool.terminal.si.c \
library/libuser1/actor.c




#2
LOCAL_C_INCLUDES := $(LOCAL_PATH)/library
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
LOCAL_LDLIBS := -lm -llog -ljnigraphics -landroid -lEGL -lGLESv3
LOCAL_MODULE := finalanswer
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
