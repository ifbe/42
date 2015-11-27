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




void explaingpt(char* from,char* to)
{
	int i=0,j=0;
	QWORD temp;

	char* src;
	char* dst;
	QWORD* srcqword;
	QWORD* dstqword;
	diary("gpt disk\n");

	//保留硬盘记录，覆盖新的分区记录
	dst=to;
	dstqword=(QWORD*)to;
	for(i=0;i<0x100;i++)	//0x100*0x40=0x4000=16k
	{
		temp=dstqword[i*8];
		if( temp == 0)break;
		if( temp == 0x74726170 )break;
	}
	for(j=0x40*i; j<0x10000; j++)
	{
		dst[j] = 0;
	}
	dst+=0x40*i;
	dstqword=(QWORD*)dst;

	//放下第一个
	dstqword[0]=0x74726170;		//'part'
	dstqword[1]=0x747067;		//'gpt'
	dstqword[2]=2;
	dstqword[3]=0x21;
	dstqword[4]=0x7766554433221100;
	dstqword[5]=0xffeeddccbbaa9988;
	dstqword[6]=0x8899aabbccddeeff;
	dstqword[7]=0x0011223344556677;
	dst += 0x40;
	dstqword += 8;


	//正式开始转换
	from+=0x400;
	src=from;
	srcqword=(QWORD*)from;
	for(i=0;i<0x80;i++)	//0x80 partitions per disk
	{
		//先取数字出来
		src = from+0x80*i;
		srcqword = (QWORD*)src;
		if(srcqword [0]==0)continue;

		//类型，子类型，开始，结束
		QWORD firsthalf = srcqword [0];
		QWORD secondhalf = srcqword [1];
		QWORD startlba = srcqword [4];
		QWORD endlba = srcqword [5];
		dstqword[0]=0x74726170;	//'disk...'
		dstqword[2]=startlba;
		dstqword[3]=endlba;
		for(j=0;j<0x20;j++)
		{
			dst [0x20 + j] = src [0x38 + j*2];
		}

		//不同分区类型
		if(firsthalf==0x477284830fc63daf)
		{
			if(secondhalf==0xe47d47d8693d798e)
			{
				dstqword[1]=0x747865;		//ext
				//diary("ext\n");
			}
		}
		if(firsthalf==0x477284830fc63daf)
		{
			if(secondhalf==0xe47d47d8693d798e)
			{
				dstqword[1]=0x747865;		//ext
				//diary("ext\n");
			}
		}
		else if(firsthalf==0x11d2f81fc12a7328)
		{
			if(secondhalf==0x3bc93ec9a0004bba)
			{
				dstqword[1]=0x746166;		//fat
				//diary("fat\n");
			}
		}
		else if(firsthalf==0x11aa000048465300)
		{
			if(secondhalf==0xacec4365300011aa)
			{
				dstqword[1]=0x736668;		//hfs
				//diary("hfs\n");
			}
		}
		else if(firsthalf==0x4433b9e5ebd0a0a2)
		{
			if(secondhalf==0xc79926b7b668c087)
			{
				dstqword[1]=0x7366746e;		//ntfs
				//diary("ntfs\n");
			}
		}
		else
		{
			dstqword[1]=0x3f;
			//diary("unknown\n");
		}

		//pointer++
		dst = dst + 0x40;
		dstqword = dstqword + 8;
	}
}
