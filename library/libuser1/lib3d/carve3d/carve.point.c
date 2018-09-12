#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 18
#define pointv 8




void carvepoint(struct arena* win, u32 rgb, vec3 vc)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += 1;

	vbuf[0] = vc[0];
	vbuf[1] = vc[1];
	vbuf[2] = vc[2];
	vbuf[3] = rr;
	vbuf[4] = gg;
	vbuf[5] = bb;
}
void carvepoint_bezier(struct arena* win, u32 rgb,
	vec3 va, vec3 vb, vec3 vt)
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

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*va[0] + 2*t*(1.0-t)*vt[0] + t*t*vb[0];
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*va[1] + 2*t*(1.0-t)*vt[1] + t*t*vb[1];
		vbuf[6*j+2] = (1.0-t)*(1.0-t)*va[2] + 2*t*(1.0-t)*vt[2] + t*t*vb[2];
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
	}
}




void carvepoint_triangle(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
}
void carvepoint_rect(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
}
void carvepoint_circle(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	int j,k;
	float s,t;
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += acc;

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);

		vbuf[6*j+0] = vc[0] + r[0];
		vbuf[6*j+1] = vc[1] + r[1];
		vbuf[6*j+2] = vc[2] + r[2];
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
	}
}




void carvepoint_pyramid3()
{
}
void carvepoint_pyramid4()
{
}
void carvepoint_pyramid5()
{
}
void carvepoint_pyramid6()
{
}
void carvepoint_cone(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	int j,k;
	float s,t;
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += acc+2;

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);

		vbuf[6*j+0] = vc[0] + r[0];
		vbuf[6*j+1] = vc[1] + r[1];
		vbuf[6*j+2] = vc[2] + r[2];
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
	}

	j = acc*6;

	vbuf[j+ 0] = vc[0];
	vbuf[j+ 1] = vc[1];
	vbuf[j+ 2] = vc[2];
	vbuf[j+ 3] = rr;
	vbuf[j+ 4] = gg;
	vbuf[j+ 5] = bb;

	vbuf[j+ 6] = vc[0]+vu[0];
	vbuf[j+ 7] = vc[1]+vu[1];
	vbuf[j+ 8] = vc[2]+vu[2];
	vbuf[j+ 9] = rr;
	vbuf[j+10] = gg;
	vbuf[j+11] = bb;
}




void carvepoint_prism3()
{
}
void carvepoint_prism4()
{
}
void carvepoint_prism5()
{
}
void carvepoint_prism6()
{
}
void carvepoint_cask(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
}
void carvepoint_cylinder(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
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
	mod[pointv].vlen += acc*2;

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		q[0] = vu[0];
		q[1] = vu[1];
		q[2] = vu[2];
		quaternion_operation(r, q, j*tau/acc);

		k = j*12;

		vbuf[k+ 0] = vc[0] - vu[0] + r[0];
		vbuf[k+ 1] = vc[1] - vu[1] + r[1];
		vbuf[k+ 2] = vc[2] - vu[2] + r[2];
		vbuf[k+ 3] = rr;
		vbuf[k+ 4] = gg;
		vbuf[k+ 5] = bb;

		vbuf[k+ 6] = vc[0] + vu[0] + r[0];
		vbuf[k+ 7] = vc[1] + vu[1] + r[1];
		vbuf[k+ 8] = vc[2] + vu[2] + r[2];
		vbuf[k+ 9] = rr;
		vbuf[k+10] = gg;
		vbuf[k+11] = bb;
	}
}




void carvepoint_tetrahedron()
{
}
void carvepoint_octahedron()
{
}
void carvepoint_dodecahedron(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float a = 1.618;
	float b = 1.0/1.618;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += 20;

	for(j=0;j<20*6;j+=6)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}

	//(+-1, +-1, +-1)
	vbuf[  0] = vc[0]-vr[0]-vf[0]-vu[0];
	vbuf[  1] = vc[1]-vr[1]-vf[1]-vu[1];
	vbuf[  2] = vc[2]-vr[2]-vf[2]-vu[2];

	vbuf[  6] = vc[0]+vr[0]-vf[0]-vu[0];
	vbuf[  7] = vc[1]+vr[1]-vf[1]-vu[1];
	vbuf[  8] = vc[2]+vr[2]-vf[2]-vu[2];

	vbuf[ 12] = vc[0]-vr[0]+vf[0]-vu[0];
	vbuf[ 13] = vc[1]-vr[1]+vf[1]-vu[1];
	vbuf[ 14] = vc[2]-vr[2]+vf[2]-vu[2];

	vbuf[ 18] = vc[0]+vr[0]+vf[0]-vu[0];
	vbuf[ 19] = vc[1]+vr[1]+vf[1]-vu[1];
	vbuf[ 20] = vc[2]+vr[2]+vf[2]-vu[2];

	vbuf[ 24] = vc[0]-vr[0]-vf[0]+vu[0];
	vbuf[ 25] = vc[1]-vr[1]-vf[1]+vu[1];
	vbuf[ 26] = vc[2]-vr[2]-vf[2]+vu[2];

	vbuf[ 30] = vc[0]+vr[0]-vf[0]+vu[0];
	vbuf[ 31] = vc[1]+vr[1]-vf[1]+vu[1];
	vbuf[ 32] = vc[2]+vr[2]-vf[2]+vu[2];

	vbuf[ 36] = vc[0]-vr[0]+vf[0]+vu[0];
	vbuf[ 37] = vc[1]-vr[1]+vf[1]+vu[1];
	vbuf[ 38] = vc[2]-vr[2]+vf[2]+vu[2];

	vbuf[ 42] = vc[0]+vr[0]+vf[0]+vu[0];
	vbuf[ 43] = vc[1]+vr[1]+vf[1]+vu[1];
	vbuf[ 44] = vc[2]+vr[2]+vf[2]+vu[2];

	//(0, +-a, +-b)
	vbuf[ 48] = vc[0] - a*vf[0] - b*vu[0];
	vbuf[ 49] = vc[1] - a*vf[1] - b*vu[1];
	vbuf[ 50] = vc[2] - a*vf[2] - b*vu[2];

	vbuf[ 54] = vc[0] + a*vf[0] - b*vu[0];
	vbuf[ 55] = vc[1] + a*vf[1] - b*vu[1];
	vbuf[ 56] = vc[2] + a*vf[2] - b*vu[2];

	vbuf[ 60] = vc[0] - a*vf[0] + b*vu[0];
	vbuf[ 61] = vc[1] - a*vf[1] + b*vu[1];
	vbuf[ 62] = vc[2] - a*vf[2] + b*vu[2];

	vbuf[ 66] = vc[0] + a*vf[0] + b*vu[0];
	vbuf[ 67] = vc[1] + a*vf[1] + b*vu[1];
	vbuf[ 68] = vc[2] + a*vf[2] + b*vu[2];

	//(+-b, 0, +-a)
	vbuf[ 72] = vc[0] - b*vr[0] - a*vu[0];
	vbuf[ 73] = vc[1] - b*vr[1] - a*vu[1];
	vbuf[ 74] = vc[2] - b*vr[2] - a*vu[2];

	vbuf[ 78] = vc[0] + b*vr[0] - a*vu[0];
	vbuf[ 79] = vc[1] + b*vr[1] - a*vu[1];
	vbuf[ 80] = vc[2] + b*vr[2] - a*vu[2];

	vbuf[ 84] = vc[0] - b*vr[0] + a*vu[0];
	vbuf[ 85] = vc[1] - b*vr[1] + a*vu[1];
	vbuf[ 86] = vc[2] - b*vr[2] + a*vu[2];

	vbuf[ 90] = vc[0] + b*vr[0] + a*vu[0];
	vbuf[ 91] = vc[1] + b*vr[1] + a*vu[1];
	vbuf[ 92] = vc[2] + b*vr[2] + a*vu[2];

	//(+-a, +-b, 0)
	vbuf[ 96] = vc[0] - a*vr[0] - b*vf[0];
	vbuf[ 97] = vc[1] - a*vr[1] - b*vf[1];
	vbuf[ 98] = vc[2] - a*vr[2] - b*vf[2];

	vbuf[102] = vc[0] + a*vr[0] - b*vf[0];
	vbuf[103] = vc[1] + a*vr[1] - b*vf[1];
	vbuf[104] = vc[2] + a*vr[2] - b*vf[2];

	vbuf[108] = vc[0] - a*vr[0] + b*vf[0];
	vbuf[109] = vc[1] - a*vr[1] + b*vf[1];
	vbuf[110] = vc[2] - a*vr[2] + b*vf[2];

	vbuf[114] = vc[0] + a*vr[0] + b*vf[0];
	vbuf[115] = vc[1] + a*vr[1] + b*vf[1];
	vbuf[116] = vc[2] + a*vr[2] + b*vf[2];
}
void carvepoint_icosahedron(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float m = 0.52573111211913360602566908484788;
	float n = 0.85065080835203993218154049706301;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += 12;

	for(j=0;j<12*6;j+=6)
	{
		vbuf[j + 0] = rr;
		vbuf[j + 1] = gg;
		vbuf[j + 2] = bb;
	}

	//(+-m, 0, +-n)
	vbuf[ 0] = vc[0] - m;
	vbuf[ 1] = vc[1];
	vbuf[ 2] = vc[2] - n;

	vbuf[ 6] = vc[0] + m;
	vbuf[ 7] = vc[1];
	vbuf[ 8] = vc[2] - n;

	vbuf[12] = vc[0] - m;
	vbuf[13] = vc[1];
	vbuf[14] = vc[2] + n;

	vbuf[18] = vc[0] + m;
	vbuf[19] = vc[1];
	vbuf[20] = vc[2] + n;

	//(0, +-n, +-m)
	vbuf[24] = vc[0];
	vbuf[25] = vc[1] - n;
	vbuf[26] = vc[2] - m;

	vbuf[30] = vc[0];
	vbuf[31] = vc[1] + n;
	vbuf[32] = vc[2] - m;

	vbuf[36] = vc[0];
	vbuf[37] = vc[1] - n;
	vbuf[38] = vc[2] + m;

	vbuf[42] = vc[0];
	vbuf[43] = vc[1] + n;
	vbuf[44] = vc[2] + m;

	//(+-n, +-m, 0)
	vbuf[48] = vc[0] - n;
	vbuf[49] = vc[1] - m;
	vbuf[50] = vc[2];

	vbuf[54] = vc[0] + n;
	vbuf[55] = vc[1] - m;
	vbuf[56] = vc[2];

	vbuf[60] = vc[0] - n;
	vbuf[61] = vc[1] + m;
	vbuf[62] = vc[2];

	vbuf[66] = vc[0] + n;
	vbuf[67] = vc[1] + m;
	vbuf[68] = vc[2];
}
void carvepoint_sphere(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
#define accx (acc*2)
#define accy (acc*2+1)
	int j,k,a;
	float c,s;
	vec3 tc, tr, tf;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += accx*accy+2;

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
		}
	}

	a = accx*accy*6;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];
	vbuf[a+ 3] = rr;
	vbuf[a+ 4] = gg;
	vbuf[a+ 5] = bb;

	vbuf[a+ 6] = vc[0]+vu[0];
	vbuf[a+ 7] = vc[1]+vu[1];
	vbuf[a+ 8] = vc[2]+vu[2];
	vbuf[a+ 9] = rr;
	vbuf[a+10] = gg;
	vbuf[a+11] = bb;
}