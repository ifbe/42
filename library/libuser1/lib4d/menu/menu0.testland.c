#include "actor.h"




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct pinid* pinid = 0;
void helpin_create(void* addr)
{
	arena = addr + 0x000000;
	actor = addr + 0x100000;
	style = addr + 0x000000;
	pinid = addr + 0x100000;
}
void helpout_create(void* addr)
{
	arena = addr + 0x000000;
	actor = addr + 0x100000;
	style = addr + 0x200000;
	pinid = addr + 0x300000;
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
/*
	if((win->theone) | (win->edit) | (0 == win->irel))
	{
		drawline(win, 0xffffff, 0, h/2, w, h/2);
		drawline(win, 0xffffff, w/2, 0, w/2, h);
	}
*/
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
/*
	if((win->edit) | (0 <= win->theone) | (0 == win->irel))
	{
		carveline(win, 0xff0000, 0.0, 0.0, 0.0, 10000.0, 0.0, 0.0);
		carveline(win, 0x00ff00, 0.0, 0.0, 0.0, 0.0, 10000.0, 0.0);
		carveline(win, 0x0000ff, 0.0, 0.0, 0.0, 0.0, 0.0, 10000.0);
	}
*/
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
	for(j=0;j<12;j++)
	{
		if(0 == win->touchdown[j].z)continue;

		drawline_arrow(win, 0xff00ff,
			win->touchdown[j].x, win->touchdown[j].y,
			win->touchmove[j].x, win->touchmove[j].y
		);
	}
}
void foreground_vbo(struct arena* win)
{
	int j;
	float x,y;
	vec3 vc;
	vec3 vr;
/*
	vec3 vf;
	vec3 vu;
	vc[0] = win->target.vc[0];
	vc[1] = win->target.vc[1];
	vc[2] = win->target.vc[2];
	vr[0] = 1000000.0;
	vf[1] = 1000000.0;
	vu[2] = 1000000.0;
	vr[1] = vr[2] = vf[0] = vf[2] = vu[0] = vu[1] = 0.0;
	carvesolid_sphere(win, 0x808080, vc, vr, vf, vu);
*/
	for(j=0;j<12;j++)
	{
		if(0 == win->touchdown[j].z)continue;

		vc[0] = (float)(win->touchdown[j].x) / (float)(win->width);
		vc[0] = vc[0]*2 - 1.0;
		vc[1] = (float)(win->touchdown[j].y) / (float)(win->height);
		vc[1] = 1.0 - vc[1]*2;
		vc[2] = -0.99;
		vr[0] = (float)(win->touchmove[j].x) / (float)(win->width);
		vr[0] = vr[0]*2 - 1.0;
		vr[1] = (float)(win->touchmove[j].y) / (float)(win->height);
		vr[1] = 1.0 - vr[1]*2;
		vr[2] = -0.99;
		carveline2d_arrow(win, 0xff00ff, vc, vr);
	}
/*
	if(1)
	{
		j = (win->width + win->height) / 128;
		x = (float)j / (float)(win->width);
		y = (float)j / (float)(win->height);
		vc[0] = -x;
		vc[1] = 0.0;
		vc[2] = -0.99;
		vr[0] = x;
		vr[1] = 0.0;
		vr[2] = -0.99;
		carveline2d(win, 0xffffff, vc, vr);
		vc[0] = 0.0;
		vc[1] = -y;
		vr[0] = 0.0;
		vr[1] = y;
		carveline2d(win, 0xffffff, vc, vr);
	}
*/
	if(win != &arena[0])carvecamera(win, &arena[0]);
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
	if(_cli_ == fmt)foreground_cli(win);
	else if(_tui_ == fmt)foreground_tui(win);
	else if(_html_ == fmt)foreground_html(win);
	else if(_json_ == fmt)foreground_json(win);
	else if(_vbo_ == fmt)foreground_vbo(win);
	else foreground_pixel(win);
}




void actoroutput_void_pixel(struct arena* win)
{
	drawline(win, 0xffffff, 0, win->modey, win->width, win->modey);
	drawline(win, 0xffffff, win->modex, 0, win->modex, win->height);
}
void actoroutput_void_vbo(struct arena* win)
{
	vec3 va;
	vec3 vb;
	float x = (float)(win->modex);
	float y = (float)(win->modey);
	x = 2*x/(win->width) - 1.0;
	y = 1.0 - 2*y/(win->height);

	va[0] = -1.0;
	va[1] = y;
	va[2] = -0.5;
	vb[0] = 1.0;
	vb[1] = y;
	vb[2] = -0.5;
	carveline2d(win, 0xffffff, va, vb);

	va[0] = x;
	va[1] = 1.0;
	va[2] = -0.5;
	vb[0] = x;
	vb[1] = -1.0;
	vb[2] = -0.5;
	carveline2d(win, 0xffffff, va, vb);
}
void actoroutput_void_json(struct arena* win)
{
}
void actoroutput_void_html(struct arena* win)
{
}
void actoroutput_void_tui(struct arena* win)
{
}
void actoroutput_void_cli(struct arena* win)
{
}
void actoroutput_void(struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)actoroutput_void_cli(win);
	else if(_tui_ == fmt)actoroutput_void_tui(win);
	else if(_html_ == fmt)actoroutput_void_html(win);
	else if(_json_ == fmt)actoroutput_void_json(win);
	else if(_vbo_ == fmt)actoroutput_void_vbo(win);
	else actoroutput_void_pixel(win);
}
void actorinput_void(struct arena* win, struct event* ev)
{
	if('p' == (ev->what&0xff))
	{
		win->modex = (ev->why)&0xffff;
		win->modey = ((ev->why)>>16)&0xffff;
	}
}