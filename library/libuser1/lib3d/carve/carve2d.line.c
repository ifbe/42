#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define halfsqrt3 0.8660254037844386
#define tau (PI*2)
#define acc 24
#define linev 0x85




void carveline2d(struct arena* win, u32 rgb,
	vec3 va, vec3 vb)
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

	vbuf[ 0] = va[0];
	vbuf[ 1] = va[1];
	vbuf[ 2] = va[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = vb[0];
	vbuf[ 7] = vb[1];
	vbuf[ 8] = vb[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
}
void carveline2d_arrow(struct arena* win, u32 rgb,
	vec3 va, vec3 vb)
{
	vec3 v0;
	vec3 v1;
	vec3 vv;
	float x,y,w,h,t;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[linev].ilen;
	int vlen = mod[linev].vlen;
	u16* ibuf = (mod[linev].ibuf) + (4*ilen);
	float* vbuf = (mod[linev].vbuf) + (24*vlen);
	mod[linev].ilen += 3;
	mod[linev].vlen += 4;

	vbuf[ 0] = va[0];
	vbuf[ 1] = va[1];
	vbuf[ 2] = va[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = vb[0];
	vbuf[ 7] = vb[1];
	vbuf[ 8] = vb[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	w = (float)(win->width);
	h = (float)(win->height);
	v0[0] = (va[0]+1.0)*w/2.0;
	v0[1] = (1.0-va[1])*h/2.0;
	v1[0] = (vb[0]+1.0)*w/2.0;
	v1[1] = (1.0-vb[1])*h/2.0;

	x = v0[0]-v1[0];
	y = v0[1]-v1[1];
	t = squareroot(x*x+y*y);
	x = 16*x/t;
	y = 16*y/t;

	vv[0] = v1[0] + halfsqrt3*x + 0.5*y;
	vv[1] = v1[1] + halfsqrt3*y - 0.5*x;
	vbuf[12] = (vv[0]*2.0/w)-1.0;
	vbuf[13] = 1.0-(vv[1]*2.0/h);
	vbuf[14] = vb[2];
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vv[0] = v1[0] + halfsqrt3*x - 0.5*y;
	vv[1] = v1[1] + halfsqrt3*y + 0.5*x;
	vbuf[18] = (vv[0]*2.0/w)-1.0;
	vbuf[19] = 1.0-(vv[1]*2.0/h);
	vbuf[20] = vb[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+2;
	ibuf[3] = vlen+1;
	ibuf[4] = vlen+3;
	ibuf[5] = vlen+1;
}
void carveline2d_bezier(struct arena* win, u32 rgb,
	vec3 va, vec3 vb, vec3 vt)
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

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*va[0] + 2*t*(1.0-t)*vt[0] + t*t*vb[0];
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*va[1] + 2*t*(1.0-t)*vt[1] + t*t*vb[1];
		vbuf[6*j+2] = vt[2];

		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;

		if(j >= acc)break;
		ibuf[2*j+0] = vlen + j;
		ibuf[2*j+1] = vlen + j+1;
	}
}




void carveline2d_yshape(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
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

	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = v1[0];
	vbuf[ 7] = v1[1];
	vbuf[ 8] = v1[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = v2[0];
	vbuf[13] = v2[1];
	vbuf[14] = v2[2];
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = (v0[0]+v1[0]+v2[0])/3;
	vbuf[19] = (v0[1]+v1[1]+v2[1])/3;
	vbuf[20] = (v0[2]+v1[2]+v2[2])/3;
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
void carveline2d_triangle(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
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

	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = v1[0];
	vbuf[ 7] = v1[1];
	vbuf[ 8] = v1[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = v2[0];
	vbuf[13] = v2[1];
	vbuf[14] = v2[2];
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
void carveline2d_rect(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
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

	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = vc[0] + vr[0] - vf[0];
	vbuf[ 7] = vc[1] + vr[1] - vf[1];
	vbuf[ 8] = vc[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = vc[0] - vr[0] + vf[0];
	vbuf[13] = vc[1] - vr[1] + vf[1];
	vbuf[14] = vc[2];
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = vc[0] + vr[0] + vf[0];
	vbuf[19] = vc[1] + vr[1] + vf[1];
	vbuf[20] = vc[2];
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
void carveline2d_hexagon(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
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
	vbuf[ 0] = vc[0] + vr[0];
	vbuf[ 1] = vc[1] + vr[1];
	vbuf[ 2] = 0.0;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	//180
	vbuf[18] = vc[0] - vr[0];
	vbuf[19] = vc[1] - vr[1];
	vbuf[20] = 0.0;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	v[0] = vr[0];
	v[1] = vr[1];
	v[2] = 0.0;
	q[0] = 0.0;
	q[1] = 0.0;
	q[2] = 1.0;
	quaternion_operation(v, q, PI/3);

	//60
	vbuf[ 6] = vc[0] + v[0];
	vbuf[ 7] = vc[1] + v[1];
	vbuf[ 8] = 0.0;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	//240
	vbuf[24] = vc[0] - v[0];
	vbuf[25] = vc[1] - v[1];
	vbuf[26] = 0.0;
	vbuf[27] = rr;
	vbuf[28] = gg;
	vbuf[29] = bb;

	v[0] -= vr[0];
	v[1] -= vr[1];
	v[2] -= 0.0;

	//120
	vbuf[12] = vc[0] + v[0];
	vbuf[13] = vc[1] + v[1];
	vbuf[14] = 0.0;
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	//300
	vbuf[30] = vc[0] - v[0];
	vbuf[31] = vc[1] - v[1];
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
void carveline2d_circle(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
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
		vbuf[6*j+0] = vr[0];
		vbuf[6*j+1] = vr[1];
		vbuf[6*j+2] = 0.0;
		quaternion_operation(&vbuf[6*j], q, j*tau/lineacc);

		vbuf[6*j+0] += vc[0];
		vbuf[6*j+1] += vc[1];
		vbuf[6*j+2] += vc[2];

		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;

		ibuf[2*j+0] = vlen+j;
		ibuf[2*j+1] = vlen+(j+1)%lineacc;
	}
}