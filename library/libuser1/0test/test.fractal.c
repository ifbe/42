#include "actor.h"
static double centerx = 0.0;
static double centery = 0.0;
static double width = 1.0;
static double height = 1.0;




int fractal_check(double x, double y)
{
	int j;
	double tx,ty;
	double fx = 0.0;
	double fy = 0.0;
	for(j=0;j<255;j++)
	{
		tx = fx*fx - fy*fy + x;
		ty = 2*fx*fy + y;
		if(tx*tx + ty*ty > 4)break;
		fx = tx;
		fy = ty;
	}
	return j;
}
static void fractal_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u32 c;
	int x,y;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	int w = win->w;
	u32* buf = win->buf;
	drawline_rect( win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(y=1-hh;y<hh;y++)
	{
		for(x=1-ww;x<ww;x++)
		{
			c = fractal_check(
				centerx + 2.0*x*width/ww,
				centery + 2.0*y*height/hh
			);
			buf[w*(cy+y) + cx+x] = 0xff000000 | (c*0x010101);
		}
	}
}
static void fractal_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	carvesolid_rect(
		win, 0xffffff,
		cx, cy, cz,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
}
static void fractal_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void fractal_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void fractal_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	say("fractal(%x,%x,%x)\n",win,act,sty);
}
static void fractal_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)fractal_read_cli(win, sty, act, com);
	else if(fmt == _tui_)fractal_read_tui(win, sty, act, com);
	else if(fmt == _html_)fractal_read_html(win, sty, act, com);
	else if(fmt == _vbo_)fractal_read_vbo(win, sty, act, com);
	else fractal_read_pixel(win, sty, act, com);
}
static void fractal_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
	//say("%llx,%llx\n",ev->why, ev->what);
	u64 why = ev->why;
	u64 what = ev->what;
	if(0x2b70 == what)
	{
		why >>= 48;
		if('f' == why)
		{
			width *= 0.9;
			height *= 0.9;
		}
		else if('b' == why)
		{
			width *= 1.1;
			height *= 1.1;
		}
	}
	if(_kbd_ == what)
	{
		if(why == 0x48)centery -= height*0.1;
		else if(why == 0x4b)centerx -= width*0.1;
		else if(why == 0x4d)centerx += width*0.1;
		else if(why == 0x50)centery += height*0.1;
	}
}
static void fractal_list()
{
}
static void fractal_change()
{
}
static void fractal_stop()
{
}
static void fractal_start()
{
}
static void fractal_delete()
{
}
static void fractal_create()
{
}




void fractal_register(struct actor* p)
{
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('f', 'r', 'a', 'c', 't', 'a', 'l', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)fractal_create;
	p->ondelete = (void*)fractal_delete;
	p->onstart  = (void*)fractal_start;
	p->onstop   = (void*)fractal_stop;
	p->onlist   = (void*)fractal_list;
	p->onchoose = (void*)fractal_change;
	p->onread   = (void*)fractal_read;
	p->onwrite  = (void*)fractal_write;
}