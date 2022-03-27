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
static void* ptblnode[16];
static void* ptblfoot[16];
static int ptblcount = 0;
//fsys
static void* fsysnode[16];
static void* fsysfoot[16];
static int fsyscount = 0;
int filemanager_registerfsys(_obj* node, void* foot)
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
int filemanager_registerpart(_obj* node, void* foot)
{
	int j,ret;
	struct parsed tmp[0x80];
	say("type=%llx\n", node->type);

	//1.remember
	ptblnode[ptblcount] = node;
	ptblfoot[ptblcount] = foot;
	ptblcount += 1;

	//2.find first fat16 or fat32
	if(0 == node->ontaking)return 0;

	ret = node->ontaking((void*)node,foot, 0,0, "p",0, tmp,0);
	say("filemgr.ret=%x\n",ret);
	if(ret <= 0)return 0;

	//3.contractor
	_obj* fsys;
	struct relation* rel;
	for(j=0;j<ret;j++){
		say("%d: %llx,%llx,%llx,%llx\n", j, tmp[j].type, tmp[j].name, tmp[j].start, tmp[j].count);
		if(_fat_ == tmp[j].type){
			fsys = artery_create(_fat_,0,0,0);
			if(0 == fsys)continue;

			rel = relationcreate(fsys,0,_art_,_src_, node,(void*)(tmp[j].start<<9),_art_,_dst_);
			if(0 == rel)continue;

			artery_attach((void*)&rel->dst, (void*)&rel->src);

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
	_obj* tmp = artery_create(_fileauto_,0,0,0);
	if(0 == tmp)return -1;
	struct relation* rel = relationcreate(tmp,0,_art_,_src_, node,foot,_dev_,_dst_);
	if(0 == rel)return -2;
	artery_attach((void*)&rel->dst, (void*)&rel->src);

	//4.next
	switch(tmp->type){
	case _mbr_:
	case _gpt_:
		filemanager_registerpart(tmp, 0);
		break;
	case _fat_:
		filemanager_registerfsys(tmp, 0);
		break;
	}
	return 0;
}




int file_take(void* obj, int fd, void* arg, int off, u8* buf, int len)
{
	_obj* p;
	//void* q;

	p = fsysnode[0];
	if(0 == p)return 0;
	if(0 == p->ontaking)return 0;

	//q = fsysfoot[0];
	return p->ontaking((void*)p,obj, 0,0, 0,0, buf,len);
}
int file_give(void* obj, int fd, void* arg, int off, u8* buf, int len)
{
	return 0;
}
int file_attach()
{
	return 0;
}
int file_detach()
{
	return 0;
}




int file_reader()
{
	return 0;
}
int file_writer()
{
	return 0;
}
int file_create(char* path)
{
	return 0;
}
int file_delete()
{
	return 0;
}




int file_search(void* buf, int len)
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
	if(0 == j)say("no disk\n");

	for(j=0;j<ptblcount;j++){
		say("ptbl[%d]@%p:\n", j, ptblnode[j]);

		p = ptblnode[j];
		if(p->ontaking){
			q = ptblfoot[j];
			ret = p->ontaking(p,q, 0,0, "info",0, tmp,512);
			//say("ret=%d\n",ret);
		}
		else{
			say("there is no info\n");
		}
		say("\n");
	}
	if(0 == j)say("no ptbl\n");

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
	if(0 == j)say("no fsys\n");

	return 0;
}
int file_modify(void* buf, int len)
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
