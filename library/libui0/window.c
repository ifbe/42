#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>




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