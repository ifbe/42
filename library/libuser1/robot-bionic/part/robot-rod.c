#include "libuser.h"
void quaternion4axisandangle(float* q, float* a, float angle);
void quaternion_rotatefrom(float* o, float* v, float* q);



struct privdata{
	float angle;
};
static void robotrod_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	struct privdata* own = (void*)act->priv_256b;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	vec3 tr,tf,tt;
	int j;
	for(j=0;j<3;j++){
		tr[j] = vr[j];
		tf[j] = vf[j];
		tt[j] = vt[j];
	}
	vec3_setlen(tr, 0.05);
	vec3_setlen(tf, 0.5);
	vec3_setlen(tt, 0.05);
	gl41solid_prism4(ctx, 0x444444, vc, tr, tf, tt);
}
static void robotrod_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robotrod_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robotrod_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robotrod_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void robotrod_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void robotrod_read_byhuman_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp, struct style* tmpgeom)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-4].pchip;geom = stack[sp-4].pfoot;
	wnd = stack[sp-8].pchip;area = stack[sp-8].pfoot;
	//logtoall("%.8s %.8s %.8s\n", &man->type, &wor->type, &wnd->type);

	robotrod_draw_gl41(ent,slot, wor,tmpgeom, wnd,area);
}
static void robotrod_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	robotrod_draw_gl41(ent,slot, wor,geom, wnd,area);
}



#define _human_ hex64('h', 'u', 'm', 'a', 'n', 0, 0, 0)
static void robotrod_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		robotrod_read_byhuman_byworld_bycam_bywnd(ent,foot, stack,sp, buf);
		break;
	default:
		robotrod_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static int robotrod_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
static void robotrod_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void robotrod_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void robotrod_reader(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
}
static void robotrod_writer(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
}
static void robotrod_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void robotrod_create(_obj* act, void* str, int argc, u8** argv)
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




void robotrod_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('r', 'o', 'd', 0, 0, 0, 0, 0);

	p->oncreate = (void*)robotrod_create;
	p->ondelete = (void*)robotrod_delete;
	p->onreader = (void*)robotrod_reader;
	p->onwriter = (void*)robotrod_writer;

	p->onattach = (void*)robotrod_attach;
	p->ondetach = (void*)robotrod_detach;
	p->ontaking = (void*)robotrod_taking;
	p->ongiving = (void*)robotrod_giving;
}
