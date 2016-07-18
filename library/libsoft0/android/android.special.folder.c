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
static DIR* folderbody=0;
static struct dirent*  ent;




void killfolder()
{
}
void initfolder()
{
}
void stopfolder()
{
	if(folderbody!=0)
	{
		closedir(folderbody);
		folderbody=0;
	}
}
int startfolder(char* name)
{
	struct stat     statbuf;
	int             ret;
	stopfolder();

	ret=stat(name,&statbuf);
	if(ret==-1)
	{
		//say("(startfolder error)not exist\n");
		return -1;
	}

	if(!(statbuf.st_mode & S_IFDIR))
	{
		//say("(startfolder error)not folder\n");
		return -2;
	}

	if((statbuf.st_mode & S_IFCHR) != 0)
	{
		//say("(startfolder error)char device\n");
		return -3;
	}

	strncpy(foldername,name,256);
	folderbody=opendir(name);
	return 1;
}




void listfolder()
{
	if(folderbody==0)return;

	//
	rewinddir(folderbody);
	while(1)
	{
		ent=readdir(folderbody);
		if(ent==NULL)return;
		say("%s\n",ent->d_name);
	}
}
void choosefolder(char* name)
{
	int ret;
	if(name==0)
	{
		printf("@%s\n",foldername);
		return;
	}

	if(name[0]=='/')
	{
		snprintf(foldername,256,"%s",name);
	}
	else
	{
		ret=strlen(foldername);
		if(ret==1)ret=0;

		ret=snprintf(foldername+ret,256-ret,"/%s",name);
		printf("@%s\n",foldername);
	}

	stopfolder();
	startfolder(foldername);
}
void readfolder(char* dest)
{
	//next one
	ent=readdir(folderbody);
	if(ent==NULL)return;

	//get name
	snprintf(dest,256,"%s",ent->d_name);
}
void writefolder()
{
}
