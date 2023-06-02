#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <libhard.h>
#define _volt_ hex32('v','o','l','t')
void* device_fd2obj(int fd);


//altfunc
#define GPFSEL0   0x00
#define GPFSEL1   0x04
#define GPFSEL2   0x08
#define GPFSEL3   0x0C
#define GPFSEL4   0x10
#define GPFSEL5   0x14
//output
#define GPSET0    0x1C
#define GPSET1    0x20
#define GPCLR0    0x28
#define GPCLR1    0x2c

#define GPIOACCESS(x) (*(volatile unsigned int*)(gpio+x))

struct privdata{
	int fd;
	void* virtual;
};

char* val2str[8] = {
	"input","output","alt5","alt4",
	"alt0", "alt1",  "alt2","alt3"
};

static int setfunc(void* gpio, int pin, int func)
{
	say("setfunc:virt=%p,pin=%d,func=%d\n",gpio,pin,func);
	if(pin < 0)return 0;
	if(pin > 45)return 0;

	int hh = pin/10;
	int ll = pin%10;

	unsigned int val = GPIOACCESS(GPFSEL0 + hh*4);
	val &= ~(3 << (ll*3));
	val |= func << (ll*3);
	GPIOACCESS(GPFSEL0 + hh*4) = val;
	return 1;
}

static int setvolt(void* gpio, int pin, int volt)
{
	say("setvolt:virt=%p,pin=%d,volt=%d\n",gpio,pin,volt);
	if(pin < 0)return 0;
	if(pin > 45)return 0;

	unsigned int offs = (pin/32)*4;
	if(1 == volt){
		offs += GPSET0;
	}
	else{
		offs += GPCLR0;
	}
	GPIOACCESS(offs) = 1<<(pin%32);
	return 1;
}




int gpio_takeby(struct halfrel* st,int sp, void* arg,int cmd, void* buf,int len)
{
	return 0;
}
int gpio_giveby(struct halfrel* st,int sp, void* arg,int cmd, void* buf,int len)
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
int gpio_read(_obj* obj,void* foot, void* arg,int cmd, u8* buf,int len)
{
	return 0;
}
int gpio_write(_obj* obj,void* foot, u8* pin,int cmd, u8* val,int len)
{
	say("@gpiowrite:%p,%x,%p,%x\n", pin, cmd, val, len);
	printmemory(pin,len);
	printmemory(val,len);

	struct privdata* priv = (void*)obj->priv_256b;
	void* virt = priv->virtual;

	int j;
	switch(cmd){
	case _func_:
		for(j=0;j<len;j++)setfunc(virt, pin[j], val[j]);
		break;
	case _volt_:
		for(j=0;j<len;j++)setvolt(virt, pin[j], val[j]);
		break;
	}
	return 0;
}
int gpio_delete(_obj* obj, void* arg)
{
	struct privdata* priv = (void*)obj->priv_256b;
	if(priv->virtual){
		munmap(priv->virtual, 0x1000);
		priv->virtual = 0;
	}
	if(priv->fd){
		close(priv->fd);
		priv->fd = 0;
	}
	return 0;
}
int gpio_create(_obj* obj, void* arg, int argc, void** argv)
{
	int _dev_mem=open("/dev/gpiomem",O_RDWR|O_SYNC);
	printf("open:%x\n",_dev_mem);

	unsigned int* virtual=mmap(
                NULL,
                0x1000,
                PROT_READ|PROT_WRITE,
                MAP_SHARED,
                _dev_mem,
                0
	);
	printf("mmap:%llx\n",virtual);

	if(0 == obj)obj = device_fd2obj(_dev_mem);
	struct privdata* priv = (void*)obj->priv_256b;
	priv->fd = _dev_mem;
	priv->virtual = virtual;
	return _dev_mem;
}
