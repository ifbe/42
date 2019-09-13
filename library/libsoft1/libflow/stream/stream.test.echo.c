#include "libsoft.h"




int echo_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	float f[10];
	say("@echo_read\n");

	relationread((void*)(self->chip), _src_, f, 10);
	return 0;
}
int echo_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	return relationwrite((void*)(self->chip), _src_, buf, len);
}
int echo_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int echo_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int echo_create(struct element* ele, u8* url)
{
	say("@echo_create\n");
	return 1;
}
