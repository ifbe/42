#include "libuser.h"




int hoffdata_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//for each object in target world: read ctx
	struct actor* data = (void*)(self->chip);
	if(0 == data)return 0;

	relationread(data, _dst_, 0, 0, buf, len);
	return 0;
}
int hoffdata_write(struct halfrel* self, struct halfrel* peer, void*arg, int idx, void* buf, int len)
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
