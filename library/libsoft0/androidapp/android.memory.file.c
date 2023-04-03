#include <stdio.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "libsoft.h"
void* getassetmgr();



struct item* obj;
static AAssetManager* assetMgr;
static AAsset* asset[16] = {0};




void initfilemgr(void* addr)
{
	say("@initfilemgr\n");
	obj = addr;
	assetMgr = getassetmgr();

	AAssetDir* assetDir=AAssetManager_openDir(assetMgr, "");
	const char* file_list;
	do{
		file_list=AAssetDir_getNextFileName(assetDir);
		if(file_list)say("%s\n",file_list);
	}while (file_list);
	AAssetDir_close(assetDir);
}
void freefilemgr()
{
}
void filemanager_registersupplier()
{
}




_obj* file_create(void* path, int flag)
{
	say("@startfile:%s\n",path);

	int j;
	for(j=0;j<16;j++){
		if(0 == asset[j]){
			say("@AAssetManager_open:%s\n",path);
			asset[j] = AAssetManager_open(assetMgr, path, 0);
			if(asset[j])goto good;
			break;
		}
	}
	return 0;

good:
	obj[j].fileinfo.fd = j;
	return &obj[j];
}
int file_delete(_obj* oo)
{
	int fd = oo->fileinfo.fd;
	if(fd < 0)return -1;
	if(fd >= 16)return -2;
	if(0 == asset[fd])return -3;

	AAsset_close(asset[fd]);
	asset[fd] = 0;
	return 0;
}
int file_search()
{
	say("@filesearch\n");
	return 0;
}
int file_modify()
{
	return 0;
}




int file_take(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	int fd = oo->fileinfo.fd;
	return AAsset_read(asset[fd], buf, len);
}
int file_give(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	return 0;
}





int readfolder(void* url, int fd, void* arg, int off, void* buf, int len)
{
	int j=0;
	AAssetDir* assetDir=AAssetManager_openDir(assetMgr, url);
	const char* file_list;
	do{
		file_list=AAssetDir_getNextFileName(assetDir);
		if(file_list){
			//say("%s\n",file_list);
			j += snprintf(buf+j, len-j, "%s/\n", file_list);
		}
	}while (file_list);
	AAssetDir_close(assetDir);
	return j;
}