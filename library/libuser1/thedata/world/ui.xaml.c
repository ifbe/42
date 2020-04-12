#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




int xamlroot_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void** buf,int len)
{
	return 0;
}
int xamlroot_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void** buf,int len)
{
	return 0;
}
int xamlroot_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int xamlroot_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@xamlroot_linkup\n");
	return 0;
}




int xamlroot_search(struct entity* world)
{
	return 0;
}
int xamlroot_modify(struct entity* world)
{
	return 0;
}
int xamlroot_delete(struct entity* world)
{
	return 0;
}
int xamlroot_create(struct entity* world, void* str)
{
	return 0;
}
