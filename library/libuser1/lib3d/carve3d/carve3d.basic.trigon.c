#include "libuser.h"
#define acc 32
int trigon3d_vars(struct arena* win, int id, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	struct datapair* mod = win->mod;
	struct glsrc* src = &mod[id].src;
	int vlen = src->vbuf_h;
	int ilen = src->ibuf_h;

	*vbuf = (src->vbuf) + (36*vlen);
	*ibuf = (src->ibuf) + (6*ilen);
	src->vbuf_h += vcnt;
	src->ibuf_h += icnt;

	return vlen;
}




void carvesolid_triangle(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	vec3 n;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, 3, 1);

	n[0] = (v1[1]-v0[1])*(v2[2]-v0[2]) - (v1[2]-v0[2])*(v2[1]-v0[1]);
	n[1] = (v1[2]-v0[2])*(v2[0]-v0[0]) - (v1[0]-v0[0])*(v2[2]-v0[2]);
	n[2] = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0]);

	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = n[0];
	vbuf[ 7] = n[1];
	vbuf[ 8] = n[2];

	vbuf[ 9] = v1[0];
	vbuf[10] = v1[1];
	vbuf[11] = v1[2];
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = n[0];
	vbuf[16] = n[1];
	vbuf[17] = n[2];

	vbuf[18] = v2[0];
	vbuf[19] = v2[1];
	vbuf[20] = v2[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = n[0];
	vbuf[25] = n[1];
	vbuf[26] = n[2];

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}
void carvesolid_rect(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	vec3 n;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, 4, 2);

	n[0] = vr[1]*vf[2] - vr[2]*vf[1];
	n[1] = vr[2]*vf[0] - vr[0]*vf[2];
	n[2] = vr[0]*vf[1] - vr[1]*vf[0];

	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = n[0];
	vbuf[ 7] = n[1];
	vbuf[ 8] = n[2];

	vbuf[ 9] = vc[0] + vr[0] - vf[0];
	vbuf[10] = vc[1] + vr[1] - vf[1];
	vbuf[11] = vc[2] + vr[2] - vf[2];
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = n[0];
	vbuf[16] = n[1];
	vbuf[17] = n[2];

	vbuf[18] = vc[0] - vr[0] + vf[0];
	vbuf[19] = vc[1] - vr[1] + vf[1];
	vbuf[20] = vc[2] - vr[2] + vf[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = n[0];
	vbuf[25] = n[1];
	vbuf[26] = n[2];

	vbuf[27] = vc[0] + vr[0] + vf[0];
	vbuf[28] = vc[1] + vr[1] + vf[1];
	vbuf[29] = vc[2] + vr[2] + vf[2];
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = n[0];
	vbuf[34] = n[1];
	vbuf[35] = n[2];

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}
void carvesolid_circle(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
#define circieacc (acc*2)
	int a,b,j;
	float c,s;
	vec3 vu;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, circieacc+1, circieacc);

	vu[0] = vr[1]*vf[2] - vr[2]*vf[1];
	vu[1] = vr[2]*vf[0] - vr[0]*vf[2];
	vu[2] = vr[0]*vf[1] - vr[1]*vf[0];
	for(j=0;j<circieacc;j++)
	{
		a = j*9;
		b = j*3;

		c = cosine(j*tau/circieacc);
		s = sine(j*tau/circieacc);
		vbuf[a+0] = vc[0] + vr[0]*c + vf[0]*s;
		vbuf[a+1] = vc[1] + vr[1]*c + vf[1]*s;
		vbuf[a+2] = vc[2] + vr[2]*c + vf[2]*s;

		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;

		vbuf[a+6] = vu[0];
		vbuf[a+7] = vu[1];
		vbuf[a+8] = vu[2];

		ibuf[b+0] = vlen + circieacc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%circieacc;
	}

	a = circieacc*9;
	vbuf[a+0] = vc[0];
	vbuf[a+1] = vc[1];
	vbuf[a+2] = vc[2];
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
	vbuf[a+6] = vu[0];
	vbuf[a+7] = vu[1];
	vbuf[a+8] = vu[2];
}




void carvesolid_pyramid3()
{
}
void carvesolid_pyramid4()
{
}
void carvesolid_pyramid5()
{
}
void carvesolid_pyramid6()
{
}
void carvesolid_cone(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	int a,b,j;
	float s,t;
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, acc + 2, acc * 2);

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);

		a = j*9;
		b = j*6;

		vbuf[a+0] = vc[0] + r[0];
		vbuf[a+1] = vc[1] + r[1];
		vbuf[a+2] = vc[2] + r[2];
		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;
		vbuf[a+6] = vbuf[a+0] - vc[0];
		vbuf[a+7] = vbuf[a+1] - vc[1];
		vbuf[a+8] = vbuf[a+2] - vc[2];

		//bottom
		ibuf[b+0] = vlen+acc;
		ibuf[b+1] = vlen+j;
		ibuf[b+2] = vlen+(j+1)%acc;

		//upper
		ibuf[b+3] = vlen+acc+1;
		ibuf[b+4] = vlen+j;
		ibuf[b+5] = vlen+(j+1)%acc;
	}

	a = acc*9;

	vbuf[a+ 0] = vc[0];
	vbuf[a+ 1] = vc[1];
	vbuf[a+ 2] = vc[2];
	vbuf[a+ 3] = rr;
	vbuf[a+ 4] = gg;
	vbuf[a+ 5] = bb;
	vbuf[a+ 6] = -vu[0];
	vbuf[a+ 7] = -vu[1];
	vbuf[a+ 8] = -vu[2];

	vbuf[a+ 9] = vc[0]+vu[0];
	vbuf[a+10] = vc[1]+vu[1];
	vbuf[a+11] = vc[2]+vu[2];
	vbuf[a+12] = rr;
	vbuf[a+13] = gg;
	vbuf[a+14] = bb;
	vbuf[a+15] = vu[0];
	vbuf[a+16] = vu[1];
	vbuf[a+18] = vu[2];
}




void carvesolid_prism3()
{
}
void carvesolid_prism4(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, 24, 12);

	for(j=0;j<24*9;j+=9)
	{
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
	}

	vbuf[2*9+0] = vbuf[1*9+0] = vbuf[0*9+0] = vc[0] - vr[0] - vf[0] - vu[0];
	vbuf[2*9+1] = vbuf[1*9+1] = vbuf[0*9+1] = vc[1] - vr[1] - vf[1] - vu[1];
	vbuf[2*9+2] = vbuf[1*9+2] = vbuf[0*9+2] = vc[2] - vr[2] - vf[2] - vu[2];
	vbuf[0*9+6] = -vu[0];
	vbuf[0*9+7] = -vu[1];
	vbuf[0*9+8] = -vu[2];
	vbuf[1*9+6] = -vf[0];
	vbuf[1*9+7] = -vf[1];
	vbuf[1*9+8] = -vf[2];
	vbuf[2*9+6] = -vr[0];
	vbuf[2*9+7] = -vr[1];
	vbuf[2*9+8] = -vr[2];

	vbuf[5*9+0] = vbuf[4*9+0] = vbuf[3*9+0] = vc[0] + vr[0] - vf[0] - vu[0];
	vbuf[5*9+1] = vbuf[4*9+1] = vbuf[3*9+1] = vc[1] + vr[1] - vf[1] - vu[1];
	vbuf[5*9+2] = vbuf[4*9+2] = vbuf[3*9+2] = vc[2] + vr[2] - vf[2] - vu[2];
	vbuf[3*9+6] = -vu[0];
	vbuf[3*9+7] = -vu[1];
	vbuf[3*9+8] = -vu[2];
	vbuf[4*9+6] = -vf[0];
	vbuf[4*9+7] = -vf[1];
	vbuf[4*9+8] = -vf[2];
	vbuf[5*9+6] = vr[0];
	vbuf[5*9+7] = vr[1];
	vbuf[5*9+8] = vr[2];

	vbuf[8*9+0] = vbuf[7*9+0] = vbuf[6*9+0] = vc[0] - vr[0] + vf[0] - vu[0];
	vbuf[8*9+1] = vbuf[7*9+1] = vbuf[6*9+1] = vc[1] - vr[1] + vf[1] - vu[1];
	vbuf[8*9+2] = vbuf[7*9+2] = vbuf[6*9+2] = vc[2] - vr[2] + vf[2] - vu[2];
	vbuf[6*9+6] = -vu[0];
	vbuf[6*9+7] = -vu[1];
	vbuf[6*9+8] = -vu[2];
	vbuf[7*9+6] = vf[0];
	vbuf[7*9+7] = vf[1];
	vbuf[7*9+8] = vf[2];
	vbuf[8*9+6] = -vr[0];
	vbuf[8*9+7] = -vr[1];
	vbuf[8*9+8] = -vr[2];

	vbuf[11*9+0] = vbuf[10*9+0] = vbuf[9*9+0] = vc[0] + vr[0] + vf[0] - vu[0];
	vbuf[11*9+1] = vbuf[10*9+1] = vbuf[9*9+1] = vc[1] + vr[1] + vf[1] - vu[1];
	vbuf[11*9+2] = vbuf[10*9+2] = vbuf[9*9+2] = vc[2] + vr[2] + vf[2] - vu[2];
	vbuf[ 9*9+6] = -vu[0];
	vbuf[ 9*9+7] = -vu[1];
	vbuf[ 9*9+8] = -vu[2];
	vbuf[10*9+6] = vf[0];
	vbuf[10*9+7] = vf[1];
	vbuf[10*9+8] = vf[2];
	vbuf[11*9+6] = vr[0];
	vbuf[11*9+7] = vr[1];
	vbuf[11*9+8] = vr[2];

	vbuf[14*9+0] = vbuf[13*9+0] = vbuf[12*9+0] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[14*9+1] = vbuf[13*9+1] = vbuf[12*9+1] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[14*9+2] = vbuf[13*9+2] = vbuf[12*9+2] = vc[2] - vr[2] - vf[2] + vu[2];
	vbuf[12*9+6] = vu[0];
	vbuf[12*9+7] = vu[1];
	vbuf[12*9+8] = vu[2];
	vbuf[13*9+6] = -vf[0];
	vbuf[13*9+7] = -vf[1];
	vbuf[13*9+8] = -vf[2];
	vbuf[14*9+6] = -vr[0];
	vbuf[14*9+7] = -vr[1];
	vbuf[14*9+8] = -vr[2];

	vbuf[17*9+0] = vbuf[16*9+0] = vbuf[15*9+0] = vc[0] + vr[0] - vf[0] + vu[0];
	vbuf[17*9+1] = vbuf[16*9+1] = vbuf[15*9+1] = vc[1] + vr[1] - vf[1] + vu[1];
	vbuf[17*9+2] = vbuf[16*9+2] = vbuf[15*9+2] = vc[2] + vr[2] - vf[2] + vu[2];
	vbuf[15*9+6] = vu[0];
	vbuf[15*9+7] = vu[1];
	vbuf[15*9+8] = vu[2];
	vbuf[16*9+6] = -vf[0];
	vbuf[16*9+7] = -vf[1];
	vbuf[16*9+8] = -vf[2];
	vbuf[17*9+6] = vr[0];
	vbuf[17*9+7] = vr[1];
	vbuf[17*9+8] = vr[2];

	vbuf[20*9+0] = vbuf[19*9+0] = vbuf[18*9+0] = vc[0] - vr[0] + vf[0] + vu[0];
	vbuf[20*9+1] = vbuf[19*9+1] = vbuf[18*9+1] = vc[1] - vr[1] + vf[1] + vu[1];
	vbuf[20*9+2] = vbuf[19*9+2] = vbuf[18*9+2] = vc[2] - vr[2] + vf[2] + vu[2];
	vbuf[18*9+6] = vu[0];
	vbuf[18*9+7] = vu[1];
	vbuf[18*9+8] = vu[2];
	vbuf[19*9+6] = vf[0];
	vbuf[19*9+7] = vf[1];
	vbuf[19*9+8] = vf[2];
	vbuf[20*9+6] = -vr[0];
	vbuf[20*9+7] = -vr[1];
	vbuf[20*9+8] = -vr[2];

	vbuf[23*9+0] = vbuf[22*9+0] = vbuf[21*9+0] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[23*9+1] = vbuf[22*9+1] = vbuf[21*9+1] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[23*9+2] = vbuf[22*9+2] = vbuf[21*9+2] = vc[2] + vr[2] + vf[2] + vu[2];
	vbuf[21*9+6] = vu[0];
	vbuf[21*9+7] = vu[1];
	vbuf[21*9+8] = vu[2];
	vbuf[22*9+6] = vf[0];
	vbuf[22*9+7] = vf[1];
	vbuf[22*9+8] = vf[2];
	vbuf[23*9+6] = vr[0];
	vbuf[23*9+7] = vr[1];
	vbuf[23*9+8] = vr[2];

	//bottom
	ibuf[ 0] = vlen + 0 + 0;
	ibuf[ 1] = vlen + 0 + 3;
	ibuf[ 2] = vlen + 0 + 6;
	ibuf[ 3] = vlen + 0 + 3;
	ibuf[ 4] = vlen + 0 + 6;
	ibuf[ 5] = vlen + 0 + 9;

	//near
	ibuf[ 6] = vlen + 1 + 0;
	ibuf[ 7] = vlen + 1 + 3;
	ibuf[ 8] = vlen + 1 + 12;
	ibuf[ 9] = vlen + 1 + 3;
	ibuf[10] = vlen + 1 + 12;
	ibuf[11] = vlen + 1 + 15;

	//right
	ibuf[12] = vlen + 2 + 3;
	ibuf[13] = vlen + 2 + 9;
	ibuf[14] = vlen + 2 + 15;
	ibuf[15] = vlen + 2 + 9;
	ibuf[16] = vlen + 2 + 15;
	ibuf[17] = vlen + 2 + 21;

	//far
	ibuf[18] = vlen + 1 + 6;
	ibuf[19] = vlen + 1 + 9;
	ibuf[20] = vlen + 1 + 18;
	ibuf[21] = vlen + 1 + 9;
	ibuf[22] = vlen + 1 + 18;
	ibuf[23] = vlen + 1 + 21;

	//left
	ibuf[24] = vlen + 2 + 0;
	ibuf[25] = vlen + 2 + 6;
	ibuf[26] = vlen + 2 + 12;
	ibuf[27] = vlen + 2 + 6;
	ibuf[28] = vlen + 2 + 12;
	ibuf[29] = vlen + 2 + 18;

	//top
	ibuf[30] = vlen + 0 + 12;
	ibuf[31] = vlen + 0 + 15;
	ibuf[32] = vlen + 0 + 18;
	ibuf[33] = vlen + 0 + 15;
	ibuf[34] = vlen + 0 + 18;
	ibuf[35] = vlen + 0 + 21;
}
void carvesolid_prism5()
{
}
void carvesolid_prism6()
{
}
void carvesolid_cask(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int a,b,j;
	float c,s;
	vec3 vv;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, acc * 2, acc * 2);

	for(j=0;j<acc;j++)
	{
		a = j*18;
		b = j*6;
		c = cosine(j*tau/acc);
		s = sine(j*tau/acc);

		vv[0] = c*vr[0] - s*vf[0];
		vv[1] = c*vr[1] - s*vf[1];
		vv[2] = c*vr[2] - s*vf[2];

		vbuf[a+0] = vc[0] - vu[0] + vv[0];
		vbuf[a+1] = vc[1] - vu[1] + vv[1];
		vbuf[a+2] = vc[2] - vu[2] + vv[2];
		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;
		vbuf[a+6] = vbuf[a+0] - vc[0];
		vbuf[a+7] = vbuf[a+1] - vc[1];
		vbuf[a+8] = vbuf[a+2] - vc[2];

		vbuf[a+ 9] = vc[0] + vu[0] + vv[0];
		vbuf[a+10] = vc[1] + vu[1] + vv[1];
		vbuf[a+11] = vc[2] + vu[2] + vv[2];
		vbuf[a+12] = rr;
		vbuf[a+13] = gg;
		vbuf[a+14] = bb;
		vbuf[a+15] = vbuf[a+ 9] - vc[0];
		vbuf[a+16] = vbuf[a+10] - vc[1];
		vbuf[a+17] = vbuf[a+11] - vc[2];

		ibuf[b+0] = vlen + j*2;
		ibuf[b+1] = vlen + ((j+1)%acc)*2;
		ibuf[b+2] = vlen + 1 + j*2;

		ibuf[b+3] = vlen + 1 + ((j+1)%acc)*2;
		ibuf[b+4] = vlen + ((j+1)%acc)*2;
		ibuf[b+5] = vlen + 1 + j*2;
	}
}
void carvesolid_cylinder(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vec3 tc;
	vec3 tf;
	carvesolid_cask(win, rgb, vc, vr, vf, vu);

	tc[0] = vc[0]+vu[0];
	tc[1] = vc[1]+vu[1];
	tc[2] = vc[2]+vu[2];
	carvesolid_circle(win, rgb, tc, vr, vf);

	tc[0] = vc[0]-vu[0];
	tc[1] = vc[1]-vu[1];
	tc[2] = vc[2]-vu[2];
	tf[0] = -vf[0];
	tf[1] = -vf[1];
	tf[2] = -vf[2];
	carvesolid_circle(win, rgb, tc, vr, tf);
}




void carvesolid_tetrahedron()
{
}
void carvesolid_octahedron()
{
}
void carvesolid_dodecahedron(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float a = 1.618;
	float b = 1.0/1.618;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, 20, 36);

	//(+-1, +-1, +-1)
	vbuf[ 0] = vc[0]-vr[0]-vf[0]-vu[0];
	vbuf[ 1] = vc[1]-vr[1]-vf[1]-vu[1];
	vbuf[ 2] = vc[2]-vr[2]-vf[2]-vu[2];

	vbuf[ 9] = vc[0]+vr[0]-vf[0]-vu[0];
	vbuf[10] = vc[1]+vr[1]-vf[1]-vu[1];
	vbuf[11] = vc[2]+vr[2]-vf[2]-vu[2];

	vbuf[18] = vc[0]-vr[0]+vf[0]-vu[0];
	vbuf[19] = vc[1]-vr[1]+vf[1]-vu[1];
	vbuf[20] = vc[2]-vr[2]+vf[2]-vu[2];

	vbuf[27] = vc[0]+vr[0]+vf[0]-vu[0];
	vbuf[28] = vc[1]+vr[1]+vf[1]-vu[1];
	vbuf[29] = vc[2]+vr[2]+vf[2]-vu[2];

	vbuf[36] = vc[0]-vr[0]-vf[0]+vu[0];
	vbuf[37] = vc[1]-vr[1]-vf[1]+vu[1];
	vbuf[38] = vc[2]-vr[2]-vf[2]+vu[2];

	vbuf[45] = vc[0]+vr[0]-vf[0]+vu[0];
	vbuf[46] = vc[1]+vr[1]-vf[1]+vu[1];
	vbuf[47] = vc[2]+vr[2]-vf[2]+vu[2];

	vbuf[54] = vc[0]-vr[0]+vf[0]+vu[0];
	vbuf[55] = vc[1]-vr[1]+vf[1]+vu[1];
	vbuf[56] = vc[2]-vr[2]+vf[2]+vu[2];

	vbuf[63] = vc[0]+vr[0]+vf[0]+vu[0];
	vbuf[64] = vc[1]+vr[1]+vf[1]+vu[1];
	vbuf[65] = vc[2]+vr[2]+vf[2]+vu[2];

	//(0, +-a, +-b)
	vbuf[72] = vc[0] - a*vf[0] - b*vu[0];
	vbuf[73] = vc[1] - a*vf[1] - b*vu[1];
	vbuf[74] = vc[2] - a*vf[2] - b*vu[2];

	vbuf[81] = vc[0] + a*vf[0] - b*vu[0];
	vbuf[82] = vc[1] + a*vf[1] - b*vu[1];
	vbuf[83] = vc[2] + a*vf[2] - b*vu[2];

	vbuf[90] = vc[0] - a*vf[0] + b*vu[0];
	vbuf[91] = vc[1] - a*vf[1] + b*vu[1];
	vbuf[92] = vc[2] - a*vf[2] + b*vu[2];

	vbuf[ 99] = vc[0] + a*vf[0] + b*vu[0];
	vbuf[100] = vc[1] + a*vf[1] + b*vu[1];
	vbuf[101] = vc[2] + a*vf[2] + b*vu[2];

	//(+-b, 0, +-a)
	vbuf[108] = vc[0] - b*vr[0] - a*vu[0];
	vbuf[109] = vc[1] - b*vr[1] - a*vu[1];
	vbuf[110] = vc[2] - b*vr[2] - a*vu[2];

	vbuf[117] = vc[0] + b*vr[0] - a*vu[0];
	vbuf[118] = vc[1] + b*vr[1] - a*vu[1];
	vbuf[119] = vc[2] + b*vr[2] - a*vu[2];

	vbuf[126] = vc[0] - b*vr[0] + a*vu[0];
	vbuf[127] = vc[1] - b*vr[1] + a*vu[1];
	vbuf[128] = vc[2] - b*vr[2] + a*vu[2];

	vbuf[135] = vc[0] + b*vr[0] + a*vu[0];
	vbuf[136] = vc[1] + b*vr[1] + a*vu[1];
	vbuf[137] = vc[2] + b*vr[2] + a*vu[2];

	//(+-a, +-b, 0)
	vbuf[144] = vc[0] - a*vr[0] - b*vf[0];
	vbuf[145] = vc[1] - a*vr[1] - b*vf[1];
	vbuf[146] = vc[2] - a*vr[2] - b*vf[2];

	vbuf[153] = vc[0] + a*vr[0] - b*vf[0];
	vbuf[154] = vc[1] + a*vr[1] - b*vf[1];
	vbuf[155] = vc[2] + a*vr[2] - b*vf[2];

	vbuf[162] = vc[0] - a*vr[0] + b*vf[0];
	vbuf[163] = vc[1] - a*vr[1] + b*vf[1];
	vbuf[164] = vc[2] - a*vr[2] + b*vf[2];

	vbuf[171] = vc[0] + a*vr[0] + b*vf[0];
	vbuf[172] = vc[1] + a*vr[1] + b*vf[1];
	vbuf[173] = vc[2] + a*vr[2] + b*vf[2];

	for(j=0;j<20*9;j+=9)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
		vbuf[j + 6] = vbuf[j + 0] - vc[0];
		vbuf[j + 7] = vbuf[j + 1] - vc[1];
		vbuf[j + 8] = vbuf[j + 2] - vc[2];
	}

	//front
	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+8;
	ibuf[ 3] = vlen+0;
	ibuf[ 4] = vlen+1;
	ibuf[ 5] = vlen+12;
	ibuf[ 6] = vlen+12;
	ibuf[ 7] = vlen+13;
	ibuf[ 8] = vlen+1;

	ibuf[ 9] = vlen+4;
	ibuf[10] = vlen+5;
	ibuf[11] = vlen+10;
	ibuf[12] = vlen+4;
	ibuf[13] = vlen+5;
	ibuf[14] = vlen+14;
	ibuf[15] = vlen+14;
	ibuf[16] = vlen+15;
	ibuf[17] = vlen+5;

	//back
	ibuf[18] = vlen+2;
	ibuf[19] = vlen+3;
	ibuf[20] = vlen+9;
	ibuf[21] = vlen+2;
	ibuf[22] = vlen+3;
	ibuf[23] = vlen+12;
	ibuf[24] = vlen+12;
	ibuf[25] = vlen+13;
	ibuf[26] = vlen+3;

	ibuf[27] = vlen+6;
	ibuf[28] = vlen+7;
	ibuf[29] = vlen+11;
	ibuf[30] = vlen+6;
	ibuf[31] = vlen+7;
	ibuf[32] = vlen+14;
	ibuf[33] = vlen+14;
	ibuf[34] = vlen+15;
	ibuf[35] = vlen+7;

	//bottom
	ibuf[36] = vlen+0;
	ibuf[37] = vlen+2;
	ibuf[38] = vlen+12;
	ibuf[39] = vlen+0;
	ibuf[40] = vlen+2;
	ibuf[41] = vlen+16;
	ibuf[42] = vlen+16;
	ibuf[43] = vlen+18;
	ibuf[44] = vlen+2;

	ibuf[45] = vlen+1;
	ibuf[46] = vlen+3;
	ibuf[47] = vlen+13;
	ibuf[48] = vlen+1;
	ibuf[49] = vlen+3;
	ibuf[50] = vlen+17;
	ibuf[51] = vlen+17;
	ibuf[52] = vlen+19;
	ibuf[53] = vlen+3;

	//upper
	ibuf[54] = vlen+4;
	ibuf[55] = vlen+6;
	ibuf[56] = vlen+14;
	ibuf[57] = vlen+4;
	ibuf[58] = vlen+6;
	ibuf[59] = vlen+16;
	ibuf[60] = vlen+16;
	ibuf[61] = vlen+18;
	ibuf[62] = vlen+6;

	ibuf[63] = vlen+5;
	ibuf[64] = vlen+7;
	ibuf[65] = vlen+15;
	ibuf[66] = vlen+5;
	ibuf[67] = vlen+7;
	ibuf[68] = vlen+17;
	ibuf[69] = vlen+17;
	ibuf[70] = vlen+19;
	ibuf[71] = vlen+7;

	//left
	ibuf[72] = vlen+0;
	ibuf[73] = vlen+4;
	ibuf[74] = vlen+16;
	ibuf[75] = vlen+0;
	ibuf[76] = vlen+4;
	ibuf[77] = vlen+8;
	ibuf[78] = vlen+8;
	ibuf[79] = vlen+10;
	ibuf[80] = vlen+4;

	ibuf[81] = vlen+2;
	ibuf[82] = vlen+6;
	ibuf[83] = vlen+18;
	ibuf[84] = vlen+2;
	ibuf[85] = vlen+6;
	ibuf[86] = vlen+9;
	ibuf[87] = vlen+9;
	ibuf[88] = vlen+11;
	ibuf[89] = vlen+6;

	//right
	ibuf[90] = vlen+1;
	ibuf[91] = vlen+5;
	ibuf[92] = vlen+17;
	ibuf[93] = vlen+1;
	ibuf[94] = vlen+5;
	ibuf[95] = vlen+8;
	ibuf[96] = vlen+8;
	ibuf[97] = vlen+10;
	ibuf[98] = vlen+5;

	ibuf[99] = vlen+3;
	ibuf[100] = vlen+7;
	ibuf[101] = vlen+19;
	ibuf[102] = vlen+3;
	ibuf[103] = vlen+7;
	ibuf[104] = vlen+9;
	ibuf[105] = vlen+9;
	ibuf[106] = vlen+11;
	ibuf[107] = vlen+7;
}
void carvesolid_icosahedron(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float m = 0.52573111211913360602566908484788;
	float n = 0.85065080835203993218154049706301;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, 12, 20);

	//(+-m, 0, +-n)
	vbuf[ 0] = vc[0] - m;
	vbuf[ 1] = vc[1];
	vbuf[ 2] = vc[2] - n;

	vbuf[ 9] = vc[0] + m;
	vbuf[10] = vc[1];
	vbuf[11] = vc[2] - n;

	vbuf[18] = vc[0] - m;
	vbuf[19] = vc[1];
	vbuf[20] = vc[2] + n;

	vbuf[27] = vc[0] + m;
	vbuf[28] = vc[1];
	vbuf[29] = vc[2] + n;

	//(0, +-n, +-m)
	vbuf[36] = vc[0];
	vbuf[37] = vc[1] - n;
	vbuf[38] = vc[2] - m;

	vbuf[45] = vc[0];
	vbuf[46] = vc[1] + n;
	vbuf[47] = vc[2] - m;

	vbuf[54] = vc[0];
	vbuf[55] = vc[1] - n;
	vbuf[56] = vc[2] + m;

	vbuf[63] = vc[0];
	vbuf[64] = vc[1] + n;
	vbuf[65] = vc[2] + m;

	//(+-n, +-m, 0)
	vbuf[72] = vc[0] - n;
	vbuf[73] = vc[1] - m;
	vbuf[74] = vc[2];

	vbuf[81] = vc[0] + n;
	vbuf[82] = vc[1] - m;
	vbuf[83] = vc[2];

	vbuf[90] = vc[0] - n;
	vbuf[91] = vc[1] + m;
	vbuf[92] = vc[2];

	vbuf[99] = vc[0] + n;
	vbuf[100] = vc[1] + m;
	vbuf[101] = vc[2];

	for(j=0;j<12*9;j+=9)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
		vbuf[j + 6] = vbuf[j + 0] - vc[0];
		vbuf[j + 7] = vbuf[j + 1] - vc[1];
		vbuf[j + 8] = vbuf[j + 2] - vc[2];
	}

	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+5;

	ibuf[ 3] = vlen+0;
	ibuf[ 4] = vlen+5;
	ibuf[ 5] = vlen+10;

	ibuf[ 6] = vlen+0;
	ibuf[ 7] = vlen+10;
	ibuf[ 8] = vlen+8;

	ibuf[ 9] = vlen+0;
	ibuf[10] = vlen+8;
	ibuf[11] = vlen+4;

	ibuf[12] = vlen+0;
	ibuf[13] = vlen+1;
	ibuf[14] = vlen+4;

	ibuf[15] = vlen+3;
	ibuf[16] = vlen+2;
	ibuf[17] = vlen+6;

	ibuf[18] = vlen+3;
	ibuf[19] = vlen+6;
	ibuf[20] = vlen+9;

	ibuf[21] = vlen+3;
	ibuf[22] = vlen+9;
	ibuf[23] = vlen+11;

	ibuf[24] = vlen+3;
	ibuf[25] = vlen+11;
	ibuf[26] = vlen+7;

	ibuf[27] = vlen+3;
	ibuf[28] = vlen+2;
	ibuf[29] = vlen+7;

	ibuf[30] = vlen+1;
	ibuf[31] = vlen+11;
	ibuf[32] = vlen+9;

	ibuf[33] = vlen+5;
	ibuf[34] = vlen+7;
	ibuf[35] = vlen+11;

	ibuf[36] = vlen+10;
	ibuf[37] = vlen+2;
	ibuf[38] = vlen+7;

	ibuf[39] = vlen+8;
	ibuf[40] = vlen+6;
	ibuf[41] = vlen+2;

	ibuf[42] = vlen+4;
	ibuf[43] = vlen+9;
	ibuf[44] = vlen+6;

	ibuf[45] = vlen+1;
	ibuf[46] = vlen+11;
	ibuf[47] = vlen+5;

	ibuf[48] = vlen+5;
	ibuf[49] = vlen+7;
	ibuf[50] = vlen+10;

	ibuf[51] = vlen+10;
	ibuf[52] = vlen+2;
	ibuf[53] = vlen+8;

	ibuf[54] = vlen+8;
	ibuf[55] = vlen+6;
	ibuf[56] = vlen+4;

	ibuf[57] = vlen+4;
	ibuf[58] = vlen+9;
	ibuf[59] = vlen+1;
}
void carvesolid_sphere(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
#define accx (acc)
#define accy (acc|0x1)
	int a,b,j,k;
	float c,s;
	vec3 tc, tr, tf;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, accx*accy+2, accx*accy*2);

	for(k=0;k<accy;k++)
	{
		s = (2*k-accy+1)*PI/(2*accy+2);
		c = cosine(s);
		s = sine(s);

		tc[0] = vc[0] + vu[0]*s;
		tc[1] = vc[1] + vu[1]*s;
		tc[2] = vc[2] + vu[2]*s;
		tr[0] = vr[0]*c;
		tr[1] = vr[1]*c;
		tr[2] = vr[2]*c;
		tf[0] = vf[0]*c;
		tf[1] = vf[1]*c;
		tf[2] = vf[2]*c;

		for(j=0;j<accx;j++)
		{
			s = j*tau/accx;
			c = cosine(s);
			s = sine(s);

			a = (k*accx + j)*9;
			vbuf[a+0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+2] = tc[2] + tr[2]*c + tf[2]*s;
			vbuf[a+3] = rr;
			vbuf[a+4] = gg;
			vbuf[a+5] = bb;
			vbuf[a+6] = vbuf[a+0] - vc[0];
			vbuf[a+7] = vbuf[a+1] - vc[1];
			vbuf[a+8] = vbuf[a+2] - vc[2];

			if(k >= accy-1)continue;
			b = k*accx*6;
			ibuf[b + 6*j + 0] = vlen+(k*accx)+j;
			ibuf[b + 6*j + 1] = vlen+(k*accx)+(j+1)%accx;
			ibuf[b + 6*j + 2] = vlen+(k*accx)+accx+j;
			ibuf[b + 6*j + 3] = vlen+(k*accx)+(j+1)%accx;
			ibuf[b + 6*j + 4] = vlen+(k*accx)+accx+j;
			ibuf[b + 6*j + 5] = vlen+(k*accx)+accx+(j+1)%accx;
		}
	}

	a = accx*accy*9;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];
	vbuf[a+ 3] = rr;
	vbuf[a+ 4] = gg;
	vbuf[a+ 5] = bb;
	vbuf[a+ 6] = -vu[0];
	vbuf[a+ 7] = -vu[1];
	vbuf[a+ 8] = -vu[2];

	vbuf[a+ 9] = vc[0]+vu[0];
	vbuf[a+10] = vc[1]+vu[1];
	vbuf[a+11] = vc[2]+vu[2];
	vbuf[a+12] = rr;
	vbuf[a+13] = gg;
	vbuf[a+14] = bb;
	vbuf[a+15] = vu[0];
	vbuf[a+16] = vu[1];
	vbuf[a+17] = vu[2];

	b = (accy-1)*accx*6;
	for(j=0;j<accx;j++)
	{
		ibuf[b + (6*j) + 0] = vlen+accx*accy;
		ibuf[b + (6*j) + 1] = vlen+j;
		ibuf[b + (6*j) + 2] = vlen+(j+1)%accx;
		ibuf[b + (6*j) + 3] = vlen+accx*accy+1;
		ibuf[b + (6*j) + 4] = vlen+accx*(accy-1)+j;
		ibuf[b + (6*j) + 5] = vlen+accx*(accy-1)+(j+1)%accx;
	}
}
void carvesolid_tokamak(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, trigon3d, &vbuf, &ibuf, acc*acc*2, acc*acc);

}
