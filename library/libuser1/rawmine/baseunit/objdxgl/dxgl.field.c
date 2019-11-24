#include "libuser.h"
#define COUNT (0x100000/36)




static void field_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void field_draw_vbo3d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y,z;
	float tx,ty,tl;
	float ax,ay,az;
	float dx,dy,dz;
	float* vec;

	vec3 ta,tb;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vt = sty->f.vt;
	carveline_prism4(win, 0xffffff, vc, vr, vf, vt);

	vec = act->buf;
	for(z=0;z<20;z++){
	for(y=0;y<20;y++){
	for(x=0;x<20;x++){

		//field
		ax = x/10.0-0.95;
		ay = y/10.0-0.95;
		az = z/10.0-0.95;

		ta[0] = vc[0] + vr[0]*ax + vf[0]*ay + vt[0]*az;
		ta[1] = vc[1] + vr[1]*ax + vf[1]*ay + vt[1]*az;
		ta[2] = vc[2] + vr[2]*ax + vf[2]*ay + vt[2]*az;

		tx = ta[0] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		ty = ta[1] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		tl = squareroot(ta[2]*ta[2] + tx*tx + ty*ty) / 10000.0;
		dx = tx * tl;
		dy = ty * tl;
		dz =-ta[2] * tl;
		tb[0] = ta[0] + dx - ta[1]/1000;
		tb[1] = ta[1] + dy + ta[0]/1000;
		tb[2] = ta[2] + dz;

		carveline_arrow(win, 0x007f7f, ta, tb, vt);


		//particle
		ta[0] = vec[(z*20*20 + y*20 + x)*3 + 0];
		ta[1] = vec[(z*20*20 + y*20 + x)*3 + 1];
		ta[2] = vec[(z*20*20 + y*20 + x)*3 + 2];

		tx = ta[0] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		ty = ta[1] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		tl = squareroot(ta[2]*ta[2] + tx*tx + ty*ty) / 100000.0;
		dx = tx * tl;
		dy = ty * tl;
		dz =-ta[2] * tl;
		vec[(z*20*20 + y*20 + x)*3 + 0] += dx - ta[1]/1000;
		vec[(z*20*20 + y*20 + x)*3 + 1] += dy + ta[0]/1000;
		vec[(z*20*20 + y*20 + x)*3 + 2] += dz;

		carvepoint(win, 0xffffff, ta);
	}
	}
	}
}
static void field_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void field_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void field_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void field_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("field(%x,%x,%x)\n",win,act,sty);
}
static void field_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)field_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)field_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)field_draw_html(act, pin, win, sty);
	else if(fmt == _json_)field_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)field_draw_vbo2d(act, pin, win, sty);
		//else field_draw_vbo3d(act, pin, win, sty);
	}
	else field_draw_pixel(act, pin, win, sty);
}




static void field_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct entity* ctx = buf;
	//say("@texball_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)field_draw_vbo3d(act,pin,ctx,sty);
	}
	//field_draw(act, pin, win, sty);
}
static void field_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void field_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void field_start(struct halfrel* self, struct halfrel* peer)
{
	int x,y,z;
	float ax,ay,az;

	struct entity* act = (void*)(self->chip);
	float* vec = act->buf;

	struct fstyle* sty = (void*)(peer->foot);
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vt;

	for(z=0;z<20;z++){
	for(y=0;y<20;y++){
	for(x=0;x<20;x++){
		ax = x/10.0-0.95;
		ay = y/10.0-0.95;
		az = z/10.0-0.95;

		vec[(z*20*20 + y*20 + x)*3 + 0] = vc[0] + vr[0]*ax + vf[0]*ay + vu[0]*az;
		vec[(z*20*20 + y*20 + x)*3 + 1] = vc[1] + vr[1]*ax + vf[1]*ay + vu[1]*az;
		vec[(z*20*20 + y*20 + x)*3 + 2] = vc[2] + vr[2]*ax + vf[2]*ay + vu[2]*az;
	}
	}
	}
}




static void field_search(struct entity* act)
{
}
static void field_modify(struct entity* act)
{
}
static void field_delete(struct entity* act)
{
}
static void field_create(struct entity* act)
{
	float* buf = act->buf = memorycreate(0x100000, 0);
}




void field_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('f', 'i', 'e', 'l', 'd', 0, 0, 0);

	p->oncreate = (void*)field_create;
	p->ondelete = (void*)field_delete;
	p->onsearch = (void*)field_search;
	p->onmodify = (void*)field_modify;

	p->onstart = (void*)field_start;
	p->onstop  = (void*)field_stop;
	p->onread  = (void*)field_read;
	p->onwrite = (void*)field_write;
}
