#include "actor.h"
void yuyv2rgba(u8* src, u8* dst,
	int w1, int h1, int sx1, int sy1, int dx1, int dy1,
	int w2, int h2, int sx2, int sy2, int dx2, int dy2
);
void startvision();
void stopvision();



struct pictureobject
{
	void* buf;
	int len;
	int width;
	int height;
};
static struct pictureobject* vision = 0;




void camera_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	if(vision == 0)return;
	u8* screen;
	u8* buf;
	int j;
	int w = win->w;
	int h = win->h;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	screen = (u8*)(win->buf);
	buf = vision->buf;
	yuyv2rgba(buf, screen, 
		640, 480, 0, 0, 0, 0,
		w, h, cx-ww, cy-hh, cx+ww, cy+hh
	);
}
void camera_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
void camera_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
void camera_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
void camera_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	if(vision == 0)
	{
		say("camera(%x,%x,%x)\n",win,act,sty);
	}
	else
	{
		say("%x,%x,%x,%x\n",
			vision->buf, vision->len,
			vision->width, vision->height
		);
	}
}
static void camera_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)camera_read_cli(win, sty, act, com);
	else if(fmt == _tui_)camera_read_tui(win, sty, act, com);
	else if(fmt == _html_)camera_read_html(win, sty, act, com);
	else if(fmt == _vbo_)camera_read_vbo(win, sty, act, com);
	else camera_read_pixel(win, sty, act, com);
}
static void camera_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
	int j;
	u8* buf;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 'v')
	{
		vision = (void*)key;
	}
}
static void camera_list()
{
}
static void camera_into()
{
}
static void camera_stop()
{
	stopvision();
}
static void camera_start()
{
	startvision();
}
static void camera_delete()
{
}
static void camera_create()
{
}




void camera_register(struct actor* p)
{
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('c', 'a', 'm', 'e', 'r', 'a', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)camera_create;
	p->ondelete = (void*)camera_delete;
	p->onstart  = (void*)camera_start;
	p->onstop   = (void*)camera_stop;
	p->onlist   = (void*)camera_list;
	p->onchoose = (void*)camera_into;
	p->onread   = (void*)camera_read;
	p->onwrite  = (void*)camera_write;
}