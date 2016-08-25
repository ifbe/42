#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int code_create(char*,char*);
int fs_create(char*,char*);
int pt_create(char*,char*);
void code_delete();
void fs_delete();
void pt_delete();
//
int systemstart(int,char*);
int systemstop();
int systemlist(char*);
int systemchoose(char*);
int systemread(char* rdi,u64 rsi,u64 rcx);
int systemwrite(char* rdi,u64 rsi,u64 rcx);
//
u64 prelibation(char*);
int compare(char*,char*);	//base tool
int hexstring2data(char*,u64*);
int mem2file(char* memaddr,char* filename,u64 offset,u64 count);
int file2mem(char* memaddr,char* filename,u64 offset,u64 count);
//
int printmemory(char* addr,int count);
int say(char* str,...);		//+1




//
static char* diskhome=0;
static char* fshome=0;
static char* dirhome=0;
static char* datahome=0;




//physical function
static int memory_list(char* arg1)
{
	u64 temp=0;
	int i,j;
	char buf[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	u64 target=0;
	char* addr=diskhome;
	//printmemory(diskhome,0x200);

	//想要什么
	if(arg1==0)target=0;
	else if( compare(arg1,"disk") == 0 )target=0x6b736964;	//'disk'
	else if( compare(arg1,"file") == 0 )
	{
		addr=dirhome;
		target=0;
		//target=0x656c6966;		//'file'
		//printmemory(addr,0x200);
	}
	else if( compare(arg1,"0") == 0 )
	{
		addr=diskhome;
		target=0;
	}
	else if( compare(arg1,"1") == 0 )
	{
		addr=fshome;
		target=0;
	}
	else if( compare(arg1,"2") == 0 )
	{
		addr=dirhome;
		target=0;
	}
	else if( compare(arg1,"3") == 0 )
	{
		addr=datahome;
		target=0;
	}

	//搜到就显示
	for(i=0; i<0x400; i++)		//0x40*0x400=0x10000
	{
		temp=*(u64*)( addr + (i*0x40) );
		if(temp == 0)break;
		if( (target==0) | (temp == target) )
		{
			//[+0]:type
			say("(%-4s," , addr+(i*0x40) );

			//[+8]:id
			*(u64*)buf=*(u64*)(addr+(i*0x40)+0x8);
			temp=0;
			for(j=0;j<8;j++)
			{
				//[1,0x1f]:wrong
				if(buf[j]>0 && buf[j]<0x20) temp++;

				//[0x80,0xff]:wrong
				if(buf[j]>=0x80) temp++;
			}
			if(temp==0) say("%4s)	",buf);
			else say("%4llx)	",*(u64*)buf);

			//[+10]:start
			say("[%-4llx,",*(u64*)(addr+(i*0x40)+0x10));

			//[+18]:end
			say("%4llx]	",*(u64*)(addr+(i*0x40)+0x18));

			//[+20]:detail
			say("{%-16s}	",addr+(i*0x40)+0x20);

			//which
			say("<%x>\n",i);
		}//if
	}//for

	return 1;
}//memory_list
static int memory_choose(char* arg)
{
}
static int memory_read(char* arg1)
{
	u64 value;

	//nothing specified
	if(arg1==0)
	{
		say("read@%llx,world@%llx\n",memory_read,diskhome);
		return -1;
	}

	//what is it
	value=0;
	if( arg1[0] < '0' )value++;	//[0,0x2f]:wrong
	if( arg1[0] > 'f' )value++;	//[0x67,0xff]:wrong
	if( arg1[0] > '9' )		//[0x3a,0x60]:wrong
	{
		if( arg1[0] < 'a' )value++;
	}

	//default,read chosen memory/port/disk/socket...
	if(value==0)
	{
		hexstring2data(arg1,&value);
		systemread(datahome,value,1);
		printmemory(datahome,0x200);
		say("above is:%llx\n",value);
	}

	return 0;
}
static int memory_write(char* arg1)
{
	u64 value;

	//nothing specified
	if(arg1==0)
	{
		say("write@%llx,world@%llx\n",memory_write,diskhome);
		return -1;
	}

	//what is it
	value=0;
	if( arg1[0] < '0' )value++;	//[0,0x2f]:wrong
	if( arg1[0] > 'f' )value++;	//[0x67,0xff]:wrong
	if( arg1[0] > '9' )		//[0x3a,0x60]:wrong
	{
		if( arg1[0] < 'a' )value++;
	}

	//"read memory.400000"
	if(value==0)
	{
		say("dangerous,bye\n");
	}

	return 0;
}








static int memory_start(u64 type,char* p)
{
	int ret;

	//start
	ret=systemstart(1,p);
	if(ret<=0)return -1;

	//return
	return 1;
}
static int memory_stop()
{
	systemstop();
}
int memory_create(char* world,u64* p)
{
	//(自己)4块区域，每块1兆
	diskhome=world+0;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	//
	p[0]=0;
	p[1]=0x79726f6d656d;
	p[2]=(u64)memory_start;
	p[3]=(u64)memory_stop;
	p[4]=(u64)memory_list;
	p[5]=(u64)memory_choose;
	p[6]=(u64)memory_read;
	p[7]=(u64)memory_write;

	//
	char* q=(char*)p+0x40;

	code_create(world,q);
	q+=0x40;

	fs_create(world,q);
	q+=0x40;

	pt_create(world,q);
	q+=0x40;

	return q-(char*)p;
}
int memory_delete()
{
	code_delete();
	fs_delete();
	pt_delete();
}
