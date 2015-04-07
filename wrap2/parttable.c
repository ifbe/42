﻿#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


//每0x40个字节是一个当前目录表
struct dirbuffer
{
	BYTE name[16];		//[+0,0x7]:起始lba
	QWORD specialid;	//[+0x20,0x2f]:分区类型anscii
	QWORD unused1;
	QWORD type;			//[+0x10,0x1f]:末尾lba
	QWORD unused2;
	QWORD size;			//[0x30,0x3f]:分区名字
	QWORD unused3;
};
static struct dirbuffer* dir;	//dir=“目录名缓冲区”的内存地址（dir[0],dir[1],dir[2]是这个内存地址里面的第0，1，2字节快）

//每0x40个字节是一个磁盘分区记录，里面包含起止扇区分区类型分区名字等信息
struct mystruct
{
	unsigned long long startlba;	//[+0,0x7]:起始lba
	unsigned long long endlba;		//[+0x8,0xf]:末尾lba
	unsigned long long parttype;	//[+0x10,0x17]:分区类型anscii
	unsigned long long partname;	//[0x18,0x1f]:分区名字
	unsigned long long a[4];		//[0x20,0x3f]:unused
};
static struct mystruct mytable[128];		//0x40*0x80=0x2000

static QWORD readbuffer;		//读缓冲区的地址，缓冲区64k大小
static QWORD explainfunc;
static QWORD cdfunc;
static QWORD loadfunc;//调用的cd和load函数所在的内存地址





//从磁盘读出来的数据在[+0x400,+0x4400]=0x80个*每个0x80
//[+0,+0xf]:类型guid			raw[0],raw[1]
//[+0x10,+0x1f]:分区guid		raw[2],raw[3]
//[+0x20,+0x27]:起始lba			raw[4]
//[+0x28,+0x2f]:末尾lba			raw[5]
//[+0x30,+0x37]:属性标签		raw[6]
//[+0x38,+0x7f]:名字			raw[7]~raw[0xf]
QWORD explaingpt()
{
	say("gpt disk\n");
	QWORD* raw=(QWORD*)(readbuffer+0x400);

	int src=0,dst=0;
	for(src=0;src<0x80;src++)	//0x80 partitions per disk
	{
		QWORD firsthalf=raw[0x10*src];
		QWORD secondhalf=raw[0x10*src+1];
		if(firsthalf==0)continue;

		say("%2d    ",dst);
		//startsector
		mytable[dst].startlba=raw[0x10*src+4];

		//endsector
		mytable[dst].endlba=raw[0x10*src+5];

		//parttype
		say("[%8x,%8x]    ",mytable[dst].startlba,mytable[dst].endlba);
		if((firsthalf==0x477284830fc63daf)&&(secondhalf==0xe47d47d8693d798e))
		{
			mytable[dst].parttype=0x747865;		//ext
			say("ext\n");
		}
		else if((firsthalf==0x11d2f81fc12a7328)&&(secondhalf==0x3bc93ec9a0004bba))
		{
			mytable[dst].parttype=0x746166;		//fat
			say("fat\n");
		}
		else if((firsthalf==0x11aa000048465300)&&(secondhalf==0xacec4365300011aa))
		{
			mytable[dst].parttype=0x736668;		//hfs
			say("hfs\n");
		}
		else if((firsthalf==0x4433b9e5ebd0a0a2)&&(secondhalf==0xc79926b7b668c087))
		{
			mytable[dst].parttype=0x7366746e;		//ntfs
			say("ntfs\n");
		}
		else say("unknown\n");

		//partname

		//pointer++
		dst++;
	}
}
//从磁盘读出来的数据在[+0x1be,+0x1fd]每个0x10总共4个
//[+0]:活动标记
//[+0x1,+0x3]:开始磁头柱面扇区
//[+0x4]:分区类型
//[+0x5,+0x7]:结束磁头柱面扇区
//[+0x8,+0xb]:起始lba
//[+0xc,+0xf]:大小
QWORD explainmbr()
{
	say("mbr disk\n",0);
	QWORD raw=readbuffer+0x1be;

	int src=0,dst=0;
	for(src=0;src<4;src++)
	{
		BYTE temp=*(BYTE*)(raw+0x10*src+4);
		if(temp==0)continue;

		say("%2d    ",dst);

		//startsector
		mytable[dst].startlba=*(DWORD*)(raw+0x10*src+8);

		//endsector
		mytable[dst].endlba=*(DWORD*)(raw+0x10*src+0xc);

		//parttype
		say("[%8x,%8x]    ",mytable[dst].startlba);
		if( temp==0x4 | temp==0x6 | temp==0xb )
		{
			mytable[dst].parttype=0x746166;
			say("fat\n");
		}
		else if( temp==0x7 )
		{
			mytable[dst].parttype=0x7366746e;
			say("ntfs\n",mytable[dst].startlba);
		}
		else if( temp==0x83 )
		{
			mytable[dst].parttype=0x747865;
			say("ext\n",mytable[dst].startlba);
		}
		else say("unknown\n");
		
		//partname

		//dst
		dst++;
	}
}
void explainparttable()
{
	//清理
	BYTE* memory;
	int i;
	memory=(BYTE*)(mytable);
	for(i=0;i<0x2000;i++) memory[i]=0;

	//读出前64个扇区
	if(*(WORD*)(readbuffer+0x1fe)!=0xAA55)
	{
		say("bad disk\n");
		return;
	}

	//解释分区表
	if(*(QWORD*)(readbuffer+0x200)==0x5452415020494645)
	{
		explaingpt();
	}
	else
	{
		explainmbr();
	}
}
//if(in=0)：只读取分区表
//else if(收到的地址里面是0xffffffffffffffff)：只读取分区表
//else if(收到的地址里面是数字)：挂载这一分区
int mount(QWORD choose,QWORD in)
{
	//地址在哪儿，忘记了或者没听就再问一遍
	if(readbuffer==0)
	{
		whereisbuffer(&readbuffer);
		whereisdir(&dir);
	}

	//大于0x80，只打印一遍认出来的分区
	if(choose == 0)
	{
		readdisk(readbuffer,0,0,64);
		explainparttable();
		return -1;
	}

	//得到编号，然后得到分区位置，然后挂载
	QWORD type=mytable[in].parttype;
	QWORD start=mytable[in].startlba;
	if(type == 0x747865)
	{
		mountext(start,&explainfunc,&cdfunc,&loadfunc);
	}
	else if(type == 0x746166)
	{
		mountfat(start,&explainfunc,&cdfunc,&loadfunc);
	}
	else if(type == 0x736668)
	{
		mounthfs(start,&explainfunc,&cdfunc,&loadfunc);
	}
	else if(type == 0x7366746e)
	{
		mountntfs(start,&explainfunc,&cdfunc,&loadfunc);
	}
	return 1;
}
void explain(QWORD addr)
{
	//接收到的anscii转数字
	QWORD number;
	anscii2hex(addr,&number);

	//解释(几号文件)
	((int (*)())(explainfunc))(number);
}
void cd(QWORD addr)
{
	say("i am in\n");
	((int (*)())(cdfunc))(addr);
}
int load(QWORD addr)
{
	//寻找这个文件名，主要为了得到size
	int i;
	for(i=0;i<0x40;i++)
	{
		if(compare(addr,(char*)(&dir[i]))==0)break;
	}
	if(i==0x40)
	{
		say("file not found\n");
		return -1;
	}
	say("%-16.16s    %-16llx    %-16llx    %-16llx\n",
		(char*)(&dir[i]),dir[i].specialid,dir[i].type,dir[i].size);

	//现在分段读取保存
	QWORD totalsize=dir[i].size;
	QWORD temp;
	if(totalsize>0x100000)say("warning:large file\n");
	for(temp=0;temp<totalsize/0x100000;temp++)
	{
		((int (*)())(loadfunc))(addr,temp*0x100000);			//
		mem2file(readbuffer,addr,temp*0x100000,0x100000);		//mem地址，file名字，文件内偏移，写入多少字节
	}
	((int (*)())(loadfunc))(addr,temp*0x100000);			//
	mem2file(readbuffer,addr,temp*0x100000,totalsize%0x100000);		//mem地址，file名字，文件内偏移，写入多少字节
}








whereisparttable(unsigned long long* p)
{
	*p=(unsigned long long)mytable;
}