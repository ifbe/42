#include "libsoft.h"
int openwriteclose(void*,int, void*,int);




int stor_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int stor_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int ret;
	u8 url[64];
	_obj* art = self->pchip;
	if(art->vfmt > 999)return 0;

	ret = mysnprintf(url, 64, "picture%03d.jpg", art->vfmt);
	//if(ret <= 0)return 0;

	ret = openwriteclose(url, 0, buf, len);
	//if()return 0;

	say("%s\n",url);
	art->vfmt += 1;
	return 0;
}
int stor_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int stor_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int stor_create(_obj* art, u8* url)
{
	say("@stor_create\n");
	art->vfmt = 0;
	return 1;
}
