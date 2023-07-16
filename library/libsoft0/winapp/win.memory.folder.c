#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include "const/def.h"
void say(char*,...);
void printmemory(char*,int);




int readfolder(char* url, int fd, p64 arg, int off, char* buf, int len)
{
	int j,ret;
	char tmp[0x100];
	HANDLE dir;
	WIN32_FIND_DATA dat;

	//slash to backslash
	for(j=0;j<0x100;j++){
		if(0xa >= url[j])break;
		if('/' == url[j])tmp[j] = '\\';
		else tmp[j] = url[j];
	}

	//show all
	snprintf(tmp+j, 8, "*.*");
	//printf("readfolder:%s\n", tmp);

	dir = FindFirstFileA(tmp, &dat);
	if(INVALID_HANDLE_VALUE == dir){
		printf("error@findfirstfile:%d\n", GetLastError());
		return 0;
	}

	j = 0;
	while(1){
		ret = ('.'==dat.cFileName[0]) && (('.'==dat.cFileName[1]) | (0==dat.cFileName[1]));
		if((!ret) && (dat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
			j += snprintf(buf+j, len-j, "%s/\n", dat.cFileName);
		}
		else{
			j += snprintf(buf+j, len-j, "%s\n", dat.cFileName);
		}
		ret = FindNextFile(dir, &dat);
		if(0 == ret)break;
	}

	FindClose(dir);
	return j;
}
int writefolder(void* url, int fd, p64 arg, int off, char* buf, int len)
{
	return 0;
}




/*
static char foldername[256];
static HANDLE folderbody=0;
static WIN32_FIND_DATA finddata;



void createfolder()
{
}
void deletefolder()
{
}
int startfolder(char* name)
{
	int ret;
	ret=snprintf(foldername,256,"%s\\*",name);
	folderbody=FindFirstFile(foldername,&finddata);

	if(folderbody==INVALID_HANDLE_VALUE)
	{
		//say("(startfolder error)findfirstfile\n");
		return 0;
	}

	//if( ! (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
	//{
	//	//say("not directory\n");
	//	return 0;
	//}

	return 1;
}
void stopfolder()
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
void choosefolder(char* name)
{
	stopfolder();
	startfolder(name);
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
*/