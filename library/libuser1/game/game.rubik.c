#include "actor.h"
#define level 3
void rubikscube_generate(void*, int);
void rubikscube_solve(void*, int);




//left, right, near, far, bottom, upper
u32 rubikcolor[6] = {0x00ff00,0x0000ff,0xff0000,0xfa8010,0xffff00,0xffffff};
static u8 buffer[6][4][4];




static void rubikscube_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 bg;
	int x,y;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			bg = rubikcolor[0];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);

			//right
			bg = rubikcolor[1];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+4*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+4*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			bg = rubikcolor[2];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);

			//far
			bg = rubikcolor[3];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+6*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+6*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			bg = rubikcolor[4];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+4*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+4*level)*hh/4/level
			);

			//upper
			bg = rubikcolor[5];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2)*hh/4/level
			);
		}
	}
}
static void rubikscube_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			carvesolid_rect(
				win, 0xff00,
				cx-ww, cy-hh+(2*x+1)*hh/level, -dd+(2*y+1)*dd/level,
				0.0, -hh/(level+0.5), 0.0,
				0.0, 0.0, dd/(level+0.5)
			);

			//right
			carvesolid_rect(
				win, 0xff,
				cx+ww, cy-hh+(2*x+1)*hh/level, -dd+(2*y+1)*dd/level,
				0.0, hh/(level+0.5), 0.0,
				0.0, 0.0, dd/(level+0.5)
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			carvesolid_rect(
				win, 0xff0000,
				cx-ww+(2*x+1)*ww/level, cy-hh, -dd+(2*y+1)*dd/level,
				ww/(level+0.5), 0.0, 0.0,
				0.0, 0.0, dd/(level+0.5)
			);

			//far
			carvesolid_rect(
				win, 0xfa8010,
				cx-ww+(2*x+1)*ww/level, cy+hh, -dd+(2*y+1)*dd/level,
				-ww/(level+0.5), 0.0, 0.0,
				0.0, 0.0, dd/(level+0.5)
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			carvesolid_rect(
				win, 0xffff00,
				cx-ww+(2*x+1)*ww/level, cy-hh+(2*y+1)*hh/level, -dd,
				ww/(level+0.5), 0.0, 0.0,
				0.0, -hh/(level+0.5), 0.0
			);

			//upper
			carvesolid_rect(
				win, 0xffffff,
				cx-ww+(2*x+1)*ww/level, cy-hh+(2*y+1)*hh/level, dd,
				ww/(level+0.5), 0.0, 0.0,
				0.0, hh/(level+0.5), 0.0
			);
		}
	}
}
static void rubikscube_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rubikscube_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"rubik\" style=\"width:100%%;height:100px;background-color:#404040;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");
	win->len = len;
}
static void rubikscube_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rubikscube_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("rubik(%x,%x,%x)\n",win,act,sty);
}
static void rubikscube_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)rubikscube_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)rubikscube_read_tui(win, sty, act, pin);
	else if(fmt == _html_)rubikscube_read_html(win, sty, act, pin);
	else if(fmt == _json_)rubikscube_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)rubikscube_read_vbo(win, sty, act, pin);
	else rubikscube_read_pixel(win, sty, act, pin);
}
static void rubikscube_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	if(ev->what == _kbd_)
	{
	}
	else if(ev->what == _char_)
	{
	}
}
static void rubikscube_list()
{
}
static void rubikscube_choose()
{
}
static void rubikscube_stop(struct actor* act, struct pinid* pin)
{
}
static void rubikscube_start(struct actor* act, struct pinid* pin)
{
	rubikscube_generate(buffer, level);
}
static void rubikscube_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void rubikscube_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(6*4*4);
}




void rubikscube_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('r', 'u', 'b', 'i', 'k', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)rubikscube_create;
	p->ondelete = (void*)rubikscube_delete;
	p->onstart  = (void*)rubikscube_start;
	p->onstop   = (void*)rubikscube_stop;
	p->onlist   = (void*)rubikscube_list;
	p->onchoose = (void*)rubikscube_choose;
	p->onread   = (void*)rubikscube_read;
	p->onwrite  = (void*)rubikscube_write;
}
