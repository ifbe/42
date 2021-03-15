#include "libuser.h"




void scene3d_selected(struct entity* wnd, struct fstyle* sty)
{
	gl41line_prism4(wnd, 0xff00ff, sty->vc, sty->vr, sty->vf, sty->vt);
}




//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam, xxxx -> world,camgeom
//[-2,-1]: world,0 -> scene,0
int scene3d_world_camera_window(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct fstyle* sty;
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dsttype){
			if((rel == ent->buf0) && stack){
				scene3d_selected(stack[sp-6].pchip, rel->psrcfoot);
			}

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
int scene3d_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41full_:
		break;
	}

	scene3d_world_camera_window(ent,foot, stack,sp, arg,key, buf,len);
	return 0;
}
int scene3d_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@scene3d_giving\n");

	//remember which is raypicked
	struct relation* rel = arg;
	ent->buf0 = rel;

	//tell that thing, a ray coming
	if(rel){
		stack[sp+0].pchip = rel->psrcchip;
		stack[sp+0].pfoot = rel->psrcfoot;stack[sp+0].flag = rel->srcflag;
		stack[sp+1].pchip = rel->pdstchip;
		stack[sp+1].pfoot = rel->pdstfoot;stack[sp+1].flag = rel->dstflag;
		entity_give(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, 0, 0, buf, 0);
	}

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
	scene->buf0 = 0;
	return 0;
}
