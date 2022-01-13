#include "libsoft.h"
int openwriteclose(void*,int, void*,int);




int stor_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int stor_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	if(art->priv_u32 > 999)return 0;

	u8 url[64];
	int ret = mysnprintf(url, 64, "picture%03d.jpg", art->priv_u32);
	//if(ret <= 0)return 0;
	say("%s\n",url);

	ret = openwriteclose(url, 0, buf, len);
	//if()return 0;

	art->priv_u32 += 1;
	return 0;
}
int stor_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int stor_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int stor_create(_obj* art, u8* url)
{
	say("@stor_create\n");
	art->priv_u32 = 0;
	return 1;
}
