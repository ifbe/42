#include "libhard.h"
//
#define _lsm9ds1_ hex64('l','s','m','9','d','s','1',0)
int lsm9ds1_create(struct driver* ele, void* url, int argc, u8** argv);
int lsm9ds1_delete(struct driver* ele);
int lsm9ds1_linkup(struct halfrel* self, struct halfrel* peer);
int lsm9ds1_discon(struct halfrel* self, struct halfrel* peer);
int lsm9ds1_write( _dri* dri,int foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);
int lsm9ds1_read(  _dri* dri,int foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);
//
#define _mpu9250_ hex64('m','p','u','9','2','5','0',0)
int mpu9250_create(struct driver* ele, void* url, int argc, u8** argv);
int mpu9250_delete(struct driver* ele);
int mpu9250_linkup(struct halfrel* self, struct halfrel* peer);
int mpu9250_discon(struct halfrel* self, struct halfrel* peer);
int mpu9250_write( _dri* dri,int foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);
int mpu9250_read(  _dri* dri,int foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);




static struct driver* dri;
static int drilen = 0;
static void* bbb;
static int bbblen = 0;
void* allocdriver()
{
	void* addr = &dri[drilen];
	drilen += 1;
	return addr;
}




int driverwrite(_dri* dri,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	//say("@driverwrite\n");
	switch(dri->type){
		case _mpu9250_:return mpu9250_write(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _lsm9ds1_:return lsm9ds1_write(dri,foot, stack,sp, arg,idx, buf,len);break;
	}
	return 0;
}
int driverread(_dri* dri,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	switch(dri->type){
		case _mpu9250_:mpu9250_read(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _lsm9ds1_:lsm9ds1_read(dri,foot, stack,sp, arg,idx, buf,len);break;
	}
	return 0;
}
int driverdiscon(struct halfrel* self, struct halfrel* peer)
{
	struct driver* ele = (void*)(self->chip);
	say("@driverdiscon\n");
	switch(ele->type){
		case _mpu9250_:return mpu9250_discon(self, peer);break;
		case _lsm9ds1_:return lsm9ds1_discon(self, peer);break;
	}
	return 0;
}
int driverlinkup(struct halfrel* self, struct halfrel* peer)
{
	struct driver* ele = (void*)(self->chip);
	say("@driverlinkup\n");
	switch(ele->type){
		case _mpu9250_:return mpu9250_linkup(self, peer);break;
		case _lsm9ds1_:return lsm9ds1_linkup(self, peer);break;
	}
	return 0;
}




int driverdelete(void* this)
{
	return 0;
}
void* drivercreate(u64 type, void* url, int argc, u8** argv)
{
	struct driver* dr;
	say("@drivercreate: %.8s\n", &type);

	if(_mpu9250_ == type)
	{
		dr = allocdriver();
		if(0 == dr)return 0;

		dr->type = _mpu9250_;
		mpu9250_create(dr, url, argc, argv);
		return dr;
	}
	if(_lsm9ds1_ == type)
	{
		dr = allocdriver();
		if(0 == dr)return 0;

		dr->type = _lsm9ds1_;
		lsm9ds1_create(dr, url, argc, argv);
		return dr;
	}

	return 0;
}
int drivermodify(int argc, u8** argv)
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
		drivercreate(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
int driversearch(u8* buf, int len)
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




void freedriver()
{
	say("[6,8):driver freeing\n");

	say("[6,8):driver freeed\n");
}
void initdriver(u8* addr)
{
	say("[6,8):driver initing\n");

	int j;
	dri = (void*)(addr+0x000000);
	bbb = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct driver))
	for(j=0;j<0x200000;j++)addr[j] = 0;
	for(j=0;j<max;j++)dri[j].tier = _dri_;

	//drivercreate(_usb_, 0);
	say("[6,8):driver inited\n");
}
