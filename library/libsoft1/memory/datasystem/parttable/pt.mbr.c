#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void startfile(char*);
void stopfile(char*);
int readfile(u8*,u8*,u64,u64);
int writefile(u8*,u8*,u64,u64);
//
void printmemory(u64 start,u64 count);
void say(char* fmt,...);




//
static u8* fshome;
static u8* dirhome;
static u8* datahome;




int ismbr(char* addr)
{
	//第一个扇区末尾必须有0x55，0xaa这个标志
	u64 temp=*(u16*)(addr+0x1fe);
	if(temp != 0xaa55 ) return 0;

	//但是mbr没有特别的标志，只能勉强用55aa确定
	//所以只能放在check type的最后
	return 0x72626d;
}




//
static int mbrrecord(char* from,char* dst)
{
	char H,L;
	u32 flag;		//(mbr+0x1be)+0
	u32 type;		//(mbr+0x1be)+4
	u32 start;		//(mbr+0x1be)+8
	u32 size;		//(mbr+0x1be)+c
	u64* dstqword;

	//
	type=*(u8*)(from+4);
	if(type==0)return 0;
	start=*(u32*)(from+8);
	size=*(u32*)(from+0xc);

	//类型，子类型，开始，结束
	dstqword = (u64*)dst;
	dstqword[0] = 0x74726170; 	//'part'
	dstqword[2] = start;		//start
	dstqword[3] = start + size - 1;	//end

	//拓展分区要递归
	if( (type==0x5) | (type==0xf) )
	{
		say("extend@start\n");
		dstqword[1]=0x646e65747865;
		return 66666;
	}

	//其他普通分区
	if( (type==0x4) | (type==0x6) | (type==0xb) )
	{
		//say("fat\n");
		dstqword[1]=0x746166;
	}
	else if( type==0x7 )
	{
		//say("ntfs\n");
		dstqword[1]=0x7366746e;
	}
	else if( type==0x83 )
	{
		//say("ext\n");
		dstqword[1]=0x747865;
	}
	else
	{
		//say("unknown:%x\n",type);
		H = ( (type>>4)&0xf ) + 0x30;
		if(H>0x39)H += 7;
		L = (type&0xf) + 0x30;
		if(L>0x39)L += 7;

		dstqword[1] = H + (L<<8);
	}
	return 0x10;	//这次翻译了多少
}




//mbr:			[+0x1be,+0x1fd],每个0x10,总共4个
//[+0]:活动标记
//[+0x1,+0x3]:开始磁头柱面扇区
//[+0x4]:分区类型
//[+0x5,+0x7]:结束磁头柱面扇区
//[+0x8,+0xb]:起始lba
//[+0xc,+0xf]:大小
static void mbr_explain(char* src,char* dst)
{
	int j,ret;
	u64* dstqword;

	//
	say("mbr disk\n",0);
	for(j=0; j<0x10000; j++)
	{
		dst[j] = 0;
	}
	dstqword=(u64*)dst;

	//主分区
	ret=mbrrecord(src+0x1be,dst);
	if(ret>0)
	{
		dst += 0x80;
		dstqword = (u64*)dst;
	}

	ret=mbrrecord(src+0x1ce,dst);
	if(ret>0)
	{
		dst += 0x80;
		dstqword = (u64*)dst;
	}

	ret=mbrrecord(src+0x1de,dst);
	if(ret>0)
	{
		dst+=0x80;
		dstqword = (u64*)dst;
	}

	ret=mbrrecord(src+0x1ee,dst);
	if(ret>0)
	{
		dst+=0x80;
		dstqword = (u64*)dst;
	}

	//逻辑分区
}




static void mbr_list()
{
	int j;
	for(j=0;j<0x80*0x80;j+=0x80)
	{
		if(*(u64*)(fshome+j) == 0)break;

		say("(%04s,%04s)	[%08llx,%08llx] %s\n",
			fshome+j, fshome+j+8,
			*(u64*)(fshome+j+0x10), *(u64*)(fshome+j+0x18),
			fshome+j+0x40
		);
	}
}
static void mbr_choose()
{
}
static void mbr_read()
{
	readfile(datahome, 0, 0, 0x8000);
	mbr_explain(datahome, fshome);

	mbr_list();
}
static void mbr_write()
{
}
static void mbr_start(char* p)
{
	startfile(p);
}
static void mbr_stop(char* p)
{
	stopfile(p);
}
void mbr_create(void* world, u64* p)
{
	fshome = world+0x100000;
	dirhome = world+0x200000;
	datahome = world+0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x72626d;

	p[10]=(u64)mbr_start;
	p[11]=(u64)mbr_stop;
	p[12]=(u64)mbr_list;
	p[13]=(u64)mbr_choose;
	p[14]=(u64)mbr_read;
	p[15]=(u64)mbr_write;
}
void mbr_delete()
{
}

