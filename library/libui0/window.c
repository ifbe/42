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
unsigned char* whereisunicodetable()
{
	//return (unsigned char*)&_binary_unicode_unicode_start;
	return unicodetable;
}




int initunicodetable()
{
	//从当前目录读取unicode点阵字符文件进入内存
	FILE* fp=fopen("unicode.bin","r");		//打开unicode.bin
	if(fp==NULL)
	{
		printf("(failed,ignored)open unicode.bin\n");
		return 0;
	}

	//有这个文件就申请内存
	unicodetable=(char*)malloc(0x200000);		//申请2MB
	if(unicodetable==NULL)
	{
		printf("(failed,ignored)malloc unicode.bin\n");
		return 0;
	}

	//读进内存
	int ret=fread(unicodetable , 0x200 , 0x1000 , fp);	//读取2MB
	//printmemory(unicodetable,0x1000);
	//if(ret<0x200)				//windows读取成功了还报错？
	//{
	//	printf("(%x)(failed,ignored)readunicode.bin\n",ret);
	//	if(fp!=NULL)fclose(fp);
	//	return 0;
	//}

	//关闭
	if(fp!=NULL)fclose(fp);				//关闭unicode.bin
	return 1;
}
void killunicodetable()
{
	//2m
	if(unicodetable!=0)
	{
		free(unicodetable);
		unicodetable=0;
	}
}




void windowinit(char* type,char* addr)
{
	if(type==0)
	{
		//开窗口
		initwindowworker();

		//unicode点阵表
		initunicodetable();
	}
}
void windowkill()
{
	//unicode点阵表
	killunicodetable();

	//1024*1024*4
	killwindowworker();
}
