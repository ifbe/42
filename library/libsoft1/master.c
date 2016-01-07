#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//servent
void initservent(char*);
int hello(char*);
int ls(char*);
int cd(char*);
int load(char*);
int store(char*);
//libsoft1
int compare(char*,char*);	//base tool
int hexstring2data(char*,QWORD*);
int buf2arg(char*,char**,char**);
//libsoft0/
int listmemory(char*);
int intomemory(char*);
int readmemory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int writememory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int mem2file(char* memaddr,char* filename,QWORD offset,QWORD count);
int file2mem(char* memaddr,char* filename,QWORD offset,QWORD count);
//libboot1
int printmemory(char* addr,int count);
int say(char* str,...);		//+1
int diary(char* str,...);	//+2
int history(char* str,...);	//+3
//libboot0
int platformread(char*);
int platformwrite(char*);




//每个1M
static char* diskhome;		//+0m
static char* fshome;		//+1m
static char* dirhome;		//+2m
static char* datahome;		//+3m




//physical function
void masterlist(char* arg1)
{
	QWORD temp=0;
	int i,j;
	char buf[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	QWORD target=0;
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
		temp=*(QWORD*)( addr + (i*0x40) );
		if(temp == 0)break;
		if( (target==0) | (temp == target) )
		{
			//[+0]:type
			diary("(%-4s," , addr+(i*0x40) );

			//[+8]:id
			*(QWORD*)buf=*(QWORD*)(addr+(i*0x40)+0x8);
			temp=0;
			for(j=0;j<8;j++)
			{
				//[1,0x1f]:wrong
				if(buf[j]>0 && buf[j]<0x20) temp++;

				//[0x80,0xff]:wrong
				if(buf[j]>=0x80) temp++;
			}
			if(temp==0) diary("%4s)	",buf);
			else diary("%4llx)	",*(QWORD*)buf);

			//[+10]:start
			diary("[%-4llx,",*(QWORD*)(addr+(i*0x40)+0x10));

			//[+18]:end
			diary("%4llx]	",*(QWORD*)(addr+(i*0x40)+0x18));

			//[+20]:detail
			diary("{%-16s}	",addr+(i*0x40)+0x20);

			//which
			diary("<%d>\n",i);
		}//if
	}//for
}//masterlist
void masterinto(char* arg)
{
	int ret=0;
	QWORD temp=0;

	//0.如果传进来0，仅重新扫描所有硬盘
	if(arg == 0)
	{
		listmemory(diskhome);
		return;
	}

	//1.如果传进来1，打开默认的硬盘或文件
	if((QWORD)arg == 1)
	{
		masterinto(diskhome+0x20);
		return;
	}

	//2.其他情况，比如要\\.\PhysicalDrive0
	//选中并且喊仆人自己读开头64个扇区，来检查“东西”种类
	intomemory(arg);
	ret=hello(0);
}
int masterread(char* arg1)
{
	QWORD value;

	//nothing specified
	if(arg1==0)
	{
		diary("masterread@%llx,world@%llx\n",masterread,diskhome);
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
		readmemory(datahome,value,0,1);
		printmemory(datahome,0x200);
		diary("above is:%llx\n",value);
		return 0;
	}

	else return platformread(arg1);
}
int masterwrite(char* arg1)
{
	QWORD value;

	//nothing specified
	if(arg1==0)
	{
		diary("masterwrite@%llx,world@%llx\n",masterwrite,diskhome);
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
		diary("dangerous,bye\n");
		return 0;
	}

	else return platformwrite(arg1);
}








void command(char* buffer)
{
//-----------------1.把收到的命令检查并翻译一遍-------------------
	char* arg0;
	char* arg1;
	buf2arg(buffer,&arg0,&arg1);
	if(arg0==0)return;




//----------------------2.实际的活都找专人来干-----------------------------
	//help
	int ret=compare( arg0 , "help" );
	if(ret==0)
	{
		//physical(master)
		diary("help ?		(list all known)\n");
		diary("list ?		(list all known)\n");
		diary("into ?		(choose a disk)\n");
		diary("read ?		(hex print a sector)\n");
		diary("write ?		(no)\n\n");

		//logical(servent)
		diary("hello ?		(no)\n");
		diary("ls ?		(list file)\n");
		diary("cd ?		(change directory)\n");
		diary("load ?		(load this file)\n");
		diary("store ?		(store this file)\n");

		return;
	}
	//physical 1
	ret=compare( arg0 , "list" );
	if(ret==0)
	{
		masterlist(arg1);
		return;
	}
	//physical 2
	ret=compare( arg0 , "into" );
	if(ret==0)
	{
		masterinto(arg1);
		return;
	}
	//physical 3
	ret=compare( arg0 , "read" );
	if(ret==0)
	{
		masterread(arg1);
		return;
	}
	//physical 4
	ret=compare( arg0 , "write" );	//dangerous
	if(ret==0)
	{
		masterwrite(arg1);
		return;
	}




	//logical 0 (servent 0) (check)
	ret=compare( arg0 , "hello");
	if(ret==0)
	{
		if(arg1==0)
		{
			hello(0);
		}
		else
		{
			QWORD value;
			hexstring2data(arg1,&value);
			hello(diskhome+value*0x40);
			return;
		}
	}
	//logical 1 (servent 1) (search)
	ret=compare( arg0 , "ls");
	if(ret==0)
	{
		ls(arg1);
		return;
	}
	//logical 2 (servent 2) (enter)
	ret=compare( arg0 , "cd" );
	if(ret==0)
	{
		cd(arg1);
		return;
	}
	//logical 3 (servent 3) (get)
	ret=compare( arg0 , "load" );
	if(ret==0)
	{
		load(arg1);
		return;
	}
	//logical 4 (servent 4) (put)
	ret=compare( arg0 , "store" );	//very dangerous
	if(ret==0)
	{
		store(arg1);
		return;
	}
}








void initmaster(char* world)
{
	//(自己)4块区域，每块1兆
	diskhome=world+0;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	//(仆人)最开始看着谁
	initservent(diskhome);

	//扫描一遍所有认识的东西，选中找到的第一个
	masterinto(0);
}
void killmaster()
{
}
