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
int compare(char*,char*);
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
static int now=0;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int arterylist(char* p)
{
	int j;
	int count;
	u64 type;
	u64 id;

	//@somewhere
	if(now!=0)
	{
		return worker[now].list(p);
	}

	//@nowhere
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
int arterychoose(char* p)
{
	int ret;
	if(p == 0)return -1;

	//cd .(current directory)
	if(p[0] == '.')
	{
		//cd ..(parent directory)
		if(p[1]=='.')
		{
			//cd ...(the void)
			if(p[2]=='.')
			{
				worker[0].type=0x3234;
				now=0;
				return 0;
			}
		}
	}

	//i am not in the void?
	if(now != 0)
	{
		//say("");
		return worker[now].choose(p);
	}

	//search somewhere to go
	for(ret=1;ret<256;ret++)
	{
		if(compare(p,(char*)&worker[ret].id)==0)
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
	now=ret;
	worker[0].type=worker[ret].id;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int arteryread(char* p)
{
	if(now==0)return 0;
	return worker[now].read(p);
}
int arterywrite(char* p)
{
	if(now==0)return 0;
	return worker[now].write(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int arterystart(u8* p)
{
	if(now==0)return 0;
	return worker[now].start(p);
}
int arterystop(char* p)
{
	if(now==0)return 0;
	return worker[now].stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
		worker=(struct elements*)addr;
		fshome  = addr+0x100000;
		dirhome = addr+0x200000;
		datahome= addr+0x300000;

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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
