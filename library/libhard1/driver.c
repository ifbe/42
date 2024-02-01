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
void driver_init(u8* addr, int size)
{
	logtoall("[6,8):driver initing\n");

	int j;
	for(j=0;j<0x200000;j++)addr[j] = 0;

	dri = (void*)(addr+0x000000);
	drilen = maxitem-1;
	for(j=0;j<maxitem;j++)dri[j].tier = _dri_;

	bbb = (void*)(addr+0x100000);

	//drivercreate(_usb_, 0);
	logtoall("[6,8):driver inited\n");
}
void driver_exit()
{
	logtoall("[6,8):driver exiting\n");

	logtoall("[6,8):driver exited\n");
}




void driver_recycle()
{
}
void* driver_alloc()
{
	void* addr = &dri[drilen];
	drilen -= 1;
	return addr;
}
void* driver_alloc_fromtype(u64 type)
{
	_obj* obj = driver_alloc();
	if(0 == obj)return 0;

	//obj->tier = tier;		//should be tier: bootup
	//obj->kind = kind;		//should be class: usb
	obj->type = type;		//should be type: xhci
	//obj->vfmt = vfmt;		//should be model: intelxhci
	return obj;
}




int driver_create(_obj* obj, void* arg, int argc, u8** argv)
{
	logtoall("@drivercreate\n");

	switch(obj->type){
	case _mpu9250_:
		mpu9250_create(obj, arg, argc, argv);
		break;
	case _lsm9ds1_:
		lsm9ds1_create(obj, arg, argc, argv);
		break;
	case _ads1115_:
		ads1115_create(obj, arg, argc, argv);
		break;
	case _gpiotest_:
		gpiotest_create(obj, arg, argc, argv);
		break;
	case _l298n_:
		l298n_create(obj, arg, argc, argv);
		break;
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




int driver_attach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	struct item* ele = (void*)(self->chip);
	logtoall("@driverattach\n");
	switch(ele->type){
		case _mpu9250_:return mpu9250_attach(self, peer);break;
		case _lsm9ds1_:return lsm9ds1_attach(self, peer);break;
		case _ads1115_:return ads1115_attach(self, peer);break;
		case _gpiotest_:return gpiotest_attach(self, peer);break;
		case _l298n_:return l298n_attach(self, peer);break;
	}
	return 0;
}
int driver_detach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	struct item* ele = (void*)(self->chip);
	logtoall("@driverdetach\n");
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
	//logtoall("@driverwrite\n");
	switch(dri->type){
		case _mpu9250_:return mpu9250_write(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _lsm9ds1_:return lsm9ds1_write(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _ads1115_:return ads1115_give(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _gpiotest_:return gpiotest_give(dri,foot, stack,sp, arg,idx, buf,len);break;
		case _l298n_:return l298n_give(dri,foot, stack,sp, arg,idx, buf,len);break;
	}
	return 0;
}




int drivercommand_insert(u8* name, u8* arg)
{
	return 0;
}
int drivercommand_remove(u8* name)
{
	return 0;
}
int drivercommand_search(u8* name)
{
	int j;
	_obj* act;
	if(0 == name){
		for(j=0;j<maxitem;j++){
			act = &dri[j];
			if(0 == act->type)continue;
			logtoall("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->kind, &act->type, &act->vfmt);
		}
		if(0 == j)logtoall("empty driver\n");
	}
	else{
		for(j=0;j<0x100;j++){
			if(0 == dri[j].type)break;
			if(0 == cmp(&dri[j].type, name))logtoall("name=%d,node=%p\n", name, &dri[j]);
			break;
		}
	}
	return 0;
}
int drivercommand_modify(int argc, u8** argv)
{
	return 0;
}
void* drivercommand(int argc, u8** argv)
{
	if(argc < 2){
		logtoall("driver insert name arg\n");
		logtoall("driver search name\n");
	}
	else if(0 == ncmp(argv[1], "insert", 6)){
		//driver create name arg
		drivercommand_insert(argv[2], argv[3]);
	}
	else if(0 == ncmp(argv[1], "search", 6)){
		//driver search <name>
		drivercommand_search((argc<3) ? 0 : argv[2]);
	}

	return 0;
}
