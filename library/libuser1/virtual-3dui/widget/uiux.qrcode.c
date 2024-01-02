#include "libuser.h"
#define DATBUF listptr.buf0
#define DATLEN listu64.data2
int qrcode_generate(void* src, void* dst, int len);




static void qrcode_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u8* sbuf;
	u32 color;
	int x,y,slen;
	int x1,y1,x2,y2;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	sbuf = act->DATBUF;
	slen = act->DATLEN;
	for(y=0;y<slen;y++)
	{
		for(x=0;x<slen;x++)
		{
			if( sbuf[(y*slen)+x] == 0 )color=0;
			else color=0xffffffff;

			x1 = cx + (x*ww*2/slen) - ww;
			y1 = cy + (y*hh*2/slen) - hh;
			x2 = cx + ((x+1)*ww*2/slen) - ww;
			y2 = cy + ((y+1)*hh*2/slen) - hh;
			drawsolid_rect(
				win, color,
				x1, y1, x2, y2
			);
		}
//say("\n");
	}
}
static void qrcode_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	vec3 tc, tr, tf, tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	//gl41opaque_rect(ctx, 0x444444, vc, vr, vf);

	tr[0] = vr[0] / 49;
	tr[1] = vr[1] / 49;
	tr[2] = vr[2] / 49;
	tf[0] = vf[0] / 49;
	tf[1] = vf[1] / 49;
	tf[2] = vf[2] / 49;

	int x,y,rgb;
	u8* sbuf = act->DATBUF;
	int slen = act->DATLEN;
	for(y=0;y<49;y++)
	{
		for(x=0;x<49;x++)
		{
			if( sbuf[(y*slen)+x] == 0 )rgb = 0;
			else rgb = 0xffffffff;

			tc[0] = vc[0] + (x-24)*vr[0]*2/49 + (y-24)*vf[0]*2/49;
			tc[1] = vc[1] + (x-24)*vr[1]*2/49 + (y-24)*vf[1]*2/49;
			tc[2] = vc[2] + (x-24)*vr[2]*2/49 + (y-24)*vf[2]*2/49 - 0.1;
			gl41solid_rect(ctx, rgb, tc, tr, tf);
		}
	}
}
static void qrcode_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void qrcode_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void qrcode_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y;
	int width = win->whdf.width;
	int height = win->whdf.height;
	u8* p = (u8*)(win->tuitext.buf);

	u8* sbuf = act->DATBUF;
	int slen = act->DATLEN;
	for(y=0;y<100;y++)
	{
		if(y >= slen)break;
		if(y >= height)break;
		for(x=0;x<100;x++)
		{
			if(x >= slen)break;
			if(x >= width/2)break;
			if( sbuf[(y*slen)+x] != 0 )continue;

			p[( (y*width+x*2)<<2 ) + 3] = 7;
			p[( (y*width+x*2)<<2 ) + 7] = 7;
		}
	}
}
static void qrcode_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("qrcode(%x,%x,%x)\n",win,act,sty);
}




static void qrcode_read_bycam(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	qrcode_draw_gl41(ent,foot, wor,geom, wnd,area);
}




static void qrcode_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->hfmt){
	case _rgba_:
		qrcode_draw_pixel(ent, foot, wnd, area);
		break;
	default:
		qrcode_read_bycam(ent,foot, stack,sp, arg,key);
		break;
	}
}
static void qrcode_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void qrcode_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void qrcode_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void qrcode_search(_obj* act)
{
}
static void qrcode_modify(_obj* act)
{
}
static void qrcode_delete(_obj* act)
{
	if(0 == act)return;
	if(act->DATBUF)memoryfree(act->DATBUF);
}
static void qrcode_create(_obj* act)
{
	if(0 == act)return;
	act->DATBUF = memoryalloc(49*49, 0);
	act->DATLEN = 49;
	qrcode_generate("haha", act->DATBUF, act->DATLEN);
}




void qrcode_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('q', 'r', 'c', 'o', 'd', 'e', 0, 0);

	p->oncreate = (void*)qrcode_create;
	p->ondelete = (void*)qrcode_delete;
	p->onreader = (void*)qrcode_search;
	p->onwriter = (void*)qrcode_modify;

	p->onattach = (void*)qrcode_attach;
	p->ondetach = (void*)qrcode_detach;
	p->ontaking = (void*)qrcode_taking;
	p->ongiving = (void*)qrcode_giving;
}
