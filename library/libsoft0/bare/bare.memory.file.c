#include "libhard.h"




static void* nodetable[16];
static void* foottable[16];
static int filecount = 0;
int filemanager_registersupplier(void* node, void* foot)
{
	say("@filemanager_registersupplier: [%d]=%p\n", filecount, node);
	nodetable[filecount] = node;
	foottable[filecount] = foot;
	filecount += 1;

	struct artery* tmp = arterycreate(_fileauto_,0,0,0);
	if(0 == tmp)return -1;
	struct relation* rel = relationcreate(tmp,0,_art_,_src_, node,foot,_dev_,_dst_);
	if(0 == rel)return -2;
	arterylinkup((void*)&rel->dst, (void*)&rel->src);

	//say("type=%llx\n", tmp->type);
	nodetable[filecount] = tmp;
	foottable[filecount] = 0;
	filecount += 1;
	return 0;
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
		say("[%d]=%p{\n", j, nodetable[j]);

		p = nodetable[j];
		if(p->ontaking){
			q = foottable[j];
			ret = p->ontaking(p,q, 0,0, "info",0, tmp,512);
			//say("ret=%d\n",ret);
		}
		else{
			say("there is no info\n");
		}
		say("}[%d]\n\n",j);
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