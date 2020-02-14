#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>




void readfolder(void* url, int fd, void* arg, int off, char* buf, int len)
{
	int j;
	DIR* dir;
	struct dirent* ent;

	dir = opendir(url);
	if(0 == dir)return;

	j = 0;
	while(1){
		ent = readdir(dir);
		if(0 == ent)break;

		if(ent->d_type == DT_LNK)break;
		j += snprintf(buf+j, len-j, "%s\n", ent->d_name);
		//printf("%s\n", ent->d_name);
	}

	closedir(dir);
}
void writefolder(void* url, int fd, void* arg, int off, char* buf, int len)
{
}
void listfolder()
{
}
void choosefolder()
{
}
void startfolder()
{
}
void stopfolder()
{
}
