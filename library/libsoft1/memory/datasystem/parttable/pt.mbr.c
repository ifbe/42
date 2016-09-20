#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void systemread(char* rdi,u64 rsi,u64 rcx);
void systemwrite(char* rdi,u64 rsi,u64 rcx);
void printmemory(u64 start,u64 count);
void say(char* fmt,...);




int ismbr(char* addr)
{
        //第一个扇区末尾必须有0x55，0xaa这个标志
        u64 temp=*(u16*)(addr+0x1fe);
        if(temp != 0xaa55 ) return 0;

	//但是mbr没有特别的标志，只能勉强用55aa确定
	//所以只能放在check type的最后
	return 0x72626d;
}




//from：那一条长度为16B的项目的地址，to：希望的位置
static int mbrrecord(char* from,char* to)
{
	u32 flag;		//(mbr+0x1be)+0
	u32 type;		//(mbr+0x1be)+4
	u32 start;		//(mbr+0x1be)+8
	u32 size;		//(mbr+0x1be)+c
	u64* dst;
	type=*(u8*)(from+4);
	if(type==0)return 0;

	//类型，子类型，开始，结束
	dst=(u64*)to;
	start=*(u32*)(from+8);
	size=*(u32*)(from+0xc);
        dst[0]=0x74726170; 	//'part'
	dst[2]=start;		//start
	dst[3]=start + size - 1;	//end

	//拓展分区要递归
	if( (type==0x5) | (type==0xf) )
	{
		say("extend@start\n");
		dst[1]=0x646e65747865;
		return 66666;
	}

	//其他普通分区
	if( (type==0x4) | (type==0x6) | (type==0xb) )
	{
		//say("fat\n");
		dst[1]=0x746166;
	}
	else if( type==0x7 )
	{
		//say("ntfs\n");
		dst[1]=0x7366746e;
	}
	else if( type==0x83 )
	{
		//say("ext\n");
		dst[1]=0x747865;
	}
	else
	{
		//say("unknown:%x\n",type);
		dst[1]=type;
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
void explainmbr(char* buffer,char* to)
{
	char* dst;
	u64* dstqword;

	int i,j,ret;
	u64 temp;
	say("mbr disk\n",0);

	//除了硬盘记录以外，全部干掉
	dst=to;
	dstqword=(u64*)to;
	for(i=0;i<0x100;i++)  //0x100*0x40=0x4000=16k
	{
		temp=dstqword[i*8];
		if(temp == 0x6b736964)continue;
		else break;
	}
	for(j=0x40*i; j<0x10000; j++)
	{
		dst[j] = 0;
	}
	dst+=0x40*i;
	dstqword=(u64*)dst;

	//首先是主分区，最多4个
	ret=mbrrecord(buffer+0x1be,dst);
	if(ret>0)
	{
		dst+=0x40;
		dstqword+=8;
	}

	ret=mbrrecord(buffer+0x1ce,dst);
	if(ret>0)
	{
		dst+=0x40;
		dstqword+=8;
	}

	ret=mbrrecord(buffer+0x1de,dst);
	if(ret>0)
	{
		dst+=0x40;
		dstqword+=8;
	}

	ret=mbrrecord(buffer+0x1ee,dst);
	if(ret>0)
	{
		dst+=0x40;
		dstqword+=8;
	}
}
/*
	//在这四个里面，寻找逻辑分区并解释
	u64 offset=0;
	while(1)
	{
		u64 type=*(u64*)(to+offset+0x10);
		if(type==0)break;		//到这就没东西了

		u64 start=*(u64*)(to+offset);
		if( type==5 | type==0xf )
		{
			//say("sector:%x\n",*(u32*)(to+offset));
			systemread(
				buffer,
				*(u32*)(to+offset),
				1
			);
			//printmemory(buffer+0x1be,0x40);

			char* remember=(char*)dstqword;
			mbrrecord(buffer+0x1be,dstqword);
			if(remember!=(char*)dstqword)
			{
				*(u32*)(remember+0)+=start;
				//say("1st:%x\n",*(u32*)remember);
			}

			remember=dstqword;
			mbrrecord(buffer+0x1ce,dstqword);
			if(remember!=dstqword)
			{
				*(u32*)(remember+0)+=start;
				//say("2st:%x\n",*(u32*)remember);
			}
		}

		//轮到下一个了
		offset+=0x40;
	}//while
*/
/*
	//最后打印所有的
	offset=0;
	while(1)
	{
		u64 addr=to+offset*0x40+0x10;
		u64 type=*(u64*)addr;
		if(type==0)break;

		say("%d:%s\n",offset,(char*)addr);

		offset++;
	}
*/
