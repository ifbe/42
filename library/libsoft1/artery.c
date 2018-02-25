#include "artery.h"
#define _file_ hex32('f','i','l','e')
#define _FILE_ hex32('F','I','L','E')
//
int flow_create(void*);
int flow_delete();
int file_create(void*);
int file_delete();
int wire_create(void*);
int wire_delete();
//
int sound_explain(void*);
int vision_explain(void*);
int network_explain(void*);
//
int file_write(void*, int);
int netmgr_write(void*);
int readshell(int fd, char* buf, int off, int len);
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
int cmp(void*,void*);
int ncmp(void*,void*,int);




static struct object* obj = 0;
static struct element* worker = 0;
static u8* dirhome = 0;
static u8* datahome = 0;
int artery_explain(struct event* ev)
{
	int ret;
	u64 where;
	u64 type;
	struct uartinfo* info;

	ret = (ev->what)&0xff;
	if(ret == 's')return sound_explain(ev);
	else if(ret == 'v')return vision_explain(ev);

	where = ev->where;
	type = obj[where].sock;
	if(type == hex32('u','a','r','t'))
	{
		info = (void*)(&obj[where].info);
		while(1)
		{
			ret = 0x100000 - (info->enq);
			if(ret > 256)ret = 256;

			ret = readshell(where, (info->buf)+(info->enq), 0, ret);
			if(ret <= 0)break;

			info->enq = ((info->enq)+ret)%0x100000;
		}
		ev->why = (u64)info;
		ev->what = __uart__;
		return 42;
	}
	else
	{
		return network_explain(ev);
	}
}




//--------------------------------------------------------
void arterycreate(u8* addr)
{
	obj = (struct object*)(addr+0x0);
	worker = (struct element*)(addr+0x100000);
	dirhome = addr+0x200000;
	datahome = addr+0x300000;

	file_create(addr);
	flow_create(addr);
	wire_create(addr);

	//say("[8,c):createed artery\n");
}
void arterydelete()
{
	//
	//say("[8,c):deleteing artery\n");

	//
	wire_delete();
	flow_delete();
	file_delete();

	//
	datahome = 0;
	dirhome = 0;
	worker = 0;
	obj = 0;
}
int arterystart(char* name, int flag)
{
	return 0;
}
int arterystop()
{
	return 0;
}
int arterylist()
{
	int j;
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].sock)continue;
		say("[%x]=%x\n", j, obj[j].sock);
	}
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
		file_write(name, _file_);
	}
	else if(ncmp(type, "file", 4) == 0)
	{
		file_write(name, _file_);
	}
	else if(ncmp(type, "FILE", 4) == 0)
	{
		file_write(name, _FILE_);
	}
	else
	{
		netmgr_write(buf);
	}
	return 0;
}
int arteryread(int fd, u8* buf, int off, int len)
{
	return 0;
}
int arterywrite(int fd, u8* buf, int off, int len)
{
	return 0;
}




/*
static u8 cmd[256];
static int len=0;
static int theone = 0;
static int combo = 0;
int arterylist(u8* p)
{
	int j;
	int count;
	u64 type;
	u64 id;
	if(theone > 0)return worker[theone].list(p);

	//
	count = 0;
	for(j=1;j<0x100;j++)
	{
		type=worker[j].type;
		id=worker[j].id;
		if(id==0)
		{
			if(count%8!=0)say("\n");
			break;
		}

		if(type==0)
		{
			say("\n%s:\n",&id);
			count=0;
		}
		else
		{
			if((count>0)&&(count%8==0))say("\n");

			say("	%s",&id);
			count++;
		}
	}
	return 0;
}
int arterychoose(u8* p)
{
	int ret;
	if(p == 0)return 0;
	if(theone > 0)return worker[theone].choose(p);

	//search somewhere to go
	for(ret=1;ret<256;ret++)
	{
		if(cmp(p, &worker[ret].id) == 0)break;
	}
	if(ret>=256)
	{
		say("not found\n");
		return 0;
	}

	//now go into it
	theone = ret;
	worker[0].type = 0;
	worker[0].id = worker[theone].id;
	return 0;
}
int arterywrite(u8* buffer)
{
	u8* argv[8];
	int ret;
	int argc;
	if(buffer == 0)return 0;

	//passthrough?
	if( (buffer[0] == 0x1b) && (buffer[1] != '[') )combo++;
	else combo = 0;

	if(combo >= 2)
	{
		theone = 0;
		combo = 0;
		len = 0;
		worker[0].type = 0;

		//
		say("\n");
		return 0;
	}

	//passthrough!
	if( (theone != 0) && (worker[0].type > 0) )
	{
		worker[theone].write(buffer);
		return 0;
	}




	//------------------------------------------------------
	ret = 0;
	for(argc=0;argc<255;argc++)
	{
		if(buffer[argc] == 0)break;

		//print
		if( (buffer[argc] == 0x8) | (buffer[argc] == 0x7f) )
		{
			say("\b \b");
			if(len>0)
			{
				cmd[len] = 0;
				len--;
			}
		}
		else
		{
			if(buffer[argc] == 0x1b)continue;
			if(buffer[argc] == 0xd)buffer[argc] = 0xa;
			say("%c",buffer[argc]);

			if(buffer[argc] == 0xa)
			{
				cmd[len] = 0;
				len = 0;

				ret = 1;
			}
			else if(len<256)
			{
				cmd[len] = buffer[argc];
				len++;
			}
			else len = 0;
		}
	}
	if(ret == 0)return 0;
	//say("here:%s\n",cmd);




	//---------------------------------------------------
	//convert
	buf2arg(cmd, 256, &argc, argv);
	if(argc==0)return 0;

	//"enter key"
	if(argv[0]==0)return 0;

	//"#"
	if(argv[0][0]=='#')return 0;

	//q
	if( (argv[0][0]=='q') | (cmp(argv[0],"exit") == 0) )
	{
		eventwrite(0,0,0,0);
		return 0;
	}

	//feed child
	if(theone != 0)
	{
		worker[theone].write(argv[0]);

		if(ncmp(argv[0], "...", 3) == 0)theone = 0;
		return 1;
	}




	//---------------------------------------------------
	//feed self
	if(cmp(argv[0], "create") == 0)arterylist(0);
	else if(cmp(argv[0], "delete") == 0)arterylist(0);
	else if(cmp(argv[0], "start") == 0)arterylist(0);
	else if(cmp(argv[0], "stop") == 0)arterylist(0);
	else if(cmp(argv[0], "ls") == 0)arterylist(0);
	else if(cmp(argv[0], "cd") == 0)arterylist(0);
	else if(cmp(argv[0], "read") == 0)arterylist(0);
	else if(cmp(argv[0], "write") == 0)arterylist(0);

	//search somewhere to go
	for(ret=1;ret<256;ret++)
	{
		if(cmp(argv[0], &worker[ret].id) == 0)
		{
			theone = ret;
			worker[0].type = 0;
			worker[0].id = worker[theone].id;
			break;
		}
	}
	return 1;
}
*/
