#include "libhard.h"
//hw
int inithardware();
int freehardware();
//i2c
_obj* i2c_alloc(u64, u8*);
int i2c_create(_obj* obj, void*, int, u8**);
int i2c_delete(_obj* obj);
int i2c_read(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len);
int i2c_write(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len);
//spi
_obj* spi_alloc(u64, u8*);
int spi_create(_obj* obj, void*, int, u8**);
int spi_delete(_obj* obj);
int spi_read(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len);
int spi_write(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len);
//gpio
_obj* gpio_alloc(u64, u8*);
int gpio_create(_obj* obj, void*, int, void*);
int gpio_delete(_obj* obj);
int gpio_read(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len);
int gpio_write(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len);




static struct item* dev;
static int devlen = 0;
static void* aaa;
static int aaalen = 0;




#define maxdevlen (0x100000/sizeof(struct item))
void device_init(u8* addr, int size)
{
	logtoall("[4,6):device initing\n");

	int j;
	for(j=0;j<0x200000;j++)addr[j]=0;

	dev = (void*)(addr+0x000000);
	devlen = maxdevlen-1;
	for(j=0;j<maxdevlen;j++)dev[j].tier = _dev_;

//#define maxaaalen
	aaa = (void*)(addr+0x100000);
	aaalen = 0;

	logtoall("[4,6):device inited\n");
}
void device_exit()
{
	logtoall("[4,6):device exiting\n");

	freehardware();

	logtoall("[4,6):device exited\n");
}




int device_obj2fd(_obj* obj)
{
	u8* a0 = (u8*)dev;
	u8* a1 = (u8*)obj;
	return (a1-a0)/sizeof(struct item);
}
void* device_fd2obj(int fd)
{
	return &dev[fd];
}




void device_recycle(_obj* obj)
{
	obj->type = 0;
}
void* device_alloc()
{
	void* addr = &dev[devlen];
	devlen -= 1;
	return addr;
}
void* device_alloc_fromtype(u64 type)
{
	_obj* obj = device_alloc();
	if(0 == obj)return 0;

	//obj->tier = tier;		//should be tier: bootup
	//obj->kind = kind;		//should be class: usb
	obj->type = type;		//should be type: xhci
	//obj->vfmt = vfmt;		//should be model: intelxhci
	return obj;
}
void* device_alloc_frompath(u64 type, u8* path)
{
	_obj* obj = 0;
	switch(type){
	case _i2c_:
		obj = i2c_alloc(type,path);
		obj->type = type;
		break;
	case _spi_:
		obj = spi_alloc(type,path);
		obj->type = type;
		break;
	case _gpio_:
		obj = gpio_alloc(type,path);
		obj->type = type;
		break;
	}

	return obj;
}
void* device_alloc_fromfd(u64 type, int fd)
{
	_obj* obj = device_fd2obj(fd);

	obj->type = type;
	return obj;
}




int device_create(_obj* obj, void* name, int argc, u8** argv)
{
	switch(obj->type){
	case _cpu_:
		break;
	case _irq_:
		break;
	case _tmr_:
		break;
	case _pci_:
		break;
	case _ahci_:
		break;
	case _xhci_:
		break;
	case _usb_:
		break;
	case _mmc_:
		break;
	case _eth_:
		break;
	case _uart_:
		break;
	case _i2c_:
		i2c_create(obj, name, argc, argv);
		break;
	case _spi_:
		spi_create(obj, name, argc, argv);
		break;
	case _gpio_:
		gpio_create(obj, name, argc, argv);
		break;
	}
	return 0;
}
int device_delete(_obj* this)
{
	return 0;
}
int device_reader(struct item* dev,void* foot, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@device_reader\n");
	switch(dev->type){
		case _gpio_:return gpio_read(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}
int device_writer(struct item* dev,void* foot, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@device_writer\n");
	switch(dev->type){
		case _gpio_:return gpio_write(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}




int device_attach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	logtoall("@deviceattach\n");
	return 0;
}
int device_detach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	logtoall("@devicedetach\n");
	return 0;
}
int device_takeby(_obj* dev,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@deviceread\n");
	if(dev->ontaking){
		return dev->ontaking(dev,foot, stack,sp, arg,cmd, buf,len);
	}

	switch(dev->type){
		case _i2c_:return i2c_read(dev,foot, arg,cmd, buf,len);break;
		case _spi_:return spi_read(dev,foot, arg,cmd, buf,len);break;
		case _gpio_:return gpio_read(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}
int device_giveby(_obj* dev,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@device_giveby\n");
	u8 t[2];
	if(0 == buf){
		t[0] = len;
		buf = t;
		len = 1;
	}

	switch(dev->type){
		case _i2c_:return i2c_write(dev,foot, arg,cmd, buf,len);break;
		case _spi_:return spi_write(dev,foot, arg,cmd, buf,len);break;
		case _gpio_:return gpio_write(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}




int devicecommand_insert(u8* name, u8* arg)
{
	return 0;
}
int devicecommand_remove(u8* name)
{
	return 0;
}
int devicecommand_search(u8* name)
{
	int j;
	_obj* act;
	if(0 == name){
		for(j=0;j<maxdevlen;j++){
			act = &dev[j];
			if(0 == act->type)continue;
			logtoall("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->kind, &act->type, &act->vfmt);
		}
		if(0 == j)logtoall("empty device\n");
	}
	else{
		for(j=0;j<0x100;j++){
			if(0 == dev[j].type)break;
			if(0 == cmp(&dev[j].type, name))logtoall("name=%d,node=%p\n", name, &dev[j]);
			break;
		}
	}
	return 0;
}
int devicecommand_modify(int argc, u8** argv)
{
	return 0;
}
void* devicecommand(int argc, u8** argv)
{
	if(argc < 2){
		logtoall("device insert name arg\n");
		logtoall("device search name\n");
	}
	else if(0 == ncmp(argv[1], "insert", 6)){
		//device create name arg
		devicecommand_insert(argv[2], argv[3]);
	}
	else if(0 == ncmp(argv[1], "search", 6)){
		//device search <name>
		devicecommand_search((argc<3) ? 0 : argv[2]);
	}

	return 0;
}
