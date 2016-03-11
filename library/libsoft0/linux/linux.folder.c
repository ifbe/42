#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
void say(char*,...);
static char foldername[256];
static DIR* folderbody;
static struct dirent*  ent;




void openfolder(char* name)
{
	struct stat     statbuf;
	int             ret;

	ret=stat(name,&statbuf);
	if(ret==-1)
	{
		say("not exist\n");
		return;
	}

	if(!(statbuf.st_mode & S_IFDIR))
	{
		say("not folder\n");
		return;
	}

	strncpy(foldername,name,256);
	folderbody=opendir(name);
}
void closefolder()
{
	closedir(folderbody);
}




void listfolder()
{
	rewinddir(folderbody);
	while(1)
	{
		ent=readdir(folderbody);
		if(ent==NULL)return;
		say("%s/%s\n",foldername,ent->d_name);
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
	ent=readdir(folderbody);
	if(ent==NULL)return;

	//get name
	snprintf(dest,256,"%s/%s",foldername,ent->d_name);
}
void writefolder()
{
}
