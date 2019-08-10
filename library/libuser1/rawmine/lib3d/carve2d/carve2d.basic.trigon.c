#include "libuser.h"
#define acc 24
int trigon2d_vars(struct actor* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	struct datapair* mod = win->gl_solid;
	struct glsrc* src = &mod[trigon2d].src;
	int vlen = src->vbuf_h;
	int ilen = src->ibuf_h;

	*vbuf = (src->vbuf) + (24*vlen);
	*ibuf = (src->ibuf) + (6*ilen);
	src->vbuf_h += vcnt;
	src->ibuf_h += icnt;

	return vlen;
}




void carvesolid2d_triangle(struct actor* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon2d_vars(win, 0, &vbuf, &ibuf, 3, 1);

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
void carvesolid2d_rect(struct actor* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon2d_vars(win, 0, &vbuf, &ibuf, 4, 2);

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
void carvesolid2d_prism4(struct actor* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon2d_vars(win, 0, &vbuf, &ibuf, 24, 12);

	for(j=0;j<24*6;j+=6)
	{
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
	}

	vbuf[2*6+0] = vbuf[1*6+0] = vbuf[0*6+0] = vc[0] - vr[0] - vf[0] - vu[0];
	vbuf[2*6+1] = vbuf[1*6+1] = vbuf[0*6+1] = vc[1] - vr[1] - vf[1] - vu[1];
	vbuf[2*6+2] = vbuf[1*6+2] = vbuf[0*6+2] = vc[2] - vr[2] - vf[2] - vu[2];
	//vbuf[0*6+6] = -vu[0];
	//vbuf[0*6+7] = -vu[1];
	//vbuf[0*6+8] = -vu[2];
	//vbuf[1*6+6] = -vf[0];
	//vbuf[1*6+7] = -vf[1];
	//vbuf[1*6+8] = -vf[2];
	//vbuf[2*6+6] = -vr[0];
	//vbuf[2*6+7] = -vr[1];
	//vbuf[2*6+8] = -vr[2];

	vbuf[5*6+0] = vbuf[4*6+0] = vbuf[3*6+0] = vc[0] + vr[0] - vf[0] - vu[0];
	vbuf[5*6+1] = vbuf[4*6+1] = vbuf[3*6+1] = vc[1] + vr[1] - vf[1] - vu[1];
	vbuf[5*6+2] = vbuf[4*6+2] = vbuf[3*6+2] = vc[2] + vr[2] - vf[2] - vu[2];
	//vbuf[3*6+6] = -vu[0];
	//vbuf[3*6+7] = -vu[1];
	//vbuf[3*6+8] = -vu[2];
	//vbuf[4*6+6] = -vf[0];
	//vbuf[4*6+7] = -vf[1];
	//vbuf[4*6+8] = -vf[2];
	//vbuf[5*6+6] = vr[0];
	//vbuf[5*6+7] = vr[1];
	//vbuf[5*6+8] = vr[2];

	vbuf[8*6+0] = vbuf[7*6+0] = vbuf[6*6+0] = vc[0] - vr[0] + vf[0] - vu[0];
	vbuf[8*6+1] = vbuf[7*6+1] = vbuf[6*6+1] = vc[1] - vr[1] + vf[1] - vu[1];
	vbuf[8*6+2] = vbuf[7*6+2] = vbuf[6*6+2] = vc[2] - vr[2] + vf[2] - vu[2];
	//vbuf[6*6+6] = -vu[0];
	//vbuf[6*6+7] = -vu[1];
	//vbuf[6*6+8] = -vu[2];
	//vbuf[7*6+6] = vf[0];
	//vbuf[7*6+7] = vf[1];
	//vbuf[7*6+8] = vf[2];
	//vbuf[8*6+6] = -vr[0];
	//vbuf[8*6+7] = -vr[1];
	//vbuf[8*6+8] = -vr[2];

	vbuf[11*6+0] = vbuf[10*6+0] = vbuf[9*6+0] = vc[0] + vr[0] + vf[0] - vu[0];
	vbuf[11*6+1] = vbuf[10*6+1] = vbuf[9*6+1] = vc[1] + vr[1] + vf[1] - vu[1];
	vbuf[11*6+2] = vbuf[10*6+2] = vbuf[9*6+2] = vc[2] + vr[2] + vf[2] - vu[2];
	//vbuf[ 9*6+6] = -vu[0];
	//vbuf[ 9*6+7] = -vu[1];
	//vbuf[ 9*6+8] = -vu[2];
	//vbuf[10*6+6] = vf[0];
	//vbuf[10*6+7] = vf[1];
	//vbuf[10*6+8] = vf[2];
	//vbuf[11*6+6] = vr[0];
	//vbuf[11*6+7] = vr[1];
	//vbuf[11*6+8] = vr[2];

	vbuf[14*6+0] = vbuf[13*6+0] = vbuf[12*6+0] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[14*6+1] = vbuf[13*6+1] = vbuf[12*6+1] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[14*6+2] = vbuf[13*6+2] = vbuf[12*6+2] = vc[2] - vr[2] - vf[2] + vu[2];
	//vbuf[12*6+6] = vu[0];
	//vbuf[12*6+7] = vu[1];
	//vbuf[12*6+8] = vu[2];
	//vbuf[13*6+6] = -vf[0];
	//vbuf[13*6+7] = -vf[1];
	//vbuf[13*6+8] = -vf[2];
	//vbuf[14*6+6] = -vr[0];
	//vbuf[14*6+7] = -vr[1];
	//vbuf[14*6+8] = -vr[2];

	vbuf[17*6+0] = vbuf[16*6+0] = vbuf[15*6+0] = vc[0] + vr[0] - vf[0] + vu[0];
	vbuf[17*6+1] = vbuf[16*6+1] = vbuf[15*6+1] = vc[1] + vr[1] - vf[1] + vu[1];
	vbuf[17*6+2] = vbuf[16*6+2] = vbuf[15*6+2] = vc[2] + vr[2] - vf[2] + vu[2];
	//vbuf[15*6+6] = vu[0];
	//vbuf[15*6+7] = vu[1];
	//vbuf[15*6+8] = vu[2];
	//vbuf[16*6+6] = -vf[0];
	//vbuf[16*6+7] = -vf[1];
	//vbuf[16*6+8] = -vf[2];
	//vbuf[17*6+6] = vr[0];
	//vbuf[17*6+7] = vr[1];
	//vbuf[17*6+8] = vr[2];

	vbuf[20*6+0] = vbuf[19*6+0] = vbuf[18*6+0] = vc[0] - vr[0] + vf[0] + vu[0];
	vbuf[20*6+1] = vbuf[19*6+1] = vbuf[18*6+1] = vc[1] - vr[1] + vf[1] + vu[1];
	vbuf[20*6+2] = vbuf[19*6+2] = vbuf[18*6+2] = vc[2] - vr[2] + vf[2] + vu[2];
	//vbuf[18*6+6] = vu[0];
	//vbuf[18*6+7] = vu[1];
	//vbuf[18*6+8] = vu[2];
	//vbuf[19*6+6] = vf[0];
	//vbuf[19*6+7] = vf[1];
	//vbuf[19*6+8] = vf[2];
	//vbuf[20*6+6] = -vr[0];
	//vbuf[20*6+7] = -vr[1];
	//vbuf[20*6+8] = -vr[2];

	vbuf[23*6+0] = vbuf[22*6+0] = vbuf[21*6+0] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[23*6+1] = vbuf[22*6+1] = vbuf[21*6+1] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[23*6+2] = vbuf[22*6+2] = vbuf[21*6+2] = vc[2] + vr[2] + vf[2] + vu[2];
	//vbuf[21*6+6] = vu[0];
	//vbuf[21*6+7] = vu[1];
	//vbuf[21*6+8] = vu[2];
	//vbuf[22*6+6] = vf[0];
	//vbuf[22*6+7] = vf[1];
	//vbuf[22*6+8] = vf[2];
	//vbuf[23*6+6] = vr[0];
	//vbuf[23*6+7] = vr[1];
	//vbuf[23*6+8] = vr[2];

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




void carvesolid2d_circle(struct actor* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	int a,b,j;
	float c,s;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = trigon2d_vars(win, 0, &vbuf, &ibuf, acc+1, acc);

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
void carvesolid2d_sphere(struct actor* win, u32 rgb,
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
	int vlen = trigon2d_vars(win, 0, &vbuf, &ibuf, accx*accy+2, accx*accy*2);

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

			a = (k*accx + j)*6;
			vbuf[a+0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+2] = tc[2] + tr[2]*c + tf[2]*s;
			vbuf[a+3] = rr;
			vbuf[a+4] = gg;
			vbuf[a+5] = bb;
			//vbuf[a+6] = vbuf[a+0] - vc[0];
			//vbuf[a+7] = vbuf[a+1] - vc[1];
			//vbuf[a+8] = vbuf[a+2] - vc[2];

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

	a = accx*accy*6;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];
	vbuf[a+ 3] = rr;
	vbuf[a+ 4] = gg;
	vbuf[a+ 5] = bb;
	//vbuf[a+ 6] = -vu[0];
	//vbuf[a+ 7] = -vu[1];
	//vbuf[a+ 8] = -vu[2];

	vbuf[a+ 6] = vc[0]+vu[0];
	vbuf[a+ 7] = vc[1]+vu[1];
	vbuf[a+ 8] = vc[2]+vu[2];
	vbuf[a+ 9] = rr;
	vbuf[a+10] = gg;
	vbuf[a+11] = bb;
	//vbuf[a+15] = vu[0];
	//vbuf[a+16] = vu[1];
	//vbuf[a+17] = vu[2];

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
