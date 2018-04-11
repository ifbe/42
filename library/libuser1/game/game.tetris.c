#include "actor.h"
#define WIDTH 32
#define HEIGHT 32
int tetris_generate(void* buf, int w, int h);
int tetris_left(void* buf, int w, int h);
int tetris_right(void* buf, int w, int h);
int tetris_up(void* buf, int w, int h);
int tetris_down(void* buf, int w, int h);




typedef struct stucture
{
	int x;
	int y;
	int type;
	int direction;
	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;
	int x4;
	int y4;
}structure;
static structure that;
static u8 buf[WIDTH*HEIGHT];




static void tetris_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 c;
	int x,y;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			//say("%d ",buf[y*WIDTH+x]);
			if(0 == buf[y*WIDTH+x])continue;
			else if(1 == buf[y*WIDTH+x])c = 0xffffff;
			else c = 0x00ff00;

			drawsolid_rect(win, c,
				cx-ww+1 + ((x+0)*2*ww)/WIDTH,
				cy-hh+1 + ((y+0)*2*hh)/HEIGHT,
				cx-ww-1 + ((x+1)*2*ww)/WIDTH,
				cy-hh-1 + ((y+1)*2*hh)/HEIGHT);
		}
		//say("\n");
	}
	//say("\n");
/*
	//print cubies
	cubie(win, 1,
		cx+(that.x1-16)*ww, cy+(that.y1-20)*hh,
		cx+(that.x1-15)*ww, cy+(that.y1-19)*hh);
	cubie(win, 1,
		cx+(that.x2-16)*ww, cy+(that.y2-20)*hh,
		cx+(that.x2-15)*ww, cy+(that.y2-19)*hh);
	cubie(win, 1,
		cx+(that.x3-16)*ww, cy+(that.y3-20)*hh,
		cx+(that.x3-15)*ww, cy+(that.y3-19)*hh);
	cubie(win, 1,
		cx+(that.x4-16)*ww, cy+(that.y4-20)*hh,
		cx+(that.x4-15)*ww, cy+(that.y4-19)*hh);
*/
}
static void tetris_read_vbo(
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
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			carvesolid_prism4(
				win, 0xffffff,
				(cx-ww)+(2*x+1)*ww/WIDTH, (cy-ww)+(2*y+1)*hh/HEIGHT, ww/WIDTH,
				ww/(WIDTH+1), 0.0, 0.0,
				0.0, hh/(HEIGHT+1), 0.0,
				0.0, 0.0, ww/WIDTH
			);
		}
	}
}
static void tetris_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tetris_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"tetris\" style=\"width:50%%;height:100px;float:left;background-color:#111111;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");
	win->len = len;
}
static void tetris_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	int w = win->stride;
	int h = win->height;
	char* p = (char*)(win->buf);

	for(x=0;x<w*h*4;x++)p[x]=0;
	if(h>=HEIGHT)
	{
		for(y=0;y<HEIGHT;y++)
		{
			for(x=0;x<WIDTH;x++)
			{
				if(buf[y*WIDTH+x])
				{
					p[(y*w+x)<<2]='#';
				}
			}
		}
		p[(that.x1 + that.y1*w)<<2]='#';
		p[(that.x2 + that.y2*w)<<2]='#';
		p[(that.x3 + that.y3*w)<<2]='#';
		p[(that.x4 + that.y4*w)<<2]='#';
	}
	else
	{
		for(y=0;y<h;y++)
		{
			for(x=0;x<WIDTH;x++)
			{
				if(buf[WIDTH*(y+HEIGHT-h) + x])
				{
					p[(y*w+x)<<2]='#';
				}
			}
		}
		p[(that.x1 + (that.y1-HEIGHT+h)*w)<<2]='#';
		p[(that.x2 + (that.y2-HEIGHT+h)*w)<<2]='#';
		p[(that.x3 + (that.y3-HEIGHT+h)*w)<<2]='#';
		p[(that.x4 + (that.y4-HEIGHT+h)*w)<<2]='#';
	}
}
static void tetris_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tetris_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)tetris_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)tetris_read_tui(win, sty, act, pin);
	else if(fmt == _html_)tetris_read_html(win, sty, act, pin);
	else if(fmt == _json_)tetris_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)tetris_read_vbo(win, sty, act, pin);
	else tetris_read_pixel(win, sty, act, pin);
}
static void tetris_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == _kbd_)
	{
		if(key==0x48)tetris_up(buf, WIDTH, HEIGHT);
		else if(key==0x4b)tetris_left(buf, WIDTH, HEIGHT);
		else if(key==0x4d)tetris_right(buf, WIDTH, HEIGHT);
		else if(key==0x50)tetris_down(buf, WIDTH, HEIGHT);
	}
	else if(type == _char_)
	{
		if(key=='a')tetris_left(buf, WIDTH, HEIGHT);
		else if(key=='d')tetris_right(buf, WIDTH, HEIGHT);
		else if(key=='w')tetris_up(buf, WIDTH, HEIGHT);
		else if(key=='s')tetris_down(buf, WIDTH, HEIGHT);
		else if(key==' ')
		{
			for(ret=0;ret<20;ret++)
			{
				if(1 == tetris_down(buf, WIDTH, HEIGHT))return;
			}
		}
	}
}
static void tetris_list()
{
}
static void tetris_choose()
{
}
static void tetris_stop(struct actor* act, struct pinid* pin)
{
}
static void tetris_start(struct actor* act, struct pinid* pin)
{
	tetris_generate(buf, WIDTH, HEIGHT);
}
static void tetris_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void tetris_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buf;
	if(_copy_ == act->type)act->buf = memorycreate(WIDTH*HEIGHT);
}




void tetris_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('t', 'e', 't', 'r', 'i', 's', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)tetris_create;
	p->ondelete = (void*)tetris_delete;
	p->onstart  = (void*)tetris_start;
	p->onstop   = (void*)tetris_stop;
	p->onlist   = (void*)tetris_list;
	p->onchoose = (void*)tetris_choose;
	p->onread   = (void*)tetris_read;
	p->onwrite  = (void*)tetris_write;
}
