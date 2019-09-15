#include "libsoft.h"




int echo_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float f[10];
	struct element* ele;
	say("@echo_read\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	relationread((void*)(self->chip), _src_, 0, 0, f, 10);
	return 0;
}
int echo_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct element* ele;
	say("@echo_write\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	return relationwrite(ele, _src_, 0, 0, buf, len);
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
