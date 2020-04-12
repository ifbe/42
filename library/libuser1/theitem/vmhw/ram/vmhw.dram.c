#include "libuser.h"
int copypath(u8* path, u8* data);




static void vmddr_search(struct entity* act, u8* buf)
{
}
static void vmddr_modify(struct entity* act, u8* buf)
{
}
static void vmddr_delete(struct entity* act, u8* buf)
{
	//write mem to file

	//free 64m
}
static void vmddr_create(struct entity* act, void* arg, int argc, u8** argv)
{
	//alloc 64m
	act->buf0 = memorycreate(0x100000*64, 0);
	if(0 == act->buf0)return;

	//read mem from file
	int j,k;
	u32 off;
	u8* p;
	u8* q;
	for(j=0;j<argc;j++){
		p = argv[j];
		if(('0' == p[0]) && ('x' == p[1])){
			k = hexstr2u32(p+2, &off);
			say("off=%x\n", off);

			q = act->buf0 + 0x100000*63;
			copypath(q, p+3+k);
			say("url=%s\n", q);

			p = act->buf0 + off;
			openreadclose(q, 0, p, 0x100000);
			printmemory(p, 0x200);
		}
	}
}




static int vmddr_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	if(_mmio_ == foot){
		u8* ram = ent->buf0;
		if(0 == ram)return 0;

		int j;
		for(j=0;j<len;j++)buf[j] = ram[j+key];
		return len;
	}
	return 0;
}
static int vmddr_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
static int vmddr_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vmddr_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vmddr_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('d','d','r', 0);

	p->oncreate = (void*)vmddr_create;
	p->ondelete = (void*)vmddr_delete;
	p->onsearch = (void*)vmddr_search;
	p->onmodify = (void*)vmddr_modify;

	p->onlinkup = (void*)vmddr_linkup;
	p->ondiscon = (void*)vmddr_discon;
	p->onread  = (void*)vmddr_read;
	p->onwrite = (void*)vmddr_write;
}
