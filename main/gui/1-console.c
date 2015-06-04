#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//log位置
static BYTE* logbuf;
static QWORD offset=0;

//键盘输入
static BYTE buffer[128];//键盘输入专用
static bufcount=0;

//
static int complex=0;		//主体华丽程度



void consoleinit()
{
	whereislogbuf(&logbuf);
}
void printconsole0()
{
	//显示哪儿开始的一块
	int x,y;

	//
	DWORD temp=*(DWORD*)(logbuf+0xffff0);
	QWORD showaddr=temp-offset;				//代表末尾位置而不是开头
	if(showaddr<0x80*38)showaddr=0x80*38;

	//总共38行，必须保证showaddr>=0x80*38
	for(y=0;y<38;y++)
	{
		string(0,y,logbuf+showaddr+0x80*(y-38));
	}

	//键盘输入区
	for(y=640-16;y<640;y++)
		for(x=128;x<896;x++)
			point(x,y,0xffffffff);
	for(y=640-16;y<640;y++)
	{
		point(128,y,0);
		point(895,y,0);
	}
	for(x=128;x<896;x++)
	{
		point(x,639-16,0);
		point(x,639,0);
	}
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
	background0();

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
consolemessage(DWORD type,DWORD key)
{
	if(type==1)
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
	else if(type==3)
	{
		if(key<0xff0000)		//滚轮上
		{
			DWORD temp=*(DWORD*)(logbuf+0xffff0);
			if(temp>=0x80*38)		//不够一页不用上翻
			{
				if(offset<temp-0x80*38)offset+=0x80;
			}
		}
		else if(key>0xff0000)	//滚轮下
		{
			if(offset>=0x80)offset-=0x80;
		}
	}
}