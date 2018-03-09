#include "artery.h"
#define _file_ hex32('f','i','l','e')
#define _FILE_ hex32('F','I','L','E')
//
int file_create(void*);
int file_delete();
int netmgr_create(void*);
int netmgr_delete();
//
int file_cd(void*, int);
int file_write(void*);
int netmgr_cd(void*, int);
int netmgr_write(void*);
//
int cmp(void*,void*);
int ncmp(void*,void*,int);




static struct object* obj = 0;
static struct element* worker = 0;
static u8* dirhome = 0;
static u8* datahome = 0;




int arterydelete()
{
	return 0;
}
int arterycreate()
{
	return 0;
}
int arterystop()
{
	return 0;
}
int arterystart(char* name, int flag)
{
	return 0;
}
void* arteryread(int fd)
{
	return &obj[fd];
}
int arterywrite(struct event* ev)
{
	return netmgr_write(ev);
}
int arterylist(u8* buf)
{
	say("empth artery\n");
	return 0;
}
int arterychoose(u8* buf)
{
/*
	if(0 == ncmp(buf, "i2c ", 4))
	{
		if(0 == ncmp(buf+4, "ls", 2))i2c_list();
		else
		{
			i2c_choose(buf+4);
		}
	}
*/
	int j;
	u8* type = 0;
	u8* name = buf;

	for(j=0;j<0x1000;j++)
	{
		if(0 == ncmp(buf+j, "://", 3))
		{
			say("type=%.*s, name=%.*s\n", j, buf, 256, buf+j+3);
			type = buf;
			name = buf+j+3;
			break;
		}
	}

	if(0 == type)
	{
		file_cd(name, _file_);
	}
	else if(ncmp(type, "file", 4) == 0)
	{
		file_cd(name, _file_);
	}
	else if(ncmp(type, "FILE", 4) == 0)
	{
		file_cd(name, _FILE_);
	}
	else
	{
		netmgr_cd(buf, 0);
	}
	return 0;
}




void freeartery()
{
	//say("[8,c):freeing artery\n");

	netmgr_delete();
	file_delete();

	datahome = 0;
	dirhome = 0;
	worker = 0;
	obj = 0;
}
void initartery(u8* addr)
{
	obj = (struct object*)(addr+0x0);
	worker = (struct element*)(addr+0x100000);
	dirhome = addr+0x200000;
	datahome = addr+0x300000;

	file_create(addr);
	netmgr_create(addr);

	//say("[8,c):inited artery\n");
}