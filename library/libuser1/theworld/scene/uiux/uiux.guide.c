#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)




void guide3d_draw_gl41(struct entity* scene, struct entity* wnd)
{
	struct relation* rel;
	struct entity* ent;
	struct fstyle* sty;
	vec3 tc = {0.0, 0.0, 0.0};
	//say("@guide3d_read: %.8s\n", &scene->fmt);

	rel = scene->orel0;
	while(1){
		if(0 == rel)break;
		ent = rel->pdstchip;
		sty = rel->psrcfoot;
		if(sty){
			gl41line(wnd, 0xffffff, sty->vc, tc);
		}
		rel = samesrcnextdst(rel);
	}
}
int guide3d_read_bycam(struct entity* ent,int foot, struct halfrel* stack,int sp, void* arg, int key)
{
	if(stack && ('v' == key)){
		struct halfrel* aa[2];
		int ret = relationsearch(ent, _tar_, &aa[0], &aa[1]);
		if(ret <= 0)return 0;

		struct entity* tar = aa[1]->pchip;
		if(0 == tar)return 0;

		struct entity* wnd = stack[sp-4].pchip;
		if(0 == wnd)return 0;

		guide3d_draw_gl41(tar, wnd);
	}
	return 0;
}




int guide3d_read(struct entity* ent,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return guide3d_read_bycam(ent,foot, stack,sp, arg,key);
}
int guide3d_write(struct entity* ent,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
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




int guide3d_search(struct entity* scene)
{
	return 0;
}
int guide3d_modify(struct entity* scene)
{
	return 0;
}
int guide3d_delete(struct entity* scene)
{
	return 0;
}
int guide3d_create(struct entity* scene, void* str)
{
	say("@guide3d_create\n");
	return 0;
}
