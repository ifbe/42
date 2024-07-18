#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)




void border2d_draw_gl41(_obj* scene, _obj* wnd)
{
	struct relation* rel;
	_obj* ent;
	struct fstyle* sty;
	vec3 tc,tr,tf;
	int j;
	//logtoall("@border2d_read: %.8s.%.8s\n", &wnd->type, &scene->type);

	rel = scene->orel0;
	while(1){
		if(0 == rel)break;
		ent = rel->pdstchip;
		sty = rel->psrcfoot;
		if(sty){
			//logtoall("%llx,%llx\n",ent,sty);
			for(j=0;j<3;j++){
				tr[j] = sty->vr[j];
				tf[j] = sty->vf[j];
				tc[j] = sty->vc[j];
			}
			gl41line_rect(wnd, 0xffffff, tc,tr,tf);

			for(j=0;j<3;j++)tc[j] += tf[j];
			vec3_setlen(tf, 32);
			for(j=0;j<3;j++)tc[j] += tf[j];
			gl41solid_rect(wnd, 0x404040, tc,tr,tf);

			vec3_setlen(tr, 32);
			vec3_setlen(tf, 32);
			for(j=0;j<3;j++)tc[j] += sty->vt[j]/1000;
			gl41string_center(wnd, 0xffffff, tc,tr,tf, (void*)&ent->type, 8);
		}
		rel = samesrcnextdst(rel);
	}
}




int border2d_read_byworld_bycam_bywnd(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg, int key)
{
	struct halfrel* aa[2];
	int ret = relationsearch(ent, _tar_, &aa[0], &aa[1]);
	if(ret <= 0)return 0;

	_obj* tar = aa[1]->pchip;
	if(0 == tar)return 0;

	_obj* wnd = stack[sp-6].pchip;
	if(0 == wnd)return 0;

	border2d_draw_gl41(tar, wnd);
	return 0;
}




int border2d_taking(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		return border2d_read_byworld_bycam_bywnd(ent,foot, stack,sp, arg,key);
	}
	return 0;
}
int border2d_giving(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int border2d_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int border2d_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@border2d_attach\n");
	return 0;
}




int border2d_search(_obj* scene)
{
	return 0;
}
int border2d_modify(_obj* scene)
{
	return 0;
}
int border2d_delete(_obj* scene)
{
	return 0;
}
int border2d_create(_obj* scene, void* str)
{
	logtoall("@border2d_create\n");
	return 0;
}
