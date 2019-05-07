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
fmt:terrain
file:"datafile/jpg/cartoon.jpg"
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
leftpart{
c:0.0, 0.0
q:0.5, 1.0
}
rightpart{
c:0.5, 0.0
q:0.5, 1.0
}
fullscreen{
c:0.0, 0.0
q:1.0, 1.0
}
vp1{
l:-1.0, 0.0, 0.0
r: 1.0, 0.0, 0.0
b: 0.0,-0.829,-0.5591929
u: 0.0, 0.829, 0.5591929
n: 0.0, 0.5591929,-0.829
c: 0.0,-1118.3858, 1658.0
}
vp2{
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
(window, leftpart, _win_, _vp_) -> (3dworld, 0, _win_, _out_)
(window, rightpart, _win_, _vp_) -> (3dworld, 0, _win_, _out_)
(window, fullscreen, _win_, _vp_) -> (uiworld, 0, _win_, _out_)

(3dworld, sty1, _win_, _bg_) -> (texball, p1, _act_, 0)
(3dworld, sty2, _win_, _bg_) -> (terrain, p2, _act_, 0)

(uiworld, sty3, _win_, _aid_) -> (aidgeom, p3, _act_, 0)
(uiworld, sty4, _win_, _aid_) -> (aidfont, p4, _act_, 0)

(uiworld, sty5, _win_, _ui_) -> (vjoy, p5, _act_, 0)
(uiworld, sty6, _win_, _ui_) -> (vkbd, p6, _act_, 0)
(uiworld, sty7, _win_, _ui_) -> (corner, p7, _act_, 0)
(uiworld, sty8, _win_, _ui_) -> (pointer, p8, _act_, 0)
</relation>