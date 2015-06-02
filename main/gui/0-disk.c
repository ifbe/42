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
	string(0x10,10,"i can recognize these disks");
	string(0x10,33,"u can choose a specific one");

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
	for(y=512;y<512+48;y++)
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
	background0();

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
	int val=0;
	/*
	if(y>320-128)
	{
		if(y<320+128)
		{
			if(x<32)
			{
				killmehelpit(0,1);
				return;
			}
			if(x>1024-32)
			{
				killmehelpit(0,2);
				return;
			}
		}
	}
	*/
	if(x>256)
	{
		if(x<768)
		{
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
	}
}