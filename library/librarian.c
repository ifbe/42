#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
//libui1
void initpalette(char*);
void killpalette();
//libui0
void initwindow(char*);
void killwindow();
//libsoft1
void initmaster(char*);
void killmaster();
//libsoft0
void initmemory(char*);
void killmemory();
//libboot1
void initstd(char*);	//listen,say,diary,and
void killstd();
//argc,argv......
char* explainarg();




//pixels
static char* palette=0;		//4m
char* whereispalette()
{
	//unsigned int*
	return palette;
}




//indexs
static char*   world=0;		//4m
static char*    body=0;		//4m
static char*  memory=0;		//4m
static char*    face=0;		//4m
char* whereisworld()
{
	return world;
}
char* whereisbody()
{
	return body;
}
char* whereismemory()
{
	return memory;
}
char* whereisface()
{
	return face;
}




//省的写一大串初始化代码，这里是几种常见初始化过程
void worldandmemory()		//8m
{
	int i=0;
	char* wantfile=explainarg();

	//[0,4)
	world=malloc(0x800000);
	if(world==NULL){printf("no enough momery\n");exit(-1);}
	else for(i=0;i<0x800000;i++)world[i]=0;
	initstd(world+0);
	//[8,c)
	memory=world+0x400000;
	initmemory( memory );
	initmaster( memory );
}
void initall()		//8+4m
{
	int i=0;
	char* wantfile=explainarg();

	//[0,4)
	world=malloc(0x400000*4);
	if(world==NULL){printf("no enough momery\n");exit(-1);}
	else for(i=0;i<0x1000000;i++)world[i]=0;
	initstd(world+0);
	//[4,7)
	body=world+0x400000;
	//[8,c)
	memory=world+0x800000;
	initmemory( memory );
	initmaster( memory );
	//[c,f)
	face=world+0xc00000;

	//444444444
	palette=malloc(0x400000);
	initwindow( palette );
	initpalette( palette );
}
__attribute__((destructor)) void cleanall()
{
	//4+4+4+4
	if(face != 0)
	{
		face=0;
	}
	if(memory != 0)
	{
		killmemory();
		memory=0;
	}
	if(body != 0)
	{
		body=0;
	}
	if(world != 0)
	{
		free(world);
		world=0;
	}

	//1024*1024*4
	if(palette != 0)
	{
		killwindow();
		palette=0;
	}
}
