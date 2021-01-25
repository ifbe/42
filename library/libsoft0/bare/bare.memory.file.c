#include "libhard.h"




static void* nodetable[16];
static void* foottable[16];
static int filecount = 0;
void filemanager_registersupplier(void* node, void* foot)
{
	say("@filemanager_registersupplier: [%d]=%p\n", filecount, node);
	nodetable[filecount] = node;
	foottable[filecount] = foot;
	filecount += 1;
}




int readfile(void* obj, int fd, void* arg, int off, u8* buf, int len)
{
	return 0;
}
int writefile(void* obj, int fd, void* arg, int off, u8* buf, int len)
{
	return 0;
}
int stopfile()
{
	return 0;
}
int startfile(char* path)
{
	return 0;
}




int filesearch(void* buf, int len)
{
	int j,ret;
	u8 tmp[512];
	struct item* p;
	void* q;
	say("@filesearch\n");

	for(j=0;j<filecount;j++){
		say("[%d]=%p\n", j, nodetable[j]);

		p = nodetable[j];
		if(p->ontaking){
			q = foottable[j];
			ret = p->ontaking(p,q, 0,0, "info",0, tmp,512);
			//say("ret=%d\n",ret);
		}
	}
	return 0;
}
int filemodify(void* buf, int len)
{
	return 0;
}
int deletefile()
{
	return 0;
}
int createfile()
{
	return 0;
}




void initfilemgr()
{
	say("@initfilemgr\n");
}
void freefilemgr()
{
}