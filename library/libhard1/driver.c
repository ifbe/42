#include "libhard.h"
//
#define _lsm9ds1_ hex64('l','s','m','9','d','s','1',0)
int lsm9ds1_create(struct item* ele, void* arg, int argc, u8** argv);
int lsm9ds1_delete(struct item* ele);
int lsm9ds1_attach(struct halfrel* self, struct halfrel* peer);
int lsm9ds1_detach(struct halfrel* self, struct halfrel* peer);
int lsm9ds1_write( struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int lsm9ds1_read(  struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
//
#define _mpu9250_ hex64('m','p','u','9','2','5','0',0)
int mpu9250_create(struct item* ele, void* arg, int argc, u8** argv);
int mpu9250_delete(struct item* ele);
int mpu9250_attach(struct halfrel* self, struct halfrel* peer);
int mpu9250_detach(struct halfrel* self, struct halfrel* peer);
int mpu9250_write( struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int mpu9250_read(  struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
//
#define _ads1115_ hex64('a','d','s','1','1','1','5',0)
int ads1115_create(struct item* ele, void* arg, int argc, u8** argv);
int ads1115_delete(struct item* ele);
int ads1115_read(  struct item* dri,void* foot, p64 arg,int idx, u8* buf,int len);
int ads1115_write( struct item* dri,void* foot, p64 arg,int idx, u8* buf,int len);
int ads1115_attach(struct halfrel* self, struct halfrel* peer);
int ads1115_detach(struct halfrel* self, struct halfrel* peer);
int ads1115_take(  struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int ads1115_give(  struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
//
#define _gpiotest_ hex64('g','p','i','o','t','e','s','t')
int gpiotest_create(struct item* ele, void* arg, int argc, u8** argv);
int gpiotest_delete(struct item* ele);
int gpiotest_read(  struct item* dri,void* foot, p64 arg,int idx, u8* buf,int len);
int gpiotest_write( struct item* dri,void* foot, p64 arg,int idx, u8* buf,int len);
int gpiotest_attach(struct halfrel* self, struct halfrel* peer);
int gpiotest_detach(struct halfrel* self, struct halfrel* peer);
int gpiotest_take(  struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int gpiotest_give(  struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
#define _l298n_ hex64('l','2','9','8','n', 0, 0, 0)
int l298n_create(struct item* ele, void* arg, int argc, u8** argv);
int l298n_delete(struct item* ele);
int l298n_read(  struct item* dri,void* foot, p64 arg,int idx, u8* buf,int len);
int l298n_write( struct item* dri,void* foot, p64 arg,int idx, u8* buf,int len);
int l298n_attach(struct halfrel* self, struct halfrel* peer);
int l298n_detach(struct halfrel* self, struct halfrel* peer);
int l298n_take(  struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int l298n_give(  struct item* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);




static struct item* dri;
static int drilen = 0;
static void* bbb;
static int bbblen = 0;




#define maxitem (0x100000/sizeof(struct item))
void driver_init(u8* addr)
{
	say("[6,8):driver initing\n");

	int j;
	for(j=0;j<0x200000;j++)addr[j] = 0;

	dri = (void*)(addr+0x000000);
	drilen = maxitem-1;
	for(j=0;j<maxitem;j++)dri[j].tier = _dri_;

	bbb = (void*)(addr+0x100000);

	//drivercreate(_usb_, 0);
	say("[6,8):driver inited\n");
}
void driver_exit()
{
	say("[6,8):driver exiting\n");

	say("[6,8):driver exited\n");
}




void* driver_alloc()
{
	void* addr = &dri[drilen];
	drilen -= 1;
	return addr;
}
void driver_recycle()
{
}
void* driver_alloc_prep(u64 tier, u64 type, u64 hfmt, u64 vfmt)
{
	return 0;
}




void* driver_create(u64 type, void* arg, int argc, u8** argv)
{
	struct item* dr;
	say("@drivercreate: %.8s\n", &type);

	switch(type){
	case _mpu9250_:
		dr = driver_alloc();
		if(0 == dr)return 0;

		dr->type = _mpu9250_;
		mpu9250_create(dr, arg, argc, argv);
		return dr;
	case _lsm9ds1_:
		dr = driver_alloc();
		if(0 == dr)return 0;

		dr->type = _lsm9ds1_;
		lsm9ds1_create(dr, arg, argc, argv);
		return dr;
	case _ads1115_:
		dr = driver_alloc();
		if(0 == dr)return 0;

		dr->type = _ads1115_;
		ads1115_create(dr, arg, argc, argv);
		return dr;
	case _gpiotest_:
		dr = driver_alloc();
		if(0 == dr)return 0;

		dr->type = _gpiotest_;
		gpiotest_create(dr, arg, argc, argv);
		return dr;
	case _l298n_:
		dr = driver_alloc();
		if(0 == dr)return 0;

		dr->type = _l298n_;
		l298n_create(dr, arg, argc, argv);
		return dr;
	}

	return 0;
}
int driver_delete(_obj* this)
{
	return 0;
}
int driver_reader(struct item* dri,void* foot, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int driver_writer(struct item* dri,void* foot, p64 arg, int idx, void* buf, int len)
{
	return 0;
}




int driver_attach(struct halfrel* self, struct halfrel* peer)
{
	struct item* ele = (void*)(self->chip);
	say("@driverattach\n");
	switch(ele->type){
		case _mpu9250_:return mpu9250_attach(self, peer);break;
		case _lsm9ds1_:return lsm9ds1_attach(self, peer);break;
		case _ads1115_:return ads1115_attach(self, peer);break;
		case _gpiotest_:return gpiotest_attach(self, peer);break;
		case _l298n_:return l298n_attach(self, peer);break;
	}
	return 0;
}
int driver_detach(struct halfrel* self, struct halfrel* peer)
{
	struct item* ele = (void*)(self->chip);
	say("@driverdetach\n");
	switch(ele->type){
		case _mpu9250_:return mpu9250_detach(self, peer);break;
		case _lsm9ds1_:return lsm9ds1_detach(self, peer);break;
		case _ads1115_:return ads1115_detach(self, peer);break;
		case _gpiotest_:return gpiotest_detach(self, peer);break;
		case _l298n_:return l298n_detach(self, peer);break;
	}
	return 0;
}
int driver_takeby(struct item* dri,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	switch(dri->type){
		case _mpu9250_:mpu9250_read(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _lsm9ds1_:lsm9ds1_read(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _ads1115_:ads1115_take(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _gpiotest_:gpiotest_take(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _l298n_:l298n_take(dri,foot, stack,sp, arg,idx, buf,len);break;
	}
	return 0;
}
int driver_giveby(struct item* dri,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	//say("@driverwrite\n");
	switch(dri->type){
		case _mpu9250_:return mpu9250_write(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _lsm9ds1_:return lsm9ds1_write(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _ads1115_:return ads1115_give(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _gpiotest_:return gpiotest_give(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _l298n_:return l298n_give(dri,foot, stack,sp, arg,idx, buf,len);break;
	}
	return 0;
}




int driver_insert(u8* buf, int len)
{
	return 0;
}
int driver_remove(u8* buf, int len)
{
	return 0;
}
int driver_search(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<64;j++)
	{
		if(0 == dri[j].type)continue;
		say("[%04x]: %.8s\n", j, &dri[j].type);
		k++;
	}

	if(0 == k)say("empth driver\n");
	return 0;
}
int driver_modify(int argc, u8** argv)
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
		driver_create(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
