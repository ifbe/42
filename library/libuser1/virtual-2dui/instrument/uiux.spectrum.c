#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define slice 16


struct privdata{
	volatile int TABLEN;
	void* TABBUF[];
};




static void spectrum_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spectrum_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int x,y;
	int r,g,b;
	float dx,dy,dz;
	short* buf;

	vec3 ta,tb;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_prism4(ctx, 0xffff00, vc, vr, vf, vu);

	struct privdata* priv = (void*)act->priv_256b;
	int now = priv->TABLEN;
	int idx;
	for(y=0;y<slice;y++){
		idx = (slice-1+now-y)%slice;
		buf = priv->TABBUF[idx];
		//logtoall("%d:%d\n",y,idx);
		if(0 == buf)break;

		for(x=0;x<512;x++){
			dx = (x-255.5) / 255.5;
			dy = y*2.0/slice-1.0;
			ta[0] = vc[0] + vr[0] * dx + vf[0] * dy;
			ta[1] = vc[1] + vr[1] * dx + vf[1] * dy;
			ta[2] = vc[2] + vr[2] * dx + vf[2] * dy;

			dz = 10 * buf[x] / 32768.0;
			tb[0] = ta[0] + vu[0] * dz;
			tb[1] = ta[1] + vu[1] * dz;
			tb[2] = ta[2] + vu[2] * dz;

			r = buf[x] > 3276 ? 255 : buf[x]*255/3276;
			b = 255-r;
			gl41line(ctx, (r<<16)|b, ta, tb);
		}
	}
}
static void spectrum_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spectrum_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spectrum_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spectrum_draw_tui256(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u8* tuibuf = win->tuitext.buf;
	int tuiwidth = win->whdf.width;
	int tuiheight= win->whdf.height;

	struct privdata* priv = (void*)act->priv_256b;
	int now = priv->TABLEN;

	int idx = (slice-1+now)%slice;
	short* buf = priv->TABBUF[idx];
	if(0 == buf)return;

	int x,y,k;
	for(x=0;x<64;x++){
		int sum = 0;
		for(k=0;k<8;k++)sum += buf[x*8+k];

		y = 20-sum/16;
		if(y<0)y=0;
		if(y+1>tuiheight)y = tuiheight-1;

		tuibuf[4*(tuiwidth*y+x)+0] = 'a';
		tuibuf[4*(tuiwidth*y+x)+2] = 7;
	}
}
static void spectrum_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("spectrum(%x,%x,%x)\n",win,act,sty);
}




void spectrum_data(_obj* act, void* type, void* buf, int len)
{
	struct privdata* priv = (void*)act->priv_256b;
	logtoall("@spectrum_write.pcm: %d,%llx\n", len, buf);

	priv->TABBUF[priv->TABLEN] = buf;
	priv->TABLEN = (priv->TABLEN+1) % slice;
}




static void spectrum_take_bywnd(_obj* ent,void* slot, _obj* wnd, void* area, _syn* stack,int sp)
{
	switch(wnd->vfmt){
	case _cli_:
		break;
	case _tui_:
		spectrum_draw_tui(ent,0, wnd, area);
		break;
	case _tui256_:
		spectrum_draw_tui256(ent,0, wnd, area);
		break;
	case _rgba8888_:
		spectrum_draw_pixel(ent,0, wnd, area);
		break;
	}
}

static void spectrum_take_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	spectrum_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void spectrum_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	if(0 == stack)return;

	switch(wnd->type){
	case _wnd_:
	case _render_:
		spectrum_take_bywnd(ent,slot, wnd,area, stack,sp);
		break;
	default:
		spectrum_take_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void spectrum_giving(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	switch(stack[sp-1].foottype){
	case _pcm_:
		spectrum_data(ent,slot, buf,len);
		break;
	}
}
static void spectrum_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void spectrum_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@spectrum_attach\n");
}




static void spectrum_search(_obj* act)
{
}
static void spectrum_modify(_obj* act)
{
}
static void spectrum_delete(_obj* act)
{
}
static void spectrum_create(_obj* act)
{
}




void spectrum_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('s', 'p', 'e', 'c', 't', 'r', 'u', 'm');

	p->oncreate = (void*)spectrum_create;
	p->ondelete = (void*)spectrum_delete;
	p->onreader = (void*)spectrum_search;
	p->onwriter = (void*)spectrum_modify;

	p->onattach = (void*)spectrum_attach;
	p->ondetach = (void*)spectrum_detach;
	p->ontaking = (void*)spectrum_taking;
	p->ongiving = (void*)spectrum_giving;
}
