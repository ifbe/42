#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int startfile();
int stopfile();
int readfile(u8*,u8*,u64,u64);
int writefile(u8*,u8*,u64,u64);
//
void printmemory(char*,int);
void say(char* fmt,...);




//
static u8* fshome;
static u8* dirhome;
static u8* datahome;




int isgpt(char* addr)
{
	//第一个扇区末尾必须有0x55，0xaa这个标志
	u64 temp=*(u16*)(addr+0x1fe);
	if(temp != 0xaa55 ) return 0;

	//第二个扇区开头必须是"EFI PART"
	temp=*(u64*)(addr+0x200);
	if( temp != 0x5452415020494645 ) return 0;

	//检查crc32校验正确还是错误
	//

	//最终确定
	return 0x747067;	//'gpt'
}




//gpt:			[+0x400,+0x4400],每个0x80,总共0x80个
//[+0,+0xf]:类型guid
//[+0x10,+0x1f]:分区guid
//[+0x20,+0x27]:起始lba
//[+0x28,+0x2f]:末尾lba
//[+0x30,+0x37]:属性标签
//[+0x38,+0x7f]:名字
static void gpt_explain(char* src,char* dst)
{
	int i=0,j=0;
	u64* srcqword;
	u64* dstqword = (u64*)dst;
	for(j=0;j<0x10000;j++)
	{
		dst[j] = 0;
	}

	//check
	i = isgpt(src);
	if(i == 0)
	{
		say("not gpt\n");
		return;
	}

	//
	src += 0x400;
	srcqword = (u64*)src;

	//
	for(i=0;i<0x80;i++)	//0x80 partitions per disk
	{
		//先取数字出来
		src += 0x80;
		srcqword = (u64*)src;
		if(srcqword[0] == 0)continue;

		//类型，子类型，开始，结束
		u64 firsthalf = srcqword [0];
		u64 secondhalf = srcqword [1];
		u64 startlba = srcqword [4];
		u64 endlba = srcqword [5];

		//不同分区类型
		if(firsthalf==0x477284830fc63daf)
		{
			if(secondhalf==0xe47d47d8693d798e)
			{
				dstqword[1]=0x747865;		//ext
				//say("ext\n");
			}
		}
		if(firsthalf==0x477284830fc63daf)
		{
			if(secondhalf==0xe47d47d8693d798e)
			{
				dstqword[1]=0x747865;		//ext
				//say("ext\n");
			}
		}
		else if(firsthalf==0x11d2f81fc12a7328)
		{
			if(secondhalf==0x3bc93ec9a0004bba)
			{
				dstqword[1]=0x746166;		//fat
				//say("fat\n");
			}
		}
		else if(firsthalf==0x11aa000048465300)
		{
			if(secondhalf==0xacec4365300011aa)
			{
				dstqword[1]=0x736668;		//hfs
				//say("hfs\n");
			}
		}
		else if(firsthalf==0x4433b9e5ebd0a0a2)
		{
			if(secondhalf==0xc79926b7b668c087)
			{
				dstqword[1]=0x7366746e;		//ntfs
				//say("ntfs\n");
			}
		}
		else
		{
			dstqword[1]=0x3f;
			//say("unknown\n");
		}

		dstqword[0]=0x74726170;	//'disk...'
		dstqword[2]=startlba;
		dstqword[3]=endlba;
		for(j=0;j<0x40;j++)
		{
			dst[0x40 + j] = src[0x38 + j*2];
		}

		//pointer++
		dst = dst + 0x80;
		dstqword = (u64*)dst;
	}
}




static void gpt_list()
{
	int j;
	for(j=0;j<0x80*0x80;j+=0x80)
	{
		if(*(u64*)(fshome+j) == 0)break;

		say("(%04s,%04s)	[%08llx,%08llx]	%s\n",
			fshome+j, fshome+j+8,
			*(u64*)(fshome+j+0x10), *(u64*)(fshome+j+0x18),
			fshome+j+0x40
		);
	}
}
static int gpt_choose(char* p)
{
	int ret;
	stopfile(p);
	if(p == 0)return 0;

	ret = startfile(p);
	if(ret <= 0)return -1;

	readfile(datahome, 0, 0, 0x8000);
	gpt_explain(datahome, fshome);

	gpt_list();
	return 0;
}
static void gpt_read()
{
}
static void gpt_write()
{
}
static void gpt_start(char* p)
{
}
static void gpt_stop(char* p)
{
}
void gpt_create(void* world, u64* p)
{
	fshome = world+0x100000;
	dirhome = world+0x200000;
	datahome = world+0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x747067;

	p[10]=(u64)gpt_start;
	p[11]=(u64)gpt_stop;
	p[12]=(u64)gpt_list;
	p[13]=(u64)gpt_choose;
	p[14]=(u64)gpt_read;
	p[15]=(u64)gpt_write;
}
void gpt_delete()
{
}
