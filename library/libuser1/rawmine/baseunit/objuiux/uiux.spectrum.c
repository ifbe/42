#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define slice 16




static void spectrum_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void spectrum_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y;
	int r,g,b;
	float dx,dy,dz;
	void** tab;
	short* buf;

	vec3 ta,tb;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carveline_prism4(win, 0xffff00, vc, vr, vf, vu);

	tab = act->buf;
	if(0 == tab)return;
	//printmemory(tab, 8*4);

	for(y=0;y<slice;y++){
		buf = tab[y];
		if(0 == buf)break;

		for(x=0;x<1024;x++){
			dx = x / 512.0 - 1.0;
			dy = y*2.0/slice-1.0;
			ta[0] = vc[0] + vr[0] * dx + vf[0] * dy;
			ta[1] = vc[1] + vr[1] * dx + vf[1] * dy;
			ta[2] = vc[2] + vr[2] * dx + vf[2] * dy;

			dz = buf[x] / 32768.0;
			tb[0] = ta[0] + vu[0] * dz;
			tb[1] = ta[1] + vu[1] * dz;
			tb[2] = ta[2] + vu[2] * dz;

			r = (x/100)*26+13;
			g = ((x%100)/10)*26+13;
			b = (x%10)*26+13;
			carveline(win, (r<<16)|(g<<8)|b, ta, tb);
		}
	}
}
static void spectrum_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void spectrum_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"spectrum\" style=\"width:50%%;height:100px;float:left;background-color:#7ae129;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void spectrum_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void spectrum_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("spectrum(%x,%x,%x)\n",win,act,sty);
}
static void spectrum_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(0 == act->buf)return;

	if(fmt == _cli_)spectrum_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)spectrum_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)spectrum_draw_html(act, pin, win, sty);
	else if(fmt == _json_)spectrum_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)spectrum_draw_vbo2d(act, pin, win, sty);
		//else spectrum_draw_vbo3d(act, pin, win, sty);
	}
	else spectrum_draw_pixel(act, pin, win, sty);
}




void spectrum_data(struct actor* act, int type, void* buf, int len)
{
	int j,idx;
	void** tab;
	say("@spectrum_write.pcm: %d,%llx\n", len, buf);

	idx = act->len;
	tab = act->buf;

	for(j=15;j>0;j--)tab[j] = tab[j-1];
	tab[0] = buf;

	act->len = (idx+1) % slice;
}




static void spectrum_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	if(ctx){
		if(_gl41data_ == ctx->type)spectrum_draw_vbo3d(act,pin,ctx,sty);
	}
}
static void spectrum_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	//struct actor* win = (void*)(peer->chip);
	//struct style* sty = (void*)(peer->foot);
	if(_pcm_ == self->flag){
		spectrum_data(act, 0, buf, len);
	}
}
static void spectrum_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void spectrum_start(struct halfrel* self, struct halfrel* peer)
{
	say("@spectrum_start\n");
}




static void spectrum_search(struct actor* act)
{
}
static void spectrum_modify(struct actor* act)
{
}
static void spectrum_delete(struct actor* act)
{
}
static void spectrum_create(struct actor* act)
{
	act->buf = memorycreate(0x1000, 0);
	act->len = 0;
}




void spectrum_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'p', 'e', 'c', 't', 'r', 'u', 'm');

	p->oncreate = (void*)spectrum_create;
	p->ondelete = (void*)spectrum_delete;
	p->onsearch = (void*)spectrum_search;
	p->onmodify = (void*)spectrum_modify;

	p->onstart = (void*)spectrum_start;
	p->onstop  = (void*)spectrum_stop;
	p->onread  = (void*)spectrum_read;
	p->onwrite = (void*)spectrum_write;
}
