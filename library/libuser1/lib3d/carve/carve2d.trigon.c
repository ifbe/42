#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 18
#define trigonv 0x86
void quaternionoperation(float*, float*, float);




void carvesolid2d_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[trigonv].ilen;
	int vlen = mod[trigonv].vlen;
	u16* ibuf = (mod[trigonv].ibuf) + (6*ilen);
	float* vbuf = (mod[trigonv].vbuf) + (24*vlen);
	mod[trigonv].ilen += 1;
	mod[trigonv].vlen += 3;

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

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}
void carvesolid2d_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[trigonv].ilen;
	int vlen = mod[trigonv].vlen;
	u16* ibuf = (mod[trigonv].ibuf) + (6*ilen);
	float* vbuf = (mod[trigonv].vbuf) + (24*vlen);
	mod[trigonv].ilen += 2;
	mod[trigonv].vlen += 4;

	vbuf[ 0] = cx - rx - fx;
	vbuf[ 1] = cy - ry - fy;
	vbuf[ 2] = cz;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = cx + rx - fx;
	vbuf[ 7] = cy + ry - fy;
	vbuf[ 8] = cz;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = cx - rx + fx;
	vbuf[13] = cy - ry + fy;
	vbuf[14] = cz;
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = cx + rx + fx;
	vbuf[19] = cy + ry + fy;
	vbuf[20] = cz;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}
void carvesolid2d_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz)
{
	int a,b,j;
	float c,s;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[trigonv].ilen;
	int vlen = mod[trigonv].vlen;
	u16* ibuf = (mod[trigonv].ibuf) + (6*ilen);
	float* vbuf = (mod[trigonv].vbuf) + (24*vlen);
	mod[trigonv].ilen += acc;
	mod[trigonv].vlen += acc+1;

	for(j=0;j<acc;j++)
	{
		a = j*6;
		b = j*3;

		c = cosine(j*tau/acc);
		s = sine(j*tau/acc);
		vbuf[a+0] = cx + rx*c + fx*s;
		vbuf[a+1] = cy + ry*c + fy*s;
		vbuf[a+2] = cz;

		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;

		ibuf[b+0] = vlen + acc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%acc;
	}

	a = acc*6;
	vbuf[a+0] = cx;
	vbuf[a+1] = cy;
	vbuf[a+2] = 0.0;
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
}