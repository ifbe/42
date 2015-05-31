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
static int complex=0;		//主体华丽程度



void consoleinit()
{
	whereislogbuf(&logbuf);
}
consolekbd(key)
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
void consolebg()
{
	int x,y;
	unsigned int color,i=0;
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0xe0e0e0);
		}
	}
	//上下
	for(y=0;y<16;y++)
	{
		color=(QWORD)y*0x0e0e0e;

		for(x=y;x<1024-y;x++)
		{
			point(x,y,color);
			point(x,639-y,color);
		}
	}
	//左右
	for(x=0;x<16;x++)
	{
		color=(QWORD)x*0x0e0e0e;

		for(y=x;y<640-x;y++)
		{
			point(x,y,color);
			point(1023-x,y,color);
		}
	}
}
void printconsole0()
{
	//背景
	int x,y;

	//内容
	QWORD offsety=*(DWORD*)(logbuf+0xffff0);
	int linenum=offsety/0x80;
	if(offsety<0x80*36)		//[0,0x80*35]
	{
		for(y=0;y<linenum;y++)
		{
			string(0,y+2,logbuf+0x80*y);
		}
	}
	else
	{
		for(y=0;y<36;y++)
		{
			string(0,y+2,logbuf+offsety+0x80*(y-36));
		}
	}

	//键盘输入区
	for(y=640-16;y<640;y++)
		for(x=128;x<896;x++)
			point(x,y,0xffffffff);
	string(0x10,39,buffer);
}
void printconsole1()
{
	
}
void printconsole2()
{
	
}
void console()
{
	consolebg();

	if(complex==0)
	{
		printconsole0();
	}
	else if(complex==1)
	{
		printconsole1();
	}
	else
	{
		printconsole2();
	}
}