#include "libsoft.h"




int search_read(struct halfrel* self, struct halfrel* peer, u8* arg, int idx, void** buf, int len)
{
	int j;
	struct artery* art;
	struct str* str;
	say("@search\n");

	art = self->pchip;
	str = (void*)art->data;

	for(j=0;j<8;j++){
		if(arg[j] <= 0x20)break;
		str->buf[j] = arg[j];
	}
	str->len = j;

	buf[0] = str;
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
int search_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int search_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int search_delete(struct artery* ele)
{
	return 0;
}
int search_create(struct artery* ele, u8* url)
{
	say("@search_create\n");
	return 1;
}
