#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int interface_init(void* world,void* func);
int    memory_init(void* world,void* func);
int       net_init(void* world,void* func);
int   special_init(void* world,void* func);
//
int interface_kill();
int    memory_kill();
int       net_kill();
int   special_kill();
//
QWORD prelibation(void*);
int compare(char*,char*);
int systemread( char* memory,QWORD sector,QWORD count);
int systemwrite(char* memory,QWORD sector,QWORD count);
int buf2arg(BYTE* buf,int max,int* argc,BYTE** argv);
int buf2typename(BYTE* buf,int max,QWORD* type,BYTE** name);
void printmemory(char*,int);
void say(char*,...);




//[0x000000,0x0fffff]:worker
//[0x100000,0x1fffff]:fs
//[0x200000,0x2fffff]:dir
//[0x300000,0x3fffff]:data
static struct elements
{
	//[0,7]:种类
	QWORD type;

	//[8,f]:名字
	QWORD id;

	//[0x10,0x17]
	int (*start)(QWORD,char*);
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
	QWORD type;
	QWORD id;

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
int arterystart(BYTE* p)
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
void arteryinit(char* module,char* addr)
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
		p+=interface_init(addr,p);
		p+=memory_init(addr,p);
		p+=net_init(addr,p);
		special_init(addr,p);

		say("[8,c):inited artery\n");
	}

	//
	else
	{
		//initmodule(module);
		say("(initmodule fail)%s\n",module);
	}
}
void arterykill(char* module)
{
	say("[8,c):killing artery\n");
	special_kill();
	net_kill();
	memory_kill();
	interface_kill();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
