#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
void filelist();
void filetarget(QWORD);
void fileread(QWORD,QWORD,QWORD,QWORD);
void fileinit(QWORD);




static char*   world=0;

static char* palette=0;

static char* rootbuf=0;
static char*  dirbuf=0;
static char*   fsbuf=0;
static char* databuf=0;

static char*   inbuf=0;
static char*  outbuf=0;
static char*  logbuf=0;
static char* listbuf=0;




//+0x100000
void say(char* rcx,QWORD rdx,QWORD r8,QWORD r9)
{
	int start=0;
	int temp=0;
	QWORD offsety;
	QWORD offsetx;
	while(1)			//举例“123443%d\n\n    44532”
	{
		if(rcx[temp] == 0)		//0
		{
			offsety=*(DWORD*)(outbuf+0xffff0);
			offsetx=*(DWORD*)(outbuf+0xffff8);
			int writecount=snprintf(outbuf+offsety+offsetx,0x80-offsetx,
					rcx+start,rdx,r8,r9);

			*(DWORD*)(outbuf+0xffff8)+=writecount;

			break;
		}
		if( rcx[temp] < 0x10)	//"\n":0xa?		0xd?
		{
			offsety=*(DWORD*)(outbuf+0xffff0);
			offsetx=*(DWORD*)(outbuf+0xffff8);
			snprintf(outbuf+offsety+offsetx,0x80-offsetx,
					rcx+start,rdx,r8,r9);

			*(DWORD*)(outbuf+0xffff8)=0;
			*(DWORD*)(outbuf+0xffff0)=(offsety+0x80)%0xffff0;
			temp+=1;
			start=temp;

			continue;
		}
		temp++;
	}
}

//+0x200000
void diary(char* fmt , ...)
{
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
}

//+0x300000
void list()			//ls?
{
	filelist();
}
void target(QWORD what)		//choose是soft做的...名字不能一样...
{
	filetarget(what);
}
void readmemory(QWORD buf,QWORD sector,QWORD ignore,DWORD count)		//read?
{
	//if(this is disk or file)
	fileread(buf,sector,0,count);
}
void writememory()		//write?
{
	
}
char* whereisworld()
{
	return world;
}








//打开几号窗户
void openwindow()
{
	initwindow(palette);
}
//关掉所有窗户
void closewindow()
{
	killwindow();
}
//往当前屏幕上扔东西
void writewindow()
{
	writescreen(1);
}
//从当前屏幕上捡东西
void readwindow()
{
}
void setwindow(QWORD first,QWORD second)
{
	//设置当前窗口大小
	if( first == 'size' )
	{
		windowsize(second);
	}

	//设置当前屏幕属性
	if( first == 'hide' )
	{
		windowhide();
	}
}
void getwindow(QWORD first,QWORD second)
{
	//想要当前窗口大小
	if( first == 'size' )
	{
		return getwindowsize();
	}

	//想要其他东西
}








//特殊函数1：贿赂图书管理员，来帮忙初始化很多东西，给多少钱，做多少事
//__attribute__((constructor)) void bribelibrarian(int money)
void bribelibrarian(int money)
{
	//只作为转换器使用
	if(money == 0)
	{
		printf("all by your self!\n");
	}

	//只初始化一个窗口
	else if(money == 4)
	{
		//从系统手上申请内存
		world=malloc(0x400000);
		if(world==NULL)
		{
			printf("no enough money\n");
			exit(-1);
		}

		//分赃，全部给窗口工人
		palette=world;
	}

	//只初始化处理函数
	else if(money == 8)
	{
		//从系统手上申请内存
		world=malloc(0x800000);
		if(world==NULL)
		{
			printf("no enough money\n");
			exit(-1);
		}

		//分赃，全部给处理工人
		rootbuf=	world+0x0;
		dirbuf=		world+0x100000;
		fsbuf=		world+0x200000;
		databuf=	world+0x300000;

		inbuf=		world+0x400000;
		outbuf=		world+0x500000;
		logbuf=		world+0x600000;
		listbuf=	world+0x700000;

		int i=0;
		for(i=0;i<0x800000;i++)rootbuf[i]=0;
		fileinit(listbuf);
	}

	//初始化处理函数与窗口
	else if(money == 12)
	{
		//从系统手上申请内存
		world=malloc(0xc00000);
		if(world==NULL)
		{
			printf("no enough money\n");
			exit(-1);
		}

		//部分给窗口工人
		palette=	world+0x800000;		//[+0x800000,+0xbfffff]

		//分赃给处理工
		rootbuf=	world+0x0;
		dirbuf=		world+0x100000;
		fsbuf=		world+0x200000;
		databuf=	world+0x300000;

		inbuf=		world+0x400000;
		outbuf=		world+0x500000;
		logbuf=		world+0x600000;
		listbuf=	world+0x700000;

		int i=0;
		for(i=0;i<0x800000;i++)rootbuf[i]=0;
		fileinit(listbuf);
	}

	//初始化驱动与处理函数
	else if(money == 16)
	{
		//从系统手上申请内存
		world=malloc( 0x1000000 );
		if(world==NULL)
		{
			printf("no enough money\n");
			exit(-1);
		}

		//分赃给驱动工人
		//nullbuf=	world+0x0;
		//kernelbuf=	world+0x100000;
		//heapbuf=	world+0x200000;
		//stackbuf=	world+0x300000;
		//ahcibuf=	world+0x400000;
		//satabuf=	world+0x500000;
		//xhcibuf=	world+0x600000;
		//usbbuf=	world+0x700000;

		//部分给处理工
		rootbuf=	world+0x800000;
		dirbuf=		world+0x900000;
		fsbuf=		world+0xa00000;
		databuf=	world+0xb00000;
		inbuf=		world+0xc00000;
		outbuf=		world+0xd00000;
		logbuf=		world+0xe00000;
		listbuf=	world+0xf00000;

		int i=0;
		for(i=0;i<0x800000;i++)rootbuf[i]=0;
		fileinit(listbuf);
	}

	//防止误调用
	else
	{
		printf("byebye!");
		exit(-1);
	}
}
//特殊函数2：图书管理员，负责检查，用户是否防止忘记释放，如果是的话就帮助释放
__attribute__((destructor)) void byelibrarian()
{
	if(world != 0)
	{
		free(world);
		world=0;
	}
}
