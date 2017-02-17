#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
void say(char*,...);




//
static char foldername[0x1000];
//
static DIR* folderbody=0;
static struct dirent*  ent;
//
struct stack
{
	DIR* folder;
	char* tail;
};
static struct stack stack[32];		//16 is not enough
//
static char path[0x1000];		//some very long
//
static struct stat statbuf;
//
static int rsp=0;




char* traverse_read()
{
	struct dirent* ent;

	while(1)
	{
		//empty name
		if(stack[rsp].tail == 0)
		{
			//empty stack
			if(rsp == 0)
			{
				return 0;
			}

			//stack pop, new name
			else
			{
				rsp--;
				stack[rsp].tail[0]=0;
				continue;
			}
		}

		//have name, not opened
		if(stack[rsp].folder == 0)
		{
			//try to open dir
			if(path[0]==0)stack[rsp].folder = opendir("/");
			else stack[rsp].folder = opendir(path);

			//opened successfully
			if(stack[rsp].folder != 0)continue;

			//can not open, it is leaf !!!
			else
			{
				stack[rsp].tail=0;
				return path;
			}
		}

		//folder opened, take one
		ent=readdir(stack[rsp].folder);

		//nothing left in this folder
		if(ent == 0)
		{
			closedir(stack[rsp].folder);
			stack[rsp].tail=0;

			continue;
		}

		//delete nondir
		if(ent->d_type == DT_LNK)
		{
			continue;
		}

		//ignore . .. .*
		if(ent->d_name[0] == '.')
		{
			continue;
		}

		//push
		stack[rsp+1].folder = 0;
		stack[rsp+1].tail = stack[rsp].tail + snprintf(
			stack[rsp].tail,
			1024,
			"/%s",
			ent->d_name
		);
		rsp++;
	}
}
void traverse_write(char* p)
{
	int j;
	if(p[0]==0)return;

	//clear everything
	while(1)
	{
		if(rsp==0)break;

		if(stack[rsp].folder != 0)closedir(stack[rsp].folder);
		if(stack[rsp].tail !=0)stack[rsp].tail = 0;
		rsp--;
	}

	//convert "/some/dir/" to "/some/dir"
	strncpy(path , p , 1024);
	j=strlen(p);
	if(p[j-1]=='/')
	{
		path[j-1]=0;
		stack[0].folder=0;
		stack[0].tail=path+j-1;
	}
	else
	{
		stack[0].folder=0;
		stack[0].tail=path+j;
	}
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
	int ret;
	struct stat statbuf;
	static DIR* temp;

	//check folder
	ret=stat(name,&statbuf);
	if(ret==-1)
	{
		//say("(startfolder error)not exist\n");
		ret = -1;
		goto byebye;
	}
	if(!(statbuf.st_mode & S_IFDIR))
	{
		//say("(startfolder error)not folder\n");
		ret = -2;
		goto byebye;
	}
	if((statbuf.st_mode & S_IFCHR) != 0)
	{
		//say("(startfolder error)char device\n");
		ret = -3;
		goto byebye;
	}

	//try open
	temp=opendir(name);
	if(temp <= 0)
	{
		ret = -4;
		goto byebye;
	}

	//real open
	strncpy(foldername,name,0x1000);
	stopfolder();
	folderbody=temp;
	ret=1;

byebye:
	if(ret<0)say("can't open:%s\n",name);
	return ret;
}
void deletefolder()
{
}
void createfolder()
{
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
	int haha;
	char try[0x1000];

	if(name==0)
	{
		printf("%s\n",foldername);
		return;
	}

	if(name[0]=='/')
	{
		startfolder(name);
		return;
	}
	else if(name[0]=='.')
	{
		if(name[1]==0)return;

		if(name[1]=='.')
		{
			if(foldername[1]==0)return;

			ret=0;
			haha=0;
			while(1)
			{
				if(foldername[ret]==0)break;

				if(foldername[ret]=='/')
				{
					haha=ret;
				}

				ret++;
			}

			strncpy(try,foldername,0x1000);
			if(haha == 0)try[1]=0;
			else try[haha]=0;

			startfolder(try);
			return;
		}
	}

	//default,child node
	strncpy(try,foldername,0x1000);
	ret=strlen(try);
	if(ret==1)ret=0;

	snprintf(try+ret,256-ret,"/%s",name);
	startfolder(try);
}
void readfolder(char* dest)
{
	//next one
	ent=readdir(folderbody);
	if(ent==NULL)dest[0]=0;
	else snprintf(dest,256,"%s",ent->d_name);
}
void writefolder()
{
}
