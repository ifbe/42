#include "actor.h"




void carvearrorkey2d(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u8* buf, int t)
{
	u32 c;
	carvesolid2d_circle(
		win, 0x404040,
		cx, cy, cz-0.1,
		rx, ry, rz,
		fx, fy, fz
	);

	//x-: left
	if(buf[0]&0x80)c = 0xff00ff;
	else c = 0x808080;
/*	carvesolid2d_rect(
		win, c,
		cx-rx*2/3, cy-ry*2/3, cz-rz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);*/
	carvesolid2d_circle(
		win, c,
		cx-rx*2/3, cy-ry*2/3, cz-rz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx-rx/2, cy-ry/2, cz-rz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[0]&0x7f
	);

	//x+: right
	if(buf[1]&0x80)c = 0xff00ff;
	else c = 0x808080;
/*	carvesolid2d_rect(
		win, c,
		cx+rx*2/3, cy+ry*2/3, cz+rz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);*/
	carvesolid2d_circle(
		win, c,
		cx+rx*2/3, cy+ry*2/3, cz+rz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx+rx*5/6, cy+ry*5/6, cz+rz*5/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[1]&0x7f
	);

	//y-: near
	if(buf[2]&0x80)c = 0xff00ff;
	else c = 0x808080;
/*	carvesolid2d_rect(
		win, c,
		cx-fx*2/3, cy-fy*2/3, cz-fz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);*/
	carvesolid2d_circle(
		win, c,
		cx-fx*2/3, cy-fy*2/3, cz-fz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx+rx/6-fx*2/3, cy+ry/6-fy*2/3, cz+rz/6-fz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[2]&0x7f
	);

	//y+: far
	if(buf[3]&0x80)c = 0xff00ff;
	else c = 0x808080;
/*	carvesolid2d_rect(
		win, c,
		cx+fx*2/3, cy+fy*2/3, cz+fz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);*/
	carvesolid2d_circle(
		win, c,
		cx+fx*2/3, cy+fy*2/3, cz+fz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx+rx/6+fx*2/3, cy+ry/6+fy*2/3, cz+rz/6+fz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[3]&0x7f
	);

	//z-: trigger
	if(buf[4]&0x80)c = 0xff00ff;
	else c = 0x808080;
/*	carvesolid2d_rect(
		win, c,
		cx-rx*2/3+fx*4/3, cy-ry*2/3+fy*4/3, cz-rz*2/3+fz*4/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);*/
	carvesolid2d_circle(
		win, c,
		cx-rx*2/3+fx*4/3, cy-ry*2/3+fy*4/3, cz-rz*2/3+fz*4/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx-rx/2+fx*4/3, cy-ry/2+fy*4/3, cz-rz/2+fz*4/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[4]&0x7f
	);

	//z+: bumper
	if(buf[5]&0x80)c = 0xff00ff;
	else c = 0x808080;
/*	carvesolid2d_rect(
		win, c,
		cx+rx*2/3+fx*4/3, cy+ry*2/3+fy*4/3, cz+rz*2/3+fz*4/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);*/
	carvesolid2d_circle(
		win, c,
		cx+rx*2/3+fx*4/3, cy+ry*2/3+fy*4/3, cz+rz*2/3+fz*4/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx+rx*5/6+fx*4/3, cy+ry*5/6+fy*4/3, cz+rz*5/6+fz*4/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[5]&0x7f
	);

	//press
	if(buf[6]&0x80)c = 0xff00ff;
	else c = 0x808080;
/*	carvesolid2d_rect(
		win, c,
		cx, cy, cz-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);*/
	carvesolid2d_circle(
		win, c,
		cx, cy, cz-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx+rx/6, cy+ry/6, cz+rz/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[6]&0x7f
	);

	//select or start
	if(buf[7]&0x80)c = 0xff00ff;
	else c = 0x808080;
/*	carvesolid2d_rect(
		win, c,
		cx+rx*t*4/3+fx*2/3, cy+ry*t*4/3+fy*2/3, cz+rz*t*4/3+fz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);*/
	carvesolid2d_circle(
		win, c,
		cx+rx*t*4/3+fx*2/3, cy+ry*t*4/3+fy*2/3, cz+rz*t*4/3+fz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx+rx*t*4/3+rx/6+fx*2/3, cy+ry*t*4/3+ry/6+fy*2/3, cz+rz*t*4/3+rz/6+fz*2/3-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[7]&0x7f
	);
}