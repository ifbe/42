#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>




static char*   world=0;
static char* palette=0;
char* whereisworld()
{
	return world;
}
char* whereispalette()
{
	return palette;
}




//特殊函数1：贿赂图书管理员，来帮忙初始化很多东西，给多少钱，做多少事
void onlywindow()
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
void onlysoftware()
{
	int i=0;
	char* p=0;

	//从系统手上申请内存
	world=malloc(0x800000);
	if(world==NULL)
	{
		printf("no enough money\n");
		exit(-1);
	}
	for(i=0;i<0x800000;i++)rootbuf[i]=0;

	//分赃，全部给处理工人
	initinput(  world+0x400000 );
	initoutput( world+0x500000 );
	initdiary(  world+0x600000 );
	initmemory( world+0x700000 );
	initmaster( world );
}
void windowandsoftware()
{
	//从系统手上申请内存
	int i=0;
	world=malloc(0xc00000);
	if(world==NULL)
	{
		printf("no enough money\n");
		exit(-1);
	}
	for(i=0;i<0x800000;i++)rootbuf[i]=0;

	//部分给窗口工人
	palette=	world+0x800000;		//[+0x800000,+0xbfffff]

	//分赃，全部给处理工人
	initinput(  world+0x400000 );
	initoutput( world+0x500000 );
	initdiary(  world+0x600000 );
	initmemory( world+0x700000 );

	initmaster(world);
}
//__attribute__((constructor)) void bribelibrarian(int money)
void bribelibrarian(int money)
{
	//只初始化一个窗口
	if(money == 4) onlywindow();

	//只初始化处理函数
	else if(money == 8) onlysoftware();

	//初始化处理函数与窗口
	else if(money == 12) windowandsofware();

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
