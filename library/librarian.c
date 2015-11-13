#include<stdio.h>
#include<stdlib.h>
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//ui
void initwindow(char*,QWORD);
void killwindow();
QWORD currentresolution();
//soft
char* explainarg();
void initin(char*);
void initout(char*);
void initlog(unsigned long long);
void initlist(unsigned long long);
void target(char*);
//hard
//void usb();




//ui
static unsigned long long resolution=(768<<16)+1024;
static char* data;
//soft
static char* world;
	//[0x000000,0x0fffff]:diskhome
	//[0x100000,0x1fffff]:fshome
	//[0x200000,0x2fffff]:dirhome
	//[0x300000,0x3fffff]:datahome

	//[0x400000,0x4fffff]:stdin
	//[0x500000,0x5fffff]:stdout
	//[0x600000,0x6fffff]:log
	//[0x700000,0x7fffff]:list
//hard
//static void* what;




__attribute__((constructor(101))) void initworld()
{
	//打开方式，打开目标（因为stdin和stdout的缘故，必须在其他init之前）
	int i;
	char* p=explainarg();

	//8MB的世界
	world = (char*)malloc(0x800000);		//8M
        data = (char*)malloc(0x400000);              //4M
	if(world == NULL){printf("can't allloc memory for world\n");exit(-1);}
	if(data == NULL){printf("can't allloc memory for screen\n");exit(-1);}
	for(i=0;i<0x800000;i++)world[i]=0;

	//init [+0x400000,+0x4fffff](stdin )		waitevent()
	initin(world+0x400000);

	//init [+0x500000,+0x5fffff](stdout)		say()
	initout(world+0x500000);

	//init [+0x600000,+0x6fffff](stderr)		diary()
	initlog(world+0x600000);

	//init [+0x700000,+0x7fffff](mylist)		enumerate()
	initlist(world+0x700000);

	//screen
        initwindow(data,resolution);

	//选一个满意的目标就离开
	target(p);
}
__attribute__((destructor)) void killworld()
{
	//window
        killwindow();
        free(data);

	//killlist();
	//killlog();
	//killout();
	//killin();
	free(world);
}




//ui
char* screendata()
{
        return data;
}
QWORD screenresolution()
{
        return currentresolution();
}
//soft
unsigned long long whereisworld()
{
	return (QWORD)world;
}
//hard
//void usb()
