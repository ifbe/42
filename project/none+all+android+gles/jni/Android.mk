LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)




#libboot0
LOCAL_SRC_FILES += \
library/libboot/0/logcat/logcat.c

#libboot1
LOCAL_SRC_FILES += \
library/libboot/1/main.c \
library/libboot/1/stdin.c \
library/libboot/1/stdout.c \
library/libboot/1/stdev.c \
library/libboot/1/stdrel.c

#libhard0
ifeq ($(TARGET_ARCH),x86)
LOCAL_SRC_FILES += \
library/libhard/0/cpu/x86/x86.fp.c \
library/libhard/0/cpu/x86/x86.port.c \
library/libhard/0/device.c
endif
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
library/libhard/0/cpu/x64/x64.fp.c \
library/libhard/0/cpu/x64/x64.port.c \
library/libhard/0/device.c
endif
ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
library/libhard/0/cpu/arm/arm.fp.c \
library/libhard/0/device.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += \
library/libhard/0/cpu/arm64/arm64.fp.c \
library/libhard/0/device.c
endif
ifeq ($(TARGET_ARCH),mips)
LOCAL_SRC_FILES += \
library/libhard/0/cpu/mips/mips.fp.c \
library/libhard/0/device.c
endif
ifeq ($(TARGET_ARCH),mips64)
LOCAL_SRC_FILES += \
library/libhard/0/cpu/mips64/mips64.fp.c \
library/libhard/0/device.c
endif

#libhard1
LOCAL_SRC_FILES += \
library/libhard/1/driver.c

#libsoft0
LOCAL_SRC_FILES += \
library/libsoft/0/android/android.flow.motion.sensor.c \
library/libsoft/0/android/android.flow.sound.audiotrack.c \
library/libsoft/0/android/android.flow.vision.qcamera2.c \
library/libsoft/0/linux/linux.flow.epoll.c \
library/libsoft/0/linux/linux.flow.socket.c \
library/libsoft/0/linux/linux.memory.file.c \
library/libsoft/0/linux/linux.memory.folder.c \
library/libsoft/0/linux/linux.memory.managed.c \
library/libsoft/0/linux/linux.system.process.c \
library/libsoft/0/linux/linux.system.random.c \
library/libsoft/0/linux/linux.system.shell.c \
library/libsoft/0/linux/linux.system.signal.c \
library/libsoft/0/linux/linux.system.time.c \
library/libsoft/0/linux/linux.system.thread.c \
library/libsoft/0/linux/linux.wire.bt.c \
library/libsoft/0/linux/linux.wire.gpio.c \
library/libsoft/0/linux/linux.wire.i2c.c \
library/libsoft/0/linux/linux.wire.spi.c \
library/libsoft/0/linux/linux.wire.uart.c \
library/libsoft/0/linux/linux.wire.usb.c \
library/libsoft/0/linux/linux.wire.wifi.c \
library/libsoft/0/system.c

#libsoft1
LOCAL_SRC_FILES += \
library/libsoft/1/libalgo/liblang/assembly/asm.x64.c \
library/libsoft/1/libalgo/liblang/assembly/asm.arm.c \
library/libsoft/1/libalgo/liblang/program/prog.purec.c \
library/libsoft/1/libalgo/liblang/program/prog.cpp.c \
library/libsoft/1/libalgo/liblang/lang.c \
library/libsoft/1/libalgo/libstr/conv/conv.arg.c \
library/libsoft/1/libalgo/libstr/conv/conv.ascii.c \
library/libsoft/1/libalgo/libstr/conv/conv.bigint.c \
library/libsoft/1/libalgo/libstr/conv/conv.file.c \
library/libsoft/1/libalgo/libstr/conv/conv.num.c \
library/libsoft/1/libalgo/libstr/conv/conv.url.c \
library/libsoft/1/libalgo/libstr/conv/conv.utf8.c \
library/libsoft/1/libalgo/libstr/count/count.item.c \
library/libsoft/1/libalgo/libstr/count/count.line.c \
library/libsoft/1/libalgo/libstr/diff/diff.cmp.c \
library/libsoft/1/libalgo/libstr/diff/diff.wild.c \
library/libsoft/1/libalgo/libstr/move/move.copy.c \
library/libsoft/1/libalgo/libstr/move/move.swap.c \
library/libsoft/1/libalgo/crypt/crypt.aes.c \
library/libsoft/1/libalgo/crypt/crypt.des.c \
library/libsoft/1/libalgo/crypt/crypt.rsa.c \
library/libsoft/1/libalgo/encdec/encdec.base64.c \
library/libsoft/1/libalgo/encdec/encdec.qrcode.c \
library/libsoft/1/libalgo/graph/graph.forcedirected.c \
library/libsoft/1/libalgo/graph/graph.kruskal.c \
library/libsoft/1/libalgo/graph/graph.prim.c \
library/libsoft/1/libalgo/hash/hash.crc8.c \
library/libsoft/1/libalgo/hash/hash.crc16.c \
library/libsoft/1/libalgo/hash/hash.crc32.c \
library/libsoft/1/libalgo/hash/hash.md5.c \
library/libsoft/1/libalgo/hash/hash.sha1.c \
library/libsoft/1/libalgo/hash/hash.sha256.c \
library/libsoft/1/libalgo/hash/hash.sha512.c \
library/libsoft/1/libalgo/hash/hash.string.c \
library/libsoft/1/libalgo/keyexch/keyexch.dh.c \
library/libsoft/1/libalgo/keyexch/keyexch.ecdhe.c \
library/libsoft/1/libalgo/linear/linear.disjoint.c \
library/libsoft/1/libalgo/linear/linear.memory.c \
library/libsoft/1/libalgo/linear/linear.queue.c \
library/libsoft/1/libalgo/linear/linear.stack.c \
library/libsoft/1/libalgo/matching/match.bm.c \
library/libsoft/1/libalgo/matching/match.kmp.c \
library/libsoft/1/libalgo/matching/match.rough.c \
library/libsoft/1/libalgo/notation/nota.calc.c \
library/libsoft/1/libalgo/notation/nota.postfix.c \
library/libsoft/1/libalgo/notation/nota.prefix.c \
library/libsoft/1/libalgo/solve/solve.2048.c \
library/libsoft/1/libalgo/solve/solve.chess.c \
library/libsoft/1/libalgo/solve/solve.maze.c \
library/libsoft/1/libalgo/solve/solve.rubik.c \
library/libsoft/1/libalgo/solve/solve.snake.c \
library/libsoft/1/libalgo/solve/solve.sudoku.c \
library/libsoft/1/libalgo/solve/solve.tetris.c \
library/libsoft/1/libalgo/solve/solve.xiangqi.c \
library/libsoft/1/libalgo/sorting/sort.bubble.c \
library/libsoft/1/libalgo/sorting/sort.gnome.c \
library/libsoft/1/libalgo/sorting/sort.heap.c \
library/libsoft/1/libalgo/sorting/sort.insert.c \
library/libsoft/1/libalgo/sorting/sort.merge.c \
library/libsoft/1/libalgo/sorting/sort.quick.c \
library/libsoft/1/libalgo/sorting/sort.select.c \
library/libsoft/1/libalgo/transform/trfm.cosine.c \
library/libsoft/1/libalgo/transform/trfm.fourier.c \
library/libsoft/1/libalgo/transform/trfm.laplace.c \
library/libsoft/1/libalgo/tree/tree.binary.c \
library/libsoft/1/libalgo/tree/tree.redblack.c \
library/libsoft/1/libalgo/tree/tree.bplus.c \
library/libsoft/1/libchem/chem.equation.c \
library/libsoft/1/libfile/audio/audio.ape.c \
library/libsoft/1/libfile/audio/audio.mp3.c \
library/libsoft/1/libfile/audio/audio.ogg.c \
library/libsoft/1/libfile/audio/audio.wav.c \
library/libsoft/1/libfile/audio/tone.drum.c \
library/libsoft/1/libfile/audio/tone.flute.c \
library/libsoft/1/libfile/audio/tone.piano.c \
library/libsoft/1/libfile/audio/tone.violin.c \
library/libsoft/1/libfile/certificate/cert.pem.c \
library/libsoft/1/libfile/certificate/cert.x509.c \
library/libsoft/1/libfile/compress/comp.7z.c \
library/libsoft/1/libfile/compress/comp.cpio.c \
library/libsoft/1/libfile/compress/comp.gz.c \
library/libsoft/1/libfile/compress/comp.tar.c \
library/libsoft/1/libfile/compress/comp.zip.c \
library/libsoft/1/libfile/document/doc.doc.c \
library/libsoft/1/libfile/document/doc.docx.c \
library/libsoft/1/libfile/document/doc.latex.c \
library/libsoft/1/libfile/document/doc.pdf.c \
library/libsoft/1/libfile/document/doc.ppt.c \
library/libsoft/1/libfile/document/doc.pptx.c \
library/libsoft/1/libfile/document/doc.xls.c \
library/libsoft/1/libfile/document/doc.xlsx.c \
library/libsoft/1/libfile/electronic/ee.brd.c \
library/libsoft/1/libfile/electronic/ee.dsn.c \
library/libsoft/1/libfile/electronic/ee.pcbdoc.c \
library/libsoft/1/libfile/electronic/ee.schdoc.c \
library/libsoft/1/libfile/executable/exec.elf.c \
library/libsoft/1/libfile/executable/exec.macho.c \
library/libsoft/1/libfile/executable/exec.pe.c \
library/libsoft/1/libfile/filesystem/fs.ext.c \
library/libsoft/1/libfile/filesystem/fs.fat.c \
library/libsoft/1/libfile/filesystem/fs.hfs.c \
library/libsoft/1/libfile/filesystem/fs.ntfs.c \
library/libsoft/1/libfile/font/font.fon.c \
library/libsoft/1/libfile/font/font.otf.c \
library/libsoft/1/libfile/font/font.ttf.c \
library/libsoft/1/libfile/model/model.3mf.c \
library/libsoft/1/libfile/model/model.dwg.c \
library/libsoft/1/libfile/model/model.stl.c \
library/libsoft/1/libfile/parttable/pt.apple.c \
library/libsoft/1/libfile/parttable/pt.gpt.c \
library/libsoft/1/libfile/parttable/pt.mbr.c \
library/libsoft/1/libfile/picture/pic.bmp.c \
library/libsoft/1/libfile/picture/pic.flif.c \
library/libsoft/1/libfile/picture/pic.heif.c \
library/libsoft/1/libfile/picture/pic.jpg.c \
library/libsoft/1/libfile/picture/pic.png.c \
library/libsoft/1/libfile/picture/pic.webp.c \
library/libsoft/1/libfile/vdisk/disk.raw.c \
library/libsoft/1/libfile/vdisk/disk.vhd.c \
library/libsoft/1/libfile/vdisk/disk.vmdk.c \
library/libsoft/1/libfile/video/video.avi.c \
library/libsoft/1/libfile/video/video.gif.c \
library/libsoft/1/libfile/video/video.mkv.c \
library/libsoft/1/libfile/video/video.mp4.c \
library/libsoft/1/libfile/video/video.rmvb.c \
library/libsoft/1/libfile/file.c \
library/libsoft/1/libmath/math.bigint.c \
library/libsoft/1/libmath/math.matrix.c \
library/libsoft/1/libmath/math.notation.c \
library/libsoft/1/libmath/math.prime.c \
library/libsoft/1/libmath/math.quaternion.c \
library/libsoft/1/libmath/math.vector.c \
library/libsoft/1/libnet/0.phy/phy.bt.c \
library/libsoft/1/libnet/0.phy/phy.eth.c \
library/libsoft/1/libnet/0.phy/phy.raw.c \
library/libsoft/1/libnet/0.phy/phy.wlan.c \
library/libsoft/1/libnet/1.link/link.arp.c \
library/libsoft/1/libnet/1.link/link.ipv4.c \
library/libsoft/1/libnet/1.link/link.ipv6.c \
library/libsoft/1/libnet/1.link/link.ipx.c \
library/libsoft/1/libnet/1.link/link.pppoe.c \
library/libsoft/1/libnet/1.link/link.slip.c \
library/libsoft/1/libnet/2.ip/ip46.icmp.c \
library/libsoft/1/libnet/2.ip/ip46.tcp.c \
library/libsoft/1/libnet/2.ip/ip46.udp.c \
library/libsoft/1/libnet/2.ip/ipx.spx.c \
library/libsoft/1/libnet/3.udp/udp.dns.c \
library/libsoft/1/libnet/3.udp/udp.hole.c \
library/libsoft/1/libnet/3.udp/udp.quic.c \
library/libsoft/1/libnet/3.udp/udp.tftp.c \
library/libsoft/1/libnet/4.tcp/tcp.dl.c \
library/libsoft/1/libnet/4.tcp/tcp.ftp.c \
library/libsoft/1/libnet/4.tcp/tcp.http.c \
library/libsoft/1/libnet/4.tcp/tcp.proxy.c \
library/libsoft/1/libnet/4.tcp/tcp.rdp.c \
library/libsoft/1/libnet/4.tcp/tcp.rtmp.c \
library/libsoft/1/libnet/4.tcp/tcp.sql.c \
library/libsoft/1/libnet/4.tcp/tcp.vnc.c \
library/libsoft/1/libnet/4.tcp/tcp.ssh.c \
library/libsoft/1/libnet/4.tcp/tcp.tls.c \
library/libsoft/1/libnet/4.tcp/tcp.ws.c \
library/libsoft/1/libphys/phys.elec.c \
library/libsoft/1/libphys/phys.mech.c \
library/libsoft/1/artery.c

#libuser0
LOCAL_SRC_FILES += \
library/libuser/0/osandroid/android.gles.c \
library/libuser/0/osandroid/android.cam.c \
library/libuser/0/osandroid/android.mic.c \
library/libuser/0/bygl/gl.modbuf.c \
library/libuser/0/bygl/gl.shader.c \
library/libuser/0/bygl/gl.texture.c \
library/libuser/0/bygl/gl.vertex.c \
library/libuser/0/byremote/remote.http.c \
library/libuser/0/byremote/remote.ws.c \
library/libuser/0/arena.c

#libuser1
LOCAL_SRC_FILES += \
library/libuser/1/lib1d/cli/print.cli.c \
library/libuser/1/lib1d/tui/print.tui.c \
library/libuser/1/lib1d/json/json.c \
library/libuser/1/lib1d/latex/latex.c \
library/libuser/1/lib1d/xml/html.c \
library/libuser/1/lib1d/xml/xaml.c \
library/libuser/1/lib1d/xml/xml.c \
library/libuser/1/lib1d/lib1d.c \
library/libuser/1/lib2d/draw/draw.line.c \
library/libuser/1/lib2d/draw/draw.solid.c \
library/libuser/1/lib2d/draw/draw.shape.c \
library/libuser/1/lib2d/draw/draw.ascii.c \
library/libuser/1/lib2d/draw/draw.utf8.c \
library/libuser/1/lib2d/draw/draw.vt100.c \
library/libuser/1/lib2d/photo/photo.blur.c \
library/libuser/1/lib2d/photo/photo.channel.c \
library/libuser/1/lib2d/photo/photo.convert.c \
library/libuser/1/lib2d/photo/photo.gray.c \
library/libuser/1/lib2d/photo/photo.mosaic.c \
library/libuser/1/lib2d/photo/photo.rotate.c \
library/libuser/1/lib2d/photo/photo.scale.c \
library/libuser/1/lib2d/lib2d.c \
library/libuser/1/lib3d/carve/carve.ascii.c \
library/libuser/1/lib3d/carve/carve.point.c \
library/libuser/1/lib3d/carve/carve.line.c \
library/libuser/1/lib3d/carve/carve.trigon.c \
library/libuser/1/lib3d/carve/carve.other.c \
library/libuser/1/lib3d/carve/carve2d.ascii.c \
library/libuser/1/lib3d/carve/carve2d.point.c \
library/libuser/1/lib3d/carve/carve2d.line.c \
library/libuser/1/lib3d/carve/carve2d.trigon.c \
library/libuser/1/lib3d/carve/carve2d.other.c \
library/libuser/1/lib3d/intersect/intersect.obb.c \
library/libuser/1/lib3d/intersect/intersect.ray.c \
library/libuser/1/lib3d/intersect/intersect.sphere.c \
library/libuser/1/lib3d/model/model.magnify.c \
library/libuser/1/lib3d/model/model.minify.c \
library/libuser/1/lib3d/model/model.rotate.c \
library/libuser/1/lib3d/lib3d.c \
library/libuser/1/lib4d/asset.file.c \
library/libuser/1/lib4d/asset.file.image.c \
library/libuser/1/lib4d/asset.file.model.c \
library/libuser/1/lib4d/asset.file.sound.c \
library/libuser/1/lib4d/asset.func.c \
library/libuser/1/lib4d/lib4d.c \
library/libuser/1/libbgfg/bgfg.gif.c \
library/libuser/1/libbgfg/bgfg.jpg.c \
library/libuser/1/libbgfg/bgfg.c \
library/libuser/1/libmode/mode0.testland.c \
library/libuser/1/libmode/mode1.console.c \
library/libuser/1/libmode/mode2.overview.c \
library/libuser/1/libmode/mode3.detail.c \
library/libuser/1/libmode/mode4.edit2d.c \
library/libuser/1/libmode/mode4.edit3d.c \
library/libuser/1/libmode/mode5.camera.c \
library/libuser/1/libmode/mode5.posture.c \
library/libuser/1/libmode/mode6.deliver.c \
library/libuser/1/libmode/mode7.oneonone.c \
library/libuser/1/libmode/mode.c \
library/libuser/1/libmenu/menu.list.c \
library/libuser/1/libmenu/menu.nav.c \
library/libuser/1/libmenu/menu.tag.c \
library/libuser/1/libmenu/menu.c \
library/libuser/1/libvkbd/vkbd.joystick.c \
library/libuser/1/libvkbd/vkbd.keyboard.c \
library/libuser/1/libvkbd/vkbd.c \
library/libuser/1/thealgo/algo.algo.c \
library/libuser/1/thealgo/algo.bintree.c \
library/libuser/1/thealgo/algo.bplus.c \
library/libuser/1/thealgo/algo.graph.c \
library/libuser/1/thegame/card/game.mahjong.c \
library/libuser/1/thegame/card/game.poker.c \
library/libuser/1/thegame/chess/game.chess.c \
library/libuser/1/thegame/chess/game.pegged.c \
library/libuser/1/thegame/chess/game.weiqi.c \
library/libuser/1/thegame/chess/game.xiangqi.c \
library/libuser/1/thegame/classic/game.klotski.c \
library/libuser/1/thegame/classic/game.snake.c \
library/libuser/1/thegame/classic/game.tetris.c \
library/libuser/1/thegame/game.2048.c \
library/libuser/1/thegame/game.maze.c \
library/libuser/1/thegame/game.ooxx.c \
library/libuser/1/thegame/game.rubik.c \
library/libuser/1/thegame/game.sudoku.c \
library/libuser/1/thehack/hack.fs.c \
library/libuser/1/thehack/hack.joystick.c \
library/libuser/1/thehack/hack.keyboard.c \
library/libuser/1/thehack/hack.rawdump.c \
library/libuser/1/thehack/hack.switch.c \
library/libuser/1/theitem/item.clock.c \
library/libuser/1/theitem/item.earth.c \
library/libuser/1/theitem/item.house.c \
library/libuser/1/theitem/item.human.c \
library/libuser/1/theitem/item.light.c \
library/libuser/1/theitem/item.mirror.c \
library/libuser/1/theitem/item.model.c \
library/libuser/1/theitem/item.piano.c \
library/libuser/1/theitem/item.picture.c \
library/libuser/1/theitem/item.portal.c \
library/libuser/1/theitem/item.skydome.c \
library/libuser/1/theitem/item.terrian.c \
library/libuser/1/theitem/item.tree.c \
library/libuser/1/theitem/item.water.c \
library/libuser/1/thetest/test.codeimg.c \
library/libuser/1/thetest/test.doodle.c \
library/libuser/1/thetest/test.example.c \
library/libuser/1/thetest/test.fractal.c \
library/libuser/1/thetest/test.palette.c \
library/libuser/1/thetest/test.planet.c \
library/libuser/1/thetool/tool.browser.c \
library/libuser/1/thetool/tool.calculator.c \
library/libuser/1/thetool/tool.camera.c \
library/libuser/1/thetool/tool.circuit.c \
library/libuser/1/thetool/tool.editor.c \
library/libuser/1/thetool/tool.font.c \
library/libuser/1/thetool/tool.hex.c \
library/libuser/1/thetool/tool.qrcode.c \
library/libuser/1/thetool/tool.sketchpad.c \
library/libuser/1/thetool/tool.spectrum.c \
library/libuser/1/thetool/tool.terminal.c \
library/libuser/1/actor.c




#2
LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/library/libsoft \
$(LOCAL_PATH)/library/libuser
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
LOCAL_LDLIBS := -lm -llog -ljnigraphics -landroid -lEGL -lGLESv3
LOCAL_MODULE := finalanswer
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
