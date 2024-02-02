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
	logtoall("type=%llx\n", node->type);

	//1.remember
	ptblnode[ptblcount] = node;
	ptblfoot[ptblcount] = foot;
	ptblcount += 1;

	//2.get inner fs list
	if(0 == node->ontaking)return 0;

	ret = node->ontaking((void*)node,foot, 0,0, 0,_part_, tmp,0);
	logtoall("filemgr.ret=%x\n",ret);
	if(ret <= 0)return 0;

	//3.mount all i know
	_obj* fsys;
	struct relation* rel;
	for(j=0;j<ret;j++){
		logtoall("%d: %llx,%llx,%llx,%llx\n", j, tmp[j].type, tmp[j].name, tmp[j].start, tmp[j].count);
		switch(tmp[j].type){
		case _fat_:
		case _ntfs_:
		case _ext_:
		case _hfs_:
			fsys = artery_alloc_fromtype(tmp[j].type);
			if(0 == fsys)continue;
			artery_create(fsys, 0, 0, 0);

			rel = relationcreate(fsys,0,_art_,_src_, node,(void*)(tmp[j].start<<9),_art_,_dst_);
			if(0 == rel)continue;

			relationattach((void*)&rel->dst, (void*)&rel->src);

			filemanager_registerfsys(fsys, 0);
		}
	}
	return 0;
}
int filemanager_registerdisk(void* node, void* foot)
{
	logtoall("@filemanager_registerdisk: [%d]=%p\n", diskcount, node);

	//1.remember
	disknode[diskcount] = node;
	diskfoot[diskcount] = foot;
	diskcount += 1;

	//2.check

	//3.check what is in the disk
	_obj* tmp = artery_alloc_fromtype(_fileauto_);
	if(0 == tmp)return -1;
	artery_create(tmp, 0, 0, 0);

	struct relation* rel = relationcreate(tmp,0,_art_,_src_, node,foot,_dev_,_dst_);
	if(0 == rel)return -2;
	relationattach((void*)&rel->dst, (void*)&rel->src);

	//4.mount it
	switch(tmp->type){
	case _mbr_:
	case _gpt_:
		filemanager_registerpart(tmp, 0);
		break;
	case _fat_:
	case _ntfs_:
	case _ext_:
	case _hfs_:
		filemanager_registerfsys(tmp, 0);
		break;
	}
	return 0;
}




int file_reader(void* obj, int fd, p64 arg, int cmd, u8* buf, int len)
{
	_obj* node = fsysnode[0];
	if(0 == node)return 0;
	if(0 == node->ontaking)return 0;

	void* slot = fsysfoot[0];
	return node->ontaking((void*)node,slot, 0,0, (p64)obj,0, buf,len);
}
int file_writer(void* obj, int fd, p64 arg, int cmd, u8* buf, int len)
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




int file_take()
{
	return 0;
}
int file_give()
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




int file_search_disk()
{
	int j,ret;
	struct item* node;
	void* slot;
	logtoall("--------disk--------\n");

	for(j=0;j<diskcount;j++){
		logtoall("disk[%d]@%p:\n", j, disknode[j]);

		node = disknode[j];
		if(node->ontaking){
			slot = diskfoot[j];
			ret = node->ontaking(node,slot, 0,0, 0,_info_, 0,0);
			//logtoall("ret=%d\n",ret);
		}
		else{
			logtoall("there is no info\n");
		}
		logtoall("\n");
	}
	if(0 == j)logtoall("no disk\n\n");
}
int file_search_ptbl()
{
	int j,ret;
	struct item* node;
	void* slot;
	logtoall("--------ptbl--------\n");

	for(j=0;j<ptblcount;j++){
		logtoall("ptbl[%d]@%p:\n", j, ptblnode[j]);

		node = ptblnode[j];
		if(node->ontaking){
			slot = ptblfoot[j];
			ret = node->ontaking(node,slot, 0,0, 0,_info_, 0,0);
			//logtoall("ret=%d\n",ret);
		}
		else{
			logtoall("there is no info\n");
		}
		logtoall("\n");
	}
	if(0 == j)logtoall("no ptbl\n\n");
}
int file_search_fsys()
{
	int j,ret;
	struct item* node;
	void* slot;
	logtoall("--------fsys--------\n");

	for(j=0;j<fsyscount;j++){
		logtoall("fsys[%d]@%p:\n", j, fsysnode[j]);

		node = fsysnode[j];
		if(node->ontaking){
			slot = fsysfoot[j];
			ret = node->ontaking(node,slot, 0,0, 0,_info_, 0,0);
			//logtoall("ret=%d\n",ret);
		}
		else{
			logtoall("there is no info\n");
		}
		logtoall("\n");
	}
	if(0 == j)logtoall("no fsys\n\n");
}
int file_search_fsys_x(int id, u8* path)
{
	if(id < 0)return 0;
	if(id > fsyscount)return 0;

	struct item* node = fsysnode[id];
	void* slot = fsysfoot[id];
	if(node->ontaking){
		node->ontaking(node,slot, 0,0, (p64)path,_path_, 0,0);
	}
}
int file_search(u8* buf, int len)
{
	logtoall("@filesearch:buf=%p,len=%x\n", buf,len);
	if(buf){
		logtoall("arg={%s}\n", buf);
		if(0 == ncmp(buf, "/disk", 5)){
			file_search_disk();
		}
		else if(0 == ncmp(buf, "/ptbl", 5)){
			file_search_ptbl();
		}
		else if(0 == ncmp(buf, "/fsys", 5)){
			if('/' == buf[5]){
				file_search_fsys_x(buf[6]-'0', buf+7);
			}
			else{
				file_search_fsys();
			}
		}
	}
	else{
		file_search_disk();
		file_search_ptbl();
		file_search_fsys();
	}
	return 0;
}
int file_modify(void* buf, int len)
{
	return 0;
}




void initfilemgr()
{
	logtoall("@initfilemgr\n");
}
void freefilemgr()
{
}
