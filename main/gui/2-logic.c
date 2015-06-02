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
void logic0background()
{
	int x,y;
	background0();

	//外框
	/*
	for(x=0;x<1024;x++)
	{
		point(x,0,0xcc00);
		point(x,639,0xcc00);
	}
	for(y=0;y<640;y++)
	{
		point(0,y,0xcc00);
		point(1023,y,0xcc00);
	}*/
	for(y=0;y<32;y++)
	{
		point(0x100,y,0);
		point(0x200,y,0);
		point(0x300,y,0);
	}
	for(x=0;x<1024;x++)
	{
		point(x,0x20,0);
		point(x,639-32,0);
	}
}
logic0part()
{
	int x,y,i;

	//分区内容
	DWORD color;
	QWORD start;
	for(i=0;i<8;i++)
	{
		//break?
		start=*(QWORD*)(partbuffer+i*0x40);
		if(start==0)break;

		//打印
		color=~start;
		for(y=640-32;y<640;y++)
		{
			for(x=128*i;x<128*i+128;x++)
			{
				point(x,y,color);
			}
		}
		hexadecimal(16*i,38,start);
		string(16*i,39,partbuffer+i*0x40+0x10);
	}

	//选中
	for(y=640-32;y<640;y+=2)
	{
		for(x=choosepart*128;x<choosepart*128+128;x+=2)
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
	string(0x10,1,"name");
	string(0x30,1,"id");
	string(0x50,1,"type");
	string(0x70,1,"size");
	for(y=0;y<36;y++)
	{
		if(*(DWORD*)(p+0x40*y) == 0) break;
		string(0,y+2,p+0x40*y);
		hexadecimal(0x30,y+2,*(QWORD*)(p+0x40*y+0x10));
		hexadecimal(0x50,y+2,*(QWORD*)(p+0x40*y+0x20));
		hexadecimal(0x70,y+2,*(QWORD*)(p+0x40*y+0x30));
	}

	//选中
	for(y=32+choosefile*16;y<48+choosefile*16;y+=2)
	{
		for(x=0;x<1024;x+=2)
		{
			point(x,y,0);
		}
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
void logic0message(DWORD type,DWORD key)
{
	if(type!=2)return;

	int x=key&0xffff;
	int y=(key>>16)&0xffff;
	/*
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
	*/
	if(y>639-32)		//选分区
	{
		choosepart=x/128;
		QWORD arg1=0x30+choosepart;
		realcommand("mount",&arg1);
	}
	else if(y>32)		//选文件
	{
		choosefile=(y-32)/16;
	}
}