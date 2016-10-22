#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int interface_create(void* world,void* func);
int    memory_create(void* world,void* func);
int       net_create(void* world,void* func);
int   special_create(void* world,void* func);
//
int interface_delete();
int    memory_delete();
int       net_delete();
int   special_delete();
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
int cmp(char*,char*);
int ncmp(char*,char*,int);
//
void eventread(u64* who, u64* what, u64* how);
void eventwrite(u64,u64);
//
void printmemory(char*,int);
void say(char*,...);




//[0x000000,0x0fffff]:worker
//[0x100000,0x1fffff]:fs
//[0x200000,0x2fffff]:dir
//[0x300000,0x3fffff]:data
static struct elements
{
	//种类，名字
	u64 type;
	u64 id;
	u64 what1;
	u64 what2;

	//
	u64 aa;
	u64 bb;
	u64 cc;
	u64 dd;

	//[0x40,0x47]
	int (*create)();
	char padding0[ 8 - sizeof(char*) ];

	//[0x48,0x4f]
	int (*delete)();
	char padding1[ 8 - sizeof(char*) ];

	//[0x50,0x57]
	int (*start)(char*);
	char padding2[ 8 - sizeof(char*) ];

	//[0x58,0x5f]
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[0x60,0x67]
	int (*list)(char*);
	char padding4[ 8 - sizeof(char*) ];

	//[0x68,0x6f]
	int (*choose)(char*);
	char padding5[ 8 - sizeof(char*) ];

	//[0x70,0x77]
	int (*read)(char*);
	char padding6[ 8 - sizeof(char*) ];

	//[0x78,0x7f]
	int (*write)(char*);
	char padding7[ 8 - sizeof(char*) ];
}*worker;
static unsigned char*   fshome;
static unsigned char*  dirhome;
static unsigned char* datahome;




//pci->usb->video->h264->stream
//bin->partworker->filesystem->dir->file....
//tcp->http->websocket->what
//static int stack[16]={0};
static u8* stack;
static int rsp=0;
//
static char cmd[256];
static int dst=0;
//
static int shutup=0;
static int combo=0;




int arterylist(char* p)
{
	int j;
	int count;
	u64 type;
	u64 id;

	//0
	if(rsp == 0)
	{
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
				say("	[%s]",&id);
				count++;
				if(count%8==0)say("\n");
			}
		}
		return 0;
	}

	//1,2,3,4,5......
	else
	{
		id = stack[rsp];
		return worker[id].list(p);
	}
}
int arterychoose(char* p)
{
	int ret;
	u64 who;
	if(p == 0)return -1;

	if(rsp == 0)
	{
		//search somewhere to go
		for(ret=1;ret<256;ret++)
		{
			if(cmp(p,(char*)&worker[ret].id)==0)
			{
				break;
			}
		}
		if(ret>=256)
		{
			say("not found\n");
			return 0;
		}

		//now go into it
		rsp++;
		stack[rsp] = ret;

		return 0;
	}

	//1,2,3,5,6......
	else if(rsp == 1)
	{
		who = stack[1];
		if(who != 0)ret = worker[who].choose(p);
		if(ret > 0)rsp++;
	}

	else
	{
		who = stack[1];
		if(who != 0)ret = worker[who].choose(p);
	}
}




int arteryread(char* p)
{
	u64 who;
	if(rsp == 0)
	{
		say("@arteryread\n");
		return 0;
	}
	else
	{
		who = stack[rsp];
		return worker[who].read(p);
	}
}
int arterywrite(char* p)
{
	u64 who;
	if(rsp==0)
	{
		say("@arterywrite\n");
		return 0;
	}
	else
	{
		who = stack[rsp];
		return worker[who].write(p);
	}
}




int arterystart(u8* p)
{
	u64 who;
	if(rsp==0)
	{
		say("@arterystart\n");
		return 0;
	}
	else
	{
		who = stack[rsp];
		return worker[who].start(p);
	}
}
int arterystop(char* p)
{
	u64 who;
	if(rsp==0)
	{
		say("@arterystop\n");
		return 0;
	}
	else
	{
		who = stack[rsp];
		return worker[who].stop();
	}
}




void arterycreate(char* module,char* addr)
{
	int i;
	char* p;

	//
	if(module==0)
	{
		for(i=0;i<0x100000;i++)addr[i] = 0;
		addr[0] = 0x34;
		addr[1] = 0x32;

		//0
		fshome  = addr+0x100000;
		dirhome = addr+0x200000;
		datahome= addr+0x300000;

		rsp = 0;
		stack = addr;

		worker=(struct elements*)addr;

		//
		p = addr+0x80;
		p += interface_create(addr,p);
		p += memory_create(addr,p);
		p += net_create(addr,p);
		p += special_create(addr,p);

		say("[8,c):createed artery\n");
	}

	//
	else
	{
		//createmodule(module);
		say("(createmodule fail)%s\n",module);
	}
}
void arterydelete(char* module)
{
	say("[8,c):deleteing artery\n");
	special_delete();
	net_delete();
	memory_delete();
	interface_delete();
}




int arteryprompt()
{
	u64 who;
	if(shutup == 1)return 0;

	if(rsp == 0)say("[void]");
	else if(rsp == 1)
	{
		who = stack[rsp];
		say("[%s]",&worker[who].id);
	}

	shutup = 1;
	return 1;
}
int arterycommand(char* buffer)
{
	int ret;
	int argc;
	u64 who;
	u8* argv[8];
	//say("command=%s\n",buffer);
	//printmemory(buffer,16);




//1111111111111111111111111111111111111111111111111111111111111
	//error
	if(buffer == 0)goto finish;

	//special
	//say("	(%x)\n",buffer[0]);
	if( (buffer[0] == 0x1b) && (buffer[1] != '[') )combo++;
	else combo = 0;

	//
	if( (rsp > 1) && (combo < 2) )
	{
		//pass through
		who = stack[1];
		if(who>0)worker[who].write(buffer);

		return 0;
	}
	else if(combo >= 2)
	{
		if(rsp <= 0)
		{
			eventwrite(0,0);
			return 0;
		}

		if(rsp > 1)
		{
			//tell it, i'm leaving
			who = stack[1];
			if(who>0)worker[who].choose(0);
		}
		rsp--;

		//cmd[]=0;
		dst = 0;

		combo = 0;
		shutup = 0;

		say("\n");
		return 0;
	}




	//print
	if(rsp <= 1)
	{
		ret = 0;
		for(argc=0;argc<255;argc++)
		{
			if(buffer[argc] == 0)break;

			if( (buffer[argc] == 0x8) | (buffer[argc] == 0x7f) )
			{
				say("\b \b");
				if(dst>0)
				{
					cmd[dst] = 0;
					dst--;
				}
			}
			else
			{
				if(buffer[argc] == 0x1b)continue;
				if(buffer[argc] == 0xd)buffer[argc] = 0xa;
				say("%c",buffer[argc]);

				if(buffer[argc] == 0xa)
				{
					cmd[dst] = 0;
					dst = 0;

					ret = 1;
					shutup = 0;
				}
				else if(dst<256)
				{
					cmd[dst] = buffer[argc];
					dst++;
				}
				else dst = 0;
			}
		}
		if(ret == 0)return 0;
	}
	//say("here:%s\n",cmd);

	//convert
	buf2arg(cmd,128,&argc,argv);
	if(argc==0)return 0;




//222222222222222222222222222222222222222222222222222222222222
	//"enter key"
	if(argv[0]==0)goto finish;

	//"#"
	if(argv[0][0]=='#')return 0;

	//q
	if(argv[0][0]=='q')
	{
		eventwrite(0,0);
		return 0;
	}

	//exit
	ret=cmp(argv[0],"exit");
	if(ret==0)
	{
		eventwrite(0,0);
		return 0;
	}

	//'help'
	ret=cmp(argv[0],"help");
	if(ret==0)
	{
		//"create","destory","start","stop"
		say("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		say("create ?	     =create=make=fabricate\n");
		say("delete ?	     =destory=smash=wreck\n");
		say("start ?	    =open=mount=enter\n");
		say("stop ?	     =close=unmount=leave\n");

		//"observe","change","get","put"
		say("ls ?	       =list=summary=view=check\n");
		say("cd ?	       =choose=into=switch=clap\n");
		say("read ?	     =load=get=eat=copy\n");
		say("write ?	    =store=put=spit=paste\n");
		say("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		goto finish;
	}




//33333333333333333333333333333333333333333333333333333333333
	//"create","destory","start","stop"
	ret=cmp( argv[0] , "create");	       //eg:   dynamicmodule
	if(ret==0)
	{
		//arterycreate(argv[1]);
		goto finish;
	}
	ret=cmp( argv[0] , "delete");
	if(ret==0)
	{
		//arterydelete(argv[1]);
		goto finish;
	}
	ret=cmp( argv[0] , "start");
	if(ret==0)
	{
		arterystart(argv[1]);
		goto finish;
	}
	ret=cmp( argv[0] , "stop");
	if(ret==0)
	{
		arterystop(argv[1]);
		goto finish;
	}

	//"observe","change","get","put"
	ret=cmp( argv[0] , "ls" );
	if(ret==0)
	{
		arterylist(argv[1]);
		goto finish;
	}
	ret=cmp( argv[0] , "cd" );
	if(ret==0)
	{
		arterychoose(argv[1]);
		goto finish;
	}
	ret=cmp( argv[0] , "read" );
	if(ret==0)
	{
		arteryread(argv[1]);
		goto finish;
	}
	ret=cmp( argv[0] , "write" );  //dangerous
	if(ret==0)
	{
		arterywrite(argv[1]);
		goto finish;
	}

finish:
	return 1;
}
