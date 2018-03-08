#include "actor.h"
void yuyv2rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3
);
void videostart();
void videostop();
void* videoread(int);



struct pictureobject
{
	u64 buf;
	u64 len;
	u64 width;
	u64 height;
};
static int cur = -1;




void camera_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	int w = win->w;
	int h = win->h;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	u8* src;
	u8* dst;
	struct pictureobject* img;

	img = videoread(cur);
	if(0 == img)return;

	src = (u8*)(img->buf);
	if(0 == src)return;

	dst = (u8*)(win->buf);
	if(0 == dst)return;

	yuyv2rgba(
		  src, 0, 640, 480,     0,     0,     0,     0,
		  dst, 0,   w,   h, cx-ww, cy-hh, cx+ww, cy+hh
	);
}
void camera_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void camera_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void camera_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void camera_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct pictureobject* img;
	say("camera(%x,%x,%x)\n",win,act,sty);

	img = videoread(cur);
	if(0 == img)return;

	say(
		"%llx,%llx,%llx,%llx\n",
		img->buf, img->len, img->width, img->height
	);
}
static void camera_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)camera_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)camera_read_tui(win, sty, act, pin);
	else if(fmt == _html_)camera_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)camera_read_vbo(win, sty, act, pin);
	else camera_read_pixel(win, sty, act, pin);
}
static void camera_write(
	struct actor* act, struct pinid* pin,
	struct event* ev)
{
	int j;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 'v')cur = key;
}
static void camera_list()
{
}
static void camera_into()
{
}
static void camera_stop(struct actor* act, struct pinid* pin)
{
	videostop();
}
static void camera_start(struct actor* act, struct pinid* pin)
{
	videostart();
}
static void camera_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)stopmemory(act->buf);
}
static void camera_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = startmemory(128);
}




void camera_register(struct actor* p)
{
	p->type = _orig_;
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
