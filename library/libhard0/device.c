#include "libhard.h"
//gpio
int gpiodelete();
int gpiocreate();
//i2c
int i2c_create(void*, int, int, u8**);
int i2c_delete(int);
int i2c_read(int fd, int addr, u8* buf, int len);
int i2c_write(int fd, int addr, u8* buf, int len);
//spi
int spi_create(void*, int, int, u8**);
int spi_delete(int);
int spi_read(int fd, int addr, u8* buf, int len);
int spi_write(int fd, int addr, u8* buf, int len);
//16550
//pl011




static struct device* dev;
static int devlen = 0;
static void* aaa;
static int aaalen = 0;
void* allocdevice()
{
	void* addr = &dev[devlen];
	devlen += 1;
	return addr;
}




int deviceread(_dev* dev,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	int fd = dev->selffd;
	switch(dev->type){
		case _i2c_:return i2c_read(fd, idx, buf, len);break;
		case _spi_:return spi_read(fd, idx, buf, len);break;
	}
	return 0;
}
int devicewrite(_dev* dev,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	u8 t[2];
	if(0 == buf){
		t[0] = len;
		buf = t;
		len = 1;
	}

	int fd = dev->selffd;
	switch(dev->type){
		case _i2c_:return i2c_write(fd, idx, buf, len);break;
		case _spi_:return spi_write(fd, idx, buf, len);break;
	}
	return 0;
}
int devicediscon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int devicelinkup(struct halfrel* self, struct halfrel* peer)
{
	say("@devicelinkup\n");
	return 0;
}




int devicedelete(void* this)
{
	return 0;
}
void* devicecreate(u64 type, void* name, int argc, u8** argv)
{
	int fd, baud;
	u8 tmp[256];

	if(_i2c_ == type)
	{
		fd = i2c_create(name, 0, argc, argv);
		if(fd <= 0)return 0;

		dev[fd].type = _i2c_;
		dev[fd].selffd = fd;

		return &dev[fd];
	}
	if(_spi_ == type)
	{
		fd = spi_create(name, 0, argc, argv);
		if(fd <= 0)return 0;

		dev[fd].type = _spi_;
		dev[fd].selffd = fd;
		return &dev[fd];
	}
	return 0;
}
int devicemodify(int argc, u8** argv)
{
	int j;
	u64 name = 0;
	u8* tmp = (u8*)&name;
	if(argc < 2)return 0;
//say("%s,%s,%s,%s\n",argv[0],argv[1],argv[2],argv[3]);
	if(0 == ncmp(argv[1], "create", 6))
	{
		for(j=0;j<8;j++)
		{
			if(argv[2][j] <= 0x20)break;
			tmp[j] = argv[2][j];
		}
		say("%llx,%llx\n",name, argv[3]);
		devicecreate(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
int devicesearch(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<64;j++)
	{
		if(0 == dev[j].type)continue;
		say("[%04x]: %.8s\n", j, &dev[j].type);
		k++;
	}

	if(0 == k)say("empth device\n");
	return 0;
}




void freedevice()
{
	say("[4,6):device freeing\n");

	gpiodelete();

	say("[4,6):device freeed\n");
}
void initdevice(u8* addr)
{
	say("[4,6):device initing\n");

	int j;
	dev = (void*)(addr+0x000000);
	aaa = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct device))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)dev[j].tier = _dev_;

	gpiocreate();

	say("[4,6):device inited\n");
}
