#include "libuser.h"
#define STRBUF buf0




void button_draw_pixel(
	struct entity* act, struct style* part,
	struct entity* ctx, struct style* area)
{
	int x,y;
	int time;
	int w = ctx->width;
	int h = ctx->height;
	u32* pix = ctx->buf;

	time = 511*(timeread()%1000000)/1000000;
	if(time > 255)time = 511-time;
	for(y=0;y<8;y++){
	for(x=0;x<8;x++){
		pix[y*w+x] = (0x010000*time) | (0x001f00*y) | (0x00001f*x);
	}
	}
}




void button_draw_vbo(
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
	carveopaque_rect(ctx, 0x40ffd010, tc, vr, vf);
	carvestring_center(ctx, 0xff0000, vc, vr ,vf, act->STRBUF, 0);
}
static void button_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* camg;

//world -> button
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		act = self->pchip;part = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wor = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)button_draw_vbo(act,part, win,geom, wnd,area);
	}
}




static void button_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* ent = self->pchip;
	struct entity* sup = peer->pchip;
	if(_rgba_ == sup->fmt)button_draw_pixel(ent, self->pfoot, sup, peer->pfoot);
	else button_read_bycam(self, peer, arg, idx, buf, len);
}
static void button_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* ent = self->pchip;
	say("@button_write\n");
	relationwrite(ent,_ev_, 0, 0, "calibrate\n", 10);
}
static void button_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void button_start(struct halfrel* self, struct halfrel* peer)
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

	p->onstart = (void*)button_start;
	p->onstop  = (void*)button_stop;
	p->onread  = (void*)button_read;
	p->onwrite = (void*)button_write;
}
