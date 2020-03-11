#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




int uiuxroot_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* wnd = peer->pchip;
	gl41data_before(wnd);
	gl41data_tmpcam(wnd);

	struct entity* ent = self->pchip;
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		stack[rsp+0] = (void*)(rel->src);
		stack[rsp+1] = (void*)(rel->dst);
		entityread(stack[rsp+1],stack[rsp+0],stack,rsp+2,buf,len);
		rel = samesrcnextdst(rel);
	}

	gl41data_after(wnd);
	return 0;
}




int uiuxroot_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	//say("@uiuxroot_read\n");
	struct entity* sup = peer->pchip;
	switch(sup->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		uiuxroot_read_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	}
	return 0;
}
int uiuxroot_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//say("@uiuxroot_write\n");
	relationwrite(self->pchip, _evto_, stack,rsp, buf,len);
	return 0;
}
int uiuxroot_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int uiuxroot_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@uiuxroot_linkup\n");
	return 0;
}




int uiuxroot_search(struct entity* world)
{
	return 0;
}
int uiuxroot_modify(struct entity* world)
{
	return 0;
}
int uiuxroot_delete(struct entity* world)
{
	return 0;
}
int uiuxroot_create(struct entity* world, void* str)
{
	return 0;
}
