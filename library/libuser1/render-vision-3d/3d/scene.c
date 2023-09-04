#include "libuser.h"




void scene3d_selected(_obj* wnd, struct fstyle* sty)
{
	gl41line_prism4(wnd, 0xff00ff, sty->vc, sty->vr, sty->vf, sty->vt);
}




//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam, xxxx -> world,camgeom
//[-2,-1]: world,0 -> scene,0
int scene3d_world_camera_window(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	struct fstyle* sty;
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dstnodetype){
			if((rel == ent->priv_ptr) && stack){
				scene3d_selected(stack[sp-6].pchip, rel->psrcfoot);
			}

			stack[sp-2].pfoot = rel->psrcfoot;

			stack[sp-1].pchip = rel->pdstchip;
			stack[sp-1].pfoot = rel->pdstfoot;
			stack[sp-1].foottype = rel->dstfoottype;
			entity_takeby(stack[sp-1].pchip,stack[sp-1].pfoot, stack,sp, arg,key, buf, len);
		}
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int scene3d_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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

	scene3d_world_camera_window(ent,foot, stack,sp, arg,key, buf,len);
	return 0;
}
int scene3d_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	say("@scene3d_giving\n");

	//remember which is raypicked
	struct relation* rel = (void*)arg;
	ent->priv_ptr = rel;

	//tell that thing, a ray coming
	if(rel){
		stack[sp+0].pchip = rel->psrcchip;
		stack[sp+0].pfoot = rel->psrcfoot;
		stack[sp+0].foottype = rel->srcfoottype;
		stack[sp+1].pchip = rel->pdstchip;
		stack[sp+1].pfoot = rel->pdstfoot;
		stack[sp+1].foottype = rel->dstfoottype;
		entity_giveby(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, 0, 0, buf, 0);
	}

	return 0;
}
int scene3d_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int scene3d_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@scene3d_attach\n");
	return 0;
}




int scene3d_search(_obj* scene)
{
	return 0;
}
int scene3d_modify(_obj* scene)
{
	return 0;
}
int scene3d_delete(_obj* scene)
{
	return 0;
}
int scene3d_create(_obj* scene, void* str)
{
	say("@scene3d_create\n");
	scene->priv_ptr = 0;
	return 0;
}
