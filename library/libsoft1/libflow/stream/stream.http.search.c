#include "libsoft.h"




int search_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int search_write(struct halfrel* self, struct halfrel* peer, u8* arg, int idx, void* buf, int len)
{
	int j;
	say("@search_write\n");
	if(0 == arg)return 0;

	for(j=0;j<256;j++){
		if(arg[j] <= 0x20)break;
	}
	relationwrite(self->pchip, _src_, 0, 0, arg, j);
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
