#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void say(void*, ...);




int check_gpt(u8* addr)
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
void parse_gpt(u8* src, u8* dst)
{
	int i=0,j=0;
	u64* srcqword;
	u64* dstqword;

	i = check_gpt(src);
	if(i > 0)say("gpt\n");
	else{say("not gpt\n");return;}

	src += 0x400;
	for(j=0;j<0x10000;j++)dst[j] = 0;
	for(i=0;i<0x80;i++)	//0x80 partitions per disk
	{
		//先取数字出来
		srcqword = (u64*)src;
		dstqword = (u64*)dst;
		if(srcqword[0] == 0)
		{
			src += 0x80;
			continue;
		}

		//类型，子类型，开始，结束
		u64 firsthalf = srcqword[0];
		u64 secondhalf = srcqword[1];
		u64 startlba = srcqword[4];
		u64 endlba = srcqword[5];

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
		src += 0x80;
		dst += 0x80;
	}
}
