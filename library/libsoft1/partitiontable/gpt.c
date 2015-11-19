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




QWORD explaingpt(char* from,char* to)
{
	diary("gpt disk\n");
	from+=0x400;

	int i=0;
	QWORD* src=(QWORD*)from;
	QWORD* dst=(QWORD*)to;
	for(i=0;i<0x80;i++)	//0x80 partitions per disk
	{
		//先取数字出来
		src=(QWORD*)( from + 0x80*i );
		if(src[0]==0)continue;

		//类型，大小，开始，结束
		QWORD firsthalf=src[0];
		QWORD secondhalf=src[1];
		QWORD startlba=src[4];
		QWORD endlba=src[5];

		dst[0]=0x74726170;
		dst[1]=0x200*(endlba-startlba);
		dst[2]=startlba;
		dst[3]=endlba;

		//不同分区类型
		if(firsthalf==0x477284830fc63daf)
		{
			if(secondhalf==0xe47d47d8693d798e)
			{
				dst[4]=0x747865;		//ext
				diary("ext\n");
			}
		}
		else if(firsthalf==0x11d2f81fc12a7328)
		{
			if(secondhalf==0x3bc93ec9a0004bba)
			{
			dst[4]=0x746166;		//fat
			diary("fat\n");
			}
		}
		else if(firsthalf==0x11aa000048465300)
		{
			if(secondhalf==0xacec4365300011aa)
			{
				dst[4]=0x736668;		//hfs
				diary("hfs\n");
			}
		}
		else if(firsthalf==0x4433b9e5ebd0a0a2)
		{
			if(secondhalf==0xc79926b7b668c087)
			{
				dst[4]=0x7366746e;		//ntfs
				diary("ntfs\n");
			}
		}
		else
		{
			dst[4]=0x66666666;
			diary("unknown\n");
		}

		//pointer++
		dst = dst+8;
	}
}
