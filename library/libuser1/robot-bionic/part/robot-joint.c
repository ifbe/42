#include "libuser.h"
void quaternion4axisandangle(float* q, float* a, float angle);
void quaternion_rotatefrom(float* o, float* v, float* q);



struct privdata{
	float angle;
};
static void robotjoint_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	struct privdata* own = (void*)act->priv_256b;

	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	vec3 tc,tr,tf,tt;
	int j;


	//stator
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vt[j]/2;
		tt[j] = vt[j]/2;
	}
	gl41solid_cylinder(ctx, 0x444444, tc, vr, vf, tt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vt[j];
		tr[j] = tc[j] + vr[j];
	}
	gl41line(ctx, 0xff, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vt[j];
		tr[j] = tc[j] + vf[j];
	}
	gl41line(ctx, 0xff00ff, tc, tr);


	//rotor
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/2;
		tt[j] = vt[j]/2;
	}
	gl41solid_cylinder(ctx, 0x888888, tc, vr, vf, tt);

	float a = own->angle*PI/180;
	vec4 q;
	quaternion4axisandangle(q, vt, a);

	//rotor.r
	vec3 vec;
	quaternion_rotatefrom(vec, vr, q);
	//logtoall("%f,%f,%f\n",vec[0],vec[1],vec[2]);
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j];
		tr[j] = tc[j] + vec[j];
	}
	gl41line(ctx, 0xff, tc, tr);

	//rotor.f
	quaternion_rotatefrom(vec, vf, q);
	//logtoall("%f,%f,%f\n",vec[0],vec[1],vec[2]);
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j];
		tr[j] = tc[j] + vec[j];
	}
	gl41line(ctx, 0xff00ff, tc, tr);
}
static void robotjoint_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robotjoint_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robotjoint_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robotjoint_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robotjoint_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void robotjoint_read_byhuman_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp, struct style* tmpgeom)
{
	logtoall("%p\n",ent);
	//struct style* xxxx;
	//_obj* man;struct style* part;
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	//xxxx = stack[sp-1].pfoot;
	//man = stack[sp-2].pchip;part = stack[sp-2].pfoot;
	wor = stack[sp-4].pchip;geom = stack[sp-4].pfoot;
	wnd = stack[sp-8].pchip;area = stack[sp-8].pfoot;
	//logtoall("%.8s %.8s %.8s\n", &man->type, &wor->type, &wnd->type);

	//if(0 == xxxx)return;
	//if(0 == part)return;
/*
	logtoall("%f,%f,%f,%f @ %f,%f,%f,%f\n",
		part->fshape.vq[0], part->fshape.vq[1], part->fshape.vq[2], part->fshape.vq[3],
		xxxx->fshape.vq[0], xxxx->fshape.vq[1], xxxx->fshape.vq[2], xxxx->fshape.vq[3]
	);
*/
	robotjoint_draw_gl41(ent,slot, wor,tmpgeom, wnd,area);
}
static void robotjoint_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	robotjoint_draw_gl41(ent,slot, wor,geom, wnd,area);
}



#define _human_ hex64('h', 'u', 'm', 'a', 'n', 0, 0, 0)
static void robotjoint_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("test\n");
	//printmemory(&stack[sp], 0x80);
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	//logtoall("test2:%p,%p,%p,%p\n", stack[sp-4].pchip, stack[sp-3].pchip, stack[sp-2].pchip, stack[sp-1].pchip);

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	case _human_:
		robotjoint_read_byhuman_byworld_bycam_bywnd(ent,foot, stack,sp, buf);
		break;
	default:
		robotjoint_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static int robotjoint_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
static void robotjoint_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void robotjoint_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void robotjoint_reader(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
}
static void robotjoint_writer(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
}
static void robotjoint_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void robotjoint_create(_obj* act, void* str, int argc, u8** argv)
{
	if(0 == act)return;
	struct privdata* own = (void*)act->priv_256b;

	int j;
	for(j=0;j<argc;j++){
		//logtoall("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "angle:", 6)){
			decstr2float(argv[j]+6, &own->angle);
		}
	}
}




void robotjoint_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('j', 'o', 'i', 'n', 't', 0, 0, 0);

	p->oncreate = (void*)robotjoint_create;
	p->ondelete = (void*)robotjoint_delete;
	p->onreader = (void*)robotjoint_reader;
	p->onwriter = (void*)robotjoint_writer;

	p->onattach = (void*)robotjoint_attach;
	p->ondetach = (void*)robotjoint_detach;
	p->ontaking = (void*)robotjoint_taking;
	p->ongiving = (void*)robotjoint_giving;
}
