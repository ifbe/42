#include "libuser.h"




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct pinid* pinid = 0;
void bgfg_create(void* addr)
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
	for(j=0;j<16;j++)
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
	struct mystring** hp = win->hp;

	hp[1]->len = mysnprintf(
		hp[1]->buf, 0x100000,
		"<html><head>\n"
		"<meta charset=\"UTF-8\">\n"
		"<style type=\"text/css\">\n"
		"*{margin:0;padding:0;}\n"
	);
	hp[2]->len = mysnprintf(
		hp[2]->buf, 0x100000,
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
		if(0 == win->input[j].z0)continue;

		drawline_arrow(win, 0xff00ff,
			win->input[j].x0, win->input[j].y0,
			win->input[j].x1, win->input[j].y1
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
		if(0 == win->input[j].z0)continue;

		vc[0] = (float)(win->input[j].x0) / (float)(win->width);
		vc[0] = vc[0]*2 - 1.0;
		vc[1] = (float)(win->input[j].y0) / (float)(win->height);
		vc[1] = 1.0 - vc[1]*2;
		vc[2] = -0.99;
		vr[0] = (float)(win->input[j].x1) / (float)(win->width);
		vr[0] = vr[0]*2 - 1.0;
		vr[1] = (float)(win->input[j].y1) / (float)(win->height);
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
	struct mystring** hp = win->hp;

	len = hp[1]->len;
	buf = hp[1]->buf + len;
	hp[1]->len += mysnprintf(buf, 0x100000-len, "</style></head>\n");

	len = hp[2]->len;
	buf = hp[2]->buf + len;
	hp[2]->len += mysnprintf(buf, 0x100000-len, "</body></html>\n");
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