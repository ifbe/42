#include "libuser.h"
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);




typedef struct perdata{
	void* buffer;
	void* belong;
}datalist;
struct perworld{
	//common
	datalist* light;
	datalist* sound;
	datalist* smell;
	datalist* heat;

	//3d
	datalist* vertex;
	datalist* texture;

	//for gl41
	datalist* glthing;

	//for vulkan
	datalist* vkthing;

	//for dx11
	datalist* dx11thing;

	//for dx12u
	datalist* dx12thing;

	//for metal
	datalist* metal;
};




int virtual_traverse(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dstnodetype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			entity_take(rel->pdstchip,0, stack,sp+2, arg,key, 0,0);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}




int virtual_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@virtual_read\n");
	_obj* wnd = stack[sp-2].pchip;
	if(0 == wnd)return 0;

	switch(wnd->type){
	case _camrts_:
		wnd = stack[sp-4].pchip;
		//fallthrough
	case _wnd_:
		gl41data_before(wnd);
		gl41data_01cam(wnd);
		virtual_traverse(ent,foot, stack,sp, arg,key);
		gl41data_after(wnd);
		break;
	default:
		virtual_traverse(ent,foot, stack,sp, arg,key);
		break;
	}//switch
	return 0;
}
int virtual_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@virtual_write\n");
	give_data_into_peer(ent,_evto_, stack,sp, arg,key, buf,len);
	return 0;
}
int virtual_detach(struct halfrel* self, struct halfrel* peer)
{
	say("virtual_detach\n");
	return 0;
}
int virtual_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@virtual_attach\n");
	return 0;
}




int virtual_search(_obj* world)
{
	return 0;
}
int virtual_modify(_obj* world)
{
	return 0;
}
int virtual_delete(_obj* world)
{
	return 0;
}
int virtual_create(_obj* world, void* str)
{
	say("@virtual_create\n");
	return 0;
}
