#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "libhard.h"
void* device_fd2obj(int fd);


struct privdata{
	int fd;
	void* virtual;
};




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
	say("@gpioread:%p,%x,%p,%x\n", arg, cmd, buf, len);
	return 0;
}
int gpio_write(_obj* obj,void* foot, u8* arg,int cmd, u8* buf,int len)
{
	say("@gpiowrite:%p,%x,%p,%x\n", arg, cmd, buf, len);
	return 0;
}
int gpio_delete(_obj* obj, void* arg)
{
	return 0;
}
int gpio_create(_obj* obj, void* arg, int argc, void** argv)
{
	return 0;
}
