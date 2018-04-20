#include "actor.h"




void carvearrorkey2d(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz)
{
	carvesolid2d_circle(
		win, 0x404040,
		cx, cy, cz,
		rx, ry, rz,
		fx, fy, fz
	);

	//up
	carvesolid2d_circle(
		win, 0x808080,
		cx+fx/2, cy+fy/2, cz+fz/2-0.002,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx+fx/2, cy+fy/2, cz+fz/2-0.002,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		'f'
	);

	//down
	carvesolid2d_circle(
		win, 0x808080,
		cx-fx/2, cy-fy/2, cz-fz/2-0.002,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx-fx/2, cy-fy/2, cz-fz/2-0.002,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		'n'
	);

	//left
	carvesolid2d_circle(
		win, 0x808080,
		cx-rx/2, cy-ry/2, cz-rz/2-0.002,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx-rx/2, cy-ry/2, cz-rz/2-0.002,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		'l'
	);

	//right
	carvesolid2d_circle(
		win, 0x808080,
		cx+rx/2, cy+ry/2, cz+rz/2-0.002,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3
	);
	carve2d_ascii(
		win, 0xffffff,
		cx+rx/2, cy+ry/2, cz+rz/2-0.002,
		rx/3, ry/3, rz/3,
		fx/3, fy/3, fz/3,
		'r'
	);
}