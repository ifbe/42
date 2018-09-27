#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 18
int point2d_vars(struct arena* win, int id, float** vbuf, int vcnt)
{
	struct datapair* mod = win->mod;
	struct glsrc* src = &mod[id].src;
	int vlen = src->vbuf_h;

	*vbuf = (src->vbuf) + (24*vlen);
	src->vbuf_h += vcnt;

	return vlen;
}




void carvepoint2d(struct arena* win, u32 rgb, vec3 vc)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	point2d_vars(win, point2d, &vbuf, 1);

	vbuf[0] = vc[0];
	vbuf[1] = vc[1];
	vbuf[2] = vc[2];
	vbuf[3] = rr;
	vbuf[4] = gg;
	vbuf[5] = bb;
}
void carvepoint2d_bezier(struct arena* win, u32 rgb,
	vec3 va, vec3 vb, vec3 vt)
{
	int j;
	float t;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	point2d_vars(win, point2d, &vbuf, acc);

	for(j=0;j<=acc;j++)
	{
		t = (float)j / acc;

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*va[0] + 2*t*(1.0-t)*vt[0] + t*t*vb[0];
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*va[1] + 2*t*(1.0-t)*vt[1] + t*t*vb[1];
		vbuf[6*j+2] = vt[2];
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
	}
}




void carvepoint2d_triangle(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
}
void carvepoint2d_yshape(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
}
void carvepoint2d_rect(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
}
void carvepoint2d_circle(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	int j,k;
	float s,t;
	float q[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	point2d_vars(win, point2d, &vbuf, acc);

	q[0] = 0.0;
	q[1] = 0.0;
	q[2] = 1.0;
	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = 0.0;
		quaternion_operation(r, q, j*tau/acc);

		vbuf[6*j+0] = vc[0] + r[0];
		vbuf[6*j+1] = vc[1] + r[1];
		vbuf[6*j+2] = vc[2] + r[2];
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
	}
}