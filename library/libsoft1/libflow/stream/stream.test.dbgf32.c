#include "libsoft.h"




int dbgf32_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int dbgf32_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
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
int dbgf32_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dbgf32_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int dbgf32_create(struct artery* ele, u8* url)
{
	say("@dbgf32_create\n");
	return 1;
}
