#include "actor.h"
#define width 32
#define height 32
int snake_generate(void* buf, int w, int h);
int snake_left(void* buf, int w, int h);
int snake_right(void* buf, int w, int h);
int snake_up(void* buf, int w, int h);
int snake_down(void* buf, int w, int h);




struct snake
{
	u8 x;
	u8 y;
	u16 next;
};
static struct snake buf[width*height];




void snake_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	int t1, t2, t3, t4;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	//body
	j = 1;
	while(1)
	{
		//say("[%x]=%x,%x,%x\n", j, buf[j].x, buf[j].y, buf[j].next);
		t1 = cx-ww + buf[j].x * 2 * ww / width;
		t2 = cy-hh + buf[j].y * 2 * hh / height;
		t3 = cx-ww + (buf[j].x+1) * 2 * ww / width;
		t4 = cy-hh + (buf[j].y+1) * 2 * hh / height;
		drawsolid_rect(win, 0xffffff, t1+1, t2+1, t3-1, t4-1);

		if(buf[j].next <= 1)break;
		j = buf[j].next;
	}

	//food
	t1 = cx-ww + buf[0].x * 2 * ww / width;
	t2 = cy-hh + buf[0].y * 2 * hh / height;
	t3 = cx-ww + (buf[0].x+1) * 2 * ww / width;
	t4 = cy-hh + (buf[0].y+1) * 2 * hh / height;
	drawsolid_rect(win, 0x00ff00, t1+1, t2+1, t3-1, t4-1);
}
void snake_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float ww = sty->rx;
	float hh = sty->fy;
	float dd = sty->uz;
	carveline_rect(
		win, 0x404040,
		cx, cy, cz,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
}




static int htmlcubie(char* p, u32 color, int x, int y)
{
	return mysnprintf(
		p, 0x1000,
		"<div class=\"rect\" style=\""
		"left:%.2f%;"
		"top:%.2f%;"
		"background:#%06x;"
		"\"></div>",
		x*3.1, y*3.1, color
	);
}
void snake_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j = 0;
	char* p = (char*)(win->buf);

	p += mysnprintf(
		p, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"width:3.1%;"
		"height:3.1%;"
		"}"
		"</style>"
	);
 
	htmlcubie(p, 0xff00, buf[0].x, buf[0].y);
}
void snake_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j,t;
	int stride = win->stride;
	char* p = (char*)(win->buf);

	t = buf[0].x + buf[0].y*stride;
	p[t<<2] = '@';
}
void snake_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("snake(%x,%x,%x)\n",win,act,sty);
}
void snake_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)snake_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)snake_read_tui(win, sty, act, pin);
	else if(fmt == _html_)snake_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)snake_read_vbo(win, sty, act, pin);
	else snake_read_pixel(win, sty, act, pin);
}




void snake_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(_char_ == type)
	{
		if(key == 'w')snake_up(buf, width, height);
		else if(key == 's')snake_down(buf, width, height);
		else if(key == 'a')snake_left(buf, width, height);
		else if(key == 'd')snake_right(buf, width, height);
	}
	if(_kbd_ == type)
	{
		if(key == 0x48)snake_up(buf, width, height);
		else if(key == 0x4b)snake_left(buf, width, height);
		else if(key == 0x4d)snake_right(buf, width, height);
		else if(key == 0x50)snake_down(buf, width, height);
	}
}
static void snake_list()
{
}
static void snake_choose()
{
}
static void snake_stop(struct actor* act, struct pinid* pin)
{
}
static void snake_start(struct actor* act, struct pinid* pin)
{
	snake_generate(buf, width, height);
}
static void snake_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void snake_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buf;
	if(_copy_ == act->type)act->buf = memorycreate(width*height*4);
}




void snake_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('s', 'n', 'a', 'k', 'e', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)snake_create;
	p->ondelete = (void*)snake_delete;
	p->onstart  = (void*)snake_start;
	p->onstop   = (void*)snake_stop;
	p->onlist   = (void*)snake_list;
	p->onchoose = (void*)snake_choose;
	p->onread   = (void*)snake_read;
	p->onwrite  = (void*)snake_write;
}
