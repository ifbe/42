#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int flow_create(void* world,void* func);
int flow_delete();
int math_create(void* world,void* func);
int math_delete();
int memory_create(void* world,void* func);
int memory_delete();
int phys_create(void* world,void* func);
int phys_delete();
int system_create(void* world,void* func);
int system_delete();
int wire_create(void* world,void* func);
int wire_delete();
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
int cmp(void*,void*);
int ncmp(void*,void*,int);
//
void printmemory(void*, int);
void say(void*, ...);
//
void eventwrite(u64,u64,u64,u64);
void* eventread();




//[0x000000,0x0fffff]:worker
//[0x100000,0x1fffff]:fs
//[0x200000,0x2fffff]:dir
//[0x300000,0x3fffff]:data
struct element
{
	//[00,07]
	u64 type;

	//[08,0f]
	u64 id;

	//[10,17]
	int (*start)(u8*);
	char padding2[ 8 - sizeof(char*) ];

	//[18,1f]
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[20,27]
	int (*list)(u8*);
	char padding4[ 8 - sizeof(char*) ];

	//[28,2f]
	int (*choose)(u8*);
	char padding5[ 8 - sizeof(char*) ];

	//[30,37]
	int (*read)(u8*);
	char padding6[ 8 - sizeof(char*) ];

	//[38,3f]
	int (*write)(u8*);
	char padding7[ 8 - sizeof(char*) ];

	//[40,ff]
	u8 data[0xc0];
};
static struct element* worker = 0;
static u8* dirhome = 0;
static u8* datahome = 0;




//
static u8 cmd[256];
static int len=0;
//
static int theone = 0;
static int shutup = 0;
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




int arteryread(u8* p)
{
	if(theone > 0)worker[theone].read(p);
	else say("@arteryread\n");
	return 0;
}
int arterywrite(u8* p)
{
	if(theone > 0)worker[theone].write(p);
	else say("@arterywrite\n");
	return 0;
}




int arterystart(u8* p)
{
	if(theone > 0)worker[theone].start(p);
	else say("@arterystart\n");
	return 0;
}
int arterystop(u8* p)
{
	if(theone > 0)worker[theone].stop();
	else say("@arterystop\n");
	return 0;
}




void arterycreate(u8* type, u8* addr)
{
	u8* p;
	int j;
	if(type != 0)return;
	if( (type == 0)&&(worker != 0) )return;

	//where
	for(j=0x100000;j<0x200000;j++)addr[j] = 0;
	worker=(struct element*)(addr+0x100000);
	dirhome = addr+0x200000;
	datahome= addr+0x300000;

	//create
	p = addr+0x100100;
	p += flow_create(addr, p);
	p += math_create(addr, p);
	p += memory_create(addr, p);
	p += phys_create(addr, p);
	p += system_create(addr, p);
	p += wire_create(addr, p);
	theone = 0;

	//
	//say("[8,c):createed artery\n");
}
void arterydelete()
{
	//
	//say("[8,c):deleteing artery\n");

	//
	wire_delete();
	system_delete();
	phys_delete();
	memory_delete();
	math_delete();
	flow_delete();

	//
	worker = 0;
	dirhome = 0;
	datahome = 0;
}




int arteryprompt()
{
	if(shutup == 1)return 0;

	if(theone > 0)say("[%s]",&worker[theone].id);
	else say("[void]");

	shutup = 1;
	return 1;
}
int arterycommand(u8* buffer)
{
	int ret;
	int argc;
	u8* argv[8];
	//say("command=%s\n",buffer);
	//printmemory(buffer,16);




//------------------------------------------------------------
	//error
	if(buffer == 0)return 0;

	//passthrough?
	if( (buffer[0] == 0x1b) && (buffer[1] != '[') )combo++;
	else combo = 0;

	if(combo >= 2)
	{
		theone = 0;
		shutup = 0;
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




//------------------------------------------------------------
	//print
	ret = 0;
	for(argc=0;argc<255;argc++)
	{
		if(buffer[argc] == 0)break;

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
				shutup = 0;
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




//------------------------------------------------------------
	//convert
	buf2arg(cmd, 256, &argc, argv);
	if(argc==0)return 0;

	//"enter key"
	if(argv[0]==0)goto finish;

	//"#"
	if(argv[0][0]=='#')return 0;

	//q
	if(argv[0][0]=='q')
	{
		eventwrite(0,0,0,0);
		return 0;
	}

	//exit
	ret=cmp(argv[0],"exit");
	if(ret==0)
	{
		eventwrite(0,0,0,0);
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




//------------------------------------------------------------
	//"create","destory","start","stop"
	ret=cmp(argv[0] , "create");
	if(ret==0)
	{
		//arterycreate(argv[1]);
		goto finish;
	}
	ret=cmp(argv[0] , "delete");
	if(ret==0)
	{
		//arterydelete(argv[1]);
		goto finish;
	}
	ret=cmp(argv[0] , "start");
	if(ret==0)
	{
		arterystart(argv[1]);
		goto finish;
	}
	ret=cmp(argv[0] , "stop");
	if(ret==0)
	{
		arterystop(argv[1]);
		goto finish;
	}

	//"observe","change","get","put"
	ret=cmp(argv[0] , "ls" );
	if(ret==0)
	{
		arterylist(argv[1]);
		goto finish;
	}
	ret=cmp(argv[0] , "cd" );
	if(ret==0)
	{
		arterychoose(argv[1]);
		goto finish;
	}
	ret=cmp(argv[0] , "read" );
	if(ret==0)
	{
		arteryread(argv[1]);
		goto finish;
	}
	ret=cmp(argv[0] , "write" );  //dangerous
	if(ret==0)
	{
		arterywrite(argv[1]);
		goto finish;
	}

	//
	arterychoose(argv[0]);

finish:
	return 1;
}
