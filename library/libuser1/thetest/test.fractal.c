#include "libuser.h"




static double centerx = 0.0;
static double centery = 0.0;
static double width = 1.0;
static double height = 1.0;
static u8 buffer[16];




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
	struct actor* act, struct pinid* pin)
{
	u32 c;
	int x,y;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	int w = win->stride;
	u32* buf = win->buf;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

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
	struct actor* act, struct pinid* pin)
{
}
static void fractal_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fractal_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"fractal\" style=\"width:50%%;height:100px;float:left;background-color:#9728a7;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void fractal_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fractal_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("fractal(%x,%x,%x)\n",win,act,sty);
}
static void fractal_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)fractal_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)fractal_read_tui(win, sty, act, pin);
	else if(fmt == _html_)fractal_read_html(win, sty, act, pin);
	else if(fmt == _json_)fractal_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)fractal_read_vbo(win, sty, act, pin);
	else fractal_read_pixel(win, sty, act, pin);
}
static void fractal_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
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
static void fractal_get()
{
}
static void fractal_post()
{
}
static void fractal_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fractal_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fractal_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void fractal_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void fractal_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('f', 'r', 'a', 'c', 't', 'a', 'l', 0);

	p->oncreate = (void*)fractal_create;
	p->ondelete = (void*)fractal_delete;
	p->onstart  = (void*)fractal_start;
	p->onstop   = (void*)fractal_stop;
	p->onget    = (void*)fractal_get;
	p->onpost   = (void*)fractal_post;
	p->onread   = (void*)fractal_read;
	p->onwrite  = (void*)fractal_write;
}
