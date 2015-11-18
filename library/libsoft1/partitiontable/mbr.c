#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void diary(char* fmt,...);
void readmemory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void printmemory(QWORD start,QWORD count);




//from：那一条长度为16B的项目的地址，to：希望的位置
static void mbrrecord(char* from,QWORD* to)
{
	QWORD type=*(BYTE*)(from+4);
	if(type==0)return;

	//startsector,endsector
	QWORD start=*(DWORD*)(from+8);
	QWORD end=*(DWORD*)(from+0xc);
	QWORD* addr=(QWORD*)(*to);
	addr[0]=start;
	addr[1]=end;

	//parttype
	if( type==0x4 | type==0x6 | type==0xb )
	{
		//diary("fat\n");
		addr[2]=0x746166;
	}
	else if( type==0x7 )
	{
		//diary("ntfs\n");
		addr[2]=0x7366746e;
	}
	else if( type==0x83 )
	{
		//diary("ext\n");
		addr[2]=0x747865;
	}
	else
	{
		//diary("unknown:%x\n",type);
		addr[2]=type;
	}

	//partname

	//下一地址
	*to+=0x40;
}
//mbr:					[+0x1be,+0x1fd],每个0x10,总共4个
//[+0]:活动标记
//[+0x1,+0x3]:开始磁头柱面扇区
//[+0x4]:分区类型
//[+0x5,+0x7]:结束磁头柱面扇区
//[+0x8,+0xb]:起始lba
//[+0xc,+0xf]:大小
QWORD explainmbr(char* buffer,QWORD to)
{
	diary("mbr disk{\n",0);

	//首先是主分区，最多4个
	QWORD toaddr=to;
	mbrrecord(buffer+0x1be,&toaddr);
	mbrrecord(buffer+0x1ce,&toaddr);
	mbrrecord(buffer+0x1de,&toaddr);
	mbrrecord(buffer+0x1ee,&toaddr);

	//在这四个里面，寻找逻辑分区并解释
	QWORD offset=0;
	while(1)
	{
		QWORD start=*(QWORD*)(to+offset);
		QWORD type=*(QWORD*)(to+offset+0x10);
		if(type==0)break;		//到这就没东西了

		if( type==5 | type==0xf )
		{
			//diary("sector:%x\n",*(DWORD*)(to+offset));
			readmemory(buffer,*(DWORD*)(to+offset),0,1);
			//printmemory(buffer+0x1be,0x40);

			QWORD remember=toaddr;
			mbrrecord(buffer+0x1be,&toaddr);
			if(remember!=toaddr)
			{
				*(DWORD*)(remember+0)+=start;
				//diary("1st:%x\n",*(DWORD*)remember);
			}

			remember=toaddr;
			mbrrecord(buffer+0x1ce,&toaddr);
			if(remember!=toaddr)
			{
				*(DWORD*)(remember+0)+=start;
				//diary("2st:%x\n",*(DWORD*)remember);
			}
		}

		//轮到下一个了
		offset+=0x40;
	}

	//最后打印所有的
	offset=0;
	while(1)
	{
		QWORD addr=to+offset*0x40+0x10;
		QWORD type=*(QWORD*)addr;
		if(type==0)break;

		diary("%d:%s\n",offset,(char*)addr);

		offset++;
	}
	diary("}\n");
}
