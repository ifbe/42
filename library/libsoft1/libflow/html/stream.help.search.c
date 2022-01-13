#include "libsoft.h"




int search_read(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void** buf, int len)
{
	say("@search_read\n");

	int j;
	struct str* str = (void*)art->priv_256b;
	for(j=0;j<8;j++){
		if(arg[j] <= 0x20)break;
		str->buf[j] = arg[j];
	}
	str->len = j;

	buf[0] = str;
	return 0;
}
int search_write(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void* buf, int len)
{
	say("@search_write\n");
	if(0 == arg)return 0;

	int j;
	for(j=0;j<256;j++){
		if(arg[j] <= 0x20)break;
	}
	give_data_into_peer(art,_src_, stack,sp, 0,0, arg,j);
	return 0;
}
int search_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int search_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int search_delete(_obj* ele)
{
	return 0;
}
int search_create(_obj* ele, u8* url)
{
	say("@search_create\n");
	return 1;
}
