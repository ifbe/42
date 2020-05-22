#include "libsoft.h"




int dbgf32_read(struct artery* art,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int dbgf32_write(struct artery* art,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	int j;
	float* f;
	say("@dbgf32_write: 0x%x\n", len);

	f = buf;
	for(j=0;j<len;j++){
		if(j == len-1)say("%f\n", f[j]);
		else say("%f, ", f[j]);
	}
	return 0;
}
int dbgf32_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dbgf32_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int dbgf32_create(struct artery* ele, u8* url)
{
	say("@dbgf32_create\n");
	return 1;
}
