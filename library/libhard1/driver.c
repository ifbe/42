#include "libhard.h"
//
#define _lsm9ds1_ hex64('l','s','m','9','d','s','1',0)
int lsm9ds1_create(struct driver* ele, void* url);
int lsm9ds1_delete(struct driver* ele);
int lsm9ds1_start( struct halfrel* self, struct halfrel* peer);
int lsm9ds1_stop(  struct halfrel* self, struct halfrel* peer);
int lsm9ds1_write( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int lsm9ds1_read(  struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
#define _mpu9250_ hex64('m','p','u','9','2','5','0',0)
int mpu9250_create(struct driver* ele, void* url);
int mpu9250_delete(struct driver* ele);
int mpu9250_start( struct halfrel* self, struct halfrel* peer);
int mpu9250_stop(  struct halfrel* self, struct halfrel* peer);
int mpu9250_write( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mpu9250_read(  struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);




static struct device* dev;
static struct driver* dri;
static int devlen = 0;
static int drilen = 0;
void* allocdriver()
{
	void* addr = &dri[drilen];
	drilen += 1;
	return addr;
}




int driverwrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct driver* ele = (void*)(self->chip);
	//say("@driverwrite\n");
	switch(ele->type){
		case _mpu9250_:return mpu9250_write(self, peer, arg, idx, buf, len);break;
		case _lsm9ds1_:return lsm9ds1_write(self, peer, arg, idx, buf, len);break;
	}
	return 0;
}
int driverread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct driver* ele = (void*)(self->chip);
	switch(ele->type){
		case _mpu9250_:mpu9250_read(self, peer, arg, idx, buf, len);break;
		case _lsm9ds1_:lsm9ds1_read(self, peer, arg, idx, buf, len);break;
	}
	return 0;
}
int driverstop(struct halfrel* self, struct halfrel* peer)
{
	struct driver* ele = (void*)(self->chip);
	say("@driverstop\n");
	switch(ele->type){
		case _mpu9250_:return mpu9250_stop(self, peer);break;
		case _lsm9ds1_:return lsm9ds1_stop(self, peer);break;
	}
	return 0;
}
int driverstart(struct halfrel* self, struct halfrel* peer)
{
	struct driver* ele = (void*)(self->chip);
	say("@driverstart\n");
	switch(ele->type){
		case _mpu9250_:return mpu9250_start(self, peer);break;
		case _lsm9ds1_:return lsm9ds1_start(self, peer);break;
	}
	return 0;
}




int driverdelete()
{
	return 0;
}
void* drivercreate(u64 type, void* url)
{
	struct driver* dr;
	say("@drivercreate: %.8s\n", &type);

	if(_mpu9250_ == type)
	{
		dr = allocdriver();
		if(0 == dr)return 0;

		dr->type = _mpu9250_;
		mpu9250_create(dr, url);
		return dr;
	}
	if(_lsm9ds1_ == type)
	{
		dr = allocdriver();
		if(0 == dr)return 0;

		dr->type = _lsm9ds1_;
		lsm9ds1_create(dr, url);
		return dr;
	}

	return 0;
}
int drivermodify(int argc, char** argv)
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
		drivercreate(name, argv[3]);
	}
	return 0;
}
int driversearch(u8* buf, int len)
{
	int j;
	for(j=0;j<64;j++)
	{
		if(0 == dri[j].type)continue;
		say("[%04x]: %.8s\n", j, &dri[j].type);
	}
	return 0;
}




void freedriver()
{
	//say("[4,8):freeing driver\n");
}
void initdriver(u8* addr)
{
	int j;
	dev = (void*)(addr+0x000000);
	dri = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct driver))
	for(j=0;j<max;j++)dri[j].tier = _dri_;

	//drivercreate(_usb_, 0);
	//say("[4,8):inited driver\n");
}
