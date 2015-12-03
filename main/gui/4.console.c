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
static int resolutionx;
static int resolutiony=768;

//log位置
static char* logbuf=0;
static QWORD backward=0;

//键盘输入
static BYTE buffer[128];//键盘输入专用
static int bufcount=0;








void background4()
{
	//用指定颜色清屏
	DWORD x,y,color;

	for(x=0;x<1024*752;x++)
	{
		palette[x]=0;
	}

	//输入框颜色
	for(y=768-16;y<768;y++)
	{
		for(x=0;x<512;x++)
		{
			color=(x/2)*0x01010101;
			palette[(y*1024) + x]=color;
			palette[(y*1024) + 1023-x]=color;
		}
	}

	//滚动框颜色
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
}
void printposition(int showaddr,int count,int max)
{
	//位置
	int x,y;
	diary("printposition:%d,%d\n",showaddr,count);

	count=count*0x80;
	if(count<0x80*48)return;

	QWORD start=640*showaddr/max;
	QWORD end=640*(showaddr+count)/max;			//temp变量=max
	for(y=start;y<end;y++)
	{
		for(x=1024-16+4;x<1024-4;x++)
		{
			palette[(y*1024) + x] = 0x01234567;
		}
	}
}
void printstdout(int start,int count)
{
	//总共38行，必须保证showaddr>=0x80*行数
	int x,y;
	char* p=logbuf+start;
	diary("printstdout:%d,%d\n",start,count);
	for(y=0;y<count*0x80;y++)
	{
		string(0 , y , p + y * 0x80);
	}
}
void printstdin(int count)
{
	string(0,count,"user:");
	string(0x5,count,buffer);
}
void f4show()
{
	//显示哪儿开始的一块
	int count=(resolutiony/16) - 2 - 1;		//-windowtitle -mathproblem
	int enqueue=*(DWORD*)(logbuf+0xffff0);

	int start=enqueue-backward;				//代表末尾位置而不是开头
	if( start<0 )start=0;

	//背景(start,end)
	background4();

	printposition(start,count,enqueue);

	printstdout(start,count);

	printstdin(count);
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
			else
			{
				say("%s\n",buffer);
				command(buffer);

				for(bufcount=0;bufcount<127;bufcount++) buffer[bufcount]=0;
				bufcount=0;
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
