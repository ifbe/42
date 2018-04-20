#include "actor.h"
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void vkbd_read(void*);
void login_read(void*);




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct compo* compo = 0;
void helpout_create(void* addr)
{
	arena = addr + 0x000000;
	actor = addr + 0x100000;
	style = addr + 0x200000;
	compo = addr + 0x300000;
}




void select_2d(struct arena* win, struct style* sty)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	drawline_rect(win, 0xff00ff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
}
void select_3d(struct arena* win, struct style* sty)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	carveline_prism4(
		win, 0xff0000,
		cx, cy, cz+dd,
		ww, 0.0, 0.0,
		0.0, hh, 0.0,
		0.0, 0.0, dd
	);
}




void background_pixel(struct arena* win)
{
	int x;
	int w = win->width;
	int h = win->height;
	int s = win->stride;
	int len = s*h;
	u32* buf = (u32*)(win->buf);

	for(x=0;x<len;x++)buf[x] = 0xff000000;
	if((win->theone) | (win->edit) | (0 == win->irel))
	{
		drawline(win, 0xffffff, 0, h/2, w, h/2);
		drawline(win, 0xffffff, w/2, 0, w/2, h);
	}
}
void background_vbo(struct arena* win)
{
	int j;
	struct texandobj* mod = win->mod;
	for(j=0;j<8;j++)
	{
		mod[j].ilen = 0;
		mod[j].vlen = 0;
	}
	for(j=0x80;j<0x88;j++)
	{
		mod[j].ilen = 0;
		mod[j].vlen = 0;
	}
	if((win->edit) | (0 <= win->theone) | (0 == win->irel))
	{
		carveline(win, 0xff0000, 0.0, 0.0, 0.0, 10000.0, 0.0, 0.0);
		carveline(win, 0x00ff00, 0.0, 0.0, 0.0, 0.0, 10000.0, 0.0);
		carveline(win, 0x0000ff, 0.0, 0.0, 0.0, 0.0, 0.0, 10000.0);
	}
}
void background_json(struct arena* win)
{
	win->len = mysnprintf(win->buf, 0x100000, "{\n");
}
void background_html(struct arena* win)
{
	struct htmlpiece* hp = win->hp;

	hp[1].len = mysnprintf(
		hp[1].buf, 0x100000,
		"<html><head>\n"
		"<meta charset=\"UTF-8\">\n"
		"<style type=\"text/css\">\n"
		"*{margin:0;padding:0;}\n"
	);
	hp[2].len = mysnprintf(
		hp[2].buf, 0x100000,
		"<body style=\"width:100%%;height:100%%;\">\n"
	);
}
void background_tui(struct arena* win)
{
}
void background_cli(struct arena* win)
{
}
void background(struct arena* win)
{
	u64 fmt = win->fmt;
	//if((_vbo_ != win->fmt) | (12 == win->flag0));

	if(_cli_ == fmt)background_cli(win);
	else if(_tui_ == fmt)background_tui(win);
	else if(_html_ == fmt)background_html(win);
	else if(_json_ == fmt)background_json(win);
	else if(_vbo_ == fmt)background_vbo(win);
	else background_pixel(win);
}




void foreground_pixel(struct arena* win)
{
	int j;
	for(j=0;j<11;j++)
	{
		if(0 == win->touchdown[j].z)continue;

		drawline(win, 0xff00ff,
			win->touchdown[j].x, win->touchdown[j].y,
			win->touchmove[j].x, win->touchmove[j].y
		);
	}
	vkbd_read(win);
}
void foreground_vbo(struct arena* win)
{
	int j;
	float x0,y0,x1,y1;
	for(j=0;j<11;j++)
	{
		if(0 == win->touchdown[j].z)continue;

		x0 = (float)(win->touchdown[j].x) / (float)(win->width);
		x0 = x0*2 - 1.0;
		y0 = (float)(win->touchdown[j].y) / (float)(win->height);
		y0 = 1.0 - y0*2;
		x1 = (float)(win->touchmove[j].x) / (float)(win->width);
		x1 = x1*2 - 1.0;
		y1 = (float)(win->touchmove[j].y) / (float)(win->height);
		y1 = 1.0 - y1*2;
		carveline2d(win, 0xff00ff, x0, y0, 0.0, x1, y1, 0.0);
	}

	vkbd_read(win);
	if(win->theone < 0)
	{
		carveline_sphere(
			win, 0xffffff,
			win->target.cx, win->target.cy, win->target.cz,
			16.0, 0.0, 0.0,
			0.0, 16.0, 0.0,
			0.0, 0.0, 16.0
		);
	}
}
void foreground_json(struct arena* win)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(buf+len, 0x100000-len, "}\n");

	win->len = len;
}
void foreground_html(struct arena* win)
{
	int len;
	u8* buf;
	struct htmlpiece* hp = win->hp;

	len = hp[1].len;
	buf = hp[1].buf + len;
	hp[1].len += mysnprintf(buf, 0x100000-len, "</style></head>\n");

	len = hp[2].len;
	buf = hp[2].buf + len;
	hp[2].len += mysnprintf(buf, 0x100000-len, "</body></html>\n");
}
void foreground_tui(struct arena* win)
{
}
void foreground_cli(struct arena* win)
{
}
void foreground(struct arena* win)
{
	u64 fmt = win->fmt;
	if((0 <= win->theone) | (0 == win->irel))login_read(win);

	if(_cli_ == fmt)foreground_cli(win);
	else if(_tui_ == fmt)foreground_tui(win);
	else if(_html_ == fmt)foreground_html(win);
	else if(_json_ == fmt)foreground_json(win);
	else if(_vbo_ == fmt)foreground_vbo(win);
	else foreground_pixel(win);
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
		if(win->edit)return 0;
	}

	//bg
	background(win);

	//content
	rel = win->irel;
	while(1)
	{
		if(rel == 0)break;

		if(rel->srctype == _act_)
		{
			act = (void*)(rel->srcchip);
			sty = (void*)(rel->dstfoot);
			com = (void*)(rel->srcfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, com);
			//say("%x\n", rel);

			act->onread(win, sty, act, com);
			if(win->edit)
			{
				if(win->fmt == _vbo_)select_3d(win, sty);
				else select_2d(win, sty);
			}
		}

		rel = samedstnextsrc(rel);
	}

	//fg
	foreground(win);
	return 0;
}
