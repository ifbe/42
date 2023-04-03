#include <stdio.h>
#include <jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "libsoft.h"
void* getapp();
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
	//non-dir
	int olen=0;
	AAssetDir* assetDir=AAssetManager_openDir(assetMgr, url);
	const char* file_list;
	do{
		file_list=AAssetDir_getNextFileName(assetDir);
		if(file_list){
			//say("%s\n",file_list);
			olen += snprintf(buf+olen, len-olen, "%s\n", file_list);
		}
	}while (file_list);
	AAssetDir_close(assetDir);


	//dir and non-dir
	say("@readfolder\n");
	struct android_app* app = getapp();

	say("@AttachCurrentThread\n");
    JNIEnv* env = 0;
    (*app->activity->vm)->AttachCurrentThread(app->activity->vm, &env, 0);

	say("@activity\n");
    jclass activity_class = (*env)->GetObjectClass(env, app->activity->clazz);
	jmethodID getAssets_method = (*env)->GetMethodID(env, activity_class, "getAssets", "()Landroid/content/res/AssetManager;");

	say("@assetmanager\n");
	void* assetmanager_obj = (*env)->CallObjectMethod(env, app->activity->clazz, getAssets_method);
	say("@assetmanager.1\n");
	jclass assetmanager_class = (*env)->GetObjectClass(env, assetmanager_obj);
	say("@assetmanager.2\n");
    jmethodID list_method = (*env)->GetMethodID(env, assetmanager_class, "list", "(Ljava/lang/String;)[Ljava/lang/String;");

	say("@path\n");
	jstring path_obj = (*env)->NewStringUTF(env, url);
	say("@path.1\n");
	void* file_obj = (jobjectArray)(*env)->CallObjectMethod(env, assetmanager_obj, list_method, path_obj);
	say("@path.2\n");
	(*env)->DeleteLocalRef(env, path_obj);

	say("@GetArrayLength\n");
	int cnt = (*env)->GetArrayLength(env, file_obj);
	say("cnt=%d\n", cnt);

	for(int k=0;k<cnt;k++){
		jstring jstr = (jstring)(*env)->GetObjectArrayElement(env, file_obj, k);
		const char* name = (*env)->GetStringUTFChars(env, jstr, 0);
		if(name){
			say("%d:%s\n", k, name);
			olen += snprintf(buf+olen, len-olen, "%s/\n", name);
		}
		(*env)->DeleteLocalRef(env, jstr);
	}

	say("@DetachCurrentThread\n");
    (*app->activity->vm)->DetachCurrentThread(app->activity->vm);
	return olen;
}