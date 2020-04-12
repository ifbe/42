#include "libuser.h"




void pointer_draw_cli(struct entity* win, struct style* sty)
{
}
void pointer_draw_tui(struct entity* win, struct style* sty)
{
}
void pointer_draw_html(struct entity* win, struct style* sty)
{
}
void pointer_draw_json(struct entity* win, struct style* sty)
{
}
void pointer_draw_gl41(struct entity* win, struct style* sty)
{/*
	int j;
	float x0,y0,x1,y1;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	for(j=0;j<8;j++)
	{
		if(0 == win->mouse[j].z0)continue;

		x0 = (float)(win->mouse[j].x0) / (float)(win->width);
		x0 = x0*2 - 1.0;
		y0 = (float)(win->mouse[j].y0) / (float)(win->height);
		y0 = 1.0 - y0*2;
		x1 = (float)(win->mouse[j].xn) / (float)(win->width);
		x1 = x1*2 - 1.0;
		y1 = (float)(win->mouse[j].yn) / (float)(win->height);
		y1 = 1.0 - y1*2;

		vc[0] = x0;
		vc[1] = y0;
		vc[2] = -0.99;
		vr[0] = x1;
		vr[1] = y1;
		vr[2] = -0.99;
		gl41line2d_arrow(win, 0xff00ff, vc, vr);
	}
*/
}
void pointer_draw_pixel(struct entity* win, struct style* sty)
{/*
	int j;
	int x0,y0,x1,y1;
	for(j=0;j<12;j++)
	{
		if(0 == win->mouse[j].z0)continue;

		x0 = win->mouse[j].x0;
		y0 = win->mouse[j].y0;
		x1 = win->mouse[j].xn;
		y1 = win->mouse[j].yn;
		drawline_arrow(win, 0xff00ff, x0, y0, x1, y1);
	}*/
}




static int pointer_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
static int pointer_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
static int pointer_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int pointer_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




static int pointer_search(struct entity* act)
{
	return 0;
}
static int pointer_modify(struct entity* act)
{
	return 0;
}
static int pointer_delete(struct entity* act)
{
	return 0;
}
static int pointer_create(struct entity* act, u8* str)
{
	return 0;
}




void pointer_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'o', 'i', 'n', 't', 'e', 'r', 0);

	p->oncreate = (void*)pointer_create;
	p->ondelete = (void*)pointer_delete;
	p->onsearch = (void*)pointer_search;
	p->onmodify = (void*)pointer_modify;

	p->onlinkup = (void*)pointer_linkup;
	p->ondiscon = (void*)pointer_discon;
	p->onread  = (void*)pointer_read;
	p->onwrite = (void*)pointer_write;
}
