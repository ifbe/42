#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include "libsoft.h"




int readfolder(void* url, int fd, p64 arg, int off, void* buf, int len)
{
	int j,k;
	DIR* dir;
	struct dirent* ent;

	dir = opendir(url);
	if(0 == dir)return 0;

	j = 0;
	while(1){
		ent = readdir(dir);
		if(0 == ent)break;

		if(DT_LNK == ent->d_type)break;

		k = ('.'==ent->d_name[0]) && (('.'==ent->d_name[1]) | (0==ent->d_name[1]));
		if((!k) && (DT_DIR == ent->d_type)){
			j += snprintf(buf+j, len-j, "%s/\n", ent->d_name);
		}
		else{
			j += snprintf(buf+j, len-j, "%s\n", ent->d_name);
		}
		//printf("%s\n", ent->d_name);
	}

	closedir(dir);
	return j;
}
int writefolder(void* url, int fd, p64 arg, int off, void* buf, int len)
{
	return 0;
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
