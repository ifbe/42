#include "libuser.h"
//#define _in_ hex16('i','n')
//void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);
#define _freecam_ hex64('f','r','e','e','c','a','m', 0)
int gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);




static void* camrts_find(_obj* ent)
{
	_obj* tmp;
	struct relation* rel = ent->irel0;
	while(1){
		tmp = rel->psrcchip;
		if(_virtual_ == tmp->hfmt){
			return rel->psrcfoot;
		}
		rel = samedstnextsrc(rel);
	}
}
static void camrts_fixgeom(struct fstyle* geom)
{
	geom->vr[0] = 1.0;
	geom->vr[1] = 0.0;
	geom->vr[2] = 0.0;

	geom->vf[0] = 0.0;
	geom->vf[1] = _sin34_;
	geom->vf[2] =-_cos34_;

	geom->vt[0] = 0.0;
	geom->vt[1] = _cos34_;
	geom->vt[2] = _sin34_;
}



/*
int camrts_window_take(_obj* ent,void* foot, _syn* stack,int sp)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;

		say("@%s,rel=%p\n",__FUNCTION__, rel);
		stack[sp+0].pchip = rel->psrcchip;
		stack[sp+0].pfoot = rel->psrcfoot;
		stack[sp+0].foottype = rel->srcfoottype;
		stack[sp+1].pchip = rel->pdstchip;
		stack[sp+1].pfoot = rel->pdstfoot;
		stack[sp+1].foottype = rel->dstfoottype;
		entity_takeby(stack[sp+1].pchip, 0, stack, sp+2, 0, 0, 0, 0);

		stack[sp+0].pchip = stack[sp-1].pchip;
		stack[sp+0].pfoot = stack[sp-1].pfoot;
		stack[sp+0].foottype = stack[sp-1].foottype;
		stack[sp+1].pchip = stack[sp-2].pchip;
		stack[sp+1].pfoot = stack[sp-2].pfoot;
		stack[sp+1].foottype = stack[sp-2].foottype;
		supply_giveby(stack[sp+1].pchip, 0, stack, sp+2, 0, 0, 0, 0);

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
}*/




int camrts_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//say("@%s\n",__FUNCTION__);

	_obj* cam = stack[sp-2].pchip;
	if(0 == cam)return 0;
	//say("cam.fmt=%.8s\n", &cam->hfmt);
	if(_freecam_ != cam->hfmt)return 0;

	_obj* xxx = stack[sp-4].pchip;
	if(0 == xxx)return 0;
	//say("xxx.fmt=%.8s\n", &wnd->hfmt);
	if(_wnd_ == xxx->type){		//wnd->cam->this
		
	}
	if(_virtual_ == xxx->hfmt){		//wnd->cam->world->cam->this
		struct style* sty = stack[sp-4].pfoot;
		if(0 == sty)return 0;
		//say("%f,%f,%f\n", sty->fs.vc[0], sty->fs.vc[1], sty->fs.vc[2]);
		camrts_fixgeom(&sty->fs);
	}

	return 0;
}
int camrts_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	//say("@%s\n",__FUNCTION__);

	_obj* cam = stack[sp-2].pchip;
	if(0 == cam)return 0;
	//say("cam.fmt=%.8s\n", &cam->hfmt);
	if(_freecam_ != cam->hfmt)return 0;

	_obj* xxx = stack[sp-4].pchip;
	if(0 == xxx)return 0;
	//say("xxx.fmt=%.8s\n", &wnd->hfmt);
	if(_wnd_ == xxx->type){		//wnd->cam->this
		struct fstyle* geom = camrts_find(cam);
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
	}
	return 0;
}
int camrts_detach(struct halfrel* self, struct halfrel* peer)
{
	say("@camrts_detach\n");
	return 0;
}
int camrts_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@camrts_attach\n");
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
