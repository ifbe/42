#include "libuser.h"
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);




struct perworld{
	u32 camtype;
};




int virtual_inside_01(struct style* sty, float x, float y)
{
	if(x < sty->fs.vc[0])return 0;
	if(x > sty->fs.vq[0])return 0;
	if(y < sty->fs.vc[1])return 0;
	if(y > sty->fs.vq[1])return 0;
	return 1;
}




int virtual_takeall(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dstnodetype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			entity_takeby(rel->pdstchip,rel->pdstfoot, stack,sp+2, arg,key, 0,0);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int virtual_bycam_byrgba_take(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dstnodetype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			entity_takeby(rel->pdstchip,rel->pdstfoot, stack,sp+2, arg,key, 0,0);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}




int virtual_bywnd_ongive(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//say("@%s\n",__FUNCTION__);
	struct event* ev = buf;
	if('p' == (ev->what&0xff)){
		_obj* wnd = stack[sp-2].pchip;
		//if(0 == wnd)

		short* t = (void*)&ev->why;
		float x = t[0] * 1.0 / wnd->whdf.width;
		float y = 1.0 - t[1] * 1.0 / wnd->whdf.height;
		//say("testtest: %f,%f\n", x, y);

		struct relation* rel = ent->oreln;
		struct style* sty = 0;
		while(1){
			if(0 == rel)break;
			sty = (void*)(rel->srcfoot);
			if(sty){
				//say("testtest: (%f,%f),(%f,%f)\n", sty->fs.vc[0], sty->fs.vc[1], sty->fs.vq[0], sty->fs.vq[1]);
				if(virtual_inside_01(sty, x, y)){
					stack[sp+0].pchip = rel->psrcchip;
					stack[sp+0].pfoot = rel->psrcfoot;
					stack[sp+1].pchip = rel->pdstchip;
					stack[sp+1].pfoot = rel->pdstchip;
					entity_giveby(rel->pdstchip, rel->pdstfoot, stack, sp+2, arg,key, buf,len);
					return _done_;
				}
			}
			rel = samesrcprevdst(rel);
		}
	}

	return _thru_;
}




int virtual_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* caller_1 = stack[sp-2].pchip;
	if(0 == caller_1)return 0;

	_obj* caller_2 = stack[sp-4].pchip;
	//if(caller_2)

	//say("%.8s,%.8s\n", &caller_1->type, &caller_1->hfmt);
	switch(caller_1->type){
	case _camrts_:
		caller_1 = stack[sp-4].pchip;
		//fallthrough
	case _wnd_:
		gl41data_before(caller_1);
		gl41data_01cam(caller_1);
		virtual_takeall(ent,foot, stack,sp, arg,key);
		gl41data_after(caller_1);
		break;
	default:
		if(caller_2 && (_wnd_ == caller_2->type) && (_rgba_ == caller_2->hfmt) ){
			virtual_bycam_byrgba_take(ent,foot, stack,sp, arg,key);
		}
		else{
			virtual_takeall(ent,foot, stack,sp, arg,key);
		}
		break;
	}//switch
	return 0;
}
int virtual_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//say("@virtual_write\n");
	//give_data_into_peer(ent,_evto_, stack,sp, arg,key, buf,len);

	struct perworld* per = (void*)ent->priv_256b;
	//if(0 == per)

	_obj* caller = stack[sp-2].pchip;
	if(0 == caller)return 0;
	//say("caller:%.8s\n", &caller->type);

	//default 2d: from window
	int ret = 0;
	if(_wnd_ == caller->type){
		if(_01_ == per->camtype){
			return virtual_bywnd_ongive(ent,foot, stack,sp, arg,key, buf,len);
		}
	}

	//default 3d: from camera

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
	struct perworld* per = (void*)world->priv_256b;
	per->camtype = _01_;
	return 0;
}
