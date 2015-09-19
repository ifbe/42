#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




//in libsoft1/
void mountext(QWORD,QWORD);
void mountfat(QWORD,QWORD);
void mounthfs(QWORD,QWORD);
void mountntfs(QWORD,QWORD);
void explaingpt(QWORD,QWORD);
void explainmbr(QWORD,QWORD);

//in libsoft/
void hexstring2data(char*,QWORD*);
void buf2arg(char*,BYTE**,BYTE**);
int compare(char*,char*);
void printmemory(QWORD addr,int count);
void cleanmemory(QWORD addr,int count);

//in init/
void say(char* str,...);
void diary(char* str,...);

void choose(QWORD in);
QWORD readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
QWORD writememory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);

void mem2file(QWORD memaddr,char* filename,QWORD offset,QWORD count);
void file2mem(QWORD memaddr,char* filename,QWORD offset,QWORD count);




//每个1M
static QWORD diskhome;		//+0m
static QWORD fshome;		//+1m
static QWORD dirhome;		//+2m
static QWORD datahome;		//+3m
static QWORD diskinfo;		//+7m

//3大函数的位置
int (*explain)(QWORD id);		//((int (*)(QWORD))(explain))(value);
int (*cd)(QWORD id);		//((int (*)(QWORD))(cd))(arg1);
int (*load)(QWORD id,QWORD part);		//((int (*)(QWORD,QWORD))(load))(arg1,temp*0x100000);




//from:
//只知道是一堆数据
//to:					//[+0,+ffff],每个0x40,总共0x400个
//[0,7]  		startlba
//[8,f]  		endlba
//[10,17]		type
//[18,1f]		name
//[20,27]		cdfunc
//[28,2f]		loadfunc
//[30,37]		explainfunc
//[38,3f]		unused
void hello()		//你究竟是个什么？
{
	//读最开始的64个扇区（0x8000字节）来初步确定
	int ret=readmemory(datahome,0,0,64);
	if(ret<=0)
	{
		//读不出来，可能是内存？
		diary("it's memory?\n");
	}
	else if( *(WORD*)(datahome+0x1fe) == 0xaa55 )
	{
		//末尾有0x55，0xaa这个标志，这个是磁盘，或者要当成磁盘用

		//看看是什么类型，交给小弟处理
		QWORD temp=*(QWORD*)(datahome+0x200);
		if( temp == 0x5452415020494645 )
		{
			//cleanmemory(diskhome,0x400000);
			explaingpt(datahome,diskhome);
		}
		else
		{
			//cleanmemory(diskhome,0x400000);
			explainmbr(datahome,diskhome);
		}
	}
	else
	{
		//可能是zip,网络包,或者其他乱七八糟的结构
		diary("don't know\n");
	}
}




int searchthis(char* name,QWORD* addr)
{
	QWORD temp=dirhome;
	for(;temp<dirhome+0x1000;temp+=0x40)
	{
		//diary("%llx,%llx\n",*(QWORD*)name,*(QWORD*)temp);
		if( compare( name , (char*)temp ) == 0 )
		{
			*addr=temp;
			printmemory(temp,0x40);
			return 0;
		}
	}

	diary("file not found\n");
	return -1;
}
int mount(QWORD which)
{
	//清理buffer1
	cleanmemory(fshome,0x300000);

	//搞谁，是啥
	QWORD* this=(QWORD*)(diskhome+0x40*which);
	QWORD type=this[2];

	//不同种类的分区，不同的处理
	if(type == 0x747865)		//ext
	{
		mountext(diskhome,which);
	}
	else if(type == 0x746166)		//fat
	{
		mountfat(diskhome,which);
	}
	else if(type == 0x736668)		//hfs
	{
		mounthfs(diskhome,which);
	}
	else if(type == 0x7366746e)		//ntfs
	{
		mountntfs(diskhome,which);
	}

	//给函数指针赋值
	if(this[4] < 0xffff)return -1;
	explain=(void*)( this[4] );
	cd=(void*)( this[5] );
	load=(void*)( this[6] );
}








void command(char* buffer)
{
//-----------------1.把收到的命令检查并翻译一遍-------------------
	BYTE* arg0;
	BYTE* arg1;
	buf2arg(buffer,&arg0,&arg1);
	if(arg0==0)return;




//----------------------2.实际的活都找专人来干-----------------------------
	if(compare( arg0 , "help" ) == 0)
	{
		diary("pick ?                  (choose a disk)\n");
		diary("mount ?                 (choose a partition)\n");
		diary("explain ?               (explain inode/cluster/cnid/mft)\n");
		diary("cd dirname              (change directory)\n");
		diary("load filename           (load this file)\n");
	}
	else if(compare( arg0 , "choose" ) == 0)
	{
		cleanmemory(diskhome,0x400000);
		if( (QWORD)arg1 == 0 )
		{
			//只是打印一遍扫描到的磁盘信息
			char* p=(char*)diskinfo;
			int i=0;

			while(1)
			{
				//先检查
				if( *(DWORD*)(diskinfo+i) == 0 )break;
				if(i>100*0x100)break;

				//再打印
				diary("%s\n",diskinfo+i);
				i+=0x100;
			}
		}
		else
		{
			//检查这是个什么玩意
			choose((QWORD)arg1);
			hello();
		}
	}
	else if(compare( arg0 , "read" ) == 0)
	{
		QWORD value;
		hexstring2data(arg1,&value);
		
		readmemory(datahome,value,0,1);
		printmemory(datahome,0x200);
		diary("above is:%x,%x\n",value,value+7);
	}
	else if(compare( arg0 , "mount" ) == 0)
	{
		if(arg1==0)
		{
			hello();
			return;
		}
		else
		{
			//字符串转值
			QWORD value;
			hexstring2data(arg1,&value);

			//挂载选定分区
			mount(value);		//第几个功能，字符串地址
		}
	}
	else if(compare( arg0 , "explain" ) == 0)
	{
		QWORD value;
		hexstring2data(arg1,&value);

		diary("explainer@%llx\n",explain);
		explain(value);
	}
	else if(compare( arg0 , "cd" ) == 0)
	{
		QWORD addr;
		if( searchthis(arg1,&addr) < 0 )return;		//没找到

		//change directory
		cd( *(QWORD*)(addr+0x10) );
	}
	else if(compare( arg0 , "ls" ) == 0)
	{
		diary("name                special id          type                size\n");
		QWORD addr=dirhome;
		for(;addr<dirhome+0x1000;addr+=0x40)
		{
			if(*(QWORD*)addr==0)break;
			diary("%-16.16s    %-16llx    %-16llx    %-16llx\n",
			(char*)addr,*(QWORD*)(addr+0x10),*(QWORD*)(addr+0x20),*(QWORD*)(addr+0x30));
		}
		diary("\n");
	}
	else if(compare( arg0 , "load" ) == 0)
	{
		//寻找这个文件名，得到id，type，size
		QWORD addr;
		if( searchthis(arg1,&addr) < 0 )return;
		QWORD id=*(QWORD*)(addr+0x10);
		QWORD type=*(QWORD*)(addr+0x20);
		QWORD size=*(QWORD*)(addr+0x30);
		if(size>0x100000)diary("warning:large file\n");


		QWORD temp=0;
		//1m,1m,1m的整块搞
		for(;temp<( size&0xfffffff00000 );temp+=0x100000)
		{
			load(id,temp);
			mem2file(datahome,arg1,temp,0x100000);		//mem地址，file名字，文件内偏移，写入多少字节
		}
		//最后的零头(要是size=1m的整数倍，就没有零头)
		if(temp<size)
		{
			load(id,temp);
			mem2file(datahome,arg1,temp,size%0x100000);		//mem地址，file名字，文件内偏移，写入多少字节
		}
	}
	else
	{
		diary("%s\n",arg0);
	}
}




void initmaster(QWORD world)
{
	diskhome=world+0;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	diskinfo=world+0x700000;
	hello();
}