#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)




void guide3d_draw_gl41(_obj* scene, _obj* wnd)
{
	struct relation* rel;
	_obj* ent;
	struct fstyle* sty;
	vec3 at = {0.0, 0.0, 0.0};
	vec3 up = {0.0, 0.0, 1.0};
	//say("@guide3d_read: %.8s\n", &scene->hfmt);

	rel = scene->orel0;
	while(1){
		if(0 == rel)break;
		ent = rel->pdstchip;
		sty = rel->psrcfoot;
		if(sty){
			gl41line_arrow(wnd, 0xffffff, at, sty->vc, up);
		}
		rel = samesrcnextdst(rel);
	}
}
int guide3d_read_bycam(_obj* ent,void* foot, struct halfrel* stack,int sp, void* arg, int key)
{
	struct halfrel* aa[2];
	int ret = relationsearch(ent, _tar_, &aa[0], &aa[1]);
	if(ret <= 0)return 0;

	_obj* tar = aa[1]->pchip;
	if(0 == tar)return 0;

	_obj* wnd = stack[sp-6].pchip;
	if(0 == wnd)return 0;

	guide3d_draw_gl41(tar, wnd);
	return 0;
}




int guide3d_taking(_obj* ent,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	}

	return guide3d_read_bycam(ent,foot, stack,sp, arg,key);
}
int guide3d_giving(_obj* ent,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int guide3d_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int guide3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@guide3d_linkup\n");
	return 0;
}




int guide3d_search(_obj* scene)
{
	return 0;
}
int guide3d_modify(_obj* scene)
{
	return 0;
}
int guide3d_delete(_obj* scene)
{
	return 0;
}
int guide3d_create(_obj* scene, void* str)
{
	say("@guide3d_create\n");
	return 0;
}
