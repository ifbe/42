#include "libhard.h"
int ncmp(void*, void*, int);



static struct device* dev;
static struct driver* dri;




int driverread(void* self, void* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int driverwrite(void* self, void* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int driverstop(void* self, void* peer, void* buf, int len)
{
	return 0;
}
int driverstart(void* self, void* peer, void* buf, int len)
{
	return 0;
}




int driverdelete()
{
	return 0;
}
void* drivercreate(u64 type, void* name)
{
	int j;
	for(j=0;j<64;j++)
	{
		if(0 == dri[j].type)
		{
			dri[j].type = type;
			return &dri[j];
		}
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
		say("[%03x]: %.8s\n", j, &dri[j].type);
	}
	return 0;
}




int driverevent(void* ev)
{
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
