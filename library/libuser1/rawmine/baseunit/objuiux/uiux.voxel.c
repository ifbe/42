#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define slice 16




static void voxel_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void voxel_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y,z;
	int r,g,b;
	float dx,dy,dz,dw;
	void** tab;
	short* buf;

	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carveline_prism4(win, 0xffff00, vc, vr, vf, vu);

	tab = act->buf;
	if(0 == tab)return;
	//printmemory(tab, 8*4);

	buf = tab[0];
	if(0 == buf)return;

	for(z=0;z<10;z++){
	for(y=0;y<10;y++){
	for(x=0;x<10;x++){
		dw = buf[z*100 + y*10 + x];
		if(dw < 8)continue;

		dx = x / 5.0 - 0.9;
		dy = y / 5.0 - 0.9;
		dz = z / 5.0 - 0.9;
		dw = dw / 327680.0;
		tc[0] = vc[0] + vr[0]*dx + vf[0]*dy + vu[0]*dz;
		tc[1] = vc[1] + vr[1]*dx + vf[1]*dy + vu[1]*dz;
		tc[2] = vc[2] + vr[2]*dx + vf[2]*dy + vu[2]*dz;
		tr[0] = vr[0]*dw;
		tr[1] = vr[1]*dw;
		tr[2] = vr[2]*dw;
		tf[0] = vf[0]*dw;
		tf[1] = vf[1]*dw;
		tf[2] = vf[2]*dw;
		tu[0] = vu[0]*dw;
		tu[1] = vu[1]*dw;
		tu[2] = vu[2]*dw;

		r = z*26+13;
		g = y*26+13;
		b = x*26+13;
		carvesolid_prism4(win, (r<<16)|(g<<8)|b, tc, tr, tf, tu);
	}
	}
	}
}
static void voxel_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void voxel_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"voxel\" style=\"width:50%%;height:100px;float:left;background-color:#7ae129;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void voxel_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void voxel_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("voxel(%x,%x,%x)\n",win,act,sty);
}
static void voxel_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(0 == act->buf)return;

	if(fmt == _cli_)voxel_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)voxel_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)voxel_draw_html(act, pin, win, sty);
	else if(fmt == _json_)voxel_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)voxel_draw_vbo2d(act, pin, win, sty);
		//else voxel_draw_vbo3d(act, pin, win, sty);
	}
	else voxel_draw_pixel(act, pin, win, sty);
}




void voxel_data(struct actor* act, int type, void* buf, int len)
{
	int j,idx;
	void** tab;
	say("@voxel_write.pcm: %d,%llx\n", len, buf);

	idx = act->len;
	tab = act->buf;

	for(j=15;j>0;j--)tab[j] = tab[j-1];
	tab[0] = buf;

	act->len = (idx+1) % slice;
}




static void voxel_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	if(ctx){
		if(_gl41data_ == ctx->type)voxel_draw_vbo3d(act,pin,ctx,sty);
	}
}
static void voxel_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	//struct actor* win = (void*)(peer->chip);
	//struct style* sty = (void*)(peer->foot);
	if(_pcm_ == self->flag){
		voxel_data(act, 0, buf, len);
	}
}
static void voxel_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void voxel_start(struct halfrel* self, struct halfrel* peer)
{
	say("@voxel_start\n");
}




static void voxel_search(struct actor* act)
{
}
static void voxel_modify(struct actor* act)
{
}
static void voxel_delete(struct actor* act)
{
}
static void voxel_create(struct actor* act)
{
	act->buf = memorycreate(0x1000, 0);
	act->len = 0;
}




void voxel_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('v', 'o', 'x', 'e', 'l', 0, 0, 0);

	p->oncreate = (void*)voxel_create;
	p->ondelete = (void*)voxel_delete;
	p->onsearch = (void*)voxel_search;
	p->onmodify = (void*)voxel_modify;

	p->onstart = (void*)voxel_start;
	p->onstop  = (void*)voxel_stop;
	p->onread  = (void*)voxel_read;
	p->onwrite = (void*)voxel_write;
}
