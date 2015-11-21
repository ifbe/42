#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

void die();
//void f1init();

void string(int x,int y,char* str);
void point(int x,int y,DWORD color);
void cleanscreen();

int compare(char* first,char* second);
void command(char* in);

unsigned int* whereispalette();
char* whereismemory();




//palette
static unsigned int*  palette=0;

//log位置
static char* logbuf=0;
static QWORD backward=0;

//键盘输入
static BYTE buffer[128];//键盘输入专用
static int bufcount=0;








void background4(QWORD showaddr,QWORD temp)
{
	//用指定颜色清屏
	DWORD x,y,color;

	for(x=0;x<1024*752;x++)
	{
		palette[x]=0;
	}

	for(y=768-16;y<768;y++)
	{
		for(x=0;x<512;x++)
		{
			color=(x/2)*0x01010101;
			palette[(y*1024) + x]=color;
			palette[(y*1024) + 1023-x]=color;
		}
	}

	for(y=0;y<384;y++)
	{
		color = y*0xff/384;//0x44444488;

		for(x=1024-16;x<1024;x++)
		{
			palette[(y*1024) + x] = color;
		}
		for(x=1024-16;x<1024;x++)
		{
			palette[((767-y)*1024) + x] = color;
		}
	}

	//位置
	if(temp>=0x80*40)
	{
		QWORD end=64+(640-64*2)*showaddr/temp;			//temp变量=max
		QWORD start=end-(640-64*2)*0x80*40/temp;
		for(y=start;y<end;y++)
		{
			for(x=1024-16+4;x<1024-4;x++)
			{
				palette[(y*1024) + x] = 0x01234567;
			}
		}
	}
}
printstdout(QWORD start,QWORD lastline)
{
	//总共38行，必须保证showaddr>=0x80*行数
	int x,y;
	for(y=0;y<lastline;y++)
	{
		string(0 , y , logbuf+start+0x80*(y-lastline));
	}
}
printstdin(QWORD lastline)
{
	string(0,lastline,"user:");
	string(0x5,lastline,buffer);
}

void f4show()
{
	//显示哪儿开始的一块
	int resolutiony=768;
	int lastline=(resolutiony/16) - 2 - 1;		//-windowtitle -mathproblem
	DWORD enqueue=*(DWORD*)(logbuf+0xffff0);
	QWORD start=enqueue-backward;				//代表末尾位置而不是开头
	if(start<0x80*lastline)start=0x80*lastline;

	//背景(start,end)
	background4(start,enqueue);

	printstdout(start,lastline);

	printstdin(lastline);
}
void f4message(QWORD type,QWORD key)
{
	if(type==0x72616863)		//'kbd'
	{
		if(key==0x8)			//backspace
		{
			if(bufcount!=0)
			{
				bufcount--;
				buffer[bufcount]=0;
			}
		}
		else if(key==0xd)		//回车
		{
			if(compare( buffer , "exit" ) == 0)
			{
				die();
				return;
			}
			else command(buffer);

			for(bufcount=0;bufcount<127;bufcount++) buffer[bufcount]=0;
			bufcount=0;
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
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
		DWORD enqueue=*(DWORD*)(logbuf+0xffff0);
		if(enqueue>=0x80*40)		//大于一页才准上翻
		{
			if(backward<enqueue-0x80*40)	//还没翻到头才能继续翻
			{
				backward+=0x80;
			}
		}
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
		if(backward>=0x80)backward-=0x80;
	}
}








void f4init()
{
	if(logbuf==0)
	{
		logbuf=whereismemory()+0x500000;
		palette=whereispalette();
	}
	else	//不是第一次进来了
	{
		cleanscreen();
	}
}
