#include "libuser.h"
void* allocactor();
void* allocstyle();
int ui2d_cwrite(struct arena* win, struct style* sty, struct actor* sc, struct pinid* sf, int flag);




void corner_vbo_drag_lefttop(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
	carveline2d_rect(win, 0xffffff, tc, tr, tf);
	carvesolid2d_circle(win, 0xffffff, tc, tr, tf);
}
void corner_vbo_drag_righttop(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
	carvesolid2d_triangle(win, 0x0000ff, tc, tr, tf);

	tf[0] = -1.0;
	tf[1] = 1.0;
	tf[2] = -0.99;
	carvesolid2d_triangle(win, 0x00ffff, tc, tr, tf);

	tr[0] = 1.0;
	tr[1] = 1.0;
	tr[2] = -0.99;
	carvesolid2d_triangle(win, 0xff0000, tc, tr, tf);

	tf[0] = 1.0;
	tf[1] = -1.0;
	tf[2] = -0.99;
	carvesolid2d_triangle(win, 0xffff00, tc, tr, tf);
}
void corner_vbo_drag_leftbot(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
	carvesolid2d_rect(win, 0x404040, tc, tr, tf);

	tc[0] = 0.0;
	tc[1] = y;
	tr[0] = 1.0;
	tr[1] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0+tc[1];
	carvesolid2d_rect(win, 0x404040, tc, tr, tf);
}
void corner_vbo_drag_rightbot(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
	carvesolid2d_triangle(win, 0x808080, tc, tr, tf);
	tc[0] = 1.0;
	tc[1] = -1.0;
	carvesolid2d_triangle(win, 0x000000, tc, tr, tf);
}
void corner_vbo_drag(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int j;
	float w,h,c;
	float x0,y0,xn,yn;

	w = win->width;
	h = win->height;
	if(w<h)c = w / 32;
	else c = h / 32;

	if(win->input[10].z0)j = 10;
	else if(win->input[0].z0)j = 0;
	else return;

	x0 = win->input[j].x0;
	y0 = win->input[j].y0;
	xn = win->input[j].xn;
	yn = win->input[j].yn;

	if(y0 < c){
		if(x0 < c){
			corner_vbo_drag_lefttop(
				act, pin, win, sty,
				xn/w - 1.0, 1.0 - yn/h
			);
		}
		else if(x0+c > w){
			corner_vbo_drag_righttop(
				act, pin, win, sty,
				2.0*xn/w - 1.0, 1.0 - 2.0*yn/h
			);
		}
	}
	else if(y0+c > h){
		if(x0 < c){
			corner_vbo_drag_leftbot(
				act, pin, win, sty,
				xn/w - 1.0, -yn/h
			);
		}
		else if(x0+c > w){
			x0 = ((h-yn)*(h-yn)/(xn-w) + (w+xn)) / 2.0;
			x0 = 2*x0/w - 1.0;
			y0 = ((xn-w)*(xn-w)/(yn-h) + (yn+h)) / 2.0;
			y0 = 1.0 - 2*y0/h;
			corner_vbo_drag_rightbot(
				act, pin, win, sty,
				x0, y0, 2.0*xn/w - 1.0, 1.0 - 2.0*yn/h
			);
		}
	}
}




void corner_vbo_circle(
	struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, int t)
{
	vec3 tr, tf;
	if(1 == t){
		carvesolid2d_circle(win, rgb, vc, vr, vf);
		return;
	}

	tr[0] = vr[0]*t;
	tr[1] = vr[1]*t;
	tr[2] = vr[2]*t;
	tf[0] = vf[0]*t;
	tf[1] = vf[1]*t;
	tf[2] = vf[2]*t;
	carvesolid2d_circle(win, rgb, vc, tr, tf);
}
void corver_vbo_hover(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int j;
	float w,h,c;
	float x0,y0,xn,yn;
	vec3 vc;
	vec3 vr;
	vec3 vf;

	w = win->width;
	h = win->height;
	if(w<h)c = w / 32;
	else c = h / 32;

	x0 = win->input[10].x0;
	y0 = win->input[10].y0;
	xn = win->input[10].xn;
	yn = win->input[10].yn;

	vr[0] = 2*(float)c / (float)w;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 2*(float)c / (float)h;
	vf[2] = 0.0;

	vc[0] = 1.0;
	vc[1] = -1.0;
	vc[2] = -0.99;
	if((xn+c > w)&&(yn+c > h))j = 4;
	else j = 1;
	corner_vbo_circle(win, 0x0000ff, vc, vr, vf, j);

	vc[0] = -1.0;
	if((xn < c)&&(yn+c > h))j = 4;
	else j = 1;
	corner_vbo_circle(win, 0xff0000, vc, vr, vf, j);

	vc[1] = 1.0;
	if((xn < c)&&(yn < c))j = 4;
	else j = 1;
	corner_vbo_circle(win, 0x00ffff, vc, vr, vf, j);

	vc[0] = 1.0;
	if((xn+c > w)&&(yn < c))j = 4;
	else j = 1;
	corner_vbo_circle(win, 0xffff00, vc, vr, vf, j);
}




void corner_vbo_popup(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int j,rgb;
	vec3 tc,tr,tf;
	struct relation* rel;
	struct style* st;

	struct arena* ar;
	struct actor* ac;

	rel = act->orel0;
	if(0 == rel)return;

	st = (void*)(rel->srcfoot);
	if(0 == st)return;
	if('#' == st->uc[3])return;

	rel = win->orel0;
	while(1){
		if(0 == rel)break;
		if(_win_ == rel->dsttype){
			ar = (void*)(rel->dstchip);
			if(_ui3d_ == ar->fmt)goto found;
			if(_ui2d_ == ar->fmt)goto found;
		}
		rel = samesrcnextdst(rel);
	}
	return;

found:
	tr[0] = 0.0;
	tr[1] = -1.0;
	tr[2] = 0.0;
	tf[0] = -1.0;
	tf[1] = 0.0;
	tf[2] = 0.0;
	tc[0] = -1.0;
	tc[1] = -1.0;
	tc[2] = -0.9;
	carveopaque2d_bezier(win, 0xbfffffff, tr, tf, tc);

	tr[0] = 0.0;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0/33;
	tf[2] = 0.0;
	tc[0] = 0.0;
	tc[1] = 0.0;
	tc[2] = -0.9;

	j = 0;
	rel = ar->orel0;
	while(1){
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			ac = (void*)(rel->dstchip);
			st = (void*)(rel->srcfoot);
			if('#' == st->uc[3])rgb = 0x404040;
			else rgb = 0xffffff;

			tc[0] = -7.0/8;
			tc[1] = (-31+j*2)/32.0;
			tr[0] = 1.0/8;
			carveopaque2d_rect(win, 0x7f000080, tc, tr, tf);

			tr[0] = 1.0/32;
			carvestring2d_center(win, rgb, tc, tr, tf, (void*)&ac->fmt, 8);

			j += 1;
			if(j >= 16)break;
		}

		rel = samesrcnextdst(rel);
	}
}




void corner_read_vbo(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	corver_vbo_hover(act, pin, win, sty);

	corner_vbo_drag(act, pin, win, sty);

	corner_vbo_popup(act, pin, win, sty);
}
void corner_read_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int x0,y0,xn,yn;
	int w,h,c,t;

	w = win->width;
	h = win->height;
	if(w<h)c = w>>6;
	else c = h>>6;


//---------hover
	x0 = win->input[10].x0;
	y0 = win->input[10].y0;
	xn = win->input[10].xn;
	yn = win->input[10].yn;

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
	if(win->input[10].z0)t = 10;
	else if(win->input[0].z0)t = 0;
	else return;

	x0 = win->input[t].x0;
	y0 = win->input[t].y0;
	xn = win->input[t].xn;
	yn = win->input[t].yn;

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
}
static int corner_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	switch(win->fmt)
	{
		case _vbo_:corner_read_vbo(act, pin, win, sty);break;
		default:corner_read_pixel(act, pin, win, sty);
	}
	return 0;
}




static int corner_swrite_twig(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	struct relation* rel;
	struct arena* ar;
	struct style* st;
	int x,y,j;

	rel = act->orel0;
	if(0 == rel)return 0;

	st = (void*)(rel->srcfoot);
	if(0 == st)return 0;

	int w = win->width;
	int h = win->height;

	short* t = (void*)ev;
	if(t[0] > w/8)return 0;
	if(t[1] < h/2)return 0;

	rel = win->orel0;
	while(1){
		if(0 == rel)break;
		if(_win_ == rel->dsttype){
			ar = (void*)(rel->dstchip);
			if(_ui3d_ == ar->fmt)goto found;
			if(_ui2d_ == ar->fmt)goto found;
		}
		rel = samesrcnextdst(rel);
	}

found:
	x = 0;
	y = (h-t[1])*32/h;
	rel = ar->orel0;
	while(1){
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			if(x == y){
				st = (void*)(rel->srcfoot);

				j = st->uc[3];
				if('#' == j)j = 0;
				else j = '#';
				st->uc[3] = j;

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
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	struct relation* rel;
	struct style* st;
	int j;

	rel = act->orel0;
	if(0 == rel)return 0;

	st = (void*)(rel->srcfoot);
	if(0 == st)return 0;

	j = st->uc[3];
	if('#' == j)j = 0;
	else j = '#';
	st->uc[3] = j;

	return 1;
}
static int corner_swrite_root(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
static int corner_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int j;
	if(0x2d70 == ev->what)
	{
		j = corner_swrite_root(act, pin, win, sty, ev, len);
		if(j)return 1;

		j = corner_swrite_twig(act, pin, win, sty, ev, len);
		if(j)return 1;
	}
	return 0;
}
static int corner_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	return 0;
}
static int corner_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	return 0;
}
static int corner_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int corner_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int corner_delete(struct arena* win)
{
	return 0;
}
static int corner_create(struct actor* act, u8* str)
{
	struct actor* ac = allocactor();
	struct style* sty = allocstyle();
	sty->uc[3] = 0;
	relationcreate(ac, 0, _act_, act, sty, _act_);
	return 0;
}




void corner_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'o', 'r', 'n', 'e', 'r', 0, 0);

	p->oncreate = (void*)corner_create;
	p->ondelete = (void*)corner_delete;
	p->onstart  = (void*)corner_start;
	p->onstop   = (void*)corner_stop;
	p->oncread  = (void*)corner_cread;
	p->oncwrite = (void*)corner_cwrite;
	p->onsread  = (void*)corner_sread;
	p->onswrite = (void*)corner_swrite;
}
