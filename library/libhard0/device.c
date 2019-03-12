#include "libhard.h"
int ncmp(void*, void*, int);



static struct device* dev;




int device_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}
int device_rootread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}
int device_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}
int device_leafread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
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








int deviceevent(void* ev)
{
	return 0;
}
void* devicecommand(int argc, char** argv)
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
		devicecreate(name, argv[3]);
	}
	return 0;
}
int deviceread_all()
{
	return 0;
}
void* devicelist(u8* buf)
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

	//devicecreate(_ahci_, 0);
	//devicecreate(_xhci_, 0);
	//say("[4,8):inited device\n");
}
