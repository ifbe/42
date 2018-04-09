#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 18
#define pointv 0x84
void quaternionoperation(float*, float*, float);




void carvepoint2d(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += 1;

	vbuf[0] = cx;
	vbuf[1] = cy;
	vbuf[2] = 0.0;
	vbuf[3] = rr;
	vbuf[4] = gg;
	vbuf[5] = bb;
}
void carvepoint2d_bezier(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float xc, float yc, float zc)
{
	int j;
	float t;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += acc;

	for(j=0;j<=acc;j++)
	{
		t = (float)j / acc;

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*x1 + 2*t*(1.0-t)*xc + t*t*x2;
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*y1 + 2*t*(1.0-t)*yc + t*t*y2;
		vbuf[6*j+2] = 0.0;
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
	}
}




void carvepoint2d_triangle(
	struct arena* win, u32 rgb,
	float x0, float y0, float z0,
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
}
void carvepoint2d_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
}
void carvepoint2d_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int j,k;
	float s,t;
	float q[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += acc;

	q[0] = 0.0;
	q[1] = 0.0;
	q[2] = 1.0;
	for(j=0;j<acc;j++)
	{
		r[0] = rx;
		r[1] = ry;
		r[2] = 0.0;
		quaternionoperation(r, q, j*tau/acc);

		vbuf[6*j+0] = cx + r[0];
		vbuf[6*j+1] = cy + r[1];
		vbuf[6*j+2] = 0.0;
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
	}
}