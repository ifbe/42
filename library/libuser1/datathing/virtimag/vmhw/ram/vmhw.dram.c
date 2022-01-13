#include "libuser.h"
int copypath(u8* path, u8* data);




static void vmddr_search(_obj* act, u8* buf)
{
}
static void vmddr_modify(_obj* act, u8* buf)
{
}
static void vmddr_delete(_obj* act, u8* buf)
{
	//write mem to file

	//free 64m
}
static void vmddr_create(_obj* act, void* arg, int argc, u8** argv)
{
	//alloc 64m
	act->listptr.buf0 = memorycreate(0x100000*64, 0);
	if(0 == act->listptr.buf0)return;

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

			q = act->listptr.buf0 + 0x100000*63;
			copypath(q, p+3+k);
			say("url=%s\n", q);

			p = act->listptr.buf0 + off;
			openreadclose(q, 0, p, 0x100000);
			printmemory(p, 0x200);
		}
	}
}




static int vmddr_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	if(_mmio_ == stack[sp-1].foottype){
		u8* ram = ent->listptr.buf0;
		if(0 == ram)return 0;

		int j;
		for(j=0;j<len;j++)buf[j] = ram[j+key];
		return len;
	}
	return 0;
}
static int vmddr_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
static int vmddr_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vmddr_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vmddr_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('d','d','r', 0);

	p->oncreate = (void*)vmddr_create;
	p->ondelete = (void*)vmddr_delete;
	p->onsearch = (void*)vmddr_search;
	p->onmodify = (void*)vmddr_modify;

	p->onattach = (void*)vmddr_attach;
	p->ondetach = (void*)vmddr_detach;
	p->ontaking = (void*)vmddr_taking;
	p->ongiving = (void*)vmddr_giving;
}
