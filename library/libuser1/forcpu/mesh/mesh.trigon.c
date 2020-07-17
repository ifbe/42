#include "libuser.h"
#define circleacc 32
#define sphereaccx 16
#define sphereaccy (sphereaccx|0x1)




void carvetrigonindex_triangle_v3n3x3(float* vbuf,int vlen, u16* ibuf,int ilen, vec3 v0, vec3 v1, vec3 v2)
{
	vec3 n;
	n[0] = (v1[1]-v0[1])*(v2[2]-v0[2]) - (v1[2]-v0[2])*(v2[1]-v0[1]);
	n[1] = (v1[2]-v0[2])*(v2[0]-v0[0]) - (v1[0]-v0[0])*(v2[2]-v0[2]);
	n[2] = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0]);
	vec3_setlen(n, 1.0);

	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];
	vbuf[ 3] = n[0];
	vbuf[ 4] = n[1];
	vbuf[ 5] = n[2];

	vbuf[ 9] = v1[0];
	vbuf[10] = v1[1];
	vbuf[11] = v1[2];
	vbuf[12] = n[0];
	vbuf[13] = n[1];
	vbuf[14] = n[2];

	vbuf[18] = v2[0];
	vbuf[19] = v2[1];
	vbuf[20] = v2[2];
	vbuf[21] = n[0];
	vbuf[22] = n[1];
	vbuf[23] = n[2];

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}
void carvetrigonindex_triangle_v4n4x4(float* vbuf,int vlen, u16* ibuf,int ilen, vec3 v0, vec3 v1, vec3 v2)
{
	vec3 n;
	n[0] = (v1[1]-v0[1])*(v2[2]-v0[2]) - (v1[2]-v0[2])*(v2[1]-v0[1]);
	n[1] = (v1[2]-v0[2])*(v2[0]-v0[0]) - (v1[0]-v0[0])*(v2[2]-v0[2]);
	n[2] = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0]);

	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];
	vbuf[ 3] = 1.0;
	vbuf[ 4] = n[0];
	vbuf[ 5] = n[1];
	vbuf[ 6] = n[2];
	vbuf[ 7] = 1.0;

	vbuf[12] = v1[0];
	vbuf[13] = v1[1];
	vbuf[14] = v1[2];
	vbuf[15] = 1.0;
	vbuf[16] = n[0];
	vbuf[17] = n[1];
	vbuf[18] = n[2];
	vbuf[19] = 1.0;

	vbuf[24] = v2[0];
	vbuf[25] = v2[1];
	vbuf[26] = v2[2];
	vbuf[27] = 1.0;
	vbuf[28] = n[0];
	vbuf[29] = n[1];
	vbuf[30] = n[2];
	vbuf[31] = 1.0;

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}




void carvetrigonindex_rect_v3n3x3(float* vbuf, int vlen, u16* ibuf, int ilen, vec3 vc, vec3 vr, vec3 vf)
{
	vec3 n;
	n[0] = vr[1]*vf[2] - vr[2]*vf[1];
	n[1] = vr[2]*vf[0] - vr[0]*vf[2];
	n[2] = vr[0]*vf[1] - vr[1]*vf[0];
	vec3_setlen(n, 1.0);

	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2];
	vbuf[ 3] = n[0];
	vbuf[ 4] = n[1];
	vbuf[ 5] = n[2];

	vbuf[ 9] = vc[0] + vr[0] - vf[0];
	vbuf[10] = vc[1] + vr[1] - vf[1];
	vbuf[11] = vc[2] + vr[2] - vf[2];
	vbuf[12] = n[0];
	vbuf[13] = n[1];
	vbuf[14] = n[2];

	vbuf[18] = vc[0] - vr[0] + vf[0];
	vbuf[19] = vc[1] - vr[1] + vf[1];
	vbuf[20] = vc[2] - vr[2] + vf[2];
	vbuf[21] = n[0];
	vbuf[22] = n[1];
	vbuf[23] = n[2];

	vbuf[27] = vc[0] + vr[0] + vf[0];
	vbuf[28] = vc[1] + vr[1] + vf[1];
	vbuf[29] = vc[2] + vr[2] + vf[2];
	vbuf[30] = n[0];
	vbuf[31] = n[1];
	vbuf[32] = n[2];

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}
void carvetrigonindex_rect_v4n4x4(float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc, vec3 vr, vec3 vf)
{
	vec3 n;
	n[0] = vr[1]*vf[2] - vr[2]*vf[1];
	n[1] = vr[2]*vf[0] - vr[0]*vf[2];
	n[2] = vr[0]*vf[1] - vr[1]*vf[0];

	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2];
	vbuf[ 3] = 1.0;
	vbuf[ 4] = n[0];
	vbuf[ 5] = n[1];
	vbuf[ 6] = n[2];
	vbuf[ 7] = 1.0;

	vbuf[12] = vc[0] + vr[0] - vf[0];
	vbuf[13] = vc[1] + vr[1] - vf[1];
	vbuf[14] = vc[2] + vr[2] - vf[2];
	vbuf[15] = 1.0;
	vbuf[16] = n[0];
	vbuf[17] = n[1];
	vbuf[18] = n[2];
	vbuf[19] = 1.0;

	vbuf[24] = vc[0] - vr[0] + vf[0];
	vbuf[25] = vc[1] - vr[1] + vf[1];
	vbuf[26] = vc[2] - vr[2] + vf[2];
	vbuf[27] = 1.0;
	vbuf[28] = n[0];
	vbuf[29] = n[1];
	vbuf[30] = n[2];
	vbuf[31] = 1.0;

	vbuf[36] = vc[0] + vr[0] + vf[0];
	vbuf[37] = vc[1] + vr[1] + vf[1];
	vbuf[38] = vc[2] + vr[2] + vf[2];
	vbuf[39] = 1.0;
	vbuf[40] = n[0];
	vbuf[41] = n[1];
	vbuf[42] = n[2];
	vbuf[43] = 1.0;

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}




void carvetrigonindex_circle_v3n3x3(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf)
{
	int a,b,j;
	float c,s;
	vec3 vu;
	vu[0] = vr[1]*vf[2] - vr[2]*vf[1];
	vu[1] = vr[2]*vf[0] - vr[0]*vf[2];
	vu[2] = vr[0]*vf[1] - vr[1]*vf[0];
	vec3_setlen(vu, 1.0);

	for(j=0;j<circleacc;j++)
	{
		a = j*9;
		b = j*3;

		c = getcos(j*tau/circleacc);
		s = getsin(j*tau/circleacc);
		vbuf[a+0] = vc[0] + vr[0]*c + vf[0]*s;
		vbuf[a+1] = vc[1] + vr[1]*c + vf[1]*s;
		vbuf[a+2] = vc[2] + vr[2]*c + vf[2]*s;

		vbuf[a+3] = vu[0];
		vbuf[a+4] = vu[1];
		vbuf[a+5] = vu[2];

		ibuf[b+0] = vlen + circleacc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%circleacc;
	}

	a = circleacc*9;
	vbuf[a+0] = vc[0];
	vbuf[a+1] = vc[1];
	vbuf[a+2] = vc[2];
	vbuf[a+3] = vu[0];
	vbuf[a+4] = vu[1];
	vbuf[a+5] = vu[2];
}
void carvetrigonindex_circle_v4n4x4(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf)
{
	int a,b,j;
	float c,s;

	vec3 n;
	n[0] = vr[1]*vf[2] - vr[2]*vf[1];
	n[1] = vr[2]*vf[0] - vr[0]*vf[2];
	n[2] = vr[0]*vf[1] - vr[1]*vf[0];
	for(j=0;j<circleacc;j++)
	{
		a = j*12;
		b = j*3;

		c = getcos(j*tau/circleacc);
		s = getsin(j*tau/circleacc);
		vbuf[a+0] = vc[0] + vr[0]*c + vf[0]*s;
		vbuf[a+1] = vc[1] + vr[1]*c + vf[1]*s;
		vbuf[a+2] = vc[2] + vr[2]*c + vf[2]*s;
		vbuf[a+3] = 1.0;

		vbuf[a+4] = n[0];
		vbuf[a+5] = n[1];
		vbuf[a+6] = n[2];
		vbuf[a+7] = 1.0;

		ibuf[b+0] = vlen + circleacc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%circleacc;
	}

	a = circleacc*12;
	vbuf[a+ 0] = vc[0];
	vbuf[a+ 1] = vc[1];
	vbuf[a+ 2] = vc[2];
	vbuf[a+ 3] = 1.0;
	vbuf[a+ 4] = n[0];
	vbuf[a+ 5] = n[1];
	vbuf[a+ 6] = n[2];
	vbuf[a+ 7] = 1.0;
}




#define caskacc 16
void carvetrigonindex_cask_v3n3x3(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int a,b,j;
	float c,s;
	vec3 vv;

	for(j=0;j<caskacc;j++)
	{
		a = j*18;
		b = j*6;
		c = getcos(j*tau/caskacc);
		s = getsin(j*tau/caskacc);

		vv[0] = c*vr[0] - s*vf[0];
		vv[1] = c*vr[1] - s*vf[1];
		vv[2] = c*vr[2] - s*vf[2];

		vbuf[a+0] = vc[0] - vu[0] + vv[0];
		vbuf[a+1] = vc[1] - vu[1] + vv[1];
		vbuf[a+2] = vc[2] - vu[2] + vv[2];
		vbuf[a+3] = vbuf[a+0] - vc[0];
		vbuf[a+4] = vbuf[a+1] - vc[1];
		vbuf[a+5] = vbuf[a+2] - vc[2];

		vbuf[a+ 9] = vc[0] + vu[0] + vv[0];
		vbuf[a+10] = vc[1] + vu[1] + vv[1];
		vbuf[a+11] = vc[2] + vu[2] + vv[2];
		vbuf[a+12] = vbuf[a+ 9] - vc[0];
		vbuf[a+13] = vbuf[a+10] - vc[1];
		vbuf[a+14] = vbuf[a+11] - vc[2];

		ibuf[b+0] = vlen + j*2;
		ibuf[b+1] = vlen + ((j+1)%caskacc)*2;
		ibuf[b+2] = vlen + 1 + j*2;

		ibuf[b+3] = vlen + 1 + ((j+1)%caskacc)*2;
		ibuf[b+4] = vlen + ((j+1)%caskacc)*2;
		ibuf[b+5] = vlen + 1 + j*2;
	}
}




void carvetrigonindex_sphere_v3n3x3(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int a,b,j,k;
	float c,s;
	vec3 tc, tr, tf;

	for(k=0;k<sphereaccy;k++)
	{
		s = (2*k-sphereaccy+1)*PI/(2*sphereaccy+2);
		c = getcos(s);
		s = getsin(s);

		tc[0] = vc[0] + vu[0]*s;
		tc[1] = vc[1] + vu[1]*s;
		tc[2] = vc[2] + vu[2]*s;
		tr[0] = vr[0]*c;
		tr[1] = vr[1]*c;
		tr[2] = vr[2]*c;
		tf[0] = vf[0]*c;
		tf[1] = vf[1]*c;
		tf[2] = vf[2]*c;

		for(j=0;j<sphereaccx;j++)
		{
			s = j*tau/sphereaccx;
			c = getcos(s);
			s = getsin(s);

			a = (k*sphereaccx + j)*9;
			vbuf[a+0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+2] = tc[2] + tr[2]*c + tf[2]*s;
			vbuf[a+3] = vbuf[a+0] - vc[0];
			vbuf[a+4] = vbuf[a+1] - vc[1];
			vbuf[a+5] = vbuf[a+2] - vc[2];

			if(k >= sphereaccy-1)continue;
			b = k*sphereaccx*6;
			ibuf[b + 6*j + 0] = vlen+(k*sphereaccx)+j;
			ibuf[b + 6*j + 1] = vlen+(k*sphereaccx)+(j+1)%sphereaccx;
			ibuf[b + 6*j + 2] = vlen+(k*sphereaccx)+sphereaccx+j;
			ibuf[b + 6*j + 3] = vlen+(k*sphereaccx)+(j+1)%sphereaccx;
			ibuf[b + 6*j + 4] = vlen+(k*sphereaccx)+sphereaccx+j;
			ibuf[b + 6*j + 5] = vlen+(k*sphereaccx)+sphereaccx+(j+1)%sphereaccx;
		}
	}

	a = sphereaccx*sphereaccy*9;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];
	vbuf[a+ 3] = -vu[0];
	vbuf[a+ 4] = -vu[1];
	vbuf[a+ 5] = -vu[2];

	vbuf[a+ 9] = vc[0]+vu[0];
	vbuf[a+10] = vc[1]+vu[1];
	vbuf[a+11] = vc[2]+vu[2];
	vbuf[a+12] = vu[0];
	vbuf[a+13] = vu[1];
	vbuf[a+14] = vu[2];

	b = (sphereaccy-1)*sphereaccx*6;
	for(j=0;j<sphereaccx;j++)
	{
		ibuf[b + (6*j) + 0] = vlen+sphereaccx*sphereaccy;
		ibuf[b + (6*j) + 1] = vlen+j;
		ibuf[b + (6*j) + 2] = vlen+(j+1)%sphereaccx;
		ibuf[b + (6*j) + 3] = vlen+sphereaccx*sphereaccy+1;
		ibuf[b + (6*j) + 4] = vlen+sphereaccx*(sphereaccy-1)+j;
		ibuf[b + (6*j) + 5] = vlen+sphereaccx*(sphereaccy-1)+(j+1)%sphereaccx;
	}
}
void carvetrigonindex_sphere_v4n4x4(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int a,b,j,k;
	float c,s;
	vec3 tc, tr, tf;

	for(k=0;k<sphereaccy;k++)
	{
		s = (2*k-sphereaccy+1)*PI/(2*sphereaccy+2);
		c = getcos(s);
		s = getsin(s);

		tc[0] = vc[0] + vu[0]*s;
		tc[1] = vc[1] + vu[1]*s;
		tc[2] = vc[2] + vu[2]*s;
		tr[0] = vr[0]*c;
		tr[1] = vr[1]*c;
		tr[2] = vr[2]*c;
		tf[0] = vf[0]*c;
		tf[1] = vf[1]*c;
		tf[2] = vf[2]*c;

		for(j=0;j<sphereaccx;j++)
		{
			s = j*tau/sphereaccx;
			c = getcos(s);
			s = getsin(s);

			a = (k*sphereaccx + j)*12;
			vbuf[a+ 0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+ 1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+ 2] = tc[2] + tr[2]*c + tf[2]*s;
			vbuf[a+ 4] = vbuf[a+0] - vc[0];
			vbuf[a+ 5] = vbuf[a+1] - vc[1];
			vbuf[a+ 6] = vbuf[a+2] - vc[2];

			if(k >= sphereaccy-1)continue;
			b = k*sphereaccx*6;
			ibuf[b + 6*j + 0] = vlen+(k*sphereaccx)+j;
			ibuf[b + 6*j + 1] = vlen+(k*sphereaccx)+(j+1)%sphereaccx;
			ibuf[b + 6*j + 2] = vlen+(k*sphereaccx)+sphereaccx+j;
			ibuf[b + 6*j + 3] = vlen+(k*sphereaccx)+(j+1)%sphereaccx;
			ibuf[b + 6*j + 4] = vlen+(k*sphereaccx)+sphereaccx+j;
			ibuf[b + 6*j + 5] = vlen+(k*sphereaccx)+sphereaccx+(j+1)%sphereaccx;
		}
	}

	a = sphereaccx*sphereaccy*12;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];
	vbuf[a+ 3] = 1.0;
	vbuf[a+ 4] = -vu[0];
	vbuf[a+ 5] = -vu[1];
	vbuf[a+ 6] = -vu[2];
	vbuf[a+ 7] = 1.0;

	vbuf[a+12] = vc[0]+vu[0];
	vbuf[a+13] = vc[1]+vu[1];
	vbuf[a+14] = vc[2]+vu[2];
	vbuf[a+15] = 1.0;
	vbuf[a+16] = vu[0];
	vbuf[a+17] = vu[1];
	vbuf[a+18] = vu[2];
	vbuf[a+19] = 1.0;

	b = (sphereaccy-1)*sphereaccx*6;
	for(j=0;j<sphereaccx;j++)
	{
		ibuf[b + (6*j) + 0] = vlen+sphereaccx*sphereaccy;
		ibuf[b + (6*j) + 1] = vlen+j;
		ibuf[b + (6*j) + 2] = vlen+(j+1)%sphereaccx;
		ibuf[b + (6*j) + 3] = vlen+sphereaccx*sphereaccy+1;
		ibuf[b + (6*j) + 4] = vlen+sphereaccx*(sphereaccy-1)+j;
		ibuf[b + (6*j) + 5] = vlen+sphereaccx*(sphereaccy-1)+(j+1)%sphereaccx;
	}
}




void carvetrigonindex_prism4_v3n3x3(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vbuf[2*9+0] = vbuf[1*9+0] = vbuf[0*9+0] = vc[0] - vr[0] - vf[0] - vu[0];
	vbuf[2*9+1] = vbuf[1*9+1] = vbuf[0*9+1] = vc[1] - vr[1] - vf[1] - vu[1];
	vbuf[2*9+2] = vbuf[1*9+2] = vbuf[0*9+2] = vc[2] - vr[2] - vf[2] - vu[2];
	vbuf[0*9+3] = -vu[0];
	vbuf[0*9+4] = -vu[1];
	vbuf[0*9+5] = -vu[2];
	vbuf[1*9+3] = -vf[0];
	vbuf[1*9+4] = -vf[1];
	vbuf[1*9+5] = -vf[2];
	vbuf[2*9+3] = -vr[0];
	vbuf[2*9+4] = -vr[1];
	vbuf[2*9+5] = -vr[2];

	vbuf[5*9+0] = vbuf[4*9+0] = vbuf[3*9+0] = vc[0] + vr[0] - vf[0] - vu[0];
	vbuf[5*9+1] = vbuf[4*9+1] = vbuf[3*9+1] = vc[1] + vr[1] - vf[1] - vu[1];
	vbuf[5*9+2] = vbuf[4*9+2] = vbuf[3*9+2] = vc[2] + vr[2] - vf[2] - vu[2];
	vbuf[3*9+3] = -vu[0];
	vbuf[3*9+4] = -vu[1];
	vbuf[3*9+5] = -vu[2];
	vbuf[4*9+3] = -vf[0];
	vbuf[4*9+4] = -vf[1];
	vbuf[4*9+5] = -vf[2];
	vbuf[5*9+3] = vr[0];
	vbuf[5*9+4] = vr[1];
	vbuf[5*9+5] = vr[2];

	vbuf[8*9+0] = vbuf[7*9+0] = vbuf[6*9+0] = vc[0] - vr[0] + vf[0] - vu[0];
	vbuf[8*9+1] = vbuf[7*9+1] = vbuf[6*9+1] = vc[1] - vr[1] + vf[1] - vu[1];
	vbuf[8*9+2] = vbuf[7*9+2] = vbuf[6*9+2] = vc[2] - vr[2] + vf[2] - vu[2];
	vbuf[6*9+3] = -vu[0];
	vbuf[6*9+4] = -vu[1];
	vbuf[6*9+5] = -vu[2];
	vbuf[7*9+3] = vf[0];
	vbuf[7*9+4] = vf[1];
	vbuf[7*9+5] = vf[2];
	vbuf[8*9+3] = -vr[0];
	vbuf[8*9+4] = -vr[1];
	vbuf[8*9+5] = -vr[2];

	vbuf[11*9+0] = vbuf[10*9+0] = vbuf[9*9+0] = vc[0] + vr[0] + vf[0] - vu[0];
	vbuf[11*9+1] = vbuf[10*9+1] = vbuf[9*9+1] = vc[1] + vr[1] + vf[1] - vu[1];
	vbuf[11*9+2] = vbuf[10*9+2] = vbuf[9*9+2] = vc[2] + vr[2] + vf[2] - vu[2];
	vbuf[ 9*9+3] = -vu[0];
	vbuf[ 9*9+4] = -vu[1];
	vbuf[ 9*9+5] = -vu[2];
	vbuf[10*9+3] = vf[0];
	vbuf[10*9+4] = vf[1];
	vbuf[10*9+5] = vf[2];
	vbuf[11*9+3] = vr[0];
	vbuf[11*9+4] = vr[1];
	vbuf[11*9+5] = vr[2];

	vbuf[14*9+0] = vbuf[13*9+0] = vbuf[12*9+0] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[14*9+1] = vbuf[13*9+1] = vbuf[12*9+1] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[14*9+2] = vbuf[13*9+2] = vbuf[12*9+2] = vc[2] - vr[2] - vf[2] + vu[2];
	vbuf[12*9+3] = vu[0];
	vbuf[12*9+4] = vu[1];
	vbuf[12*9+5] = vu[2];
	vbuf[13*9+3] = -vf[0];
	vbuf[13*9+4] = -vf[1];
	vbuf[13*9+5] = -vf[2];
	vbuf[14*9+3] = -vr[0];
	vbuf[14*9+4] = -vr[1];
	vbuf[14*9+5] = -vr[2];

	vbuf[17*9+0] = vbuf[16*9+0] = vbuf[15*9+0] = vc[0] + vr[0] - vf[0] + vu[0];
	vbuf[17*9+1] = vbuf[16*9+1] = vbuf[15*9+1] = vc[1] + vr[1] - vf[1] + vu[1];
	vbuf[17*9+2] = vbuf[16*9+2] = vbuf[15*9+2] = vc[2] + vr[2] - vf[2] + vu[2];
	vbuf[15*9+3] = vu[0];
	vbuf[15*9+4] = vu[1];
	vbuf[15*9+5] = vu[2];
	vbuf[16*9+3] = -vf[0];
	vbuf[16*9+4] = -vf[1];
	vbuf[16*9+5] = -vf[2];
	vbuf[17*9+3] = vr[0];
	vbuf[17*9+4] = vr[1];
	vbuf[17*9+5] = vr[2];

	vbuf[20*9+0] = vbuf[19*9+0] = vbuf[18*9+0] = vc[0] - vr[0] + vf[0] + vu[0];
	vbuf[20*9+1] = vbuf[19*9+1] = vbuf[18*9+1] = vc[1] - vr[1] + vf[1] + vu[1];
	vbuf[20*9+2] = vbuf[19*9+2] = vbuf[18*9+2] = vc[2] - vr[2] + vf[2] + vu[2];
	vbuf[18*9+3] = vu[0];
	vbuf[18*9+4] = vu[1];
	vbuf[18*9+5] = vu[2];
	vbuf[19*9+3] = vf[0];
	vbuf[19*9+4] = vf[1];
	vbuf[19*9+5] = vf[2];
	vbuf[20*9+3] = -vr[0];
	vbuf[20*9+4] = -vr[1];
	vbuf[20*9+5] = -vr[2];

	vbuf[23*9+0] = vbuf[22*9+0] = vbuf[21*9+0] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[23*9+1] = vbuf[22*9+1] = vbuf[21*9+1] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[23*9+2] = vbuf[22*9+2] = vbuf[21*9+2] = vc[2] + vr[2] + vf[2] + vu[2];
	vbuf[21*9+3] = vu[0];
	vbuf[21*9+4] = vu[1];
	vbuf[21*9+5] = vu[2];
	vbuf[22*9+3] = vf[0];
	vbuf[22*9+4] = vf[1];
	vbuf[22*9+5] = vf[2];
	vbuf[23*9+3] = vr[0];
	vbuf[23*9+4] = vr[1];
	vbuf[23*9+5] = vr[2];

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