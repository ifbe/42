#include "libuser.h"
void termwrite(void*, int);




void stdio_take(struct supply* std,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf, int len)
{
}
void stdio_give(struct supply* std,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf, int len)
{
	say("%.*s", len, buf);
}
void stdio_delete(struct supply* win)
{
}
void stdio_create(struct supply* win)
{
	termwrite("\n", 1);
}




void initstd()
{
}
void freestd()
{
}
