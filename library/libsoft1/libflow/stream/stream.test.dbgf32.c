#include "libsoft.h"




int dbgf32_read(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int dbgf32_write(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	int j;
	float* f;
	logtoall("@dbgf32_write: foot=%.4s, len=0x%x\n", &foot, len);

	f = buf;
	for(j=0;j<len;j++){
		if(j == len-1)logtoall("%f\n", f[j]);
		else logtoall("%f, ", f[j]);
	}
	return 0;
}
int dbgf32_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dbgf32_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int dbgf32_create(_obj* ele, u8* arg)
{
	logtoall("@dbgf32_create\n");
	return 1;
}
