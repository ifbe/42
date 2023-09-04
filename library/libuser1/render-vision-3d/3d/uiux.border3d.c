#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)




void border3d_draw_gl41(_obj* scene, _obj* wnd)
{
	struct relation* rel;
	_obj* ent;
	struct fstyle* sty;
	vec3 tc,tr,tf,tt;
	int j;
	//say("@border3d_read: %.8s\n", &scene->hfmt);

	rel = scene->orel0;
	while(1){
		if(0 == rel)break;
		ent = rel->pdstchip;
		sty = rel->psrcfoot;
		if(sty){
			//say("%llx,%llx\n",ent,sty);
			for(j=0;j<3;j++){
				tr[j] = sty->vr[j];
				tf[j] = sty->vf[j];
				tt[j] = sty->vt[j];
				tc[j] = sty->vc[j];
			}
			gl41line_prism4(wnd, 0xffffff, sty->vc, sty->vr, sty->vf, sty->vt);

			for(j=0;j<3;j++)tc[j] += -tr[j] -tf[j] +tt[j];
			vec3_setlen(tr, 32);
			vec3_setlen(tf, 32);
			gl41string(wnd, 0xffffff, tc,tr,tf, (void*)&ent->hfmt, 8);
		}
		rel = samesrcnextdst(rel);
	}
}
int border3d_read_bycam(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key)
{
	struct halfrel* aa[2];
	int ret = relationsearch(ent, _tar_, &aa[0], &aa[1]);
	if(ret <= 0)return 0;

	_obj* tar = aa[1]->pchip;
	if(0 == tar)return 0;

	_obj* wnd = stack[sp-6].pchip;
	if(0 == wnd)return 0;

	border3d_draw_gl41(tar, wnd);
	return 0;
}




int border3d_taking(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
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

	return border3d_read_bycam(ent,foot, stack,sp, arg,key);
}
int border3d_giving(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int border3d_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int border3d_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@border3d_attach\n");
	return 0;
}




int border3d_search(_obj* scene)
{
	return 0;
}
int border3d_modify(_obj* scene)
{
	return 0;
}
int border3d_delete(_obj* scene)
{
	return 0;
}
int border3d_create(_obj* scene, void* str)
{
	say("@border3d_create\n");
	return 0;
}
