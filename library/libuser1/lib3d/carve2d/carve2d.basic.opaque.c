#include "libuser.h"
#define acc 24
int opaque2d_vars(struct arena* win, int id, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	struct datapair* mod = win->mod;
	struct glsrc* src = &mod[id].src;
	int vlen = src->vbuf_h;
	int ilen = src->ibuf_h;

	*vbuf = (src->vbuf) + (32*vlen);
	*ibuf = (src->ibuf) + (6*ilen);
	src->vbuf_h += vcnt;
	src->ibuf_h += icnt;

	return vlen;
}




void carveopaque2d_triangle(struct arena* win, u32 rgba,
	vec3 v0, vec3 v1, vec3 v2)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	float* vbuf;
	u16* ibuf;
	int vlen = opaque2d_vars(win, opaque2d, &vbuf, &ibuf, 3, 1);

	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];
	vbuf[ 3] = 1.0;
	vbuf[ 4] = rr;
	vbuf[ 5] = gg;
	vbuf[ 6] = bb;
	vbuf[ 7] = aa;

	vbuf[ 8] = v1[0];
	vbuf[ 9] = v1[1];
	vbuf[10] = v1[2];
	vbuf[11] = 1.0;
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = aa;

	vbuf[16] = v2[0];
	vbuf[17] = v2[1];
	vbuf[18] = v2[2];
	vbuf[19] = 1.0;
	vbuf[20] = rr;
	vbuf[21] = gg;
	vbuf[22] = bb;
	vbuf[23] = aa;

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}
void carveopaque2d_rect(struct arena* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	float* vbuf;
	u16* ibuf;
	int vlen = opaque2d_vars(win, opaque2d, &vbuf, &ibuf, 4, 2);

	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2];
	vbuf[ 3] = 1.0;
	vbuf[ 4] = rr;
	vbuf[ 5] = gg;
	vbuf[ 6] = bb;
	vbuf[ 7] = aa;

	vbuf[ 8] = vc[0] + vr[0] - vf[0];
	vbuf[ 9] = vc[1] + vr[1] - vf[1];
	vbuf[10] = vc[2];
	vbuf[11] = 1.0;
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = aa;

	vbuf[16] = vc[0] - vr[0] + vf[0];
	vbuf[17] = vc[1] - vr[1] + vf[1];
	vbuf[18] = vc[2];
	vbuf[19] = 1.0;
	vbuf[20] = rr;
	vbuf[21] = gg;
	vbuf[22] = bb;
	vbuf[23] = aa;

	vbuf[24] = vc[0] + vr[0] + vf[0];
	vbuf[25] = vc[1] + vr[1] + vf[1];
	vbuf[26] = vc[2];
	vbuf[27] = 1.0;
	vbuf[28] = rr;
	vbuf[29] = gg;
	vbuf[30] = bb;
	vbuf[31] = aa;

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}
void carveopaque2d_prism4(struct arena* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	int j;
	float* vbuf;
	u16* ibuf;
	int vlen = opaque2d_vars(win, opaque2d, &vbuf, &ibuf, 24, 12);

	for(j=0;j<24*8;j+=8)
	{
		vbuf[j+4] = rr;
		vbuf[j+5] = gg;
		vbuf[j+6] = bb;
		vbuf[j+7] = aa;
	}

	vbuf[2*8+0] = vbuf[1*8+0] = vbuf[0*8+0] = vc[0] - vr[0] - vf[0] - vu[0];
	vbuf[2*8+1] = vbuf[1*8+1] = vbuf[0*8+1] = vc[1] - vr[1] - vf[1] - vu[1];
	vbuf[2*8+2] = vbuf[1*8+2] = vbuf[0*8+2] = vc[2] - vr[2] - vf[2] - vu[2];
	vbuf[2*8+3] = vbuf[1*8+3] = vbuf[0*8+3] = 1.0;

	vbuf[5*8+0] = vbuf[4*8+0] = vbuf[3*8+0] = vc[0] + vr[0] - vf[0] - vu[0];
	vbuf[5*8+1] = vbuf[4*8+1] = vbuf[3*8+1] = vc[1] + vr[1] - vf[1] - vu[1];
	vbuf[5*8+2] = vbuf[4*8+2] = vbuf[3*8+2] = vc[2] + vr[2] - vf[2] - vu[2];
	vbuf[5*8+3] = vbuf[4*8+3] = vbuf[3*8+3] = 1.0;

	vbuf[8*8+0] = vbuf[7*8+0] = vbuf[6*8+0] = vc[0] - vr[0] + vf[0] - vu[0];
	vbuf[8*8+1] = vbuf[7*8+1] = vbuf[6*8+1] = vc[1] - vr[1] + vf[1] - vu[1];
	vbuf[8*8+2] = vbuf[7*8+2] = vbuf[6*8+2] = vc[2] - vr[2] + vf[2] - vu[2];
	vbuf[8*8+3] = vbuf[7*8+3] = vbuf[6*8+3] = 1.0;

	vbuf[11*8+0] = vbuf[10*8+0] = vbuf[9*8+0] = vc[0] + vr[0] + vf[0] - vu[0];
	vbuf[11*8+1] = vbuf[10*8+1] = vbuf[9*8+1] = vc[1] + vr[1] + vf[1] - vu[1];
	vbuf[11*8+2] = vbuf[10*8+2] = vbuf[9*8+2] = vc[2] + vr[2] + vf[2] - vu[2];
	vbuf[11*8+3] = vbuf[10*8+3] = vbuf[9*8+3] = 1.0;

	vbuf[14*8+0] = vbuf[13*8+0] = vbuf[12*8+0] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[14*8+1] = vbuf[13*8+1] = vbuf[12*8+1] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[14*8+2] = vbuf[13*8+2] = vbuf[12*8+2] = vc[2] - vr[2] - vf[2] + vu[2];
	vbuf[14*8+3] = vbuf[13*8+3] = vbuf[12*8+3] = 1.0;

	vbuf[17*8+0] = vbuf[16*8+0] = vbuf[15*8+0] = vc[0] + vr[0] - vf[0] + vu[0];
	vbuf[17*8+1] = vbuf[16*8+1] = vbuf[15*8+1] = vc[1] + vr[1] - vf[1] + vu[1];
	vbuf[17*8+2] = vbuf[16*8+2] = vbuf[15*8+2] = vc[2] + vr[2] - vf[2] + vu[2];
	vbuf[17*8+3] = vbuf[16*8+3] = vbuf[15*8+3] = 1.0;

	vbuf[20*8+0] = vbuf[19*8+0] = vbuf[18*8+0] = vc[0] - vr[0] + vf[0] + vu[0];
	vbuf[20*8+1] = vbuf[19*8+1] = vbuf[18*8+1] = vc[1] - vr[1] + vf[1] + vu[1];
	vbuf[20*8+2] = vbuf[19*8+2] = vbuf[18*8+2] = vc[2] - vr[2] + vf[2] + vu[2];
	vbuf[20*8+3] = vbuf[19*8+3] = vbuf[18*8+3] = 1.0;

	vbuf[23*8+0] = vbuf[22*8+0] = vbuf[21*8+0] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[23*8+1] = vbuf[22*8+1] = vbuf[21*8+1] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[23*8+2] = vbuf[22*8+2] = vbuf[21*8+2] = vc[2] + vr[2] + vf[2] + vu[2];
	vbuf[23*8+3] = vbuf[22*8+3] = vbuf[21*8+3] = 1.0;

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




void carveopaque2d_circle(struct arena* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	int a,b,j;
	float c,s;
	float* vbuf;
	u16* ibuf;
	int vlen = opaque2d_vars(win, opaque2d, &vbuf, &ibuf, acc+1, acc);

	for(j=0;j<acc;j++)
	{
		a = j*8;
		b = j*3;

		c = cosine(j*tau/acc);
		s = sine(j*tau/acc);
		vbuf[a+0] = vc[0] + vr[0]*c + vf[0]*s;
		vbuf[a+1] = vc[1] + vr[1]*c + vf[1]*s;
		vbuf[a+2] = vc[2] + vr[2]*c + vf[2]*s;
		vbuf[a+3] = 1.0;

		vbuf[a+4] = rr;
		vbuf[a+5] = gg;
		vbuf[a+6] = bb;
		vbuf[a+7] = aa;

		ibuf[b+0] = vlen + acc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%acc;
	}

	a = acc*8;
	vbuf[a+0] = vc[0];
	vbuf[a+1] = vc[1];
	vbuf[a+2] = vc[2];
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
}
void carveopaque2d_sphere(struct arena* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	int a,b,j,k;
	float c,s;
	vec3 tc, tr, tf;

#define accx (acc)
#define accy (acc|0x1)
	float* vbuf;
	u16* ibuf;
	int vlen = opaque2d_vars(win, opaque2d, &vbuf, &ibuf, accx*accy+2, accx*accy*2);

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

			a = (k*accx + j)*8;
			vbuf[a+0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+2] = tc[2] + tr[2]*c + tf[2]*s;
			vbuf[a+3] = 1.0;

			vbuf[a+4] = rr;
			vbuf[a+5] = gg;
			vbuf[a+6] = bb;
			vbuf[a+7] = aa;

			if(k >= accy-1)continue;
			b = k*accx*8;
			ibuf[b + 6*j + 0] = vlen+(k*accx)+j;
			ibuf[b + 6*j + 1] = vlen+(k*accx)+(j+1)%accx;
			ibuf[b + 6*j + 2] = vlen+(k*accx)+accx+j;
			ibuf[b + 6*j + 3] = vlen+(k*accx)+(j+1)%accx;
			ibuf[b + 6*j + 4] = vlen+(k*accx)+accx+j;
			ibuf[b + 6*j + 5] = vlen+(k*accx)+accx+(j+1)%accx;
		}
	}

	a = accx*accy*8;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];
	vbuf[a+ 3] = 1.0;
	vbuf[a+ 4] = rr;
	vbuf[a+ 5] = gg;
	vbuf[a+ 6] = bb;
	vbuf[a+ 7] = aa;

	vbuf[a+ 8] = vc[0]+vu[0];
	vbuf[a+ 9] = vc[1]+vu[1];
	vbuf[a+10] = vc[2]+vu[2];
	vbuf[a+11] = 1.0;
	vbuf[a+12] = rr;
	vbuf[a+13] = gg;
	vbuf[a+14] = bb;
	vbuf[a+15] = aa;

	b = (accy-1)*accx*8;
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