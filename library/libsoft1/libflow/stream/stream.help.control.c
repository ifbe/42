#include "libsoft.h"
int openreadclose(void*, int, void*, int);




int control_read(struct halfrel* self, struct halfrel* peer, u8* arg, int idx, void** buf, int len)
{
	struct artery* art;
	struct str* str;
	say("@control\n");

	art = self->pchip;
	str = (void*)art->data;
	str->len = 1;
	str->buf[0] = '!';

	buf[0] = str;
	return 0;
}
int control_write(struct halfrel* self, struct halfrel* peer, u8* arg, int idx, void* buf, int len)
{
	int j;
	struct artery* ele;
	say("@control_write\n");

	for(j=0;j<256;j++){if(arg[j] <= 0x20)break;}
	say("%.*s\n", j, arg);

	ele = self->pchip;
	relationwrite(self->pchip, _src_, "text/html", 0, ele->buf0, ele->len);
	return 0;
}
int control_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int control_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int control_delete(struct artery* ele)
{
	return 0;
}
int control_create(struct artery* ele, u8* url)
{
	say("@control_create\n");

	ele->buf0 = memorycreate(0x1000, 0);
	ele->len = openreadclose(url, 0, ele->buf0, 0x1000);
	return 1;
}
