#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


//explainmbr和explainebr公用的变量
static int src;
static int dst;




//用了别人的
void say(char* fmt,...);
void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void printmemory(QWORD start,QWORD count);




//ebr:
//
void explainebr(QWORD buffer,QWORD to)
{
	say("extend,src=%x,dst=%x\n",src,dst);

	QWORD startsector=*(DWORD*)(to+0x40*src);
	readmemory(buffer,startsector,0,1);
	printmemory(buffer,0x200);
}
//mbr:					[+0x1be,+0x1fd],每个0x10,总共4个
//[+0]:活动标记
//[+0x1,+0x3]:开始磁头柱面扇区
//[+0x4]:分区类型
//[+0x5,+0x7]:结束磁头柱面扇区
//[+0x8,+0xb]:起始lba
//[+0xc,+0xf]:大小
QWORD explainmbr(QWORD buffer,QWORD to)
{
	say("mbr disk\n",0);
	dst=0;

	//首先是主分区
	QWORD temp;
	for(src=0;src<4;src++)
	{
		temp=*(BYTE*)(buffer+(src*0x10)+0x1c2);
		if(temp==0)continue;

		//startsector,endsector
		QWORD start=*(DWORD*)(buffer+(src*0x10)+0x1c6);
		QWORD end=*(DWORD*)(buffer+(src*0x10)+0x1ca);
		*(QWORD*)(to+0x40*dst)=start;
		*(QWORD*)(to+0x40*dst+8)=end;

		//parttype
		say("%2d    ",dst);
		say("[%8x,%8x]    ",start,end);
		if( temp==0x4 | temp==0x6 | temp==0xb )
		{
			say("fat\n");
			*(QWORD*)(to+0x40*dst+0x10)=0x746166;
		}
		else if( temp==0x7 )
		{
			say("ntfs\n");
			*(QWORD*)(to+0x40*dst+0x10)=0x7366746e;
		}
		else if( temp==0x83 )
		{
			say("ext\n");
			*(QWORD*)(to+0x40*dst+0x10)=0x747865;
		}
		else
		{
			say("????%4x\n",temp);
			*(QWORD*)(to+0x40*dst+0x10)=(temp<<32)|0x3f3f3f3f;
		}

		//partname

		//dst
		dst++;
	}

	//在这四个里面，寻找逻辑分区并解释
	for(src=0;src<4;src++)
	{
		temp=*(DWORD*)(to+0x40*src+0x10+4);
		if( (temp==0x5) | (temp==0xf) )
		{
			*(QWORD*)(to+0x40*src+0x10)=0x646e65747865;	//既然懂了，现在就改
			explainebr(buffer,to);
		}
	}
}
