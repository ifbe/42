#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
void printmemory(void*, int);
void say(void*, ...);




int check_mbr(u8* addr)
{
	//第一个扇区末尾必须有0x55，0xaa这个标志
	u64 temp=*(u16*)(addr+0x1fe);
	if(temp != 0xaa55 ) return 0;

	//但是mbr没有特别的标志，只能勉强用55aa确定
	//所以只能放在check type的最后
	return 0x72626d;
}




//
static int mbrrecord(u8* from, u8* dst)
{
	u8 H,L;
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
void parse_mbr(u8* src, u8* dst)
{
	int j,ret;
	ret = check_mbr(src);
	if(ret > 0)say("mbr\n");
	else{say("not mbr\n");return;}

	//clear
	u64* dstqword=(u64*)dst;
	for(j=0; j<0x10000; j++)
	{
		dst[j] = 0;
	}

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