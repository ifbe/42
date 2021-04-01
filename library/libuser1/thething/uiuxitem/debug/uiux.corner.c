#include "libuser.h"
void* entity_alloc();
void* style_alloc();




void corner_gl41_drag_lefttop(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	float x, float y)
{
	vec3 tc,tr,tf;

	tc[0] = x;
	tc[1] = y;
	tc[2] = -0.99;
	tr[0] = 1.0+tc[0];
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = tc[1]-1.0;
	tf[2] = 0.0;
	gl41line_rect(win, 0xffffff, tc, tr, tf);
	gl41solid_circle(win, 0xffffff, tc, tr, tf);
}
void corner_gl41_drag_righttop(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	float x, float y)
{
	vec3 tc,tr,tf;

	tc[0] = x;
	tc[1] = y;
	tc[2] = -0.99;

	tr[0] = -1.0;
	tr[1] = -1.0;
	tr[2] = -0.99;
	tf[0] = 1.0;
	tf[1] = -1.0;
	tf[2] = -0.99;
	gl41solid_triangle(win, 0x0000ff, tc, tr, tf);

	tf[0] = -1.0;
	tf[1] = 1.0;
	tf[2] = -0.99;
	gl41solid_triangle(win, 0x00ffff, tc, tr, tf);

	tr[0] = 1.0;
	tr[1] = 1.0;
	tr[2] = -0.99;
	gl41solid_triangle(win, 0xff0000, tc, tr, tf);

	tf[0] = 1.0;
	tf[1] = -1.0;
	tf[2] = -0.99;
	gl41solid_triangle(win, 0xffff00, tc, tr, tf);
}
void corner_gl41_drag_leftbot(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	float x, float y)
{
	vec3 tc, tr, tf;

	tc[0] = x;
	tc[1] = 0.0;
	tc[2] = -0.9;
	tr[0] = 1.0+tc[0];
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0;
	tf[2] = 0.0;
	gl41solid_rect(win, 0x404040, tc, tr, tf);

	tc[0] = 0.0;
	tc[1] = y;
	tr[0] = 1.0;
	tr[1] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0+tc[1];
	gl41solid_rect(win, 0x404040, tc, tr, tf);
}
void corner_gl41_drag_rightbot(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	float x0, float y0, float xn, float yn)
{
	vec3 tc, tr, tf;

	tr[0] = x0;
	tr[1] = -1.0;
	tr[2] = -0.9;
	tf[0] = 1.0;
	tf[1] = y0;
	tf[2] = -0.9;
	tc[0] = xn;
	tc[1] = yn;
	tc[2] = -0.9;
	gl41solid_triangle(win, 0x808080, tc, tr, tf);
	tc[0] = 1.0;
	tc[1] = -1.0;
	gl41solid_triangle(win, 0x000000, tc, tr, tf);
}
void corner_gl41_drag(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int j;
	float w,h,c;
	float x0,y0,xn,yn;
/*
	w = win->width;
	h = win->height;
	if(w<h)c = w / 32;
	else c = h / 32;

	if(win->mouse[0].z0){
		x0 = win->mouse[0].x0;
		y0 = win->mouse[0].y0;
		xn = win->mouse[0].xn;
		yn = win->mouse[0].yn;
	}
	else if(win->touch[0].z0){
		x0 = win->touch[0].x0;
		y0 = win->touch[0].y0;
		xn = win->touch[0].xn;
		yn = win->touch[0].yn;
	}
	else return;


	if(y0 < c){
		if(x0 < c){
			corner_gl41_drag_lefttop(
				act, pin, win, sty,
				xn/w - 1.0, 1.0 - yn/h
			);
		}
		else if(x0+c > w){
			corner_gl41_drag_righttop(
				act, pin, win, sty,
				2.0*xn/w - 1.0, 1.0 - 2.0*yn/h
			);
		}
	}
	else if(y0+c > h){
		if(x0 < c){
			corner_gl41_drag_leftbot(
				act, pin, win, sty,
				xn/w - 1.0, -yn/h
			);
		}
		else if(x0+c > w){
			x0 = ((h-yn)*(h-yn)/(xn-w) + (w+xn)) / 2.0;
			x0 = 2*x0/w - 1.0;
			y0 = ((xn-w)*(xn-w)/(yn-h) + (yn+h)) / 2.0;
			y0 = 1.0 - 2*y0/h;
			corner_gl41_drag_rightbot(
				act, pin, win, sty,
				x0, y0, 2.0*xn/w - 1.0, 1.0 - 2.0*yn/h
			);
		}
	}
*/
}




void corver_gl41_hover(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float w,h,c;
	float x0,y0,xn,yn;
	vec3 vc, vr, vf;
	vec3 ta, tb, tc;
/*
	w = win->width;
	h = win->height;
	if(w<h)c = w / 32;
	else c = h / 32;

	x0 = win->mouse[0].x0;
	y0 = win->mouse[0].y0;
	xn = win->mouse[0].xn;
	yn = win->mouse[0].yn;

	vr[0] = 2*(float)c / (float)w;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 2*(float)c / (float)h;
	vf[2] = 0.0;

	//right, bot
	if((xn+c > w)&&(yn+c > h)){
		ta[0] = 0.0;
		ta[1] = -1.0;
		ta[2] = 0.0;
		tb[0] = 1.0;
		tb[1] = 0.0;
		tb[2] = 0.0;
		tc[0] = 1.0;
		tc[1] = -1.0;
		tc[2] = -0.9;
		carveopaque2d_bezier(win, 0xff0000ff, ta, tb, tc);
	}
	else{
		vc[0] = 1.0;
		vc[1] = -1.0;
		vc[2] = -0.99;
		gl41solid2d_circle(win, 0x0000ff, vc, vr, vf);
	}

	//left, bot
	if((xn < c)&&(yn+c > h)){
		ta[0] = 0.0;
		ta[1] = -1.0;
		ta[2] = 0.0;
		tb[0] = -1.0;
		tb[1] = 0.0;
		tb[2] = 0.0;
		tc[0] = -1.0;
		tc[1] = -1.0;
		tc[2] = -0.9;
		carveopaque2d_bezier(win, 0xffff0000, ta, tb, tc);
	}
	else{
		vc[0] = -1.0;
		vc[1] = -1.0;
		vc[2] = -0.99;
		gl41solid2d_circle(win, 0xff0000, vc, vr, vf);
	}

	//left, top
	if((xn < c)&&(yn < c)){
		ta[0] = 0.0;
		ta[1] = 1.0;
		ta[2] = 0.0;
		tb[0] = -1.0;
		tb[1] = 0.0;
		tb[2] = 0.0;
		tc[0] = -1.0;
		tc[1] = 1.0;
		tc[2] = -0.9;
		carveopaque2d_bezier(win, 0xff00ffff, ta, tb, tc);
	}
	else{
		vc[0] = -1.0;
		vc[1] = 1.0;
		vc[2] = -0.99;
		gl41solid2d_circle(win, 0x00ffff, vc, vr, vf);
	}

	//right, top
	if((xn+c > w)&&(yn < c)){
		ta[0] = 0.0;
		ta[1] = 1.0;
		ta[2] = 0.0;
		tb[0] = 1.0;
		tb[1] = 0.0;
		tb[2] = 0.0;
		tc[0] = 1.0;
		tc[1] = 1.0;
		tc[2] = -0.9;
		carveopaque2d_bezier(win, 0xffffff00, ta, tb, tc);
	}
	else{
		vc[0] = 1.0;
		vc[1] = 1.0;
		vc[2] = -0.99;
		gl41solid2d_circle(win, 0xffff00, vc, vr, vf);
	}
*/
}




void corner_gl41_popup(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	struct relation* rel = act->orel0;
	if(0 == rel)return;

	struct style* st = (void*)(rel->srcfoot);
	if(0 == st)return;
	if('#' == st->is.uc[3])return;

	int j,rgb;
	vec3 tc,tr,tf;

	tr[0] = 0.0;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0/33;
	tf[2] = 0.0;
	tc[0] = 0.0;
	tc[1] = 0.0;
	tc[2] = -0.9;

	for(j=0;j<8;j++){

		tc[0] = -7.0/8;
		tc[1] = (-31+j*2)/32.0;
		tr[0] = 1.0/8;
		gl41opaque_rect(win, 0x3f000080, tc, tr, tf);

		tr[0] = 1.0/32;
		gl41string_center(win, 0xffffff, tc, tr, tf, (void*)"test", 8);
	}
}




void corner_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	corver_gl41_hover(act, pin, win, sty);

	corner_gl41_drag(act, pin, win, sty);

	corner_gl41_popup(act, pin, win, sty);
}
void corner_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x0,y0,xn,yn;
	int w,h,c,t;

	w = win->width;
	h = win->height;
	if(w<h)c = w>>6;
	else c = h>>6;
/*

//---------hover
	x0 = win->mouse[0].x0;
	y0 = win->mouse[0].y0;
	xn = win->mouse[0].xn;
	yn = win->mouse[0].yn;

	if((xn < c)&&(yn+c > h))t = 4;
	else t = 1;
	drawsolid_circle(win, 0x0000ff, 0, h, c*t);

	if((xn+c > w)&&(yn+c > h))t = 4;
	else t = 1;
	drawsolid_circle(win, 0xff0000, w, h, c*t);

	if((xn < c)&&(yn < c))t = 4;
	else t = 1;
	drawsolid_circle(win, 0x00ffff, 0, 0, c*t);

	if((xn+c > w)&&(yn < c))t = 4;
	else t = 1;
	drawsolid_circle(win, 0xffff00, w, 0, c*t);


//--------------drag
	if(win->mouse[0].z0){
		x0 = win->mouse[0].x0;
		y0 = win->mouse[0].y0;
		xn = win->mouse[0].xn;
		yn = win->mouse[0].yn;
	}
	else if(win->touch[0].z0){
		x0 = win->touch[0].x0;
		y0 = win->touch[0].y0;
		xn = win->touch[0].xn;
		yn = win->touch[0].yn;
	}

	if(y0 < c)
	{
		if(x0 < c)
		{
			drawsolid_rect(win, 0x404040, 0, 0, xn, yn);
		}
		else if(x0+c > w)
		{
			drawsolid_triangle(win, 0x0000ff, xn, yn, 0, h-1, w-1, h-1);
			drawsolid_triangle(win, 0x00ffff, xn, yn, 0, h-1,   0,   0);
			drawsolid_triangle(win, 0xff0000, xn, yn, 0,   0, w-1,   0);
			drawsolid_triangle(win, 0xffff00, xn, yn, w-1, 0, w-1, h-1);
		}
	}
	else if(y0+c > h)
	{
		if(x0 < c)
		{
			drawsolid_rect(win, 0x404040, 0, 0, xn, h);
			drawsolid_rect(win, 0x404040, 0, yn, w, h);
		}
		else if(x0+c > w)
		{
			x0 = ((h-yn)*(h-yn)/(xn-w) + (w+xn)) / 2;
			y0 = ((xn-w)*(xn-w)/(yn-h) + (yn+h)) / 2;
			drawsolid_triangle(win, 0x808080, x0, h, w, y0, xn, yn);
			drawsolid_triangle(win, 0x000000, x0, h, w, y0, w, h);
		}
	}
*/
}




static int corner_event_twig(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	struct relation* rel;
	struct entity* ar;
	struct style* st;
	int x,y,j;

	rel = act->orel0;
	if(0 == rel)return 0;

	st = (void*)(rel->srcfoot);
	if(0 == st)return 0;
	if('#' == st->is.uc[3])return 0;

	int w = win->width;
	int h = win->height;

	short* t = (void*)ev;
	if(t[0] > w/8)return 0;
	if(t[1] < h/2)return 0;

	rel = win->orel0;
	while(1){
		if(0 == rel)break;
		if(_sup_ == rel->dsttype){
			ar = (void*)(rel->dstchip);
			//if(_ui3d_ == ar->fmt)goto found;
			//if(_ui2d_ == ar->fmt)goto found;
		}
		rel = samesrcnextdst(rel);
	}
	return 0;

found:
	x = 0;
	y = (h-t[1])*32/h;
	rel = ar->orel0;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			if(x == y){
				st = (void*)(rel->srcfoot);

				j = st->is.uc[3];
				if('#' == j)j = 0;
				else j = '#';
				st->is.uc[3] = j;

				break;
			}

			x += 1;
			if(x >= 16)break;
		}

		rel = samesrcnextdst(rel);
	}
	return 1;
}




static int corner_onoff(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	struct relation* rel;
	struct style* st;
	int j;

	rel = act->orel0;
	if(0 == rel)return 0;

	st = (void*)(rel->srcfoot);
	if(0 == st)return 0;

	j = st->is.uc[3];
	if('#' == j)j = 0;
	else j = '#';
	st->is.uc[3] = j;

	return 1;
}
static int corner_event_root(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	int w,h,ret;
	int x,y;

	t = (void*)ev;
	x = t[0];
	y = t[1];

	w = win->width;
	h = win->height;
	if(w<h)ret = w>>4;
	else ret = h>>4;

	if(y+ret > h)
	{
		if(x < ret)
		{
			corner_onoff(act, pin, win, sty);
			return 1;
		}
		if(x+ret > w)
		{
			return 1;
		}
	}
	if(y < ret)
	{
		if(x < ret)
		{
			return 1;
		}
		if(x+ret > w)
		{
			return 1;
		}
	}
	return 0;
}
static int corner_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	int j;
	if(0x2d70 == ev->what)
	{
		j = corner_event_root(act, pin, win, sty, ev, len);
		if(j)return 1;

		j = corner_event_twig(act, pin, win, sty, ev, len);
		if(j)return 1;
	}
	return 0;
}




static int corner_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
static int corner_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
static int corner_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int corner_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




static int corner_search(struct entity* win)
{
	return 0;
}
static int corner_modify(struct entity* win)
{
	return 0;
}
static int corner_delete(struct entity* win)
{
	return 0;
}
static int corner_create(struct entity* act, u8* str)
{
	struct entity* ac = entity_alloc();
	struct style* sty = style_alloc();

	ac->type = hex32('?','?','?', 0);
	ac->fmt = hex32('?','?','?', 0);

	sty->is.uc[3] = 0;
	relationcreate(ac, 0, _ent_, 0, act, sty, _ent_, 0);
	return 0;
}




void corner_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'o', 'r', 'n', 'e', 'r', 0, 0);

	p->oncreate = (void*)corner_create;
	p->ondelete = (void*)corner_delete;
	p->onsearch = (void*)corner_search;
	p->onmodify = (void*)corner_modify;

	p->onlinkup = (void*)corner_linkup;
	p->ondiscon = (void*)corner_discon;
	p->ontaking = (void*)corner_taking;
	p->ongiving = (void*)corner_giving;
}
