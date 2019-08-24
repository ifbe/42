#include "libuser.h"




int hoffdata_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//for each object in target world: read ctx
	struct actor* data;
	struct relation* rel;

	data = (void*)(self->chip);
	if(0 == data)return 0;

	rel = data->orel0;
	if(0 == rel)return 0;

	self = (void*)(&rel->dstchip);
	peer = (void*)(&rel->srcchip);
	actorread(self, peer, buf, len);

	return 0;
}
int hoffdata_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	return 0;
}
int hoffdata_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int hoffdata_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int hoffdata_search(struct arena* win)
{
	return 0;
}
int hoffdata_modify(struct arena* win)
{
	return 0;
}
int hoffdata_delete(struct arena* win)
{
	return 0;
}
int hoffdata_create(struct actor* act, void* flag)
{
    return 0;
}