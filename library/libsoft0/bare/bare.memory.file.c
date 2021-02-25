#include "libhard.h"




struct parsed{
	u64 type;
	u64 name;
	u64 start;
	u64 count;
}__attribute__((packed));
//disk
static void* disknode[16];
static void* diskfoot[16];
static int diskcount = 0;
//part
static void* partnode[16];
static void* partfoot[16];
static int partcount = 0;
//fsys
static void* fsysnode[16];
static void* fsysfoot[16];
static int fsyscount = 0;
int filemanager_registerfsys(struct artery* node, void* foot)
{
	//1.remember
	fsysnode[fsyscount] = node;
	fsysfoot[fsyscount] = foot;
	fsyscount += 1;

	//2.??????
	//sleep_us(1000*1000*30);
	//if(0 == node->ontaking)return 0;
	//node->ontaking((void*)node,foot, 0,0, "",0, 0,0);
	return 0;
}
int filemanager_registerpart(struct artery* node, void* foot)
{
	int j,ret;
	struct parsed tmp[0x80];
	say("type=%llx\n", node->type);

	//1.remember
	partnode[partcount] = node;
	partfoot[partcount] = foot;
	partcount += 1;

	//2.find first fat16 or fat32
	if(0 == node->ontaking)return 0;

	ret = node->ontaking((void*)node,foot, 0,0, "p",0, tmp,0);
	say("filemgr.ret=%x\n",ret);
	if(ret <= 0)return 0;

	//3.contractor
	struct artery* fsys;
	struct relation* rel;
	for(j=0;j<ret;j++){
		say("%d: %llx,%llx,%llx,%llx\n", j, tmp[j].type, tmp[j].name, tmp[j].start, tmp[j].count);
		if(_fat_ == tmp[j].type){
			fsys = arterycreate(_fat_,0,0,0);
			if(0 == fsys)continue;

			rel = relationcreate(fsys,0,_art_,_src_, node,(void*)(tmp[j].start<<9),_art_,_dst_);
			if(0 == rel)continue;

			arterylinkup((void*)&rel->dst, (void*)&rel->src);

			filemanager_registerfsys(fsys, 0);
		}
	}
	return 0;
}
int filemanager_registersupplier(void* node, void* foot)
{
	say("@filemanager_registersupplier: [%d]=%p\n", diskcount, node);

	//1.remember
	disknode[diskcount] = node;
	diskfoot[diskcount] = foot;
	diskcount += 1;

	//2.check

	//3.contractor
	struct artery* tmp = arterycreate(_fileauto_,0,0,0);
	if(0 == tmp)return -1;
	struct relation* rel = relationcreate(tmp,0,_art_,_src_, node,foot,_dev_,_dst_);
	if(0 == rel)return -2;
	arterylinkup((void*)&rel->dst, (void*)&rel->src);

	//4.next
	if( (_mbr_ == tmp->type) | (_gpt_ == tmp->type) ){
		filemanager_registerpart(tmp, 0);
	}
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

	for(j=0;j<diskcount;j++){
		say("disk[%d]@%p:\n", j, disknode[j]);

		p = disknode[j];
		if(p->ontaking){
			q = diskfoot[j];
			ret = p->ontaking(p,q, 0,0, "info",0, tmp,512);
			//say("ret=%d\n",ret);
		}
		else{
			say("there is no info\n");
		}
		say("\n");
	}
	for(j=0;j<partcount;j++){
		say("part[%d]@%p:\n", j, partnode[j]);

		p = partnode[j];
		if(p->ontaking){
			q = partfoot[j];
			ret = p->ontaking(p,q, 0,0, "info",0, tmp,512);
			//say("ret=%d\n",ret);
		}
		else{
			say("there is no info\n");
		}
		say("\n");
	}
	for(j=0;j<fsyscount;j++){
		say("fsys[%d]@%p:\n", j, fsysnode[j]);

		p = fsysnode[j];
		if(p->ontaking){
			q = fsysfoot[j];
			ret = p->ontaking(p,q, 0,0, "info",0, tmp,512);
			//say("ret=%d\n",ret);
		}
		else{
			say("there is no info\n");
		}
		say("\n");
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