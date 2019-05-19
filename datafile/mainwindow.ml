<arena>
window{
fmt:win
title:"42"
width:1024
height:1024
depth:1024
}
3dworld{
fmt:vbo
type:3d
}
uiworld{
fmt:vbo
type:ui
}
</arena>


<actor>
cambl{
fmt:freecam
}
cambr{
fmt:freecam
}
camtl{
fmt:freecam
}
camtr{
fmt:freecam
}
texball{
fmt:texball
file:"datafile/jpg/skysphere.jpg"
}
terrain{
fmt:ground
}
rabbit{
fmt:model
}

uicamera{
fmt:orthcam
}
fonthelper{
fmt:aidfont
}
geomhelper{
fmt:aidgeom
}
vjoy{
fmt:vjoy
}
vkbd{
fmt:vkbd
}
corner{
fmt:corner
}
pointer{
fmt:pointer
}
</actor>


<style>
botleft{
c:0.0, 0.0
q:0.5, 0.5
}
botright{
c:0.5, 0.0
q:0.5, 0.5
}
topleft{
c:0.0, 0.5
q:0.5, 0.5
}
topright{
c:0.5, 0.5
q:0.5, 0.5
}
rtslook{
r: 1.0, 0.0, 0.0
u: 0.0, 0.829, 0.5591929
f: 0.0, 0.5591929,-0.829
c: 0.0,-1118.3858, 1658.0
}
overlook{
r: 1.0, 0.0, 0.0
u: 0.0, 1.0, 0.0
f: 0.0, 0.0,-1.0
c: 0.0, 0.0, 1000.0
}
frontlook{
r: 1.0, 0.0, 0.0
u: 0.0, 0.0, 1.0
f: 0.0, 1.0, 0.0
c: 0.0, -1000.0, 500.0
}
sidelook{
r: 0.0, 1.0, 0.0
u: 0.0, 0.0, 1.0
f:-1.0, 0.0, 0.0
c: 1000.0, 0.0, 500.0
}

fullscreen{
c:0.0, 0.0
q:1.0, 1.0
}
ortho{
c: 0.0, 0.0, -1.0
}
sty1{
c: 0.0, 0.0, 0.0
r: 10000000.0, 0.0, 0.0
f: 0.0, 10000000.0, 0.0
u: 0.0, 0.0, 10000000.0
}
sty2{
c: 0.0, 0.0, 0.0
r: 10000000.0, 0.0, 0.0
f: 0.0, 10000000.0, 0.0
u: 0.0, 0.0, 10000000.0
}
sty3{
c: 0.0, 0.0, 0.0
r: 100.0, 0.0, 0.0
f: 0.0, 100.0, 0.0
u: 0.0, 0.0, 100.0
}
sty4{}
sty5{
c: 0.0,-0.75, 0.0
r: 1.0, 0.0, 0.0
f: 0.0, 0.25, 0.0
}
sty6{
c: 0.0,-0.75, 0.0
r: 1.0, 0.0, 0.0
f: 0.0, 0.25, 0.0
}
sty7{
c: 0.0, 0.0, 0.0
r: 10000000.0, 0.0, 0.0
f: 0.0, 10000000.0, 0.0
u: 0.0, 0.0, 10000000.0
}
sty8{
c: 0.0, 0.0, 0.0
r: 10000000.0, 0.0, 0.0
f: 0.0, 10000000.0, 0.0
u: 0.0, 0.0, 10000000.0
}
</style>


<pinid>
p0{}
p1{}
p2{}
p3{}
p4{}
p5{}
p6{}
p7{}
p8{}
</pinid>


<relation>
(window, 0, win, vp) -> (3dworld, 0, win, out)
(window, botleft, win, cam) -> (cambl, 0, act, cam)
(window, botright,win, cam) -> (cambr, 0, act, cam)
(window, topleft, win, cam) -> (camtl, 0, act, cam)
(window, topright,win, cam) -> (camtr, 0, act, cam)

(3dworld, rtslook, win, cam) -> (cambl, p0, act, geom)
(3dworld, overlook, win, cam) -> (cambr, p0, act, geom)
(3dworld, frontlook, win, cam) -> (camtl, p0, act, geom)
(3dworld, sidelook, win, cam) -> (camtr, p0, act, geom)
(3dworld, sty1, win, bg) -> (texball, p1, act, 0)
(3dworld, sty2, win, bg) -> (terrain, p2, act, 0)
(3dworld, sty3, win, bg) -> (rabbit, p3, act, 0)

(window, 0, win, vp) -> (uiworld, 0, win, out)
(window, fullscreen,win, cam) -> (uicamera, 0, act, cam)

(uiworld, ortho, win, cam) -> (uicamera, 0, act, geom)
(uiworld, 0, win, aid) -> (geomhelper, 0, act, 0)
(uiworld, 0, win, aid) -> (fonthelper, 0, act, 0)
(uiworld, sty5, win, ui) -> (vjoy, p5, act, 0)
(uiworld, sty6, win, ui) -> (vkbd, p6, act, 0)
(uiworld, sty7, win, ui) -> (corner, p7, act, 0)
(uiworld, sty8, win, ui) -> (pointer, p8, act, 0)
</relation>