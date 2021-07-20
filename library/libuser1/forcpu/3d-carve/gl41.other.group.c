#include "libuser.h"
#define acc 18
#define stlv 0x82




void carvedrone_node(_obj* ctx, u32 rgb,
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
	gl41line_yshape(ctx, 0x204020, t0, t1, t2);
	t0[2] += 16.0;
	t1[2] += 16.0;
	t2[2] += 16.0;
	gl41line_yshape(ctx, 0x204020, t0, t1, t2);

	t0[0] = (v0[0]+v1[0])/2;
	t0[1] = (v0[1]+v1[1])/2;
	t0[2] = (v0[2]+v1[2])/2;
	t1[0] = (v1[0]+v2[0])/2;
	t1[1] = (v1[1]+v2[1])/2;
	t1[2] = (v1[2]+v2[2])/2;
	t2[0] = (v2[0]+v0[0])/2;
	t2[1] = (v2[1]+v0[1])/2;
	t2[2] = (v2[2]+v0[2])/2;
	gl41line_yshape(ctx, 0xffffff, t0, t1, t2);
	t0[2] += 16.0;
	t1[2] += 16.0;
	t2[2] += 16.0;
	gl41line_yshape(ctx, 0xffffff, t0, t1, t2);

	t0[0] = v0[0];
	t0[1] = v0[1];
	t0[2] = v0[2];
	t1[0] = v1[0];
	t1[1] = v1[1];
	t1[2] = v1[2];
	t2[0] = v2[0];
	t2[1] = v2[1];
	t2[2] = v2[2];
	gl41solid_triangle(ctx, 0x808080, t0, t1, t2);
	t0[2] += 16.0;
	t1[2] += 16.0;
	t2[2] += 16.0;
	gl41solid_triangle(ctx, 0x808080, t0, t1, t2);

	t0[0] = (v0[0]+v1[0])/2;
	t0[1] = (v0[1]+v1[1])/2;
	t0[2] = (v0[2]+v1[2])/2+8.0;
	t1[0] = (v0[0]-v1[0])/2;
	t1[1] = (v0[1]-v1[1])/2;
	t1[2] = (v0[2]-v1[2])/2;
	t2[0] = 0.0;
	t2[1] = 0.0;
	t2[2] = 0.8;
	gl41solid_rect(ctx, 0x604020, t0, t1, t2);
	t0[0] = (v2[0]+v1[0])/2;
	t0[1] = (v2[1]+v1[1])/2;
	t0[2] = (v2[2]+v1[2])/2+8.0;
	t1[0] = (v2[0]-v1[0])/2;
	t1[1] = (v2[1]-v1[1])/2;
	t1[2] = (v2[2]-v1[2])/2;
	gl41solid_rect(ctx, 0x206040, t0, t1, t2);
	t0[0] = (v0[0]+v2[0])/2;
	t0[1] = (v0[1]+v2[1])/2;
	t0[2] = (v0[2]+v2[2])/2+8.0;
	t1[0] = (v0[0]-v2[0])/2;
	t1[1] = (v0[1]-v2[1])/2;
	t1[2] = (v0[2]-v2[2])/2;
	gl41solid_rect(ctx, 0x204060, t0, t1, t2);
}
void carvedrone(_obj* ctx, u32 rgb,
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
			carvedrone_node(ctx, rgb, tt, tr, vu);

			tc[0] = tt[0]+(vr[0]-xx[0])/2;
			tc[1] = tt[1]+(vr[1]-xx[1])/2;
			tc[2] = tt[2]+(vr[2]-xx[2])/2;
			tr[0] = -0.95*vr[0];
			tr[1] = -0.95*vr[1];
			tr[2] = -0.95*vr[2];
			carvedrone_node(ctx, rgb, tc, tr, vu);
		}
	}
}




void gl41solid_propeller(_obj* ctx, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vt, int dir, int dt)
{
	float a;
	vec3 rr,ff;
	vec3 tc,tr,tf;

	a = dir * dt * tau / 1000000.0;

	rr[0] = vr[0];
	rr[1] = vr[1];
	rr[2] = vr[2];
	quaternion_operation(rr, vt, a);

	ff[0] = vf[0];
	ff[1] = vf[1];
	ff[2] = vf[2];
	quaternion_operation(ff, vt, a);

	tc[0] = vc[0] + rr[0];
	tc[1] = vc[1] + rr[1];
	tc[2] = vc[2] + rr[2];
	tf[0] = ff[0]/8 + dir*vt[0]/8;
	tf[1] = ff[1]/8 + dir*vt[1]/8;
	tf[2] = ff[2]/8 + dir*vt[2]/8;
	gl41solid_circle(ctx, rgb, tc, rr, tf);

	tc[0] = vc[0] - rr[0];
	tc[1] = vc[1] - rr[1];
	tc[2] = vc[2] - rr[2];
	tf[0] = ff[0]/8 - dir*vt[0]/8;
	tf[1] = ff[1]/8 - dir*vt[1]/8;
	tf[2] = ff[2]/8 - dir*vt[2]/8;
	gl41solid_circle(ctx, rgb, tc, rr, tf);
}
void gl41opaque_propeller(_obj* ctx, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vt, int dir, int dt)
{
	float a;
	vec3 rr,ff;
	vec3 tc,tr,tf;

	a = dir * dt * tau / 1000000.0;

	rr[0] = vr[0];
	rr[1] = vr[1];
	rr[2] = vr[2];
	quaternion_operation(rr, vt, a);

	ff[0] = vf[0];
	ff[1] = vf[1];
	ff[2] = vf[2];
	quaternion_operation(ff, vt, a);

	tc[0] = vc[0] + rr[0];
	tc[1] = vc[1] + rr[1];
	tc[2] = vc[2] + rr[2];
	tf[0] = ff[0]/8 + dir*vt[0]/8;
	tf[1] = ff[1]/8 + dir*vt[1]/8;
	tf[2] = ff[2]/8 + dir*vt[2]/8;
	gl41opaque_circle(ctx, rgb, tc, rr, tf);

	tc[0] = vc[0] - rr[0];
	tc[1] = vc[1] - rr[1];
	tc[2] = vc[2] - rr[2];
	tf[0] = ff[0]/8 - dir*vt[0]/8;
	tf[1] = ff[1]/8 - dir*vt[1]/8;
	tf[2] = ff[2]/8 - dir*vt[2]/8;
	gl41opaque_circle(ctx, rgb, tc, rr, tf);
}




void carvesnowman(_obj* ctx, u32 rgb, vec3 vc)
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
	gl41solid_sphere(ctx, 0xffffff, tc, tr, tf, tu);

	//body
	tc[2] = vc[2]+32.0;
	tr[0] = 32.0;
	tf[1] = 32.0;
	tu[2] = 32.0;
	gl41solid_sphere(ctx, 0xffffff, tc, tr, tf, tu);

	//left hand
	tc[0] = vc[0]-28.0;
	tc[1] = vc[1];
	tc[2] = vc[2]+28.0+32.0;
	tr[0] = 8.0;
	tf[1] = 8.0;
	tu[2] = 8.0;
	gl41solid_sphere(ctx, 0xffffff, tc, tr, tf, tu);

	//right hand
	tc[0] = vc[0]+28.0;
	tc[1] = vc[1];
	tc[2] = vc[2]+28.0+32.0;
	gl41solid_sphere(ctx, 0xffffff, tc, tr, tf, tu);
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
		c = getcos(a);
		s = getsin(a);

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
			c = getcos(a);
			s = getsin(a);

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
#define accx 64
#define accy 63
	int a,b,j,k;
	float c,s;
	vec3 tc, tr, tf;

	for(k=0;k<accy;k++)
	{
		s = (2*k-accy+1)*PI/(2*accy+2);
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

		for(j=0;j<accx;j++)
		{
			s = j*tau/(accx-1);
			c = getcos(s);
			s = getsin(s);

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
void carveskybox(float vbuf[][6], u16* ibuf,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	//left
	vbuf[0][0] = vc[0]-vr[0]+vf[0]-vu[0];
	vbuf[0][1] = vc[1]-vr[1]+vf[1]-vu[1];
	vbuf[0][2] = vc[2]-vr[2]+vf[2]-vu[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;
	vbuf[1][0] = vc[0]-vr[0]-vf[0]-vu[0];
	vbuf[1][1] = vc[1]-vr[1]-vf[1]-vu[1];
	vbuf[1][2] = vc[2]-vr[2]-vf[2]-vu[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 0.0;
	vbuf[1][5] = 0.0;
	vbuf[2][0] = vc[0]-vr[0]+vf[0]+vu[0];
	vbuf[2][1] = vc[1]-vr[1]+vf[1]+vu[1];
	vbuf[2][2] = vc[2]-vr[2]+vf[2]+vu[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;
	vbuf[3][0] = vc[0]-vr[0]-vf[0]+vu[0];
	vbuf[3][1] = vc[1]-vr[1]-vf[1]+vu[1];
	vbuf[3][2] = vc[2]-vr[2]-vf[2]+vu[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;
	ibuf[0] = 0;
	ibuf[1] = 1;
	ibuf[2] = 2;
	ibuf[3] = 3;
	ibuf[4] = 2;
	ibuf[5] = 1;

	//right
	vbuf[4][0] = vc[0]+vr[0]-vf[0]-vu[0];
	vbuf[4][1] = vc[1]+vr[1]-vf[1]-vu[1];
	vbuf[4][2] = vc[2]+vr[2]-vf[2]-vu[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;
	vbuf[5][0] = vc[0]+vr[0]+vf[0]-vu[0];
	vbuf[5][1] = vc[1]+vr[1]+vf[1]-vu[1];
	vbuf[5][2] = vc[2]+vr[2]+vf[2]-vu[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;
	vbuf[6][0] = vc[0]+vr[0]-vf[0]+vu[0];
	vbuf[6][1] = vc[1]+vr[1]-vf[1]+vu[1];
	vbuf[6][2] = vc[2]+vr[2]-vf[2]+vu[2];
	vbuf[6][3] = 0.0;
	vbuf[6][4] = 1.0;
	vbuf[6][5] = 0.0;
	vbuf[7][0] = vc[0]+vr[0]+vf[0]+vu[0];
	vbuf[7][1] = vc[1]+vr[1]+vf[1]+vu[1];
	vbuf[7][2] = vc[2]+vr[2]+vf[2]+vu[2];
	vbuf[7][3] = 1.0;
	vbuf[7][4] = 1.0;
	vbuf[7][5] = 0.0;
	ibuf[ 6] = 4;
	ibuf[ 7] = 5;
	ibuf[ 8] = 6;
	ibuf[ 9] = 7;
	ibuf[10] = 6;
	ibuf[11] = 5;

	//near
	vbuf[ 8][0] = vc[0]-vr[0]-vf[0]-vu[0];
	vbuf[ 8][1] = vc[1]-vr[1]-vf[1]-vu[1];
	vbuf[ 8][2] = vc[2]-vr[2]-vf[2]-vu[2];
	vbuf[ 8][3] = 0.0;
	vbuf[ 8][4] = 0.0;
	vbuf[ 8][5] = 0.0;
	vbuf[ 9][0] = vc[0]+vr[0]-vf[0]-vu[0];
	vbuf[ 9][1] = vc[1]+vr[1]-vf[1]-vu[1];
	vbuf[ 9][2] = vc[2]+vr[2]-vf[2]-vu[2];
	vbuf[ 9][3] = 1.0;
	vbuf[ 9][4] = 0.0;
	vbuf[ 9][5] = 0.0;
	vbuf[10][0] = vc[0]-vr[0]-vf[0]+vu[0];
	vbuf[10][1] = vc[1]-vr[1]-vf[1]+vu[1];
	vbuf[10][2] = vc[2]-vr[2]-vf[2]+vu[2];
	vbuf[10][3] = 0.0;
	vbuf[10][4] = 1.0;
	vbuf[10][5] = 0.0;
	vbuf[11][0] = vc[0]+vr[0]-vf[0]+vu[0];
	vbuf[11][1] = vc[1]+vr[1]-vf[1]+vu[1];
	vbuf[11][2] = vc[2]+vr[2]-vf[2]+vu[2];
	vbuf[11][3] = 1.0;
	vbuf[11][4] = 1.0;
	vbuf[11][5] = 0.0;
	ibuf[12] =  8;
	ibuf[13] =  9;
	ibuf[14] = 10;
	ibuf[15] = 11;
	ibuf[16] = 10;
	ibuf[17] =  9;

	//far
	vbuf[12][0] = vc[0]+vr[0]+vf[0]-vu[0];
	vbuf[12][1] = vc[1]+vr[1]+vf[1]-vu[1];
	vbuf[12][2] = vc[2]+vr[2]+vf[2]-vu[2];
	vbuf[12][3] = 0.0;
	vbuf[12][4] = 0.0;
	vbuf[12][5] = 0.0;
	vbuf[13][0] = vc[0]-vr[0]+vf[0]-vu[0];
	vbuf[13][1] = vc[1]-vr[1]+vf[1]-vu[1];
	vbuf[13][2] = vc[2]-vr[2]+vf[2]-vu[2];
	vbuf[13][3] = 1.0;
	vbuf[13][4] = 0.0;
	vbuf[13][5] = 0.0;
	vbuf[14][0] = vc[0]+vr[0]+vf[0]+vu[0];
	vbuf[14][1] = vc[1]+vr[1]+vf[1]+vu[1];
	vbuf[14][2] = vc[2]+vr[2]+vf[2]+vu[2];
	vbuf[14][3] = 0.0;
	vbuf[14][4] = 1.0;
	vbuf[14][5] = 0.0;
	vbuf[15][0] = vc[0]-vr[0]+vf[0]+vu[0];
	vbuf[15][1] = vc[1]-vr[1]+vf[1]+vu[1];
	vbuf[15][2] = vc[2]-vr[2]+vf[2]+vu[2];
	vbuf[15][3] = 1.0;
	vbuf[15][4] = 1.0;
	vbuf[15][5] = 0.0;
	ibuf[18] = 12;
	ibuf[19] = 13;
	ibuf[20] = 14;
	ibuf[21] = 15;
	ibuf[22] = 14;
	ibuf[23] = 13;

	//bot
	vbuf[16][0] = vc[0]-vr[0]-vf[0]-vu[0];
	vbuf[16][1] = vc[1]-vr[1]-vf[1]-vu[1];
	vbuf[16][2] = vc[2]-vr[2]-vf[2]-vu[2];
	vbuf[16][3] = 0.0;
	vbuf[16][4] = 0.0;
	vbuf[16][5] = 0.0;
	vbuf[17][0] = vc[0]+vr[0]-vf[0]-vu[0];
	vbuf[17][1] = vc[1]+vr[1]-vf[1]-vu[1];
	vbuf[17][2] = vc[2]+vr[2]-vf[2]-vu[2];
	vbuf[17][3] = 1.0;
	vbuf[17][4] = 0.0;
	vbuf[17][5] = 0.0;
	vbuf[18][0] = vc[0]-vr[0]+vf[0]-vu[0];
	vbuf[18][1] = vc[1]-vr[1]+vf[1]-vu[1];
	vbuf[18][2] = vc[2]-vr[2]+vf[2]-vu[2];
	vbuf[18][3] = 0.0;
	vbuf[18][4] = 1.0;
	vbuf[18][5] = 0.0;
	vbuf[19][0] = vc[0]+vr[0]+vf[0]-vu[0];
	vbuf[19][1] = vc[1]+vr[1]+vf[1]-vu[1];
	vbuf[19][2] = vc[2]+vr[2]+vf[2]-vu[2];
	vbuf[19][3] = 1.0;
	vbuf[19][4] = 1.0;
	vbuf[19][5] = 0.0;
	ibuf[24] = 16;
	ibuf[25] = 17;
	ibuf[26] = 18;
	ibuf[27] = 19;
	ibuf[28] = 18;
	ibuf[29] = 17;

	//top
	vbuf[20][0] = vc[0]-vr[0]-vf[0]+vu[0];
	vbuf[20][1] = vc[1]-vr[1]-vf[1]+vu[1];
	vbuf[20][2] = vc[2]-vr[2]-vf[2]+vu[2];
	vbuf[20][3] = 0.0;
	vbuf[20][4] = 0.0;
	vbuf[20][5] = 0.0;
	vbuf[21][0] = vc[0]+vr[0]-vf[0]+vu[0];
	vbuf[21][1] = vc[1]+vr[1]-vf[1]+vu[1];
	vbuf[21][2] = vc[2]+vr[2]-vf[2]+vu[2];
	vbuf[21][3] = 1.0;
	vbuf[21][4] = 0.0;
	vbuf[21][5] = 0.0;
	vbuf[22][0] = vc[0]-vr[0]+vf[0]+vu[0];
	vbuf[22][1] = vc[1]-vr[1]+vf[1]+vu[1];
	vbuf[22][2] = vc[2]-vr[2]+vf[2]+vu[2];
	vbuf[22][3] = 0.0;
	vbuf[22][4] = 1.0;
	vbuf[22][5] = 0.0;
	vbuf[23][0] = vc[0]+vr[0]+vf[0]+vu[0];
	vbuf[23][1] = vc[1]+vr[1]+vf[1]+vu[1];
	vbuf[23][2] = vc[2]+vr[2]+vf[2]+vu[2];
	vbuf[23][3] = 1.0;
	vbuf[23][4] = 1.0;
	vbuf[23][5] = 0.0;
	ibuf[30] = 20;
	ibuf[31] = 21;
	ibuf[32] = 22;
	ibuf[33] = 23;
	ibuf[34] = 22;
	ibuf[35] = 21;
}




void gl41solid_bodypart(_obj* ctx, u32 rgb, vec3 t0, vec3 t1)
{
	float n;
	vec3 tc, tr, tf, tu;

	tc[0] = (t0[0]+t1[0])/2;
	tc[1] = (t0[1]+t1[1])/2;
	tc[2] = (t0[2]+t1[2])/2;

	tu[0] = t0[0] - tc[0];
	tu[1] = t0[1] - tc[1];
	tu[2] = t0[2] - tc[2];
	n = squareroot(tu[0]*tu[0] + tu[1]*tu[1] + tu[2]*tu[2]);

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
	n = 25.0 / squareroot(tr[0]*tr[0] + tr[1]*tr[1] + tr[2]*tr[2]);
	tr[0] *= n;
	tr[1] *= n;
	tr[2] *= n;

	tf[0] = tu[1]*tr[2] - tu[2]*tr[1];
	tf[1] = tu[2]*tr[0] - tu[0]*tr[2];
	tf[2] = tu[0]*tr[1] - tu[1]*tr[0];
	n = 25.0 / squareroot(tf[0]*tf[0] + tf[1]*tf[1] + tf[2]*tf[2]);
	tf[0] *= n;
	tf[1] *= n;
	tf[2] *= n;

	gl41solid_cylinder(ctx, rgb, tc, tr, tf, tu);
}




void carvearrorkey(_obj* ctx, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf,
	u8* buf, int t)
{
	int j;
	u32 c;
	vec3 tc;
	vec3 tr;
	vec3 tf;
	gl41opaque_rect(ctx, 0x80808080, vc,vr,vf);

	//gl41opaque_circle(ctx, 0x7f404040, vc, vr, vf);
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}

	//x-: left
	if(buf[0]&0x80)c = 0x7fff00ff;
	else c = 0x7f808080;
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(t-2)/4 -vf[j]*1/4;
	gl41opaque_circle(ctx, c, tc, tr, tf);
	tc[2] += 0.1;
	gl41ascii_center(ctx, 0xffffff, tc, tr, tf, buf[0]&0x7f);
		

	//x+: right
	if(buf[1]&0x80)c = 0x7fff00ff;
	else c = 0x7f808080;
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(t+2)/4 -vf[j]*1/4;
	gl41opaque_circle(ctx, c, tc, tr, tf);
	tc[2] += 0.1;
	gl41ascii_center(ctx, 0xffffff, tc, tr, tf, buf[1]&0x7f);

	//y-: near
	if(buf[2]&0x80)c = 0x7fff00ff;
	else c = 0x7f808080;
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*t/4 -vf[j]*3/4;
	gl41opaque_circle(ctx, c, tc, tr, tf);
	tc[2] += -0.1;
	gl41ascii_center(ctx, 0xffffff, tc, tr, tf, buf[2]&0x7f);

	//y+: far
	if(buf[3]&0x80)c = 0x7fff00ff;
	else c = 0x7f808080;
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*t/4 +vf[j]*1/4;
	gl41opaque_circle(ctx, c, tc, tr, tf);
	tc[2] += 0.1;
	gl41ascii_center(ctx, 0xffffff, tc, tr, tf, buf[3]&0x7f);

	//z-: trigger
	if(buf[4]&0x80)c = 0x7fff00ff;
	else c = 0x7f808080;
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(t-2)/4 +vf[j]*3/4;
	gl41opaque_circle(ctx, c, tc, tr, tf);
	tc[2] += 0.1;
	gl41ascii_center(ctx, 0xffffff, tc, tr, tf, buf[4]&0x7f);

	//z+: bumper
	if(buf[5]&0x80)c = 0x7fff00ff;
	else c = 0x7f808080;
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(t+2)/4 +vf[j]*3/4;
	gl41opaque_circle(ctx, c, tc, tr, tf);
	tc[2] += 0.1;
	gl41ascii_center(ctx, 0xffffff, tc, tr, tf, buf[5]&0x7f);

	//press
	if(buf[6]&0x80)c = 0x7fff00ff;
	else c = 0x7f808080;
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*t/4 -vf[j]*1/4;
	gl41opaque_circle(ctx, c, tc, tr, tf);
	tc[2] += 0.1;
	gl41ascii_center(ctx, 0xffffff, tc, tr, tf, buf[6]&0x7f);

	//select or start
	if(buf[7]&0x80)c = 0x7fff00ff;
	else c = 0x7f808080;
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*t*3/4 +vf[j]*1/4;
	gl41opaque_circle(ctx, c, tc, tr, tf);
	tc[2] += 0.1;
	gl41ascii_center(ctx, 0xffffff, tc, tr, tf, buf[7]&0x7f);
}
