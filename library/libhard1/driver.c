#include "libhard.h"
static struct device* dev;
static struct driver* dri;




int driverwrite_ev(void* ev)
{
	return 0;
}
int driverread_all()
{
	return 0;
}




int driverwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == dc)return driverwrite_ev(buf);
	return 0;
}
int driverread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == sc)return driverread_all();
	return 0;
}
int driverstop()
{
	return 0;
}
int driverstart()
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
int driverchoose(u8* buf)
{
	say("@driver: %s\n", buf);
	return 0;
}
int driverlist(u8* buf)
{
	int j;
	for(j=0;j<64;j++)
	{
		if(0 == dri[j].type)continue;
		say("[%03x]: %.8s\n", j, &dri[j].type);
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

	drivercreate(_usb_, 0);
	//say("[4,8):inited driver\n");
}