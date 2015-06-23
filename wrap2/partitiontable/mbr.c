#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//mbr:					[+0x1be,+0x1fd],每个0x10,总共4个
//[+0]:活动标记
//[+0x1,+0x3]:开始磁头柱面扇区
//[+0x4]:分区类型
//[+0x5,+0x7]:结束磁头柱面扇区
//[+0x8,+0xb]:起始lba
//[+0xc,+0xf]:大小


//用了别人的
void say(char* fmt,...);



QWORD explainmbr(QWORD from,QWORD to)
{
	say("mbr disk\n",0);
	from=from+0x1be;

	int src=0,dst=0;
	for(src=0;src<4;src++)
	{
		BYTE temp=*(BYTE*)(from+0x10*src+4);
		if(temp==0)continue;

		//startsector,endsector
		QWORD start=*(DWORD*)(from+0x10*src+8);
		QWORD end=*(DWORD*)(from+0x10*src+0xc);
		*(QWORD*)(to+0x40*dst)=start;
		*(QWORD*)(to+0x40*dst+8)=end;

		//parttype
		say("%2d    ",dst);
		say("[%8x,%8x]    ",start,end);
		if( temp==0x4 | temp==0x6 | temp==0xb )
		{
			*(QWORD*)(to+0x40*dst+0x10)=0x746166;
			say("fat\n");
		}
		else if( temp==0x7 )
		{
			*(QWORD*)(to+0x40*dst+0x10)=0x7366746e;
			say("ntfs\n");
		}
		else if( temp==0x83 )
		{
			*(QWORD*)(to+0x40*dst+0x10)=0x747865;
			say("ext\n");
		}
		else say("unknown\n");

		//partname

		//dst
		dst++;
	}
}
