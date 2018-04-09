#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 24
#define linev 0x85
void quaternionoperation(float*, float*, float);




void carveline2d(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[linev].ilen;
	int vlen = mod[linev].vlen;
	u16* ibuf = (mod[linev].ibuf) + (4*ilen);
	float* vbuf = (mod[linev].vbuf) + (24*vlen);
	mod[linev].ilen += 1;
	mod[linev].vlen += 2;

	vbuf[ 0] = x1;
	vbuf[ 1] = y1;
	vbuf[ 2] = 0.0;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = x2;
	vbuf[ 7] = y2;
	vbuf[ 8] = 0.0;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
}
void carveline2d_bezier(
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
	int ilen = mod[linev].ilen;
	int vlen = mod[linev].vlen;
	u16* ibuf = (mod[linev].ibuf) + (4*ilen);
	float* vbuf  = (mod[linev].vbuf) + (24*vlen);
	mod[linev].ilen += acc;
	mod[linev].vlen += acc + 1;

	for(j=0;j<=acc;j++)
	{
		t = (float)j / acc;

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*x1 + 2*t*(1.0-t)*xc + t*t*x2;
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*y1 + 2*t*(1.0-t)*yc + t*t*y2;
		vbuf[6*j+2] = 0.0;

		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;

		if(j >= acc)break;
		ibuf[2*j+0] = vlen + j;
		ibuf[2*j+1] = vlen + j+1;
	}
}
void carveline2d_yshape(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[linev].ilen;
	int vlen = mod[linev].vlen;
	u16* ibuf = (mod[linev].ibuf) + (4*ilen);
	float* vbuf  = (mod[linev].vbuf) + (24*vlen);
	mod[linev].ilen += 3;
	mod[linev].vlen += 4;

	vbuf[ 0] = x1;
	vbuf[ 1] = y1;
	vbuf[ 2] = 0.0;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = x2;
	vbuf[ 7] = y2;
	vbuf[ 8] = 0.0;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = x3;
	vbuf[13] = y3;
	vbuf[14] = 0.0;
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = (x1+x2+x3)/3;
	vbuf[19] = (y1+y2+y3)/3;
	vbuf[20] = 0.0;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+3;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+3;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}




void carveline2d_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[linev].ilen;
	int vlen = mod[linev].vlen;
	u16* ibuf = (mod[linev].ibuf) + (4*ilen);
	float* vbuf  = (mod[linev].vbuf) + (24*vlen);
	mod[linev].ilen += 3;
	mod[linev].vlen += 3;

	vbuf[ 0] = x1;
	vbuf[ 1] = y1;
	vbuf[ 2] = 0.0;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = x2;
	vbuf[ 7] = y2;
	vbuf[ 8] = 0.0;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = x3;
	vbuf[13] = y3;
	vbuf[14] = 0.0;
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+2;
	ibuf[4] = vlen;
	ibuf[5] = vlen+2;
}
void carveline2d_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[linev].ilen;
	int vlen = mod[linev].vlen;
	u16* ibuf = (mod[linev].ibuf) + (4*ilen);
	float* vbuf  = (mod[linev].vbuf) + (24*vlen);
	mod[linev].ilen += 4;
	mod[linev].vlen += 4;

	vbuf[ 0] = cx - rx - fx;
	vbuf[ 1] = cy - ry - fy;
	vbuf[ 2] = 0.0;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = cx + rx - fx;
	vbuf[ 7] = cy + ry - fy;
	vbuf[ 8] = 0.0;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = cx - rx + fx;
	vbuf[13] = cy - ry + fy;
	vbuf[14] = 0.0;
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = cx + rx + fx;
	vbuf[19] = cy + ry + fy;
	vbuf[20] = 0.0;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+3;
	ibuf[4] = vlen+3;
	ibuf[5] = vlen+2;
	ibuf[6] = vlen+0;
	ibuf[7] = vlen+2;
}
void carveline2d_hexagon(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	float v[4];
	float q[4];
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[linev].ilen;
	int vlen = mod[linev].vlen;
	u16* ibuf = (mod[linev].ibuf) + (4*ilen);
	float* vbuf  = (mod[linev].vbuf) + (24*vlen);
	mod[linev].ilen += 6;
	mod[linev].vlen += 6;

	//0
	vbuf[ 0] = cx + rx;
	vbuf[ 1] = cy + ry;
	vbuf[ 2] = 0.0;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	//180
	vbuf[18] = cx - rx;
	vbuf[19] = cy - ry;
	vbuf[20] = 0.0;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	v[0] = rx;
	v[1] = ry;
	v[2] = 0.0;
	q[0] = 0.0;
	q[1] = 0.0;
	q[2] = 1.0;
	quaternionoperation(v, q, PI/3);

	//60
	vbuf[ 6] = cx + v[0];
	vbuf[ 7] = cy + v[1];
	vbuf[ 8] = 0.0;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	//240
	vbuf[24] = cx - v[0];
	vbuf[25] = cy - v[1];
	vbuf[26] = 0.0;
	vbuf[27] = rr;
	vbuf[28] = gg;
	vbuf[29] = bb;

	v[0] -= rx;
	v[1] -= ry;
	v[2] -= 0.0;

	//120
	vbuf[12] = cx + v[0];
	vbuf[13] = cy + v[1];
	vbuf[14] = 0.0;
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	//300
	vbuf[30] = cx - v[0];
	vbuf[31] = cy - v[1];
	vbuf[32] = 0.0;
	vbuf[33] = rr;
	vbuf[34] = gg;
	vbuf[35] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+2;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
	ibuf[6] = vlen+3;
	ibuf[7] = vlen+4;
	ibuf[8] = vlen+4;
	ibuf[9] = vlen+5;
	ibuf[10] = vlen+5;
	ibuf[11] = vlen+0;
}
void carveline2d_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
#define lineacc (acc*2)
	int j;
	float q[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[linev].ilen;
	int vlen = mod[linev].vlen;
	u16* ibuf = (mod[linev].ibuf) + (4*ilen);
	float* vbuf  = (mod[linev].vbuf) + (24*vlen);
	mod[linev].ilen += lineacc;
	mod[linev].vlen += lineacc;

	q[0] = 0.0;
	q[1] = 0.0;
	q[2] = 1.0;
	for(j=0;j<lineacc;j++)
	{
		vbuf[6*j+0] = rx;
		vbuf[6*j+1] = ry;
		vbuf[6*j+2] = 0.0;
		quaternionoperation(&vbuf[6*j], q, j*tau/lineacc);

		vbuf[6*j+0] += cx;
		vbuf[6*j+1] += cy;
		vbuf[6*j+2] += 0.0;

		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;

		ibuf[2*j+0] = vlen+j;
		ibuf[2*j+1] = vlen+(j+1)%lineacc;
	}
}