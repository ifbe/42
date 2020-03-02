#include "libsoft.h"
#define _by_ hex16('b','y')
#define _to_ hex16('t','o')




int easymux_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int easymux_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct artery* art = self->pchip;
	switch(self->flag){
	case _by_:{
		relationwrite(art,_src_, 0,0, buf,len);
		break;
	}
	case _to_:{
		//this foot cannot write
		break;
	}
	case _src_:{
		relationwrite(art,_to_, 0,0, buf,len);
		break;
	}
	default:break;
	}
	return 0;
}
int easymux_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int easymux_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int easymux_delete(struct artery* ele, u8* url)
{
	return 0;
}
int easymux_create(struct artery* ele, u8* url)
{
	say("@easymux_create\n");
	return 1;
}
