#include "libuser.h"
#define _euler_ hex32('e','u','l','r')
#define _quat_ hex32('q','u','a','t')
#define _real_ hex32('r','e','a','l')
#define _imag_ hex32('i','m','a','g')


struct privdata{
	_obj* worldobj;
	void* worldgeom;
};


static void aircraft_forgl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	struct fmotion* physic = &geom->actual;
	float* q = physic->angular_x;

	vec3 vr;
	vr[0] = (1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0);
	vr[1] = (2.0 * (q[0]*q[1] + q[2]*q[3]));
	vr[2] = (2.0 * (q[0]*q[2] - q[1]*q[3]));
	vec3_setlen(vr, vec3_getlen(geom->fshape.vr));

	vec3 vf;
	vf[0] = (2.0 * (q[0]*q[1] - q[2]*q[3]));
	vf[1] = (1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0);
	vf[2] = (2.0 * (q[1]*q[2] + q[0]*q[3]));
	vec3_setlen(vf, vec3_getlen(geom->fshape.vf));

	vec3 vt;
	vt[0] = (2.0 * (q[0]*q[2] + q[1]*q[3]));
	vt[1] = (2.0 * (q[1]*q[2] - q[0]*q[3]));
	vt[2] = (1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0);
	vec3_setlen(vt, vec3_getlen(geom->fshape.vt));

	vec3 vc;
	vc[0] = physic->displace_x[0];
	vc[1] = physic->displace_x[1];
	vc[2] = physic->displace_x[2];
/*
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41line_rect(ctx, 0x00ff00, vc, vr, vf);
	gl41line_prism4(ctx, 0x00ff00, vc, vr, vf, vt);
*/
	int j;
	vec3 tc,tr,tf,tt;

	//tail
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vf[j]*7/8 + vt[j]*3/4;
		tr[j] = vr[j]/32;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/4;
	}
	gl41solid_prism4(ctx, 0x404040, tc,tr,tf,tt);

	//wing
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/2;
		tr[j] = vr[j];
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/32;
	}
	gl41solid_prism4(ctx, 0x404040, tc,tr,tf,tt);

	//body
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] =-vt[j]/8;
		tt[j] = vf[j];
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tr, tf, tt);

	//wheel
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vf[j]/2 + vt[j]*1/8;
		tr[j] = vf[j]/8;
		tf[j] = vt[j]/8;
		tt[j] = vr[j]/32;
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tr, tf, tt);		//front
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vr[j]/16 - vf[j]/4 + vt[j]*1/8;
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tr, tf, tt);		//rear left
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j]/16 - vf[j]/4 + vt[j]*1/8;
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tr, tf, tt);		//rear right
}
static void aircraft_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}
}
static void aircraft_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void aircraft_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void aircraft_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void aircraft_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void aircraft_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor = stack[sp-2].pchip;
	struct style* geom = stack[sp-2].pfoot;
	_obj* wnd = stack[sp-6].pchip;
	struct style* area = stack[sp-6].pfoot;
	aircraft_forgl41(ent,slot, wor,geom, wnd,area);
}
static void aircraft_write_quaternion(struct style* sty, float* q)
{
	//logtoall("aircraft_write_quaternion: %f,%f,%f,%f\n", q[0], q[1], q[2], q[3]);
	int j;
	for(int j=0;j<4;j++)sty->fmotion.angular_x[j] = q[j];
}




static void aircraft_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		aircraft_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void aircraft_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	struct privdata* priv = (void*)ent->priv_256b;

	u32 type = stack[sp-1].foottype;
	switch(type){
	case _quat_:
		aircraft_write_quaternion(priv->worldgeom, buf);
		break;
	}
}
static void aircraft_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void aircraft_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* me = self->pchip;
	_obj* you = peer->pchip;
	struct privdata* priv = (void*)me->priv_256b;

	switch(you->type){
	case _virtual_:
		priv->worldobj = you;
		priv->worldgeom = peer->pfoot;
		break;
	}
}




static void aircraft_modify(_obj* act)
{
}
static void aircraft_search(_obj* act)
{
}
static void aircraft_delete(_obj* act)
{
}
static void aircraft_create(_obj* act)
{
}




void aircraft_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('a', 'i', 'r', 'c', 'r', 'a', 'f', 't');

	p->oncreate = (void*)aircraft_create;
	p->ondelete = (void*)aircraft_delete;
	p->onreader = (void*)aircraft_search;
	p->onwriter = (void*)aircraft_modify;

	p->onattach = (void*)aircraft_attach;
	p->ondetach = (void*)aircraft_detach;
	p->ontaking = (void*)aircraft_taking;
	p->ongiving = (void*)aircraft_giving;
}
