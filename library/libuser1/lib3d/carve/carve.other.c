#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 18
#define stlv 0x82
void quaternionoperation(float*, float*, float);
u32 getrandom();




void carveaxis(struct arena* win)
{
	carveline(win, 0xff0000, 0.0, 0.0, 0.0, 10000.0, 0.0, 0.0);
	carveline(win, 0x00ff00, 0.0, 0.0, 0.0, 0.0, 10000.0, 0.0);
	carveline(win, 0x0000ff, 0.0, 0.0, 0.0, 0.0, 0.0, 10000.0);
}
void carvedrone_node(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	float j;
	float up[3];
	float tt[3];
	float v0[3];
	float v1[3];
	float v2[3];

	up[0] = ux;
	up[1] = uy;
	up[2] = uz;
	tt[0] = rx;
	tt[1] = ry;
	tt[2] = rz;
	quaternionoperation(tt, up, tau/3);

	v0[0] = cx + rx;
	v0[1] = cy + ry;
	v0[2] = cz + rz;
	v1[0] = cx + tt[0];
	v1[1] = cy + tt[1];
	v1[2] = cz + tt[2];
	v2[0] = cx - tt[0] - rx;
	v2[1] = cy - tt[1] - ry;
	v2[2] = cz - tt[2] - rz;

	carveline_yshape(
		win, 0x204020,
		v0[0], v0[1], v0[2],
		v1[0], v1[1], v1[2],
		v2[0], v2[1], v2[2]
	);
	carveline_yshape(
		win, 0x204020,
		v0[0], v0[1], v0[2]+16.0,
		v1[0], v1[1], v1[2]+16.0,
		v2[0], v2[1], v2[2]+16.0
	);

	carveline_yshape(
		win, 0xffffff,
		(v0[0]+v1[0])/2, (v0[1]+v1[1])/2, (v0[2]+v1[2])/2,
		(v1[0]+v2[0])/2, (v1[1]+v2[1])/2, (v1[2]+v2[2])/2,
		(v2[0]+v0[0])/2, (v2[1]+v0[1])/2, (v2[2]+v0[2])/2
	);
	carveline_yshape(
		win, 0xffffff,
		(v0[0]+v1[0])/2, (v0[1]+v1[1])/2, (v0[2]+v1[2])/2+16.0,
		(v1[0]+v2[0])/2, (v1[1]+v2[1])/2, (v1[2]+v2[2])/2+16.0,
		(v2[0]+v0[0])/2, (v2[1]+v0[1])/2, (v2[2]+v0[2])/2+16.0
	);

	carveline(
		win, 0xffffff,
		cx, cy, cz,
		cx, cy, cz+16.0
	);
	carvesolid_triangle(
		win, 0x808080,
		v0[0], v0[1], v0[2],
		v1[0], v1[1], v1[2],
		v2[0], v2[1], v2[2]
	);
	carvesolid_triangle(
		win, 0x808080,
		v0[0], v0[1], v0[2]+16.0,
		v1[0], v1[1], v1[2]+16.0,
		v2[0], v2[1], v2[2]+16.0
	);

	carvesolid_rect(
		win, 0x604020,
		(v0[0]+v1[0])/2, (v0[1]+v1[1])/2, (v0[2]+v1[2])/2+8.0,
		(v0[0]-v1[0])/2, (v0[1]-v1[1])/2, (v0[2]-v1[2])/2,
		0.0, 0.0, 8.0
	);
	carvesolid_rect(
		win, 0x206040,
		(v2[0]+v1[0])/2, (v2[1]+v1[1])/2, (v2[2]+v1[2])/2+8.0,
		(v2[0]-v1[0])/2, (v2[1]-v1[1])/2, (v2[2]-v1[2])/2,
		0.0, 0.0, 8.0
	);
	carvesolid_rect(
		win, 0x204060,
		(v0[0]+v2[0])/2, (v0[1]+v2[1])/2, (v0[2]+v2[2])/2+8.0,
		(v0[0]-v2[0])/2, (v0[1]-v2[1])/2, (v0[2]-v2[2])/2,
		0.0, 0.0, 8.0
	);
}
void carvedrone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int x,y;
	float up[3];
	float tt[3];
	float xx[3];
	float yy[3];
	float v0[3];
	float v1[3];
	float v2[3];

	up[0] = ux;
	up[1] = uy;
	up[2] = uz;
	tt[0] = rx;
	tt[1] = ry;
	tt[2] = rz;
	quaternionoperation(tt, up, tau/3);
	v0[0] = rx;
	v0[1] = ry;
	v0[2] = rz;
	v1[0] = tt[0];
	v1[1] = tt[1];
	v1[2] = tt[2];
	v2[0] = -tt[0]-rx;
	v2[1] = -tt[1]-ry;
	v2[2] = -tt[2]-rz;

	xx[0] = v1[0]-v2[0];
	xx[1] = v1[1]-v2[1];
	xx[2] = v1[2]-v2[2];
	yy[0] = rx*3/2;
	yy[1] = ry*3/2;
	yy[2] = rz*3/2;
	for(y=-2;y<=2;y++)
	{
		for(x=-2;x<=2;x++)
		{
			tt[0] = cx+xx[0]*x+yy[0]*y;
			tt[1] = cy+xx[1]*x+yy[1]*y;
			tt[2] = cz+xx[2]*x+yy[2]*y;
			if(y%2)
			{
				tt[0] += xx[0]/2;
				tt[1] += xx[1]/2;
				tt[2] += xx[2]/2;
			}
			carvedrone_node(
				win, rgb,
				tt[0], tt[1], tt[2],
				0.95*rx, 0.95*ry, 0.95*rz,
				ux, uy, uz
			);
			carvedrone_node(
				win, rgb,
				tt[0]+(rx-xx[0])/2, tt[1]+(ry-xx[1])/2, tt[2]+(rz-xx[2])/2,
				-0.95*rx, -0.95*ry, -0.95*rz,
				ux, uy, uz
			);
		}
	}
/*
	//0
	carvedrone_node(
		win, rgb,
		cx, cy, cz,
		rx, ry, rz,
		ux, uy, uz
	);

	//123
	carvedrone_node(
		win, rgb,
		cx+v0[0]*2, cy+v0[1]*2, cz+v0[2]*2,
		-v0[0], -v0[1], -v0[2],
		ux, uy, uz
	);
	carvedrone_node(
		win, rgb,
		cx+v1[0]*2, cy+v1[1]*2, cz+v1[2]*2,
		-v1[0], -v1[1], -v1[2],
		ux, uy, uz
	);
	carvedrone_node(
		win, rgb,
		cx+v2[0]*2, cy+v2[1]*2, cz+v2[2]*2,
		-v2[0], -v2[1], -v2[2],
		ux, uy, uz
	);

	//456
	carvedrone_node(
		win, rgb,
		cx+v0[0]+v1[0], cy+v0[1]+v1[1], cz+v0[2]+v1[2],
		-v2[0], -v2[1], -v2[2],
		ux, uy, uz
	);
	carvedrone_node(
		win, rgb,
		cx+v1[0]+v2[0], cy+v1[1]+v2[1], cz+v1[2]+v2[2],
		-v0[0], -v0[1], -v0[2],
		ux, uy, uz
	);
	carvedrone_node(
		win, rgb,
		cx+v2[0]+v0[0], cy+v2[1]+v0[1], cz+v2[2]+v0[2],
		-v1[0], -v1[1], -v1[2],
		ux, uy, uz
	);

	//789abc
	carvedrone_node(
		win, rgb,
		cx+v1[0]-v0[0], cy+v1[1]-v0[1], cz+v1[2]-v0[2],
		v0[0], v0[1], v0[2],
		ux, uy, uz
	);
	carvedrone_node(
		win, rgb,
		cx+v2[0]-v0[0], cy+v2[1]-v0[1], cz+v2[2]-v0[2],
		v0[0], v0[1], v0[2],
		ux, uy, uz
	);
	carvedrone_node(
		win, rgb,
		cx+v0[0]-v1[0], cy+v0[1]-v1[1], cz+v0[2]-v1[2],
		v1[0], v1[1], v1[2],
		ux, uy, uz
	);
	carvedrone_node(
		win, rgb,
		cx+v2[0]-v1[0], cy+v2[1]-v1[1], cz+v2[2]-v1[2],
		v1[0], v1[1], v1[2],
		ux, uy, uz
	);
	carvedrone_node(
		win, rgb,
		cx+v0[0]-v2[0], cy+v0[1]-v2[1], cz+v0[2]-v2[2],
		v2[0], v2[1], v2[2],
		ux, uy, uz
	);
	carvedrone_node(
		win, rgb,
		cx+v1[0]-v2[0], cy+v1[1]-v2[1], cz+v1[2]-v2[2],
		v2[0], v2[1], v2[2],
		ux, uy, uz
	);
*/
}




void carvestarry_random(struct arena* win)
{
}
void carveneural_random(struct arena* win)
{
}
void carvestl(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float sx, float sy, float sz,
	void* stlbuf, int stllen, float f)
{
	float* p;
	int j, k, ret;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int vlen = mod[stlv].vlen;
	float* vbuf = (mod[stlv].vbuf) + (36*vlen);

	ret = *(u32*)(stlbuf+80);
	ret = ret%(0x1000000/36);
	mod[stlv].vlen += ret*3;

	for(j=0;j<ret;j++)
	{
		p = (void*)stlbuf + 84 + j*50;
		k = j*27;

		vbuf[k+ 0] = cx + (p[3]-sx)*f;
		vbuf[k+ 1] = cy + (p[4]-sy)*f;
		vbuf[k+ 2] = (p[5]-sz)*f;
		vbuf[k+ 3] = 1.0;
		vbuf[k+ 4] = 1.0;
		vbuf[k+ 5] = 1.0;
		vbuf[k+ 6] = p[0];
		vbuf[k+ 7] = p[1];
		vbuf[k+ 8] = p[2];

		vbuf[k+ 9] = cx + (p[6]-sx)*f;
		vbuf[k+10] = cy + (p[7]-sy)*f;
		vbuf[k+11] = (p[8]-sz)*f;
		vbuf[k+12] = 1.0;
		vbuf[k+13] = 1.0;
		vbuf[k+14] = 1.0;
		vbuf[k+15] = p[0];
		vbuf[k+16] = p[1];
		vbuf[k+17] = p[2];

		vbuf[k+18] = cx + (p[9]-sx)*f;
		vbuf[k+19] = cy + (p[10]-sy)*f;
		vbuf[k+20] = (p[11]-sz)*f;
		vbuf[k+21] = 1.0;
		vbuf[k+22] = 1.0;
		vbuf[k+23] = 1.0;
		vbuf[k+24] = p[0];
		vbuf[k+25] = p[1];
		vbuf[k+26] = p[2];
	}
}