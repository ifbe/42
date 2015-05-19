#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//硬盘信息
static BYTE* diskinfo;

//
static int choose=0;
static int complex=0;		//主体华丽程度




void initdiskman()
{
	whereisdiskinfo(&diskinfo);
}
disk_mouse(int x,int y)
{
	int val=0;

	if(y>64)		//[80,128),[144,192),[208,256),[272,320)......
	{
		if(y<512)
		{
			val=(y-64)/16;
			if( (val%4) != 0 )		//val=4n不行，val=4n+1,4n+2,4n+3都可以
			{
				choose=val/4;

				char* arg0="disk";
				DWORD arg1=0x30+choose;
				realcommand(arg0,&arg1);
			}
		}
	}
}
disk_kbd()
{
}
disk_bg()
{
	int x,y;

	//背景色
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0x88888888);
		}
	}

	string(0x30,3,"i can recognize these disks");
	for(x=128;x<1024-128;x++)
	{
		point(x,64,0xffffffff);
	}

	string(0x30,31,"u can choose a specific one");
	for(x=128;x<1024-128;x++)
	{
		point(x,512,0xffffffff);
	}
}
void printdisk0()
{
	//背景
	char* p=(char*)diskinfo;
	int x,y,i;

	//内容
	for(i=0;i<10;i++)
	{
		if(*(DWORD*)(diskinfo+0x100*i) == 0)break;

		for(y=80+64*i;y<128+64*i;y++)
		{
			for(x=256;x<768;x++)
			{
				point(x,y,0xffffffff);
			}
		}
		string(0x20,6+4*i,diskinfo+0x100*i);
		string(0x20,7+4*i,diskinfo+0x100*i+0x80);
	}
	//选中
	for(y=80+64*choose;y<128+64*choose;y+=2)
	{
		for(x=256;x<768;x+=2)
		{
			point(x,y,0);
		}
	}

	//手工输入
	for(y=512+16;y<512+64;y++)
	{
		for(x=256;x<768;x++)
		{
			point(x,y,0xffffffff);
		}
	}
	string(0x20,32+2,diskinfo);

}
void printdisk1()
{
}
void printdisk2()
{
	
}
void printdisk()
{
	disk_bg();

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