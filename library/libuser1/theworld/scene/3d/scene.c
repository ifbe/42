#include "libuser.h"




//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam, xxxx -> world,camgeom
//[-2,-1]: world,0 -> scene,0
int scene3d_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dsttype){
			stack[sp-2].pfoot = rel->psrcfoot;

			stack[sp-1].pchip = rel->pdstchip;
			stack[sp-1].pfoot = rel->pdstfoot;
			stack[sp-1].flag = rel->dstflag;
			entity_take(stack[sp-1].pchip,stack[sp-1].pfoot, stack,sp, arg,key, buf, len);
		}
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int scene3d_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int scene3d_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int scene3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@scene3d_linkup\n");
	return 0;
}




int scene3d_search(struct entity* scene)
{
	return 0;
}
int scene3d_modify(struct entity* scene)
{
	return 0;
}
int scene3d_delete(struct entity* scene)
{
	return 0;
}
int scene3d_create(struct entity* scene, void* str)
{
	say("@scene3d_create\n");
	return 0;
}
