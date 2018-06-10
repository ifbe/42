#include "libuser.h"




static struct object* obj = 0;
static struct element* ele = 0;
static struct arena* arena = 0;
static struct actor* actor = 0;




void posture_create(void* addr)
{
	obj = addr - 0x400000;
	ele = addr - 0x300000;
	arena = addr + 0x000000;
	actor = addr + 0x100000;
}




void actoroutput_posture_vbo(struct arena* win, struct style* sty)
{
	int j;
	vec3 tc, tr, tf;
	float* vc = win->target.vc;
	float* vr = win->target.vr;
	float* vf = win->target.vf;
	float* vu = win->target.vu;
	carveaxis(win);

	j = win->menudata;
	if(j < 0)goto skip;
	if(j >= 64)goto skip;
	if(0 == actor[j].type)goto skip;
	actor[j].onread(win, &win->target, &actor[j], 0);

skip:
	tc[0] = vc[0] + vu[0];
	tc[1] = vc[1] + vu[1];
	tc[2] = vc[2] + vu[2];
	carveline_prism4(win, 0x00ff00, tc, vr, vf, vu);
	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	carvestring_center(win, 0x00ff00, tc, tr, tf,
		(u8*)&actor[j].name, 8);
}
void actoroutput_posture_pixel(struct arena* win, struct style* sty)
{
	int j;
	int cx = win->target.vc[0];
	int cy = win->target.vc[1];
	int cz = win->target.vc[2];
	int rx = win->target.vr[0];
	int ry = win->target.vr[1];
	int rz = win->target.vr[2];
	int fx = win->target.vf[0];
	int fy = win->target.vf[1];
	int fz = win->target.vf[2];
	int ux = win->target.vu[0];
	int uy = win->target.vu[1];
	int uz = win->target.vu[2];

	j = win->menudata;
	if(j < 0)goto skip;
	if(j >= 64)goto skip;
	if(0 == actor[j].type)goto skip;
	actor[j].onread(win, &win->target, &actor[j], 0);

skip:
	drawline(win, 0x00ff00, cx-rx-fx, cy-ry-fy, cx-rx+fx, cy-ry+fy);
	drawline(win, 0x00ff00, cx+rx-fx, cy+ry-fy, cx+rx+fx, cy+ry+fy);
	drawline(win, 0x00ff00, cx-rx-fx, cy-ry-fy, cx+rx-fx, cy+ry-fy);
	drawline(win, 0x00ff00, cx-rx+fx, cy-ry+fy, cx+rx+fx, cy+ry+fy);
	drawstring_fit(win, 0x00ff00, cx-16, cy-32, cx+16, cy+32, (u8*)&actor[j].name, 8);
}
int actoroutput_posture(struct arena* win, struct style* st)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	//origin world
	rel = win->irel;
	while(1)
	{
		if(rel == 0)break;

		if(rel->srctype == _act_)
		{
			act = (void*)(rel->srcchip);
			sty = (void*)(rel->dstfoot);
			pin = (void*)(rel->srcfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, pin);
			//say("%x\n", rel);

			act->onread(win, sty, act, pin);
		}

		rel = samedstnextsrc(rel);
	}

	//chosen actor
	if(_vbo_ == win->fmt)actoroutput_posture_vbo(win, sty);
	else actoroutput_posture_pixel(win, sty);
	return 0;
}