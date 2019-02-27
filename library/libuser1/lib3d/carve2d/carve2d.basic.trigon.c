#include "libuser.h"
#define acc 24
int trigon2d_vars(struct arena* win, int id, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	struct datapair* mod = win->mod;
	struct glsrc* src = &mod[id].src;
	int vlen = src->vbuf_h;
	int ilen = src->ibuf_h;

	*vbuf = (src->vbuf) + (24*vlen);
	*ibuf = (src->ibuf) + (6*ilen);
	src->vbuf_h += vcnt;
	src->ibuf_h += icnt;

	return vlen;
}




void carvesolid2d_triangle(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon2d_vars(win, trigon2d, &vbuf, &ibuf, 3, 1);

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

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}
void carvesolid2d_rect(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon2d_vars(win, trigon2d, &vbuf, &ibuf, 4, 2);

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

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}
void carvesolid2d_circle(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	int a,b,j;
	float c,s;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon2d_vars(win, trigon2d, &vbuf, &ibuf, acc+1, acc);

	for(j=0;j<acc;j++)
	{
		a = j*6;
		b = j*3;

		c = cosine(j*tau/acc);
		s = sine(j*tau/acc);
		vbuf[a+0] = vc[0] + vr[0]*c + vf[0]*s;
		vbuf[a+1] = vc[1] + vr[1]*c + vf[1]*s;
		vbuf[a+2] = vc[2] + vr[2]*c + vf[2]*s;

		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;

		ibuf[b+0] = vlen + acc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%acc;
	}

	a = acc*6;
	vbuf[a+0] = vc[0];
	vbuf[a+1] = vc[1];
	vbuf[a+2] = vc[2];
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
}