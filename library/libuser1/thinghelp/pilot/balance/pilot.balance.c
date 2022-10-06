#include "libuser.h"
#define REL_WORLD listptr.buf0
#define REL_DRONE listptr.buf1
#define ENT_DBGUI listptr.buf2
void quaternion2axisangle(float* q, float* a);
void axisangle2quaternion(float* a, float* q);
void quaternion_multiplyfrom(float* o, float* l, float* r);


#define balancelog say


void balancer_measure(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	vec4 aa;
	quaternion2axisangle(sty->actual.angular_x, aa);
	say("%s: %f,%f,%f,%f\n",__func__, aa[0],aa[1],aa[2],aa[3]);
}
void balancer_compute(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	say("%s\n",__func__);
}
void balancer_operate(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	say("%s\n",__func__);
}




void balancer_checkplace(_obj* ent)
{
	struct halfrel* tmp[2];
	int ret = relationsearch(ent, _dst_, &tmp[0], &tmp[1]);
	if(ret <= 0)return;

	_obj* drone = tmp[1]->pchip;
	if(0 == drone)return;

	_obj* world;
	struct relation* rel = drone->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(	(_virtual_ != world->hfmt) | (_scene3d_ != world->hfmt)){
			ent->REL_WORLD = rel->src;
			ent->REL_DRONE = rel->dst;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}




void balancer_setdesire(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	float* desire = sty->desire.displace_x;
	desire[0] = 0.0;
	desire[1] = 0.0;
	desire[2] = 10.0;
}
void balancer_changedesire(_obj* ent, float angle)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	float* expect = sty->desire.angular_x;
	expect[0] = 0.0;
	expect[1] = 0.0;
	expect[2] = getsin(angle/2);
	expect[3] = getcos(angle/2);
}




void balancer_report(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	_obj* dbg = ent->ENT_DBGUI;
	if(0 == dbg)return;

	//say("%s\n",__func__);
	dbg->onwriter(dbg, 0, 0, 0, sty, 0);
}




int balancer_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	balancelog("@balancer_read:%.4s\n",&foot);
	return 0;
}
int balancer_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	//balancelog("@balancer_write:%.4s\n",&foot);
	if(_clk_ == stack[sp-1].foottype){
		balancer_checkplace(ent);
		balancer_setdesire(ent);

		balancer_measure(ent);
		balancer_compute(ent);
		balancer_operate(ent);

		balancer_report(ent);
	}
	if(_evby_ == stack[sp-1].foottype){
		balancer_checkplace(ent);
		balancer_changedesire(ent, buf[0]*PI/50 - PI);
	}
	return 0;
}
int balancer_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int balancer_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	_obj* dbg = peer->pchip;
	say("%s: %.4s, %.8s\n", __func__, &self->foottype, &dbg->type);

	switch(self->foottype){
	case _dbg_:
		ent->ENT_DBGUI = dbg;
		break;
	}
	return 0;
}




int balancer_reader()
{
	return 0;
}
int balancer_writer()
{
	return 0;
}
int balancer_delete(_obj* ent)
{
	return 0;
}
int balancer_create(_obj* ent, void* str)
{
	balancelog("@balancer_create\n");
	ent->REL_WORLD = 0;
	ent->REL_DRONE = 0;
	ent->ENT_DBGUI = 0;
	ent->whdf.fx0 = ent->whdf.fy0 = ent->whdf.fz0 = 0.0;
	ent->whdf.fxn = ent->whdf.fyn = ent->whdf.fzn = 0.0;
	return 0;
}
