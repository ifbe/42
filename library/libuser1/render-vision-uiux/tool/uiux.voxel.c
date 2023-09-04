#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define slice 16
#define TABBUF listptr.buf0
#define TABLEN listu64.data2




static void voxel_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void voxel_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,z;
	int r,g,b;
	float dx,dy,dz,dw;
	void** tab;
	short* buf;

	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_prism4(wnd, 0xffff00, vc, vr, vf, vu);

	tab = act->TABBUF;
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
		gl41solid_prism4(wnd, (r<<16)|(g<<8)|b, tc, tr, tf, tu);
	}
	}
	}
}
static void voxel_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void voxel_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void voxel_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void voxel_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("voxel(%x,%x,%x)\n",win,act,sty);
}




void voxel_data(_obj* act, int type, void* buf, int len)
{
	int j,idx;
	void** tab;
	say("@voxel_write.pcm: %d,%llx\n", len, buf);

	idx = act->TABLEN;
	tab = act->TABBUF;

	for(j=15;j>0;j--)tab[j] = tab[j-1];
	tab[0] = buf;

	act->TABLEN = (idx+1) % slice;
}




static void voxel_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	voxel_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void voxel_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _tui_:
		break;
	case _rgba_:
		break;
	case _htmlroot_:
		break;
	case _gl41list_:
		break;
	case _dx11list_:
	case _mt20list_:
	case _vk12list_:
		say("caller@%p\n", caller);
		break;
	default:
		voxel_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void voxel_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	switch(stack[sp-1].foottype){
	case _pcm_:
		voxel_data(ent, 0, buf, len);
		break;
	}
}
static void voxel_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void voxel_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@voxel_attach\n");
}




static void voxel_search(_obj* act)
{
}
static void voxel_modify(_obj* act)
{
}
static void voxel_delete(_obj* act)
{
}
static void voxel_create(_obj* act)
{
	act->TABBUF = memorycreate(0x1000, 0);
	act->TABLEN = 0;
}




void voxel_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('v', 'o', 'x', 'e', 'l', 0, 0, 0);

	p->oncreate = (void*)voxel_create;
	p->ondelete = (void*)voxel_delete;
	p->onreader = (void*)voxel_search;
	p->onwriter = (void*)voxel_modify;

	p->onattach = (void*)voxel_attach;
	p->ondetach = (void*)voxel_detach;
	p->ontaking = (void*)voxel_taking;
	p->ongiving = (void*)voxel_giving;
}
