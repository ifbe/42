#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//硬盘信息
static BYTE* diskinfo;

//
static int choose=0;
static int complex=0;		//主体华丽程度




void diskinit()
{
	whereisdiskinfo(&diskinfo);
}
void printdisk0()
{
	//背景
	char* p=(char*)diskinfo;
	int x,y,i;

	for(y=128;y<640-128;y++)
	{
		for(x=256;x<768;x++)
		{
			point(x,y,0xffffff);
		}
	}
	for(y=128;y<640-128;y++)
	{
		point(256,y,0);
		point(767,y,0);
	}
	for(x=256;x<768;x++)
	{
		point(x,128,0);
		point(x,639-128,0);
	}
	//
	for(x=256;x<768;x++)point(x,320,0);

	//内容
	string(0x30,0xa,"disk:");
	for(i=0;i<10;i++)
	{
		if(*(DWORD*)(diskinfo+0x100*i) == 0)break;

		for(y=128+32*i+4;y<160+32*i-4;y++)
		{
			for(x=512;x<768;x++)
			{
				point(x,y,0xff);
			}
		}
		string(0x40,8+i*2,diskinfo+0x100*i);
		//string(0x40,8+i,diskinfo+0x100*i+0x80);
	}
	//选中
	for(y=128+32*choose;y<160+32*choose;y+=2)
	{
		for(x=512;x<768;x+=2)
		{
			point(x,y,0);
		}
	}

	//手工输入
	string(0x40,19,diskinfo);

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
diskmessage(DWORD type,DWORD key)
{
	if(type!=2)return;

	int x=key&0xffff;
	int y=(key>>16)&0xffff;
	if(x>256)
	{
		if(x<768)
		{
			if(y>128)		//[80,128),[144,192),[208,256),[272,320)......
			{
				if(y<320)
				{
					//int val=(y-128)/16;
					//if( ( (val%4) == 1 ) | ( (val%4) == 2 ) )
					//{
					//	choose=val/4;
						choose=(y-128)/32;
						char* arg0="disk";
						DWORD arg1=0x30+choose;
						realcommand(arg0,&arg1);
					//}
				}
			}
		}
	}
}