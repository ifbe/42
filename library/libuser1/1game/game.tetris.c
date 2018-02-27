#include "actor.h"
#define width 32
#define height 32
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
static u8 buf[width*height];




static void tetris_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u32 c;
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			//say("%d ",buf[y*width+x]);
			if(0 == buf[y*width+x])continue;
			else if(1 == buf[y*width+x])c = 0xffffff;
			else c = 0x00ff00;

			drawsolid_rect(win, c,
				cx-ww+1 + ((x+0)*2*ww)/width,
				cy-hh+1 + ((y+0)*2*hh)/height,
				cx-ww-1 + ((x+1)*2*ww)/width,
				cy-hh-1 + ((y+1)*2*hh)/height);
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
	struct actor* act, struct compo* com)
{
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			carvesolid_prism4(
				win, 0xffffff,
				(cx-ww)+(2*x+1)*ww/width, (cy-ww)+(2*y+1)*hh/height, ww/width,
				ww/(width+1), 0.0, 0.0,
				0.0, hh/(height+1), 0.0,
				0.0, 0.0, ww/width
			);
		}
	}
}




static int htmlcubie(char* p, int x, int y)
{
	return mysnprintf(
		p, 0x1000,
		"<div class=\"rect\" style=\""
		"left:%.2f%;"
		"top:%.2f%;"
		"\">%d</div>",
		x*3.1, y*2.5, buf[y*width+x]
	);
}
static void tetris_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int x,y;
	char* p = (char*)(win->buf);

	p += mysnprintf(
		p, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"background:#fff;"
		"position:absolute;"
		"width:3.1%;"
		"height:2.5%;"
		"}"
		"</style>"
	);
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			if(buf[y*width+x] == 0)continue;
			p += htmlcubie(p, x, y);
		}
	}

	p += htmlcubie(p, that.x1, that.y1);
	p += htmlcubie(p, that.x2, that.y2);
	p += htmlcubie(p, that.x3, that.y3);
	p += htmlcubie(p, that.x4, that.y4);
}
static void tetris_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int x,y;
	int w = win->w;
	int h = win->h;
	char* p = (char*)(win->buf);

	for(x=0;x<w*h*4;x++)p[x]=0;
	if(h>=height)
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<width;x++)
			{
				if(buf[y*width+x])
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
			for(x=0;x<width;x++)
			{
				if(buf[width*(y+height-h) + x])
				{
					p[(y*w+x)<<2]='#';
				}
			}
		}
		p[(that.x1 + (that.y1-height+h)*w)<<2]='#';
		p[(that.x2 + (that.y2-height+h)*w)<<2]='#';
		p[(that.x3 + (that.y3-height+h)*w)<<2]='#';
		p[(that.x4 + (that.y4-height+h)*w)<<2]='#';
	}
}
static void tetris_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void tetris_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)tetris_read_cli(win, sty, act, com);
	else if(fmt == _tui_)tetris_read_tui(win, sty, act, com);
	else if(fmt == _vbo_)tetris_read_vbo(win, sty, act, com);
	else if(fmt == _html_)tetris_read_html(win, sty, act, com);
	else tetris_read_pixel(win, sty, act, com);
}
static void tetris_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
	int ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == _kbd_)
	{
		if(key==0x48)tetris_up(buf, width, height);
		else if(key==0x4b)tetris_left(buf, width, height);
		else if(key==0x4d)tetris_right(buf, width, height);
		else if(key==0x50)tetris_down(buf, width, height);
	}
	else if(type == _char_)
	{
		if(key=='a')tetris_left(buf, width, height);
		else if(key=='d')tetris_right(buf, width, height);
		else if(key=='w')tetris_up(buf, width, height);
		else if(key=='s')tetris_down(buf, width, height);
		else if(key==' ')
		{
			for(ret=0;ret<20;ret++)
			{
				if(1 == tetris_down(buf, width, height))return;
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
static void tetris_start()
{
	tetris_generate(buf, width, height);
}
static void tetris_stop()
{
}
static void tetris_delete()
{
}
static void tetris_create()
{
}




void tetris_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
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