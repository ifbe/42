#include "libuser.h"
#define DATBUF buf0
#define DATLEN data1
int qrcode_generate(void* src, void* dst, int len);




static void qrcode_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u8* sbuf;
	u32 color;
	int x,y,slen;
	int x1,y1,x2,y2;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
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
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc, tr, tf, tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void qrcode_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void qrcode_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	int width = win->width;
	int height = win->height;
	u8* p = (u8*)(win->textbuf);

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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("qrcode(%x,%x,%x)\n",win,act,sty);
}




static void qrcode_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		qrcode_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void qrcode_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
		case _rgba_:qrcode_draw_pixel(ent, slot, wnd, area);break;
		default:qrcode_read_bycam(ent,foot, stack,sp, arg,key);
	}
}
static void qrcode_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void qrcode_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void qrcode_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void qrcode_search(struct entity* act)
{
}
static void qrcode_modify(struct entity* act)
{
}
static void qrcode_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf0);
}
static void qrcode_create(struct entity* act)
{
	if(0 == act)return;
	act->DATBUF = memorycreate(49*49, 0);
	act->DATLEN = 49;
	qrcode_generate("haha", act->DATBUF, act->DATLEN);
}




void qrcode_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('q', 'r', 'c', 'o', 'd', 'e', 0, 0);

	p->oncreate = (void*)qrcode_create;
	p->ondelete = (void*)qrcode_delete;
	p->onsearch = (void*)qrcode_search;
	p->onmodify = (void*)qrcode_modify;

	p->onlinkup = (void*)qrcode_linkup;
	p->ondiscon = (void*)qrcode_discon;
	p->ontaking = (void*)qrcode_taking;
	p->ongiving = (void*)qrcode_giving;
}
