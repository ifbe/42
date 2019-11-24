#include "libuser.h"




static void stair_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}/*
static void stair_draw_vbo2d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int j;
	vec3 tc, tr, tf, tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carveline2d_rect(win, 0xffffff, vc, vr, vf);

	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(j=-7;j<8;j+=2)
	{
		tc[0] = vc[0] + vr[0]*j/8 + vf[0]*j/8;
		tc[1] = vc[1] + vr[1]*j/8 + vf[1]*j/8;
		tc[2] = vc[2] + vr[2]*j/8 + vf[2]*j/8;
		carvesolid2d_rect(win, 0x808080, tc, tr, tf);
	}
}*/
static void stair_draw_vbo3d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int z;
	vec3 tc, tr, tf, tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carveline_prism4(win, 0xffffff, vc, vr, vf, vu);

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	tu[0] = vu[0]/16;
	tu[1] = vu[1]/16;
	tu[2] = vu[2]/16;

	for(z=1;z<16;z+=2)
	{
		tc[0] = vc[0] + vr[0]/2 + vf[0]*(z-8)/8 + vu[0]*z/16;
		tc[1] = vc[1] + vr[1]/2 + vf[1]*(z-8)/8 + vu[1]*z/16;
		tc[2] = vc[2] + vr[2]/2 + vf[2]*(z-8)/8 + vu[2]*z/16;
		carvesolid_prism4(win, 0x808080, tc, tr, tf, tu);
	}
	for(z=1;z<16;z+=2)
	{
		tc[0] = vc[0] - vr[0]/2 + vf[0]*(z-8)/8 - vu[0]*z/16;
		tc[1] = vc[1] - vr[1]/2 + vf[1]*(z-8)/8 - vu[1]*z/16;
		tc[2] = vc[2] - vr[2]/2 + vf[2]*(z-8)/8 - vu[2]*z/16;
		carvesolid_prism4(win, 0x808080, tc, tr, tf, tu);
	}
}
static void stair_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stair_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stair_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stair_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stair_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)stair_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)stair_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)stair_draw_html(act, pin, win, sty);
	else if(fmt == _json_)stair_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)stair_draw_vbo2d(act, pin, win, sty);
		//else stair_draw_vbo3d(act, pin, win, sty);
	}
	else stair_draw_pixel(act, pin, win, sty);
}




static void stair_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct entity* ctx = buf;
	//say("@drone_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)stair_draw_vbo3d(act,pin,ctx,sty);
	}
	//stair_draw(act, pin, win, sty);
}
static void stair_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void stair_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void stair_start(struct halfrel* self, struct halfrel* peer)
{
}




static void stair_search(struct entity* act)
{
}
static void stair_modify(struct entity* act)
{
}
static void stair_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void stair_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void stair_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 't', 'a', 'i', 'r', 0, 0, 0);

	p->oncreate = (void*)stair_create;
	p->ondelete = (void*)stair_delete;
	p->onsearch = (void*)stair_search;
	p->onmodify = (void*)stair_modify;

	p->onstart = (void*)stair_start;
	p->onstop  = (void*)stair_stop;
	p->onread  = (void*)stair_read;
	p->onwrite = (void*)stair_write;
}
