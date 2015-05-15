#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//硬盘信息
static BYTE* diskinfo;

//
static int complex=0;		//主体华丽程度




void initdiskman()
{
	whereisdiskinfo(&diskinfo);
}
void printdisk0()
{
	//背景
	int x,y,i;
	char* p=(char*)diskinfo;

	//内容
	for(i=0;i<10;i++)
	{
		if(*(DWORD*)(diskinfo+0x100*i) == 0)break;

		for(x=256;x<768;x++)
		{
			for(y=64+64*i;y<64+48+64*i;y++)
			{
				point(x,y,0xffffffff);
			}
		}
		string(0x20,4+4*i,diskinfo+0x100*i);
		string(0x20,5+4*i,diskinfo+0x100*i+0x80);
	}

	//选了谁
	string(0x20,36,diskinfo);

}
void printdisk1()
{
}
void printdisk2()
{
	
}
void printdisk()
{
	if(complex==0)
	{
		printdisk0();
	}
	else if(complex==1)
	{
		printdisk1();
	}
	else
	{
		printdisk2();
	}
}
mouseinputfordisk()
{
	
}
kbdinputfordisk()
{
	
}