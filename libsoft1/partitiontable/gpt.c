#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//gpt:					[+0x400,+0x4400],每个0x80,总共0x80个
//[+0,+0xf]:类型guid
//[+0x10,+0x1f]:分区guid
//[+0x20,+0x27]:起始lba
//[+0x28,+0x2f]:末尾lba
//[+0x30,+0x37]:属性标签
//[+0x38,+0x7f]:名字

//用了别人的
void diary(char* fmt,...);




QWORD explaingpt(QWORD from,QWORD to)
{
	diary("gpt disk\n");
	from+=0x400;

	int src=0,dst=0;
	for(src=0;src<0x80;src++)	//0x80 partitions per disk
	{
		//先取数字出来
		QWORD firsthalf=*(QWORD*)(from+0x80*src);
		if(firsthalf==0)continue;

		QWORD secondhalf=*(QWORD*)(from+0x80*src+8);
		QWORD startlba=*(QWORD*)(from+0x80*src+0x20);
		QWORD endlba=*(QWORD*)(from+0x80*src+0x28);
		diary("%2d    ",dst);
		diary("[%8x,%8x]    ",startlba,endlba);




		//开始与结束位置
		*(QWORD*)(to+0x40*dst)=startlba;
		*(QWORD*)(to+0x40*dst+8)=endlba;

		//不同分区类型
		if((firsthalf==0x477284830fc63daf)&&(secondhalf==0xe47d47d8693d798e))
		{
			*(QWORD*)(to+0x40*dst+0x10)=0x747865;		//ext
			diary("ext\n");
		}
		else if((firsthalf==0x11d2f81fc12a7328)&&(secondhalf==0x3bc93ec9a0004bba))
		{
			*(QWORD*)(to+0x40*dst+0x10)=0x746166;		//fat
			diary("fat\n");
		}
		else if((firsthalf==0x11aa000048465300)&&(secondhalf==0xacec4365300011aa))
		{
			*(QWORD*)(to+0x40*dst+0x10)=0x736668;		//hfs
			diary("hfs\n");
		}
		else if((firsthalf==0x4433b9e5ebd0a0a2)&&(secondhalf==0xc79926b7b668c087))
		{
			*(QWORD*)(to+0x40*dst+0x10)=0x7366746e;		//ntfs
			diary("ntfs\n");
		}
		else diary("unknown\n");

		//分区名

		//pointer++
		dst++;
	}
}
