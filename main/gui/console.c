#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//log位置
static BYTE* logbuf;

//键盘输入
static BYTE buffer[128];//键盘输入专用
static bufcount=0;

//
static int showconsole=0;
static int complex=0;		//主体华丽程度



void touchconsole()
{
	showconsole^=1;
}
void initconsole()
{
	whereislogbuf(&logbuf);
}
void printlog0()
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
	QWORD offsety=*(DWORD*)(logbuf+0xffff0);
	int linenum=offsety/0x80;
	if(offsety<0x80*36)		//[0,0x80*35]
	{
		for(y=0;y<linenum;y++)
		{
			string(0x20,y,logbuf+0x80*y);
		}
	}
	else
	{
		for(y=0;y<36;y++)
		{
			string(0x20,y,logbuf+offsety+0x80*(y-36));
		}
	}

	//键盘输入区
	for(x=512;x<1024;x++)
		for(y=640-64;y<640-48;y++)
			point(x,y,0xffffffff);
	string(0x40,36,buffer);

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
void printlog1()
{
	
}
void printlog2()
{
	
}
void printlog()
{
	if(showconsole==0)return;

	if(complex==0)
	{
		printlog0();
	}
	else if(complex==1)
	{
		printlog1();
	}
	else
	{
		printlog2();
	}
}
loginput(key)
{
	if(key==0xd)
	{
		command(buffer);

		int i;
		bufcount=0;
		for(i=0;i<128;i++)
		{
			buffer[i]=0;
		}
	}
	else if(key==0x8)		//backspace
	{
		if(bufcount!=0)
		{
			bufcount--;
			buffer[bufcount]=0;
		}
	}
	else
	{
		if(bufcount<0x80)
		{
			buffer[bufcount]=key&0xff;
			bufcount++;
		}
	}
}