#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 18
#define stlv 0x82




void carvedrone_node(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float j;
	vec3 v0, v1, v2, t0, t1, t2, tt;

	tt[0] = vr[0];
	tt[1] = vr[1];
	tt[2] = vr[2];
	quaternion_operation(tt, vu, tau/3);

	v0[0] = vc[0] + vr[0];
	v0[1] = vc[1] + vr[1];
	v0[2] = vc[2] + vr[2];
	v1[0] = vc[0] + tt[0];
	v1[1] = vc[1] + tt[1];
	v1[2] = vc[2] + tt[2];
	v2[0] = vc[0] - tt[0] - vr[0];
	v2[1] = vc[1] - tt[1] - vr[1];
	v2[2] = vc[2] - tt[2] - vr[2];

	t0[0] = v0[0];
	t0[1] = v0[1];
	t0[2] = v0[2];
	t1[0] = v1[0];
	t1[1] = v1[1];
	t1[2] = v1[2];
	t2[0] = v2[0];
	t2[1] = v2[1];
	t2[2] = v2[2];
	carveline_yshape(win, 0x204020, t0, t1, t2);
	t0[2] += 16.0;
	t1[2] += 16.0;
	t2[2] += 16.0;
	carveline_yshape(win, 0x204020, t0, t1, t2);

	t0[0] = (v0[0]+v1[0])/2;
	t0[1] = (v0[1]+v1[1])/2;
	t0[2] = (v0[2]+v1[2])/2;
	t1[0] = (v1[0]+v2[0])/2;
	t1[1] = (v1[1]+v2[1])/2;
	t1[2] = (v1[2]+v2[2])/2;
	t2[0] = (v2[0]+v0[0])/2;
	t2[1] = (v2[1]+v0[1])/2;
	t2[2] = (v2[2]+v0[2])/2;
	carveline_yshape(win, 0xffffff, t0, t1, t2);
	t0[2] += 16.0;
	t1[2] += 16.0;
	t2[2] += 16.0;
	carveline_yshape(win, 0xffffff, t0, t1, t2);

	t0[0] = v0[0];
	t0[1] = v0[1];
	t0[2] = v0[2];
	t1[0] = v1[0];
	t1[1] = v1[1];
	t1[2] = v1[2];
	t2[0] = v2[0];
	t2[1] = v2[1];
	t2[2] = v2[2];
	carvesolid_triangle(win, 0x808080, t0, t1, t2);
	t0[2] += 16.0;
	t1[2] += 16.0;
	t2[2] += 16.0;
	carvesolid_triangle(win, 0x808080, t0, t1, t2);

	t0[0] = (v0[0]+v1[0])/2;
	t0[1] = (v0[1]+v1[1])/2;
	t0[2] = (v0[2]+v1[2])/2+8.0;
	t1[0] = (v0[0]-v1[0])/2;
	t1[1] = (v0[1]-v1[1])/2;
	t1[2] = (v0[2]-v1[2])/2;
	t2[0] = 0.0;
	t2[1] = 0.0;
	t2[2] = 0.8;
	carvesolid_rect(win, 0x604020, t0, t1, t2);
	t0[0] = (v2[0]+v1[0])/2;
	t0[1] = (v2[1]+v1[1])/2;
	t0[2] = (v2[2]+v1[2])/2+8.0;
	t1[0] = (v2[0]-v1[0])/2;
	t1[1] = (v2[1]-v1[1])/2;
	t1[2] = (v2[2]-v1[2])/2;
	carvesolid_rect(win, 0x206040, t0, t1, t2);
	t0[0] = (v0[0]+v2[0])/2;
	t0[1] = (v0[1]+v2[1])/2;
	t0[2] = (v0[2]+v2[2])/2+8.0;
	t1[0] = (v0[0]-v2[0])/2;
	t1[1] = (v0[1]-v2[1])/2;
	t1[2] = (v0[2]-v2[2])/2;
	carvesolid_rect(win, 0x204060, t0, t1, t2);
}
void carvedrone(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	int x,y;
	vec3 xx, yy, tt;
	vec3 v0, v1, v2;
	vec3 tc, tr, tu;

	tt[0] = vr[0];
	tt[1] = vr[1];
	tt[2] = vr[2];
	quaternion_operation(tt, vu, tau/3);
	v0[0] = vr[0];
	v0[1] = vr[1];
	v0[2] = vr[2];
	v1[0] = tt[0];
	v1[1] = tt[1];
	v1[2] = tt[2];
	v2[0] = -tt[0]-vr[0];
	v2[1] = -tt[1]-vr[1];
	v2[2] = -tt[2]-vr[2];

	xx[0] = v1[0]-v2[0];
	xx[1] = v1[1]-v2[1];
	xx[2] = v1[2]-v2[2];
	yy[0] = vr[0]*3/2;
	yy[1] = vr[1]*3/2;
	yy[2] = vr[2]*3/2;
	for(y=-4;y<=4;y++)
	{
		for(x=-4;x<=4;x++)
		{
			tt[0] = vc[0]+xx[0]*x+yy[0]*y;
			tt[1] = vc[1]+xx[1]*x+yy[1]*y;
			tt[2] = vc[2]+xx[2]*x+yy[2]*y;
			if(y%2)
			{
				tt[0] += xx[0]/2;
				tt[1] += xx[1]/2;
				tt[2] += xx[2]/2;
			}

			tr[0] = 0.95*vr[0];
			tr[1] = 0.95*vr[1];
			tr[2] = 0.95*vr[2];
			carvedrone_node(win, rgb, tt, tr, vu);

			tc[0] = tt[0]+(vr[0]-xx[0])/2;
			tc[1] = tt[1]+(vr[1]-xx[1])/2;
			tc[2] = tt[2]+(vr[2]-xx[2])/2;
			tr[0] = -0.95*vr[0];
			tr[1] = -0.95*vr[1];
			tr[2] = -0.95*vr[2];
			carvedrone_node(win, rgb, tc, tr, vu);
		}
	}
}
void carvesnowman(struct arena* win, u32 rgb, vec3 vc)
{
	vec3 tc;
	vec3 tr;
	vec3 tf;
	vec3 tu;

	//head
	tc[0] = vc[0];
	tc[1] = vc[1];
	tc[2] = vc[2]+80.0;
	tr[0] = 16.0;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 16.0;
	tf[2] = 0.0;
	tu[0] = 0.0;
	tu[1] = 0.0;
	tu[2] = 16.0;
	carvesolid_sphere(win, 0xffffff, tc, tr, tf, tu);

	//body
	tc[2] = vc[2]+32.0;
	tr[0] = 32.0;
	tf[1] = 32.0;
	tu[2] = 32.0;
	carvesolid_sphere(win, 0xffffff, tc, tr, tf, tu);

	//left hand
	tc[0] = vc[0]-28.0;
	tc[1] = vc[1];
	tc[2] = vc[2]+28.0+32.0;
	tr[0] = 8.0;
	tf[1] = 8.0;
	tu[2] = 8.0;
	carvesolid_sphere(win, 0xffffff, tc, tr, tf, tu);

	//right hand
	tc[0] = vc[0]+28.0;
	tc[1] = vc[1];
	tc[2] = vc[2]+28.0+32.0;
	carvesolid_sphere(win, 0xffffff, tc, tr, tf, tu);
}
void carveskydome(float vbuf[][6], u16* ibuf,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j,k,m,n;
	float a,c,s;
	vec3 tc, tr, tf;
/*
	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2];
	vbuf[ 3] = 0.0;
	vbuf[ 4] = 0.0;
	vbuf[ 5] = 0.0;

	vbuf[ 6] = vc[0] + vr[0] - vf[0];
	vbuf[ 7] = vc[1] + vr[1] - vf[1];
	vbuf[ 8] = vc[2] + vr[2] - vf[2];
	vbuf[ 9] = 1.0;
	vbuf[10] = 0.0;
	vbuf[11] = 0.0;

	vbuf[12] = vc[0] - vr[0] + vf[0];
	vbuf[13] = vc[1] - vr[1] + vf[1];
	vbuf[14] = vc[2] - vr[2] + vf[2];
	vbuf[15] = 0.0;
	vbuf[16] = 1.0;
	vbuf[17] = 0.0;

	vbuf[18] = vc[0] + vr[0] + vf[0];
	vbuf[19] = vc[1] + vr[1] + vf[1];
	vbuf[20] = vc[2] + vr[2] + vf[2];
	vbuf[21] = 1.0;
	vbuf[22] = 1.0;
	vbuf[23] = 0.0;

	ibuf[ 0] = 0;
	ibuf[ 1] = 1;
	ibuf[ 2] = 2;
	ibuf[ 3] = 1;
	ibuf[ 4] = 2;
	ibuf[ 5] = 3;
*/
#define ccc 8
	for(k=0;k<ccc;k++)
	{
		a = k*PI/(ccc*2);
		c = cosine(a);
		s = sine(a);

		tc[0] = vc[0] + vu[0]*s;
		tc[1] = vc[1] + vu[1]*s;
		tc[2] = vc[2] + vu[2]*s;
		tr[0] = vr[0]*c;
		tr[1] = vr[1]*c;
		tr[2] = vr[2]*c;
		tf[0] = vf[0]*c;
		tf[1] = vf[1]*c;
		tf[2] = vf[2]*c;

		for(j=0;j<(ccc*2);j++)
		{
			a = j*PI/ccc;
			c = cosine(a);
			s = sine(a);

			m = 2*ccc*k + j;
			vbuf[m][0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[m][1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[m][2] = tc[2] + tr[2]*c + tf[2]*s;
			vbuf[m][3] = 0.5 + (ccc-k)*c/(ccc*2);
			vbuf[m][4] = 0.5 + (ccc-k)*s/(ccc*2);
			vbuf[m][5] = 0.0;
		}
	}

	m = ccc*ccc*2;
	vbuf[m][0] = vc[0] + vu[0];
	vbuf[m][1] = vc[1] + vu[1];
	vbuf[m][2] = vc[2] + vu[2];
	vbuf[m][3] = 0.5;
	vbuf[m][4] = 0.5;
	vbuf[m][5] = 0.0;

	for(k=0;k<(ccc-1);k++)
	{
		n = 2*k*ccc;
		for(j=0;j<(ccc*2);j++)
		{
			m = (n+j)*6;
			ibuf[m+0] = n + j;
			ibuf[m+1] = n + (j+1)%(ccc*2);
			ibuf[m+2] = n + j + (ccc*2);
			ibuf[m+3] = n + j + (ccc*2);
			ibuf[m+4] = n + (j+1)%(ccc*2);
			ibuf[m+5] = n + (j+1)%(ccc*2) + (ccc*2);
		}
	}

	n = 2*ccc*(ccc-1);
	for(j=0;j<(ccc*2);j++)
	{
		m = (n+n+j)*3;
		ibuf[m+0] = n + j;
		ibuf[m+1] = n + (j+1)%(ccc*2);
		ibuf[m+2] = ccc*ccc*2;
	}
}
void carveplanet(float vbuf[][6], u16* ibuf,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
#define accx 16
#define accy 15
	int a,b,j,k;
	float c,s;
	vec3 tc, tr, tf;

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
			s = j*tau/(accx-1);
			c = cosine(s);
			s = sine(s);

			a = k*accx + j;
			vbuf[a][0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a][1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a][2] = tc[2] + tr[2]*c + tf[2]*s;
			vbuf[a][3] = j/(accx-1.0);
			vbuf[a][4] = 1.0-(k+1.0)/(accy+1.0);
			vbuf[a][5] = 0.0;

			if(j >= accx-1)continue;
			if(k >= accy-1)continue;

			b = ((accx-1)*k+j)*6;
			ibuf[b+0] = (k*accx)+j;
			ibuf[b+1] = (k*accx)+j+1;
			ibuf[b+2] = (k*accx)+accx+j;
			ibuf[b+3] = (k*accx)+j+1;
			ibuf[b+4] = (k*accx)+accx+j;
			ibuf[b+5] = (k*accx)+accx+j+1;
		}
	}

	a = accx*accy;
	b = accx*accy + accx-1;
	for(j=0;j<accx-1;j++)
	{
		vbuf[a+j][0] = vc[0] - vu[0];
		vbuf[a+j][1] = vc[1] - vu[1];
		vbuf[a+j][2] = vc[2] - vu[2];
		vbuf[a+j][3] = (j+0.5)/accx;
		vbuf[a+j][4] = 1.0;
		vbuf[a+j][5] = 0.0;

		vbuf[b+j][0] = vc[0] + vu[0];
		vbuf[b+j][1] = vc[1] + vu[1];
		vbuf[b+j][2] = vc[2] + vu[2];
		vbuf[b+j][3] = (j+0.5)/accx;
		vbuf[b+j][4] = 0.0;
		vbuf[b+j][5] = 0.0;
	}

	a = (accy-1)*(accx-1)*6;
	for(j=0;j<accx-1;j++)
	{
		b = a + j*6;
		ibuf[b + 0] = accx*accy+j;
		ibuf[b + 1] = j;
		ibuf[b + 2] = j+1;
		ibuf[b + 3] = accx*accy+accx-1+j;
		ibuf[b + 4] = accx*(accy-1)+j;
		ibuf[b + 5] = accx*(accy-1)+j+1;
	}
}




void carvesolid_bodypart(struct arena* win, u32 rgb, vec3 t0, vec3 t1)
{
	float n;
	vec3 tc, tr, tf, tu;

	tc[0] = (t0[0]+t1[0])/2;
	tc[1] = (t0[1]+t1[1])/2;
	tc[2] = (t0[2]+t1[2])/2;

	tu[0] = t0[0] - tc[0];
	tu[1] = t0[1] - tc[1];
	tu[2] = t0[2] - tc[2];

	n = tu[0]*tu[0] + tu[1]*tu[1] + tu[2]*tu[2];
	n = squareroot(n);

	if(tu[2] < 0.9*n)
	{
		tr[0] = tu[1]*1.0 - tu[2]*0.0;
		tr[1] = tu[2]*0.0 - tu[0]*1.0;
		tr[2] = tu[0]*0.0 - tu[1]*0.0;
	}
	else
	{
		tr[0] = tu[1]*0.0 - tu[2]*0.0;
		tr[1] = tu[2]*1.0 - tu[0]*0.0;
		tr[2] = tu[0]*0.0 - tu[1]*1.0;
	}
	tr[0] /= 3;
	tr[1] /= 3;
	tr[2] /= 3;

	tf[0] = tu[1]*tr[2] - tu[2]*tr[1];
	tf[1] = tu[2]*tr[0] - tu[0]*tr[2];
	tf[2] = tu[0]*tr[1] - tu[1]*tr[0];
	tf[0] /= n;
	tf[1] /= n;
	tf[2] /= n;

	carvesolid_sphere(win, 0xffffff, tc, tr, tf, tu);
}
void carvestl(
	struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu,
	struct actor* act, int flag)
{
/*
	void* stlbuf, int stllen, float f)

	float* p;
	int j, k, ret;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int vlen = mod[stlv].vlen;
	float* vbuf = (mod[stlv].vbuf) + (36*vlen);

	ret = *(u32*)(stlbuf+80);
	ret = ret%(0x1000000/36);
	mod[stlv].vlen += ret*3;

	for(j=0;j<ret;j++)
	{
		p = (void*)stlbuf + 84 + j*50;
		k = j*27;

		vbuf[k+ 0] = vc[0] + (p[3]-sx)*f;
		vbuf[k+ 1] = vc[1] + (p[4]-sy)*f;
		vbuf[k+ 2] = (p[5]-sz)*f;
		vbuf[k+ 3] = 1.0;
		vbuf[k+ 4] = 1.0;
		vbuf[k+ 5] = 1.0;
		vbuf[k+ 6] = p[0];
		vbuf[k+ 7] = p[1];
		vbuf[k+ 8] = p[2];

		vbuf[k+ 9] = vc[0] + (p[6]-sx)*f;
		vbuf[k+10] = vc[1] + (p[7]-sy)*f;
		vbuf[k+11] = (p[8]-sz)*f;
		vbuf[k+12] = 1.0;
		vbuf[k+13] = 1.0;
		vbuf[k+14] = 1.0;
		vbuf[k+15] = p[0];
		vbuf[k+16] = p[1];
		vbuf[k+17] = p[2];

		vbuf[k+18] = vc[0] + (p[9]-sx)*f;
		vbuf[k+19] = vc[1] + (p[10]-sy)*f;
		vbuf[k+20] = (p[11]-sz)*f;
		vbuf[k+21] = 1.0;
		vbuf[k+22] = 1.0;
		vbuf[k+23] = 1.0;
		vbuf[k+24] = p[0];
		vbuf[k+25] = p[1];
		vbuf[k+26] = p[2];
	}
*/
}