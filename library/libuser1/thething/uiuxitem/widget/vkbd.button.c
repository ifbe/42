#include "libuser.h"
#define STRBUF buf0
void dx11data_01cam(struct entity* wnd);
//
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




void button_draw_pixel(
	struct entity* act, struct style* part,
	struct supply* ctx, struct style* area)
{
	int x,y;
	int time;
	int w = ctx->width;
	int h = ctx->height;
	u32* pix = ctx->rgbabuf;

	time = 511*(timeread()%1000000)/1000000;
	if(time > 255)time = 511-time;
	for(y=0;y<8;y++){
	for(x=0;x<8;x++){
		pix[y*w+x] = (0x010000*time) | (0x001f00*y) | (0x00001f*x);
	}
	}
}




void button_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	vec3 tc;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	for(j=0;j<3;j++)tc[j] = vc[j]-vt[j]/64;
	gl41opaque_rect(ctx, 0x40ffd010, tc, vr, vf);
	gl41string_center(ctx, 0xff0000, vc, vr ,vf, act->STRBUF, 0);
}
static void button_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(0 == stack)return;
	if('v' != key)return;

	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	button_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void button_read_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	gl41data_before(wnd);
	button_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);

	if(_dx11full_ == wnd->fmt)dx11data_01cam(wnd);
	else gl41data_01cam(wnd);
}




static void button_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct supply* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _rgba_:
		button_draw_pixel(ent, slot, (void*)wnd, area);
		break;
	case _dx11full_:
	case _mt20full_:
	case _gl41full_:
	case _vk12full_:
		if('v' != key)break;
		button_read_bywnd(ent,slot, (void*)wnd,area);
		break;
	default:
		button_read_bycam(ent,foot, stack,sp, arg,key);
	}
}
static void button_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct event* ev = buf;
	if(0x2b70 == ev->what)give_data_into_peer(ent,_evto_, stack,sp, 0,0, "1",1);
}
static void button_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void button_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void button_search(struct entity* act)
{
}
static void button_modify(struct entity* act)
{
}
static void button_delete(struct entity* act)
{
}
static void button_create(struct entity* act, u8* arg, int argc, u8** argv)
{
	int j;
	u8* dst;
	if(0 == arg){
		act->STRBUF = "button";
		return;
	}

	dst = act->STRBUF = memorycreate(0x1000, 0);
	for(j=0;j<0x1000;j++){
		if(arg[j] < 0x20)break;
		dst[j] = arg[j];
	}
	dst[j] = 0;
}




void button_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('b', 'u', 't', 't', 'o', 'n', 0, 0);

	p->oncreate = (void*)button_create;
	p->ondelete = (void*)button_delete;
	p->onsearch = (void*)button_search;
	p->onmodify = (void*)button_modify;

	p->onlinkup = (void*)button_linkup;
	p->ondiscon = (void*)button_discon;
	p->ontaking = (void*)button_taking;
	p->ongiving = (void*)button_giving;
}
