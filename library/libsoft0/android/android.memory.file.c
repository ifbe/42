#include <android/log.h>
#include <android_native_app_glue.h>
void say(char* fmt,...);
void* getassetmgr();



static AAssetManager* assetMgr;
static AAsset* asset[16] = {0};




int readfile(void* obj, int fd, void* arg, int off, char* buf, int len)
{
	return AAsset_read(asset[fd], buf, len);
}
int writefile(void* obj, int fd, void* arg, int off, char* buf, int len)
{
	return 0;
}
int stopfile(int fd)
{
	if(fd < 0)return -1;
	if(fd >= 16)return -2;
	if(0 == asset[fd])return -3;

	AAsset_close(asset[fd]);
	asset[fd] = 0;
	return 0;
}
int startfile(char* path, int flag)
{
	say("@startfile:%s\n",path);

	int j;
	for(j=0;j<16;j++){
		if(0 == asset[j]){
			say("@AAssetManager_open:%s\n",path+9);
			asset[j] = AAssetManager_open(assetMgr, path+9, 0);
			if(asset[j])return j;
			break;
		}
	}
	return -1;
}




int filesearch(void* buf, int len)
{
	say("@filesearch\n");
	return 0;
}
int filemodify(void* buf, int len)
{
	return 0;
}
void deletefile()
{
}
void createfile()
{
}




void filemanager_registersupplier()
{
}
void initfilemgr()
{
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
