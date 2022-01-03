#include "libuser.h"
#define _in_ hex16('i','n')
#define sin34 0.5591929034707469
#define cos34 0.8290375725550416
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);




static void* camrts_find(_obj* ent)
{
	int ret;
	struct halfrel* out[4];

	ret = relationsearch(ent, _evto_, &out[0], &out[1]);
	if(ret <= 0)return 0;

	_obj* cam = out[1]->pchip;
	if(0 == cam)return 0;

	ret = relationsearch(cam, _in_, &out[0], &out[1]);
	if(ret <= 0)return 0;

	struct fstyle* geom = out[1]->pfoot;
	return geom;
}
static void camrts_fixgeom(struct fstyle* geom)
{
	geom->vr[0] = 1.0;
	geom->vr[1] = 0.0;
	geom->vr[2] = 0.0;

	geom->vf[0] = 0.0;
	geom->vf[1] = sin34;
	geom->vf[2] =-cos34;

	geom->vt[0] = 0.0;
	geom->vt[1] = cos34;
	geom->vt[2] = sin34;
}




int camrts_window_take(_obj* ent,void* foot, _syn* stack,int sp)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;

		say("@%s,rel=%p\n",__FUNCTION__, rel);
		stack[sp+0].pchip = rel->psrcchip;
		stack[sp+0].pfoot = rel->psrcfoot;
		stack[sp+0].flag = rel->srcflag;
		stack[sp+1].pchip = rel->pdstchip;
		stack[sp+1].pfoot = rel->pdstfoot;
		stack[sp+1].flag = rel->dstflag;
		entity_take(stack[sp+1].pchip, 0, stack, sp+2, 0, 0, 0, 0);

		stack[sp+0].pchip = stack[sp-1].pchip;
		stack[sp+0].pfoot = stack[sp-1].pfoot;
		stack[sp+0].flag = stack[sp-1].flag;
		stack[sp+1].pchip = stack[sp-2].pchip;
		stack[sp+1].pfoot = stack[sp-2].pfoot;
		stack[sp+1].flag = stack[sp-2].flag;
		supply_give(stack[sp+1].pchip, 0, stack, sp+2, 0, 0, 0, 0);

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int camrts_window_give(_obj* ent,void* foot, _syn* stack,int sp)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		say("%p\n",rel);
		rel = samesrcnextdst(rel);
	}
	return 0;
}




int camrts_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@%s\n",__FUNCTION__);

	_obj* caller = stack[sp-2].pchip;
	if(0 == caller)return 0;

	switch(caller->type){
	case _wnd_:
		return camrts_window_take(ent,foot, stack,sp);
	default:
		say("%.4s\n",caller->hfmt);
	}
	return 0;
}
int camrts_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	say("@%s\n",__FUNCTION__);

	_obj* caller = stack[sp-2].pchip;
	if(0 == caller)return 0;

	switch(caller->type){
	case _wnd_:
		return camrts_window_give(ent,foot, stack,sp);
	default:
		say("%.4s\n",caller->hfmt);
	}
/*
	struct fstyle* geom = camrts_find(ent);
	if(0 == geom)return 0;

	struct event* ev = buf;
	if(_char_ == ev->what){
		switch(ev->why){
			case 'a':geom->vc[0] -= 100;break;
			case 'd':geom->vc[0] += 100;break;
			case 's':geom->vc[1] -= 100;break;
			case 'w':geom->vc[1] += 100;break;
			default:return 0;
		}
	}

	if(0x4070 == ev->what){
		//short* t = (void*)ev;
		//say("%d,%d\n", t[0],t[1]);

		vec3 v;
		gl41data_convert(stack[sp-4].pchip, stack[sp-4].pfoot, ev, v);
		say("%f,%f\n", v[0],v[1]);

		if(v[0] < 0.01)geom->vc[0] -= 10.0;
		if(v[0] > 0.99)geom->vc[0] += 10.0;
		if(v[1] < 0.01)geom->vc[1] -= 10.0;
		if(v[1] > 0.99)geom->vc[1] += 10.0;
	}

	camrts_fixgeom(geom);
*/
	return 0;
}
int camrts_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@camrts_discon\n");
	return 0;
}
int camrts_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@camrts_linkup\n");
	return 0;
}




int camrts_search(_obj* win)
{
	return 0;
}
int camrts_modify(_obj* win)
{
	return 0;
}
int camrts_delete(_obj* win)
{
	return 0;
}
int camrts_create(_obj* act, void* flag)
{
	return 0;
}
