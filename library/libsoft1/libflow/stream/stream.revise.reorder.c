#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)




int reorder_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float f[10];
	struct element* ele;
	say("@reorder_read\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	relationread(ele, _src_, 0, 0, f, 10);
	return 0;
}
int reorder_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct element* ele;
	say("@reorder_write:%d\n", len);

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	return 0;
}
int reorder_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reorder_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int reorder_create(struct element* ele, u8* url)
{
	say("@reorder_create\n");
	return 1;
}
