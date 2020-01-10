#include "libuser.h"




int event3rd_read(struct halfrel* self, struct halfrel* peer, void* arg, int rsp, void* buf, int len)
{
	return 0;
}
int event3rd_write(struct halfrel* self, struct halfrel* peer, void* arg, int rsp, void* buf, int len)
{
	return 0;
}
int event3rd_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int event3rd_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int event3rd_search(struct entity* win)
{
	return 0;
}
int event3rd_modify(struct entity* win)
{
	return 0;
}
int event3rd_delete(struct entity* win)
{
	return 0;
}
int event3rd_create(struct entity* act, void* flag)
{
	return 0;
}
