#include "libhard.h"
//hw
int inithardware();
int freehardware();
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
//gpio
int gpio_create(void*,void*,int,void*);
int gpio_delete(void*);
int gpio_read(_obj* obj,void* foot, void* arg,int cmd, u8* buf,int len);
int gpio_write(_obj* obj,void* foot, void* arg,int cmd, u8* buf,int len);




static struct item* dev;
static int devlen = 0;
static void* aaa;
static int aaalen = 0;




void device_init(u8* addr)
{
	say("[4,6):device initing\n");

	int j;
	dev = (void*)(addr+0x000000);
	aaa = (void*)(addr+0x100000);
	devlen = 0;
	aaalen = 0;

#define max (0x100000/sizeof(struct item))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)dev[j].tier = _dev_;

	say("[4,6):device inited\n");
}
void device_exit()
{
	say("[4,6):device exiting\n");

	freehardware();

	say("[4,6):device exited\n");
}
void* device_alloc()
{
	void* addr = &dev[devlen];
	devlen += 1;
	return addr;
}
void device_recycle()
{
}
void* device_fd2obj(int fd)
{
	return &dev[fd];
}




void* device_create(u64 type, void* name, int argc, u8** argv)
{
	if(0 == type){
		return device_alloc();
	}
	if(_cpu_ == type){
		struct item* p = device_alloc();
		p->type = _cpu_;
		p->hfmt = _cpu_;
		return p;
	}
	if(_irq_ == type){
		struct item* p = device_alloc();
		p->type = _irq_;
		p->hfmt = _irq_;
		return p;
	}
	if(_tmr_ == type){
		struct item* p = device_alloc();
		p->type = _tmr_;
		p->hfmt = _tmr_;
		return p;
	}
	if(_pci_ == type){
		struct item* p = device_alloc();
		p->type = _bus_;
		p->hfmt = _pci_;
		return p;
	}
	if(_ahci_ == type){
		struct item* p = device_alloc();
		p->type = _ahci_;
		p->hfmt = _ahci_;
		return p;
	}
	if(_xhci_ == type){
		struct item* p = device_alloc();
		p->type = _xhci_;
		p->hfmt = _xhci_;
		return p;
	}
	if(_usb_ == type){
		struct item* p = device_alloc();
		p->type = _usb_;
		p->hfmt = _usb_;
		return p;
	}
	if(_mmc_ == type){
		struct item* p = device_alloc();
		p->type = _mmc_;
		p->hfmt = _mmc_;
		return p;
	}
	if(_eth_ == type){
		struct item* p = device_alloc();
		p->type = _eth_;
		p->hfmt = _eth_;
		return p;
	}
	if(_i2c_ == type){
		int fd = i2c_create(name, 0, argc, argv);
		if(fd <= 0)return 0;

		dev[fd].type = _i2c_;
		dev[fd].priv_fd = fd;

		return &dev[fd];
	}
	if(_spi_ == type){
		int fd = spi_create(name, 0, argc, argv);
		if(fd <= 0)return 0;

		dev[fd].type = _spi_;
		dev[fd].priv_fd = fd;
		return &dev[fd];
	}
	if(_gpio_ == type){
		int fd = gpio_create(0, name, argc, argv);
		if(fd <= 0)return 0;

		dev[fd].type = _gpio_;
		dev[fd].priv_fd = fd;
		say("return:%p\n",&dev[fd]);
		return &dev[fd];
	}
	return 0;
}
int device_delete(_obj* this)
{
	return 0;
}
int device_reader(struct item* dev,void* foot, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int device_writer(struct item* dev,void* foot, void* arg,int idx, void* buf,int len)
{
	return 0;
}




int device_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@deviceattach\n");
	return 0;
}
int device_detach(struct halfrel* self, struct halfrel* peer)
{
	say("@devicedetach\n");
	return 0;
}
int device_takeby(struct item* dev,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	//say("@deviceread\n");
	if(dev->ontaking){
		return dev->ontaking(dev,foot, stack,sp, arg,cmd, buf,len);
	}

	int fd = dev->priv_fd;
	switch(dev->type){
		case _i2c_:return i2c_read(fd, cmd, buf, len);break;
		case _spi_:return spi_read(fd, cmd, buf, len);break;
		case _gpio_:return gpio_read(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}
int device_giveby(struct item* dev,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	//say("@device_giveby\n");
	u8 t[2];
	if(0 == buf){
		t[0] = len;
		buf = t;
		len = 1;
	}

	int fd = dev->priv_fd;
	switch(dev->type){
		case _i2c_:return i2c_write(fd, cmd, buf, len);break;
		case _spi_:return spi_write(fd, cmd, buf, len);break;
		case _gpio_:return gpio_write(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}




int device_insert(u8* buf, int len)
{
	return 0;
}
int device_remove(u8* buf, int len)
{
	return 0;
}
int device_search(u8* buf, int len)
{
	int j,k=0;
	struct item* p;
	for(j=0;j<64;j++)
	{
		p = &dev[j];
		if(0 == p->type)continue;
		say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j, &p->tier, &p->type, &p->hfmt, &p->vfmt);
		k++;
	}

	if(0 == k)say("empth device\n");
	return 0;
}
int device_modify(int argc, u8** argv)
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
		device_create(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
