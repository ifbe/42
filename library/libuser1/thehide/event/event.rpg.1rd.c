#include "libuser.h"




int event1rd_read(struct halfrel* self, struct halfrel* peer, void* arg, int rsp, void* buf, int len)
{
	return 0;
}
int event1rd_write(struct halfrel* self, struct halfrel* peer, void* arg, int rsp, void* buf, int len)
{
	return 0;
}
int event1rd_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int event1rd_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int event1rd_search(struct entity* win)
{
	return 0;
}
int event1rd_modify(struct entity* win)
{
	return 0;
}
int event1rd_delete(struct entity* win)
{
	return 0;
}
int event1rd_create(struct entity* act, void* flag)
{
	return 0;
}
