#include "libuser.h"




void stdio_take(_obj* std,void* foot, struct halfrel* stack,int sp, p64 arg,int idx, void* buf, int len)
{
}
void stdio_give(_obj* std,void* foot, struct halfrel* stack,int sp, p64 arg,int idx, void* buf, int len)
{
	logtoall("%.*s", len, buf);
}




void stdio_delete(_obj* win)
{
}
void stdio_create(_obj* win)
{
}




void initstd()
{
}
void freestd()
{
}
