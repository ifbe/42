#include "libhard.h"
static struct device* dev;




int devicewrite_ev(void* ev)
{
	return 0;
}
int deviceread_all()
{
	return 0;
}




int devicewrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == dc)return devicewrite_ev(buf);
	return 0;
}
int deviceread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == sc)return deviceread_all();
	return 0;
}
int devicestop()
{
	return 0;
}
int devicestart()
{
	return 0;
}
int devicedelete()
{
	return 0;
}
void* devicecreate(u64 type, void* name)
{
	int j;
	for(j=0;j<64;j++)
	{
		if(0 == dev[j].type)
		{
			dev[j].type = type;
			return &dev[j];
		}
	}
	return 0;
}
int devicechoose(u8* buf)
{
	say("@device: %s\n", buf);
	return 0;
}
int devicelist(u8* buf)
{
	int j;
	for(j=0;j<64;j++)
	{
		if(0 == dev[j].type)continue;
		say("[%03x]: %.8s\n", j, &dev[j].type);
	}
	return 0;
}




void freedevice()
{
	//say("[4,8):freeing device\n");
}
void initdevice(u8* addr)
{
	int j;
	dev = (void*)(addr+0x000000);

#define max (0x100000/sizeof(struct device))
	for(j=0;j<0x400000;j++)addr[j]=0;
	for(j=0;j<max;j++)dev[j].tier = _dev_;

	devicecreate(_ahci_, 0);
	devicecreate(_xhci_, 0);
	//say("[4,8):inited device\n");
}