#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void uicommand(char*);
//libui
void string(int x,int y,char* str);
void backgroundcolor(DWORD);
QWORD readwindow(QWORD);
//libsoft
void command(char* in);
int compare(char* first,char* second);
char* whereisworld();
//libboot
void say(char*,...);





//palette
static unsigned int* palette=0;
static int resolutionx;
static int resolutiony=768;

//log位置
static char* logbuf=0;
static QWORD backward=0;

//键盘输入
static BYTE buffer[128];//键盘输入专用
static int bufcount=0;








static void background4()
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
	for(y=0;y<resolutiony/2;y++)
	{
		color = y*0xff/resolutiony;//0x44444488;

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
static void printposition(int start,int count,int max)
{
	//位置
	int x,y;
	if(max<0x80*45)return;

	//显示区大小/总大小
	QWORD top=resolutiony*start/max;
	QWORD bottom=resolutiony*(start+0x80*count)/max;//temp变量=max
	say("printposition:%x,%x\n",top,bottom);

	for(y=top;y<bottom;y++)
	{
		for(x=1024-16+4;x<1024-4;x++)
		{
			palette[(y*1024) + x] = 0x01234567;
		}
	}
}
static void printstdout(int start,int count)
{
	//总共38行，必须保证start>=0x80*行数
	int x,y;
	char* p=logbuf+start;
	//say("printstdout:%d,%d\n",start,count);
	for(y=0;y<count;y++)
	{
		string(0 , y , p + y * 0x80);
	}
}
static void printstdin(int count)
{
	string(0,count,"user:");
	string(0x5,count,buffer);
}








static void writeconsole(QWORD type,QWORD key)
{
	if(type==0x72616863)		//'char'
	{
		if(key==0x8)		//backspace
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
				uicommand(0);
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
			if(backward<enqueue-0x80*40)	//没到头才能继续
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
static void readconsole()
{
	//显示哪儿开始的一块
	int count=(resolutiony/16)-2-1;	//-windowtitle -mathproblem
	int enqueue=*(DWORD*)(logbuf+0xffff0);

	int start=enqueue-(count*0x80)-backward;//代表末尾位置而不是开头
	if( start<0 )start=0;

	//背景(start,end)
	background4();

	printposition(start,count,enqueue);

	printstdout(start,count);

	printstdin(count);
}
static void intoconsole()
{
	if(logbuf==0)
	{
		//never call console before that thing
		//logbuf=whereisworld()+0x100000;
		palette=(unsigned int*)readwindow(0x6572656877);
	}
	else	//不是第一次进来了
	{
		backgroundcolor(0);
	}
}
static void listconsole(QWORD* this)
{
	this[0]=0x776f646e6977;
	this[1]=0x656c6f736e6f63;
	this[2]=(0<<16)+0;   //startaddr
	this[3]=(768<<16)+1024; //endaddr
	this[4]=(QWORD)listconsole;
	this[5]=(QWORD)intoconsole;
	this[6]=(QWORD)readconsole;
	this[7]=(QWORD)writeconsole;
}








void initconsole(char* in)
{
	listconsole( (QWORD*)in );
}
void killconsole()
{
}
