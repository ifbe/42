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

//---------------------circles-----------------------
	//left
	carvesolid2d_circle(
		win, 0x808080,
		cx-rx/2, cy-ry/2, cz-rz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);

	//right
	carvesolid2d_circle(
		win, 0x808080,
		cx+rx/2, cy+ry/2, cz+rz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);

	//near
	carvesolid2d_circle(
		win, 0x808080,
		cx-fx/2, cy-fy/2, cz-fz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);

	//far
	carvesolid2d_circle(
		win, 0x808080,
		cx+fx/2, cy+fy/2, cz+fz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);

	//trigger
	carvesolid2d_circle(
		win, 0x808080,
		cx-rx/2+fx*7/6, cy-ry/2+fy*7/6, cz-rz/2+fz*7/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);

	//bumper
	carvesolid2d_circle(
		win, 0x808080,
		cx+rx/2+fx*7/6, cy+ry/2+fy*7/6, cz+rz/2+fz*7/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);

	//press
	carvesolid2d_circle(
		win, 0x808080,
		cx, cy, cz-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
/*
	//press
	carvesolid2d_circle(
		win, 0x808080,
		cx, cy, cz-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
*/
	//select or start
	carvesolid2d_circle(
		win, 0x808080,
		cx+rx*t+fx/2, cy+ry*t+fy/2, cz+rz*t+fz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);

//-----------------------characters-----------------------------
	//x-: left
	if(buf[0]&0x80)c = 0xff00ff;
	else c = 0xffffff;
	carve2d_ascii(
		win, c,
		cx+rx/6-rx/2, cy+ry/6-ry/2, cz+rz/6-rz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[0]&0x7f
	);

	//x+: right
	if(buf[1]&0x80)c = 0xff00ff;
	else c = 0xffffff;
	carve2d_ascii(
		win, c,
		cx+rx/6+rx/2, cy+ry/6+ry/2, cz+rz/6+rz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[1]&0x7f
	);

	//y-: near
	if(buf[2]&0x80)c = 0xff00ff;
	else c = 0xffffff;
	carve2d_ascii(
		win, c,
		cx+rx/6-fx/2, cy+ry/6-fy/2, cz+rz/6-fz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[2]&0x7f
	);

	//y+: far
	if(buf[3]&0x80)c = 0xff00ff;
	else c = 0xffffff;
	carve2d_ascii(
		win, c,
		cx+rx/6+fx/2, cy+ry/6+fy/2, cz+rz/6+fz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[3]&0x7f
	);

	//z-: trigger
	if(buf[4]&0x80)c = 0xff00ff;
	else c = 0xffffff;
	carve2d_ascii(
		win, c,
		cx-rx/3+fx*7/6, cy-ry/3+fy*7/6, cz-rz/3+fz*7/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[4]&0x7f
	);

	//z+: bumper
	if(buf[5]&0x80)c = 0xff00ff;
	else c = 0xffffff;
	carve2d_ascii(
		win, c,
		cx+rx/6+rx/2+fx*7/6, cy+ry/6+ry/2+fy*7/6, cz+rz/6+rz/2+fz*7/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[5]&0x7f
	);

	//press
	if(buf[6]&0x80)c = 0xff00ff;
	else c = 0xffffff;
	carve2d_ascii(
		win, c,
		cx+rx/6, cy+ry/6, cz+rz/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[6]&0x7f
	);

	//select or start
	if(buf[7]&0x80)c = 0xff00ff;
	else c = 0xffffff;
	carve2d_ascii(
		win, c,
		cx+rx*t+rx/6+fx/2, cy+ry*t+ry/6+fy/2, cz+rz*t+rz/6+fz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		buf[7]&0x7f
	);
}