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
texball{
fmt:texball
file:"datafile/jpg/skysphere.jpg"
}
terrain{
fmt:ground
}
freecam{
fmt:freecam
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
fullscreen{
c:0.0, 0.0
q:1.0, 1.0
}
view1{
l:-1.0, 0.0, 0.0
r: 1.0, 0.0, 0.0
b: 0.0,-0.829,-0.5591929
u: 0.0, 0.829, 0.5591929
n: 0.0, 0.5591929,-0.829
c: 0.0,-1118.3858, 1658.0
}
view2{
l:-1.0, 0.0, 0.0
r: 1.0, 0.0, 0.0
b: 0.0,-1.0, 0.0
u: 0.0, 1.0, 0.0
n: 0.0, 0.0,-1.0
c: 0.0, 0.0, 10000.0
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
sty3{}
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
(window, botleft, win, vp) -> (3dworld, 0, win, out)
(window, botright,win, vp) -> (3dworld, 0, win, out)
(window, topleft, win, vp) -> (3dworld, 0, win, out)
(window, topright,win, vp) -> (3dworld, 0, win, out)
(window, fullscreen, win, vp) -> (uiworld, 0, win, out)

(3dworld, view1, win, cam) -> (freecam, p0, act, cam)
(3dworld, sty1, win, bg) -> (texball, p1, act, 0)
(3dworld, sty2, win, bg) -> (terrain, p2, act, 0)

(uiworld, sty3, win, aid) -> (geomhelper, p3, act, 0)
(uiworld, sty4, win, aid) -> (fonthelper, p4, act, 0)

(uiworld, sty5, win, ui) -> (vjoy, p5, act, 0)
(uiworld, sty6, win, ui) -> (vkbd, p6, act, 0)
(uiworld, sty7, win, ui) -> (corner, p7, act, 0)
(uiworld, sty8, win, ui) -> (pointer, p8, act, 0)
</relation>