#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 18
#define stlv 0x82
void invmvp(vec3 v, struct arena* win);




void carveaxis(struct arena* win)
{
	vec3 va, vb;
	va[0] = -10000.0;
	va[1] = 0.0;
	va[2] = 0.0;
	vb[0] = 10000.0;
	vb[1] = 0.0;
	vb[2] = 0.0;
	carveline(win, 0xff0000, va, vb);
	va[0] = 0.0;
	va[1] = -10000.0;
	va[2] = 0.0;
	vb[0] = 0.0;
	vb[1] = 10000.0;
	vb[2] = 0.0;
	carveline(win, 0x00ff00, va, vb);
	va[0] = 0.0;
	va[1] = 0.0;
	va[2] = -10000.0;
	vb[0] = 0.0;
	vb[1] = 0.0;
	vb[2] = 10000.0;
	carveline(win, 0x0000ff, va, vb);
}
void carvecamera(struct arena* win, struct arena* w)
{
	vec3 va0,vb0,vc0,vd0;
	vec3 va1,vb1,vc1,vd1;
	vec3 va,vb,vv;
	float x = (float)(w->width);
	float y = (float)(w->height);

	//left,bot,near
	va0[0] = -1.0;
	va0[1] = -1.0;
	va0[2] = -1.0;

	//right,bot,near
	vb0[0] = 1.0;
	vb0[1] = -1.0;
	vb0[2] = -1.0;

	//left,top,near
	vc0[0] = -1.0;
	vc0[1] = 1.0;
	vc0[2] = -1.0;

	//right,top,near
	vd0[0] = 1.0;
	vd0[1] = 1.0;
	vd0[2] = -1.0;

	//left,bot,far
	va1[0] = -1.0;
	va1[1] = -1.0;
	va1[2] = 0.5;

	//right,bot,far
	vb1[0] = 1.0;
	vb1[1] = -1.0;
	vb1[2] = 0.5;

	//left,top,far
	vc1[0] = -1.0;
	vc1[1] = 1.0;
	vc1[2] = 0.5;

	//right,top,far
	vd1[0] = 1.0;
	vd1[1] = 1.0;
	vd1[2] = 0.5;

	invmvp(va0, w);
	invmvp(vb0, w);
	invmvp(vc0, w);
	invmvp(vd0, w);
	invmvp(va1, w);
	invmvp(vb1, w);
	invmvp(vc1, w);
	invmvp(vd1, w);

	va1[0] = va1[0]*100 - va0[0]*99;
	va1[1] = va1[1]*100 - va0[1]*99;
	va1[2] = va1[2]*100 - va0[2]*99;
	vb1[0] = vb1[0]*100 - vb0[0]*99;
	vb1[1] = vb1[1]*100 - vb0[1]*99;
	vb1[2] = vb1[2]*100 - vb0[2]*99;
	vc1[0] = vc1[0]*100 - vc0[0]*99;
	vc1[1] = vc1[1]*100 - vc0[1]*99;
	vc1[2] = vc1[2]*100 - vc0[2]*99;
	vd1[0] = vd1[0]*100 - vd0[0]*99;
	vd1[1] = vd1[1]*100 - vd0[1]*99;
	vd1[2] = vd1[2]*100 - vd0[2]*99;

	carveline(win, 0xffffff, va0, vb0);
	carveline(win, 0xffffff, va0, vc0);
	carveline(win, 0xffffff, vb0, vd0);
	carveline(win, 0xffffff, vc0, vd0);

	carveline(win, 0xffffff, va1, vb1);
	carveline(win, 0xffffff, va1, vc1);
	carveline(win, 0xffffff, vb1, vd1);
	carveline(win, 0xffffff, vc1, vd1);

	carveline(win, 0xffffff, va0, va1);
	carveline(win, 0xffffff, vb0, vb1);
	carveline(win, 0xffffff, vc0, vc1);
	carveline(win, 0xffffff, vd0, vd1);




	va[0] = (w->input[10].x1)*2/x - 1.0;
	va[1] = 1.0 - (w->input[10].y1)*2/y;
	va[2] = -1.0;
	vb[0] = (w->input[10].x1)*2/x - 1.0;
	vb[1] = 1.0 - (w->input[10].y1)*2/y;
	vb[2] = 0.5;
	invmvp(va, w);
	invmvp(vb, w);
	//say("va=%f,%f,%f\n",va[0],va[1],va[2]);
	//say("vb=%f,%f,%f\n",vb[0],vb[1],vb[2]);

	vv[0] = va[0] - va[2]*(vb[0]-va[0])/(vb[2]-va[2]);
	vv[1] = va[1] - va[2]*(vb[1]-va[1])/(vb[2]-va[2]);
	vv[2] = 0.0;
	//say("vv=%f,%f,%f\n",vv[0],vv[1],vv[2]);
	carveline(win, 0xffffff, va, vv);
}
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