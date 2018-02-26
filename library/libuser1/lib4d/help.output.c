#include "actor.h"
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)
void background(void*);
void foreground(void*);
void arenaread(void*, void*);
void arenawrite(void*, void*);
void* samepinprevchip(void*);
void* samepinnextchip(void*);
void* samechipprevpin(void*);
void* samechipnextpin(void*);




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct compo* compo = 0;
void helpout_create(void* addr)
{
	arena = addr + 0x000000;
	actor = addr + 0x100000;
	style = addr + 0x000000;
	compo = addr + 0x100000;
}




void select_2d(struct arena* win, struct style* sty)
{
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	drawline_rect(win, 0xff00ff, cx-ww, cy-hh, cx+ww, cy+hh);
}
void select_3d(struct arena* win, struct style* sty)
{
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	carveline_prism4(
		win, 0xff0000,
		cx, cy, dd/2,
		ww, 0.0, 0.0,
		0.0, hh, 0.0,
		0.0, 0.0, dd/2
	);
}




int actoroutput(struct arena* win)
{
	int j;
	struct relation* rel;

	struct actor* act;
	struct style* sty;
	struct compo* com;

	//cli silent
	if(win->fmt == _cli_)
	{
		if(win->flag0 == 12)return 0;
	}

	//bg
	background(win);

	//content
	rel = win->irel;
	while(1)
	{
		if(rel == 0)break;

		if(rel->selftype == _act_)
		{
			act = (void*)(rel->selfchip);
			sty = (void*)(rel->destfoot);
			com = (void*)(rel->selffoot);
			//say("%x,%x,%x,%x\n", win, act, sty, com);
			//say("%x\n", rel);

			act->onread(win, sty, act, com);
			if(win->flag0 == 12)
			{
				if(win->fmt == _vbo_)select_3d(win, sty);
				else select_2d(win, sty);
			}
		}

		rel = samepinnextchip(rel);
	}

	//fg
	foreground(win);

theend:
	arenawrite(win, &arena[0]);
	return 0;
}