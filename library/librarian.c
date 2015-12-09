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




//raw memory
static char* universe=0;	//16m+4k
static char* palette=0;		//4m
char* whereispalette()
{
	//unsigned int*
	return palette;
}




//processed memory
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
	QWORD i=0;
	char* wantfile=explainarg();

	//allocate memory
	//8m+4k		//强制4k对齐
	universe=malloc(2 * 0x400000 + 0x1000);
	if(universe==NULL)
	{
		printf("no enough momery\n");
		exit(-1);
	}

	//universe@[0x   0,0x801000)	->	world@[0x1000,0x801000)
	//universe@[0x 234,0x801234)	->	world@[0x1000,0x801000)
	//universe@[0x fff,0x801fff)	->	world@[0x1000,0x801000)
	//universe@[0x1000,0x802000)	->	world@[0x2000,0x802000)

	//[0,4)
	i = (QWORD)universe & 0xfff;
	world = universe + (0x1000 - i);

	for(i=0;i<0x800000;i++)world[i]=0;
	initstd(world+0);

	//[8,c)
	memory=world+0x400000;
	initmemory( memory );
	initmaster( memory );
}
void onlyface()
{
}
void initall()
{
	QWORD i=0;
	char* wantfile=explainarg();

	//allocate memory
	//16m+4m+4k	//强制4k对齐
	universe=malloc(5 * 0x400000 + 0x1000);
	if(universe==NULL)
	{
		printf("no enough momery\n");
		exit(-1);
	}

	//universe@[0x   0,0x1401000)	->	world@[0x1000,0x1401000)
	//universe@[0x 234,0x1401234)	->	world@[0x1000,0x1401000)
	//universe@[0x fff,0x1401fff)	->	world@[0x1000,0x1401000)
	//universe@[0x1000,0x1402000)	->	world@[0x2000,0x1402000)

	//[0,4)
	i = (QWORD)universe & 0xfff;
	world = universe + (0x1000 - i);

	for(i=0;i<0x1000000;i++)world[i]=0;
	initstd(world+0);

	//[4,7)
	body=world + (1*0x400000);

	//[8,c)
	memory=world + (2*0x400000);
	initmemory( memory );
	initmaster( memory );

	//[c,f)
	face=world + (3*0x400000);

	//444444444
	palette=world + (4*0x400000);
	initwindow( palette );
	initpalette( palette );
}
__attribute__((destructor)) void cleanall()
{
	//1024*1024*4
	if(palette != 0)
	{
		killwindow();
		palette=0;
	}

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
		world=0;
	}

	//
	if(universe !=0)
	{
		free(universe);
		universe=0;
	}
}
