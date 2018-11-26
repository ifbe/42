#include "libuser.h"




void carveborder2d(struct arena* win, struct style* sty, void* name)
{
	vec3 tc,tr,tf;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	int w = win->width;
	int h = win->height;

	tc[0] = vc[0] / w;
	tc[1] = vc[1] / h;
	tc[2] = -0.5;
	tr[0] = vr[0] / w;
	tr[1] = vr[1] / h;
	tr[2] = 0.0;
	tf[0] = vf[0] / w;
	tf[1] = vf[1] / h;
	tf[2] = 0.0;
	carveline2d_rect(win, 0xff00ff, tc, tr, tf);

	tc[0] = vc[0] / w;
	tc[1] = (vc[1]+vf[1]+16.0) / h;
	tc[2] = -0.5;
	tf[0] = 0.0;
	tf[1] = 16.0 / h;
	tf[2] = 0.0;
	carvesolid2d_rect(win, 0xff00ff, tc, tr, tf);

	tc[2] = -0.6;
	tr[0] = 64.0 / w;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, name, 0);
}