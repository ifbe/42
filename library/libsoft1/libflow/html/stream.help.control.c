#include "libsoft.h"
int openreadclose(void*, int, void*, int);




int control_read(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void** buf, int len)
{
	logtoall("@control_read\n");
/*
	struct str* str = (void*)art->data;
	str->len = 1;
	str->buf[0] = '!';

	buf[0] = str;*/
	return 0;
}
int control_write(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void* buf, int len)
{
	logtoall("@control_write\n");

/*	int j;
	for(j=0;j<256;j++){if(arg[j] <= 0x20)break;}
	logtoall("%.*s\n", j, arg);

	give_data_into_peer(art,_src_, stack,sp, "text/html",0, art->buf0,art->len);*/
	return 0;
}
int control_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int control_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int control_delete(_obj* ele)
{
	return 0;
}
int control_create(_obj* ele, u8* url)
{
	logtoall("@control_create\n");
/*
	ele->buf0 = memoryalloc(0x1000, 0);
	ele->len = openreadclose(url, 0, ele->buf0, 0x1000);*/
	return 1;
}
