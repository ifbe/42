#include "libuser.h"
void invmvp(vec3 v, struct arena* win);




void select_3d(struct arena* win, u32 rgb, struct style* sty, u32 flag)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline_prism4(win, rgb, vc, vr, vf, vu);
}
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

	//ray
	va[0] = (w->input[10].x1)*2/x - 1.0;
	va[1] = 1.0 - (w->input[10].y1)*2/y;
	va[2] = -1.0;
	vb[0] = (w->input[10].x1)*2/x - 1.0;
	vb[1] = 1.0 - (w->input[10].y1)*2/y;
	vb[2] = 0.5;

	invmvp(va0, w);
	invmvp(vb0, w);
	invmvp(vc0, w);
	invmvp(vd0, w);
	invmvp(va1, w);
	invmvp(vb1, w);
	invmvp(vc1, w);
	invmvp(vd1, w);
	invmvp(va, w);
	invmvp(vb, w);

	va1[0] = va0[0] + (va1[0]-va0[0])*10;
	va1[1] = va0[1] + (va1[1]-va0[1])*10;
	va1[2] = va0[2] + (va1[2]-va0[2])*10;
	vb1[0] = vb0[0] + (vb1[0]-vb0[0])*10;
	vb1[1] = vb0[1] + (vb1[1]-vb0[1])*10;
	vb1[2] = vb0[2] + (vb1[2]-vb0[2])*10;
	vc1[0] = vc0[0] + (vc1[0]-vc0[0])*10;
	vc1[1] = vc0[1] + (vc1[1]-vc0[1])*10;
	vc1[2] = vc0[2] + (vc1[2]-vc0[2])*10;
	vd1[0] = vd0[0] + (vd1[0]-vd0[0])*10;
	vd1[1] = vd0[1] + (vd1[1]-vd0[1])*10;
	vd1[2] = vd0[2] + (vd1[2]-vd0[2])*10;
	vv[0] = va[0] - va[2]*(vb[0]-va[0])/(vb[2]-va[2]);
	vv[1] = va[1] - va[2]*(vb[1]-va[1])/(vb[2]-va[2]);
	vv[2] = 0.0;

	//rect near
	carveline(win, 0xffffff, va0, vb0);
	carveline(win, 0xffffff, va0, vc0);
	carveline(win, 0xffffff, vb0, vd0);
	carveline(win, 0xffffff, vc0, vd0);

	//rect far
	carveline(win, 0xffffff, va1, vb1);
	carveline(win, 0xffffff, va1, vc1);
	carveline(win, 0xffffff, vb1, vd1);
	carveline(win, 0xffffff, vc1, vd1);

	//frame
	carveline(win, 0xffffff, va0, va1);
	carveline(win, 0xffffff, vb0, vb1);
	carveline(win, 0xffffff, vc0, vc1);
	carveline(win, 0xffffff, vd0, vd1);

	//ray
	carveline(win, 0xffffff, va, vv);
}