#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
void say(char*,...);
void printmemory(char*,int);




//
static char foldername[256];
static HANDLE folderbody=0;
static WIN32_FIND_DATA finddata;



void initfolder()
{
}
void killfolder()
{
}
int openfolder(char* name)
{
	int ret;
	ret=snprintf(foldername,256,"%s\\*",name);
	folderbody=FindFirstFile(foldername,&finddata);

	if(folderbody==INVALID_HANDLE_VALUE)
	{
		say("findfirstfile error\n");
		return 0;
	}

	if( ! (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
	{
		say("not directory\n");
		return 0;
	}

	return 1;
}
void closefolder()
{
	if(folderbody!=0)
	{
		FindClose(folderbody);
		folderbody=0;
	}
}




void listfolder()
{
	int ret;
	if(folderbody==0)return;

	FindClose(folderbody);
	folderbody=FindFirstFile(foldername,&finddata);
	while(1)
	{
		//printmemory(finddata.cFileName,16);
		say("%s\n",finddata.cFileName);

		//
		ret=FindNextFile(folderbody,&finddata);
		if(ret==0)break;
	}
}
void switchfolder(char* name)
{
	closefolder();
	openfolder(name);
}
void readfolder(char* dest)
{
	//next one
	int ret=FindNextFile(folderbody,&finddata);
	if(ret==0)return;

	strncpy(dest,finddata.cFileName,256);
}
void writefolder()
{
}
