#include "libuser.h"




struct perradar{
};
static void radar_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
}
static void radar_gl41draw(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	int j,k;

	vec4 tr,tf,tt;
	for(k=1;k<=10;k++){
		for(j=0;j<3;j++){
			tr[j] = vr[j]*k/10;
			tf[j] = vf[j]*k/10;
		}
		gl41line_circle(ctx, 0x101010*(13-k), vc, tr, tf);
	}

	float a,c,s;
	for(k=0;k<8;k++){
		a = k*tau/8;
		c = getcos(a);
		s = getsin(a);
		for(j=0;j<3;j++)tt[j] = vc[j] + vr[j]*c + vf[j]*s;
		gl41line(ctx, 0x808080, vc, tt);
	}
}
static void radar_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void radar_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void radar_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void radar_draw_tui256(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void radar_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("radar(%x,%x,%x)\n",win,act,sty);
}




static void radar_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	switch(wnd->vfmt){
	case _cli_:
		break;
	case _tui_:
		radar_draw_tui(ent,0, wnd, area);
		break;
	case _tui256_:
		radar_draw_tui256(ent,0, wnd, area);
		break;
	case _rgba8888_:
		radar_draw_pixel(ent,slot, wnd,area);
		break;
	}
}

static void radar_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->vfmt){
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		radar_gl41draw(ent,slot, wor,geom, wnd,area);
		break;
	}
}



static void radar_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
//logtoall("fmt=%.8s\n", &sup->type);

	switch(wnd->type){
	case _wnd_:
	case _render_:
		radar_read_bywnd(ent,slot, wnd,area);
		break;
	default:
		radar_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void radar_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void radar_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void radar_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@radar_attach\n");
}




static void radar_search(_obj* act)
{
}
static void radar_modify(_obj* act)
{
}
static void radar_delete(_obj* act)
{
}
static void radar_create(_obj* act, u8* arg)
{
}




void radar_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('r', 'a', 'd', 'a', 'r', 0, 0, 0);

	p->oncreate = (void*)radar_create;
	p->ondelete = (void*)radar_delete;
	p->onreader = (void*)radar_search;
	p->onwriter = (void*)radar_modify;

	p->onattach = (void*)radar_attach;
	p->ondetach = (void*)radar_detach;
	p->ontaking = (void*)radar_taking;
	p->ongiving = (void*)radar_giving;
}
