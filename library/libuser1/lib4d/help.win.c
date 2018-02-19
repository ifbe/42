#include "actor.h"
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)
void* arenastart(u64, u64);
int arenastop(void*);
void actorstart(void*, void*);
int actorstop(void*);
void actorchoose(void*);
void* relation_read(u64);
void relation_write(void* uchip, void* ufoot, u64 utype, void* bchip, u64 bfoot, u64 btype);




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static int stlen = 0;




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




void act_add()
{
}
void act_del()
{
}
void act_at(struct arena* win, struct actor* act)
{
	int w = (win->w) / 2;
	int h = (win->h) / 2;
	int d = (w+h) / 2;
	struct style* st = (void*)style + stlen;
	stlen += sizeof(struct style);

	st->i_cx = w;
	st->i_cy = h;
	st->i_cz = 0.0;
	st->i_rx = w / 2;
	st->i_ry = 0.0;
	st->i_rz = 0.0;
	st->i_fx = 0.0;
	st->i_fy = h / 2;
	st->i_fz = 0.0;
	st->i_ux = 0.0;
	st->i_uy = 0.0;
	st->i_uz = d / 2;

	relation_write(win, st, _win_, act, 0, _act_);
}




void win_add(u64 why, u64 where)
{
	void* ret = arenastart(why, where);
	if(ret == 0)
	{
		say("error@w+\n");
		return;
	}
}
void win_del(u64 why, u64 where)
{
	void* ret = (void*)where;
	arenastop(ret);
}
void win_at(u64 why, u64 where)
{
}




void wmgr_create(void* addr)
{
	arena = addr + 0;
	actor = addr + 0x100000;
	style = addr + 0x200000;
}
