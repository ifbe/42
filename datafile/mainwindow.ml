<arena>
window{
width:1024
height:1024
depth:1024
title:"42"
}
viewport1{}
viewport2{}
viewport3{}
3dworld{type:3d}
uiworld{type:ui}
trunk1{type:bg}
trunk2{type:fg}
trunk3{type:aid}
trunk4{type:ui}
</arena>


<actor>
texball{
file:"datafile/jpg/skysphere.jpg"
}
terrain{
file:"datafile/jpg/cartoon.jpg"
}
aidfont{}
aidgeom{}
vjoy{}
vkbd{}
corner{}
pointer{}
</actor>


<style>
leftpart{
c:0.0, 0.0
q:0.5, 1.0
}
rightpart{
c:0.5, 0.0
q:1.0, 1.0
}
fullscreen{
c:0.0, 0.0
q:1.0, 1.0
}
vp1{
l:-1.0, 0.0, 0.0
r: 1.0, 0.0, 0.0
b: 0.0,-cos34,-sin34
u: 0.0, cos34, sin34
n: 0.0, sin34,-cos34
c: 0.0,-2000.0 * sin34,  2000.0 * cos34
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
c: 0,0, 0,0, 0.0
r: 10000000.0, 0.0, 0.0
f: 0.0, 10000000.0, 0.0
u: 0.0, 0.0, 10000000.0
}
sty2{
c: 0,0, 0,0, 0.0
r: 10000000.0, 0.0, 0.0
f: 0.0, 10000000.0, 0.0
u: 0.0, 0.0, 10000000.0
}
sty3{}
sty4{}
sty5{
c: 0,0,-0,75, 0.0
r: 1.0, 0.0, 0.0
f: 0.0, 0.25, 0.0
}
sty6{
c: 0,0,-0,75, 0.0
r: 1.0, 0.0, 0.0
f: 0.0, 0.25, 0.0
}
sty7{
c: 0,0, 0,0, 0.0
r: 10000000.0, 0.0, 0.0
f: 0.0, 10000000.0, 0.0
u: 0.0, 0.0, 10000000.0
}
sty8{
c: 0,0, 0,0, 0.0
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
(window, leftpart, _win_) -> (viewport1, 0, _win_)
(viewport1, vp1, _win_) -> (3dworld, 0, _win_)

(3dworld, vp1, _win_) -> (trunk1, 0, _win_)

(trunk1, sty1, _win_) -> (texball, p1, _act_)
(trunk1, sty2, _win_) -> (terrain, p2, _act_)

(trunk3, sty3, _win_) -> (aidgeom, p3, _act_)
(trunk3, sty4, _win_) -> (aidfont, p4, _act_)

(trunk4, sty5, _win_) -> (vjoy, p5, _act_)
(trunk4, sty6, _win_) -> (vkbd, p6, _act_)
(trunk4, sty7, _win_) -> (corner, p7, _act_)
(trunk4, sty8, _win_) -> (pointer, p8, _act_)
</relation>