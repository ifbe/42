#include "actor.h"




void carvearrorkey2d(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	int left, int right, int near, int far, int bot, int top)
{
	u32 c;
	carvesolid2d_circle(
		win, 0x404040,
		cx, cy, cz-0.1,
		rx, ry, rz,
		fx, fy, fz
	);

//---------------------circles-----------------------
	carvesolid2d_circle(
		win, 0x808080,
		cx, cy, cz-0.99,
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

//-----------------------characters-----------------------------
c = 0xffffff;
	//s
	carve2d_ascii(
		win, c,
		cx+rx/6, cy+ry/6, cz+rz/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		's'
	);

	//z-: trigger
	//if(left >= 'a'){c = 0xffffff;}
	//else {c = 0xff00ff;left += 0x20;}
	carve2d_ascii(
		win, c,
		cx-rx/3+fx*7/6, cy-ry/3+fy*7/6, cz-rz/3+fz*7/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		't'
	);

	//z+: bumper
	//if(right >= 'a'){c = 0xffffff;}
	//else {c = 0xff00ff;right += 0x20;}
	carve2d_ascii(
		win, c,
		cx+rx/6+rx/2+fx*7/6, cy+ry/6+ry/2+fy*7/6, cz+rz/6+rz/2+fz*7/6-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		'b'
	);

	//y-: near
	if(near >= 'a'){c = 0xffffff;}
	else {c = 0xff00ff;near += 0x20;}
	carve2d_ascii(
		win, c,
		cx+rx/6-fx/2, cy+ry/6-fy/2, cz+rz/6-fz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		near
	);

	//y+: far
	if(far >= 'a'){c = 0xffffff;}
	else {c = 0xff00ff;far += 0x20;}
	carve2d_ascii(
		win, c,
		cx+rx/6+fx/2, cy+ry/6+fy/2, cz+rz/6+fz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		far
	);

	//x-: left
	if(left >= 'a'){c = 0xffffff;}
	else {c = 0xff00ff;left += 0x20;}
	carve2d_ascii(
		win, c,
		cx+rx/6-rx/2, cy+ry/6-ry/2, cz+rz/6-rz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		left
	);

	//x+: right
	if(right >= 'a'){c = 0xffffff;}
	else {c = 0xff00ff;right += 0x20;}
	carve2d_ascii(
		win, c,
		cx+rx/6+rx/2, cy+ry/6+ry/2, cz+rz/6+rz/2-0.99,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		right
	);
}