#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




int virtual_traverse(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dsttype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			entityread(rel->pdstchip,0, stack,sp+2, arg,key, 0,0);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}




int virtual_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@virtual_read\n");
	struct entity* wnd = stack[sp-2].pchip;
	if(0 == wnd)return 0;

	switch(wnd->type){
	case _wnd_:
	case _fbo_:{
		if('v' != key)break;
		gl41data_before(wnd);
		gl41data_01cam(wnd);
		virtual_traverse(ent,foot, stack,sp, arg,key);break;
		gl41data_after(wnd);
	}
	default:{
		virtual_traverse(ent,foot, stack,sp, arg,key);break;
	}
	}//switch
	return 0;
}
int virtual_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@virtual_write\n");
	relationwrite(ent,_evto_, stack,sp, arg,key, buf,len);
	return 0;
}
int virtual_discon(struct halfrel* self, struct halfrel* peer)
{
	say("virtual_discon\n");
	return 0;
}
int virtual_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@virtual_linkup\n");
	return 0;
}




int virtual_search(struct entity* world)
{
	return 0;
}
int virtual_modify(struct entity* world)
{
	return 0;
}
int virtual_delete(struct entity* world)
{
	return 0;
}
int virtual_create(struct entity* world, void* str)
{
	say("@virtual_create\n");
	return 0;
}
