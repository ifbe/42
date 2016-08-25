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
u64 prelibation(void*);
int compare(char*,char*);
int systemread( char* memory,u64 sector,u64 count);
int systemwrite(char* memory,u64 sector,u64 count);
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2typename(u8* buf,int max,u64* type,u8** name);
void printmemory(char*,int);
void say(char*,...);




//[0x000000,0x0fffff]:worker
//[0x100000,0x1fffff]:fs
//[0x200000,0x2fffff]:dir
//[0x300000,0x3fffff]:data
static struct elements
{
	//[0,7]:种类
	u64 type;

	//[8,f]:名字
	u64 id;

	//[0x10,0x17]
	int (*start)(u64,char*);
	char padding2[ 8 - sizeof(char*) ];

	//[0x18,0x1f]
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[0x20,0x27]
	int (*list)(char*);
	char padding4[ 8 - sizeof(char*) ];

	//[0x28,0x2f]
	int (*choose)(char*);
	char padding5[ 8 - sizeof(char*) ];

	//[0x30,0x37]
	int (*read)(char*);
	char padding6[ 8 - sizeof(char*) ];

	//[0x38,0x3f]
	int (*write)(char*);
	char padding7[ 8 - sizeof(char*) ];
}*table;
static unsigned char*   fshome;
static unsigned char*  dirhome;
static unsigned char* datahome;




//pci->usb->video->h264->stream
//bin->parttable->filesystem->dir->file....
//tcp->http->websocket->what
//static int stack[16]={0};
static int this=0;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int arterylist(char* p)
{
	int j;
	int count;
	u64 type;
	u64 id;

	//@somewhere
	if(this!=0)
	{
		return table[this].list(p);
	}

	//@nowhere
	for(j=1;j<0x100;j++)
	{
		type=table[j].type;
		id=table[j].id;
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

	//cd .(current directory)
	if( (p!=0) && (p[0]=='.') )
	{
		//cd ..(parent directory)
		if(p[1]=='.')
		{
			//cd ...(the void)
			if(p[2]=='.')
			{
				table[0].type=0x3234;
				this=0;
				return 0;
			}
		}
	}

	//i am not in the void?
	if(this != 0)
	{
		//say("");
		return table[this].choose(p);
	}

	//search somewhere to go
	for(ret=1;ret<256;ret++)
	{
		if(compare(p,(char*)&table[ret].id)==0)
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
	this=ret;
	table[0].type=table[ret].id;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int arteryread(char* p)
{
	if(this==0)return 0;
	return table[this].read(p);
}
int arterywrite(char* p)
{
	if(this==0)return 0;
	return table[this].write(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int arterystart(u8* p)
{
	if(this==0)return 0;
	return table[this].start(0,p);
}
int arterystop(char* p)
{
	if(this==0)return 0;
	return table[this].stop();
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
		for(i=0;i<0x10000;i++)addr[i]=0;
		addr[0]=0x34;
		addr[1]=0x32;

		//0
		table=(struct elements*)addr;
		fshome  =addr+0x100000;
		dirhome =addr+0x200000;
		datahome=addr+0x300000;

		//
		p=addr+0x40;
		p+=interface_create(addr,p);
		p+=memory_create(addr,p);
		p+=net_create(addr,p);
		special_create(addr,p);

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
