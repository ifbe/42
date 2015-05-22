#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

static QWORD partbuffer;
static QWORD dirbuffer;

static char choosepart=0;
static char choosefile=0;
static int complex=0;		//主体华丽程度




void logic0init()
{
	whereisrealworld(&partbuffer);

	whereislogicworld(&dirbuffer);
	dirbuffer+=0x100000;
}
void logic0mouse(int x,int y)
{
	if(y>320-128)
	{
		if(y<320+128)
		{
			if(x<32)
			{
				killmehelpit(2,0);
				return;
			}
		}
	}
	if(x<128)
	{
		if(y>32)
		{
			if(y<512)
			{
				choosepart=(y-32)/32;
				QWORD arg1=0x30+choosepart;
				realcommand("mount",&arg1);
			}
		}
	}
}
void logic0kbd()
{
	
}
void logic0background()
{
	QWORD x,y;
	unsigned int color,i=0;
	//清屏
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0xcccccccc);
		}
	}
	//外框
	for(x=0;x<1024;x++)
	{
		point(x,0,0xcc00);
		point(x,639,0xcc00);
	}
	for(y=0;y<640;y++)
	{
		point(0,y,0xcc00);
		point(1023,y,0xcc00);
	}

	//内框
	for(x=128;x<1024-128;x++)
	{
		point(x,32,0xcc00);
		point(x,640-32,0xcc00);
	}
	for(y=32;y<640-32;y++)
	{
		point(128,y,0xcc00);
		point(1023-128,y,0xcc00);
	}
/*
	//左上
	for(y=0;y<32;y++)
		for(x=0;x<32-y;x++)
			point(x,y,0);
	//右上
	for(y=0;y<32;y++)
		for(x=1024-32+y;x<1024;x++)
			point(x,y,0);
	//左下
	for(x=0;x<32;x++)
		for(y=640-32+x;y<640;y++)
			point(x,y,0);
	//右下
	//for(y=640-32;y<640;y++)
	//	for(x=1024-32;x<1024;x++)
	//		point(x,y,0xff00);
*/
	//左箭头
	for(x=0;x<32;x++)
		for(y=320-4*x;y<320+4*x;y++)
			point(x,y,0xffffffff);
	string(0,20,"main");
}
logic0part()
{
	//kuang
	int x,y,i;
	for(x=0;x<128;x++)
	{
		point(x,32,0xcc00);
		point(x,640-32,0xcc00);
	}
	for(y=32;y<640-32;y++)
	{
		point(0,y,0xcc00);
		point(127,y,0xcc00);
	}

	//分区内容
	DWORD color;
	QWORD start;
	for(i=0;i<128;i++)
	{
		//break?
		start=*(QWORD*)(partbuffer+i*0x40);
		if(start==0)break;

		//打印
		color=~start;
		for(y=32+i*32;y<64+i*32;y++)
		{
			for(x=0;x<128;x++)
			{
				point(x,y,color);
			}
		}
		hexadecimal(0,2+2*i,start);
	}

	//选中
	for(y=32+choosepart*32;y<64+choosepart*32;y+=2)
	{
		for(x=0;x<128;x+=2)
		{
			point(x,y,0);
		}
	}
}




void printfile0()
{
	int x,y;
	char* p;

//三.每个分区里面的文件和文件夹
	p=(char*)dirbuffer;
	string(0x10+0xa,0,"name");	//0x10+0x18*0+0xa
	string(0x28+0xa,0,"id");	//0x10+0x18*1+0xa
	string(0x40+0xa,0,"type");	//0x10+0x18*2+0xa
	string(0x58+0xa,0,"size");	//0x10+0x18*3+0xa
	for(y=16;y<32;y++)
	{
		point(0x10*8,y,0);
		point((0x10+0x18)*8,y,0);
		point((0x10+0x18*2)*8,y,0);
		point((0x10+0x18*3)*8,y,0);
		point((0x10+0x18*4)*8,y,0);
	}
	for(y=0;y<36;y++)
	{
		if(*(DWORD*)(p+0x40*y) == 0) break;
		string(0x10+0xa,y+2,p+0x40*y);
		hexadecimal(0x28+0xa,y+2,*(QWORD*)(p+0x40*y+0x10));
		hexadecimal(0x40+0xa,y+2,*(QWORD*)(p+0x40*y+0x20));
		hexadecimal(0x58+0xa,y+2,*(QWORD*)(p+0x40*y+0x30));
	}
}
void printfile1()
{
	
}
void printfile2()
{
	
}
void logic0()
{
	logic0background();
	logic0part();

	//-------------------
	if(complex==0)
	{
		printfile0();
	}
	else if(complex==1)
	{
		printfile1();
	}
	else
	{
		printfile2();
	}
}