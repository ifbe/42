#include "libsoft.h"




int search_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int search_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int search_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int search_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int search_create(struct element* ele, u8* url)
{
	say("@search_create\n");
	return 1;
}
