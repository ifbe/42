#include<stdio.h>
#include<stdlib.h>
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//soft
char* explainarg();
void initin(unsigned long long);
void initout(unsigned long long);
void initlog(unsigned long long);
void initlist(unsigned long long);
void target(char*);




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




__attribute__((constructor(101))) void initworld()
{
	//打开方式，打开目标（因为stdin和stdout的缘故，必须在其他init之前）
	int i;
	char* p=explainarg();

	//8MB的世界
	world = (char*)malloc(0x800000);		//8M
	if(world == NULL){printf("can't allloc memory for world\n");exit(-1);}
	for(i=0;i<0x800000;i++)world[i]=0;

	//init [+0x400000,+0x4fffff](stdin )		waitevent()
	initin((QWORD)world+0x400000);

	//init [+0x500000,+0x5fffff](stdout)		say()
	initout((QWORD)world+0x500000);

	//init [+0x600000,+0x6fffff](stderr)		diary()
	initlog((QWORD)world+0x600000);

	//init [+0x700000,+0x7fffff](mylist)		enumerate()
	initlist((QWORD)world+0x700000);

	//选一个满意的目标就离开
	target(p);
}
__attribute__((destructor)) void killworld()
{
	//killlist();
	//killlog();
	//killout();
	//killin();
	free(world);
}




//soft
unsigned long long whereisworld()
{
	return (QWORD)world;
}
