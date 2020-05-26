#include "libuser.h"
#define acc 32




void carvelinearray(float* vbuf, int vlen, vec3 va, vec3 vb)
{
	vbuf[ 0] = va[0];
	vbuf[ 1] = va[1];
	vbuf[ 2] = va[2];

	vbuf[ 6] = vb[0];
	vbuf[ 7] = vb[1];
	vbuf[ 8] = vb[2];
}
void carvelineindex(float* vbuf, int vlen, u16* ibuf, int ilen, vec3 va, vec3 vb)
{
	vbuf[ 0] = va[0];
	vbuf[ 1] = va[1];
	vbuf[ 2] = va[2];

	vbuf[ 6] = vb[0];
	vbuf[ 7] = vb[1];
	vbuf[ 8] = vb[2];

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
}




void carvelineindex_bezier(float* vbuf, int vlen, u16* ibuf, int ilen, vec3 va, vec3 vb, vec3 vt)
{
	int j;
	float t;
	for(j=0;j<=acc;j++)
	{
		t = (float)j / acc;

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*va[0] + 2*t*(1.0-t)*vt[0] + t*t*vb[0];
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*va[1] + 2*t*(1.0-t)*vt[1] + t*t*vb[1];
		vbuf[6*j+2] = (1.0-t)*(1.0-t)*va[2] + 2*t*(1.0-t)*vt[2] + t*t*vb[2];

		if(j >= acc)break;
		ibuf[2*j+0] = vlen + j;
		ibuf[2*j+1] = vlen + j+1;
	}
}




void carvelineindex_triangle(float* vbuf, int vlen, u16* ibuf, int ilen, vec3 v0, vec3 v1, vec3 v2)
{
	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];

	vbuf[ 6] = v1[0];
	vbuf[ 7] = v1[1];
	vbuf[ 8] = v1[2];

	vbuf[12] = v2[0];
	vbuf[13] = v2[1];
	vbuf[14] = v2[2];

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+2;
	ibuf[4] = vlen;
	ibuf[5] = vlen+2;
}




void carvelineindex_rect(float* vbuf, int vlen, u16* ibuf, int ilen, vec3 vc, vec3 vr, vec3 vf)
{
	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2];

	vbuf[ 6] = vc[0] + vr[0] - vf[0];
	vbuf[ 7] = vc[1] + vr[1] - vf[1];
	vbuf[ 8] = vc[2] + vr[2] - vf[2];

	vbuf[12] = vc[0] - vr[0] + vf[0];
	vbuf[13] = vc[1] - vr[1] + vf[1];
	vbuf[14] = vc[2] - vr[2] + vf[2];

	vbuf[18] = vc[0] + vr[0] + vf[0];
	vbuf[19] = vc[1] + vr[1] + vf[1];
	vbuf[20] = vc[2] + vr[2] + vf[2];

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+3;
	ibuf[4] = vlen+3;
	ibuf[5] = vlen+2;
	ibuf[6] = vlen+0;
	ibuf[7] = vlen+2;
}



#define lineacc (acc*2)
void carvelineindex_circle(float* vbuf, int vlen, u16* ibuf, int ilen, vec3 vc, vec3 vr, vec3 vf)
{
	int j;
	float c,s;
	float q[4];

	for(j=0;j<lineacc;j++)
	{
		c = cosine(j*tau/lineacc);
		s = sine(j*tau/lineacc);
		vbuf[6*j+0] = vc[0] + vr[0]*c + vf[0]*s;
		vbuf[6*j+1] = vc[1] + vr[1]*c + vf[1]*s;
		vbuf[6*j+2] = vc[2] + vr[2]*c + vf[2]*s;

		ibuf[2*j+0] = vlen+j;
		ibuf[2*j+1] = vlen+(j+1)%lineacc;
	}
}
