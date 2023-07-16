#include "libhard.h"


int gpio_takeby(struct halfrel* st,int sp, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
int gpio_giveby(struct halfrel* st,int sp, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
int gpio_detach(struct halfrel* st, struct halfrel* peer)
{
	return 0;
}
int gpio_attach(struct halfrel* st, struct halfrel* peer)
{
	return 0;
}
int gpio_read(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len)
{
	return 0;
}
int gpio_write(_obj* obj,void* foot, p64 pin,int cmd, u8* val,int len)
{
	say("@gpio_write:");
	return 0;
}
int gpio_delete(_obj* obj, void* arg, int argc, void** argv)
{
	return 0;
}
int gpio_create(_obj* obj, void* arg, int argc, void** argv)
{
	return 0;
}