#include "libuser.h"
void termwrite(void*, int);




void stdio_take(_obj* std,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf, int len)
{
}
void stdio_give(_obj* std,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf, int len)
{
	say("%.*s", len, buf);
}
void stdio_delete(_obj* win)
{
}
void stdio_create(_obj* win)
{
	termwrite("\n", 1);
}




void initstd()
{
}
void freestd()
{
}
