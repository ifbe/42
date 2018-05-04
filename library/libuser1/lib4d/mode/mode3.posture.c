#include "actor.h"




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




void actoroutput_posture_vbo(struct arena* win)
{
	int j;
	float cx = win->target.cx;
	float cy = win->target.cy;
	float cz = win->target.cz;
	float rx = win->target.rx;
	float ry = win->target.ry;
	float rz = win->target.rz;
	float fx = win->target.fx;
	float fy = win->target.fy;
	float fz = win->target.fz;
	float ux = win->target.ux;
	float uy = win->target.uy;
	float uz = win->target.uz;

	j = win->menudata;
	if(j < 0)goto skip;
	if(j >= 64)goto skip;
	if(0 == actor[j].type)goto skip;
	actor[j].onread(win, &win->target, &actor[j], 0);

skip:
	carveline_prism4(
		win, 0x00ff00,
		cx+ux, cy+uy, cz+uz,
		rx, ry, rz,
		fx, fy, fz,
		ux, uy, uz
	);
	carvestring_center(
		win, 0x00ff00,
		cx+ux, cy+uy, cz+uz,
		rx/2, ry/2, rz/2,
		fx/2, fy/2, fz/2,
		(u8*)&actor[j].name, 8
	);
}
void actoroutput_posture_pixel(struct arena* win)
{
	int j;
	int cx = win->target.cx;
	int cy = win->target.cy;
	int cz = win->target.cz;
	int rx = win->target.rx;
	int ry = win->target.ry;
	int rz = win->target.rz;
	int fx = win->target.fx;
	int fy = win->target.fy;
	int fz = win->target.fz;
	int ux = win->target.ux;
	int uy = win->target.uy;
	int uz = win->target.uz;

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
int actoroutput_posture(struct arena* win)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct compo* com;

	//origin world
	rel = win->irel;
	while(1)
	{
		if(rel == 0)break;

		if(rel->srctype == _act_)
		{
			act = (void*)(rel->srcchip);
			sty = (void*)(rel->dstfoot);
			com = (void*)(rel->srcfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, com);
			//say("%x\n", rel);

			act->onread(win, sty, act, com);
		}

		rel = samedstnextsrc(rel);
	}

	//chosen actor
	if(_vbo_ == win->fmt)actoroutput_posture_vbo(win);
	else actoroutput_posture_pixel(win);
	return 0;
}