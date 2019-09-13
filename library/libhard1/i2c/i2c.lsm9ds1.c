#include "libsoft.h"




int lsm9ds1_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	return 0;
}
int lsm9ds1_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	return 0;
}
int lsm9ds1_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int lsm9ds1_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int lsm9ds1_create(struct element* ele, u8* url)
{
	say("@lsm9ds1_create\n");
	return 1;
}
int lsm9ds1_delete(struct element* ele)
{
	return 0;
}
