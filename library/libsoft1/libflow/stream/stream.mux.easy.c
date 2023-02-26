#include "libsoft.h"
#define _by_ hex16('b','y')
#define _to_ hex16('t','o')




int easymux_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int easymux_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	switch(stack[sp-1].foottype){
	case _by_:
		give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);
		break;
	case _to_:
		//this foot cannot write
		break;
	case _src_:
		give_data_into_peer(art,_to_, stack,sp, 0,0, buf,len);
		break;
	}
	return 0;
}
int easymux_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int easymux_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int easymux_delete(_obj* ele, u8* arg)
{
	return 0;
}
int easymux_create(_obj* ele, u8* arg)
{
	say("@easymux_create\n");
	return 1;
}
