#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
void initwindowworker();
void killwindowworker();




//unicode字符表的位置
static unsigned char* unicodetable=0;
void mallocunicodetable()
{
	//从当前目录读取unicode点阵字符文件进入内存
	int ret=0;
	FILE* fp=fopen("unicode.bin","r");		//打开unicode.bin
	if(fp==NULL)return;

	//有这个文件就申请内存
	unicodetable=(char*)malloc(0x200000);		//申请2MB

	//读进内存
	ret=fread(unicodetable , 0x1000 , 0x200 , fp);	//读取2MB
	if(ret<0x200)return;

	//关闭
	if(fp!=NULL)fclose(fp);				//关闭unicode.bin
}
unsigned char* whereisunicodetable()
{
	//return (unsigned char*)&_binary_unicode_unicode_start;
	return unicodetable;
}




void initwindow(char* addr)
{
	//开窗口
	initwindowworker();

	//unicode点阵表
	mallocunicodetable();
}
void killwindow()
{
	//2m
	if(unicodetable!=0)
	{
		free(unicodetable);
		unicodetable=0;
	}

	//1024*1024*4
	killwindowworker();
}
