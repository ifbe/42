#include "libuser.h"




void popup_read_cli(struct arena* win, struct style* sty)
{
}
void popup_read_tui(struct arena* win, struct style* sty)
{
}
void popup_read_html(struct arena* win, struct style* sty)
{
}
void popup_read_json(struct arena* win, struct style* sty)
{
}
void popup_read_vbo(struct arena* win, struct style* sty)
{
	int j;
	float x0,y0,x1,y1;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	for(j=0;j<12;j++)
	{
		if(0 == win->input[j].z0)continue;

		x0 = (float)(win->input[j].x0) / (float)(win->width);
		x0 = x0*2 - 1.0;
		y0 = (float)(win->input[j].y0) / (float)(win->height);
		y0 = 1.0 - y0*2;
		x1 = (float)(win->input[j].xn) / (float)(win->width);
		x1 = x1*2 - 1.0;
		y1 = (float)(win->input[j].yn) / (float)(win->height);
		y1 = 1.0 - y1*2;

		vc[0] = x0;
		vc[1] = y0;
		vc[2] = -0.99;
		vr[0] = x1;
		vr[1] = y1;
		vr[2] = -0.99;
		carveline2d_arrow(win, 0xff00ff, vc, vr);

		vc[0] = (x0+x1)/2;
		vc[1] = (y0+y1)/2;
		vc[2] = -0.99;
		vr[0] = (x1-x0)/2;
		vr[1] = 0.0;
		vr[2] = -0.99;
		vf[0] = 0.0;
		vf[1] = (y1-y0)/2;
		vf[2] = -0.99;
		carveline2d_rect(win, 0x00ff00, vc, vr, vf);
	}

}
void popup_read_pixel(struct arena* win, struct style* sty)
{
	int j;
	int x0,y0,x1,y1;
	for(j=0;j<12;j++)
	{
		if(0 == win->input[j].z0)continue;

		x0 = win->input[j].x0;
		y0 = win->input[j].y0;
		x1 = win->input[j].xn;
		y1 = win->input[j].yn;
		drawline_arrow(win, 0xff00ff, x0, y0, x1, y1);
		drawline_rect(win, 0x00ff00, x0, y0, x1, y1);
	}
}
int temp_read(struct arena* cc, void* cf, struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)popup_read_cli(win, sty);
	else if(fmt == _tui_)popup_read_tui(win, sty);
	else if(fmt == _html_)popup_read_html(win, sty);
	else if(fmt == _json_)popup_read_json(win, sty);
	else if(fmt == _vbo_)popup_read_vbo(win, sty);
	else popup_read_pixel(win, sty);
	return 0;
}
int temp_write(struct arena* win, struct event* ev)
{
	return 0;
}




int menu_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
	return 0;
}
int menu_create(struct arena* win, void* str)
{
	return 0;
}
