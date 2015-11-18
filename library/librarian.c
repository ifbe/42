#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
char* explainarg();
void initmemory(char*);
void initmaster(char*);
void initwindow(char*);
void initpalette(char*);




static char*   world=0;
static char*  memory=0;
static char* palette=0;
char* whereismemory()	//8m
{
	return memory;
}
char* whereispalette()	//4m?16m?
{
	return palette;
}




//省的写一大串初始化代码，这里是几种常见初始化过程
void onlyface()		//4m
{
	//111111
	char* wantfile=explainarg();

	//222222
	world=malloc(0x400000);
	if(world==NULL){printf("no enough momery\n");exit(-1);}

	//33333
	palette=world;
	initwindow( palette );
	initpalette( palette );
}
void onlymemory()		//8m
{
	int i=0;

	//1111111
	char* wantfile=explainarg();

	//2222222
	world=malloc(0x800000);
	if(world==NULL){printf("no enough momery\n");exit(-1);}

	//333333333
	memory=world;
	for(i=0;i<0x800000;i++)memory[i]=0;
	initmemory( memory );
	initmaster( memory );
}
void memoryandface()		//8+4m
{
	int i=0;

	//111111111
	char* wantfile=explainarg();

	//22222222
	world=malloc(0xc00000);
	if(world==NULL){printf("no enough momery\n");exit(-1);}

	//333333333
	memory=world;
	for(i=0;i<0x800000;i++)memory[i]=0;
	initmemory( memory );
	initmaster( memory );

	//444444444
	palette=world+0x800000;		//[+0x800000,+0xbfffff]
	initwindow( palette );
	initpalette( palette );
}
/*
void bondandfleshandface()	//8+8+16m
{
	world=malloc(0x2000000);
	if(world==NULL){printf("no enough memory\n");exit(-1);}

	hardware=world;
	for(i=0x0;i<0x800000;i++)hardware[i]=0;
	initcpu();
	initdevice();

	memory=world+0x800000;
	for(i=0x0;i<0x800000;i++)memory[i]=0;
	initmemory( memory );
	initmaster( memory );

	palette=world+0x1000000;
	initwindow( palette );
	initpalette( palette );
}
*/
__attribute__((destructor)) void cleanall()
{
	if(world != 0)
	{
		free(world);
		world=0;
	}
}
