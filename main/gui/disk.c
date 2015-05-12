#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//硬盘信息
static BYTE* diskinfo;

//
static int showdisk=0;
static int complex=0;		//主体华丽程度




void touchdisk()
{
	showdisk^=1;
}
void initdiskman()
{
	whereisdiskinfo(&diskinfo);
}
void printdisk0()
{
	//背景
	int x,y;
	unsigned int color,i=0;
	for(y=0;y<640-32;y++)
	{
		for(x=256;x<1024;x++)
		{
			point(x,y,0xcccccccc);
		}
	}
	for(y=640-32;y<640-16;y++)
	{
		color=0xcc00+0x11110011*((15-i)*3/4);		//绿
		for(x=1024-32+i;x<1024;x++)
		{
			point(x,y,color);
		}
		i++;
	}

	//内容
	char* p=(char*)diskinfo;
	for(i=0;i<10;i++)
	{
		if(*(DWORD*)(diskinfo+0x100*i) == 0)break;

		for(x=512;x<768;x++)
		{
			for(y=64*i;y<64*i+48;y++)
			{
				point(x,y,0xffffffff);
			}
		}
		string(0x40,4*i,diskinfo+0x100*i);
		string(0x40,4*i+1,diskinfo+0x100*i+0x80);
	}

	//选了谁
	for(x=512;x<768;x++)
	{
		for(y=640-64;y<640-32;y++)
		{
			point(x,y,0xffffffff);
		}
	}
	string(0x40,36,diskinfo);

	//框框
	for(x=256;x<1024;x++)
	{
		point(x,0,0xcc00);
	}
	for(x=256;x<1024-32;x++)
	{
		point(x,640-33,0xcc00);
	}
	for(y=0;y<640-32;y++)
	{
		point(256,y,0xcc00);
	}
	for(y=0;y<640-16;y++)
	{
		point(1023,y,0xcc00);
	}
	for(x=0;x<16;x++)
	{
		point(x+1024-32,x+640-32,0xcc00);
	}
}
void printdisk1()
{
}
void printdisk2()
{
	
}
void printdisk()
{
	if(showdisk==0)return;

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