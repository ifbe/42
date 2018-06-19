#include "libuser.h"
void actorcreatefromfile(struct actor* act, char* name);




static void picture_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 tmp;
	u32* dst;
	u32* src;
	int x,y,xmax,ymax,stride;
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
	if(0 == act->buf)return;

	xmax = act->width;
	if(xmax >= ww*2)xmax = ww*2;
	ymax = act->height;
	if(ymax >= hh*2)ymax = hh*2;
	stride = win->stride;
	for(y=0;y<ymax;y++)
	{
		dst = (win->buf) + (cy-hh+y)*stride*4 + (cx-ww)*4;
		src = (act->buf) + 4*y*(act->width);
		//say("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->fmt)&0xff))
		{
			for(x=0;x<xmax;x++)dst[x] = src[x];
		}
		else
		{
			for(x=0;x<xmax;x++)
			{
				tmp = src[x];
				dst[x] = 0xff000000 | (tmp&0xff00) | ((tmp>>16)&0xff) | ((tmp&0xff)<<16);
			}
		}
	}
}
static void picture_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vr;
}
static void picture_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void picture_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"picture\" style=\"width:50%%;height:100px;float:left;background-color:#3368a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void picture_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void picture_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("picture(%x,%x,%x)\n",win,act,sty);
}
static void picture_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)picture_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)picture_read_tui(win, sty, act, pin);
	else if(fmt == _html_)picture_read_html(win, sty, act, pin);
	else if(fmt == _json_)picture_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)picture_read_vbo(win, sty, act, pin);
	else picture_read_pixel(win, sty, act, pin);
}




static void picture_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}




static void picture_list()
{
}
static void picture_change()
{
}
static void picture_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void picture_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void picture_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void picture_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "wall/wall.jpg");
}




void picture_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'i', 'c', 't', 'u', 'r', 'e', 0);

	p->oncreate = (void*)picture_create;
	p->ondelete = (void*)picture_delete;
	p->onstart  = (void*)picture_start;
	p->onstop   = (void*)picture_stop;
	p->onlist   = (void*)picture_list;
	p->onchoose = (void*)picture_change;
	p->onread   = (void*)picture_read;
	p->onwrite  = (void*)picture_write;
}
