#include "libsoft.h"
//void osfile_list(void*);
int hexstr2u64(void* str, u64* data);




//raw data
struct _perdisk{
	_obj* node;
	void* slot;
}perdisk[16];
static int diskcount = 0;

//virt disk
struct _pervd{
	_obj* node;
	void* slot;
}pervdisk[16];
static int vdcount = 0;

//part table
struct _perptbl{
	_obj* node;
	void* slot;
}perptbl[16];
static int ptblcount = 0;

//file system
struct _perfsys{
	_obj* node;
	void* slot;
}perfsys[16];
static int fsyscount = 0;




int filemanager_registerfsys(_obj* node, void* slot)
{
	perfsys[fsyscount].node = node;
	perfsys[fsyscount].slot = slot;
	fsyscount += 1;

	return 0;
}
int filemanager_registerptbl(_obj* node, void* slot)
{
	perptbl[ptblcount].node = node;
	perptbl[ptblcount].slot = slot;
	ptblcount += 1;

	return 0;
}
int filemanager_registervdisk(_obj* node, void* slot)
{
	pervdisk[vdcount].node = node;
	pervdisk[vdcount].slot = slot;
	vdcount += 1;

	return 0;
}
int filemanager_registerdisk(_obj* node, void* slot)
{
	perdisk[diskcount].node = node;
	perdisk[diskcount].slot = slot;
	diskcount += 1;

	return 0;
}


/*
struct parsedfolder{
}__attribute__((packed));
*/
int file_mount_fsys(_obj* fsys, u64 ff){
	//1.check if file exist

	//2.try to expand it
	/*
	switch(tmp[j].type){
	case vdisk:
		break;
	default:
		mount_raw
	}
	*/
	return 0;
}

struct parsedptbl{
	u64 type;
	u64 name;
	u64 start;
	u64 count;
}__attribute__((packed));
int file_mount_part(_obj* ptbl, u64 pf){

	//1.get metadata from it
	if(0 == ptbl->ontaking)return 0;

	int ret = ptbl->ontaking((void*)ptbl,0, 0,0, 0,_info_, 0,0);

	struct parsedptbl tmp[0x80];
	ret = ptbl->ontaking((void*)ptbl,0, 0,0, 0,_part_, tmp,0);
	logtoall("partition count=%x, pf=%p\n", ret, pf);
	if(ret <= 0)return 0;

	//2.try to expand it
	int j;
	_obj* fsys;
	struct relation* rel;
	for(j=0;j<ret;j++){
		if(tmp[j].start != pf)continue;
		switch(tmp[j].type){
		case _fat_:
		case _ntfs_:
		case _ext_:
		case _hfs_:
			fsys = artery_alloc_fromtype(tmp[j].type);
			if(0 == fsys)continue;
			artery_create(fsys, 0, 0, 0);

			rel = relationcreate(fsys,0,_art_,_src_, ptbl,(void*)(tmp[j].start<<9),_art_,_dst_);
			if(0 == rel)continue;

			relationattach((void*)&rel->dst, (void*)&rel->src);

			fsys->onreader(fsys, 0, (p64)"/",0, 0, 0);

			//file_mount_fsys(fsys, 0);
			filemanager_registerfsys(fsys, 0);
		}
	}
	return 0;
}

int file_mount_vdisk(_obj* vdisk, u64 vf){
	//1.get metadata from it

	//2.try to expand it
	return 0;
}

int file_mount_raw(_obj* disk, void* df){
	//1.get metadata from it

	//2.try to expand it
	_obj* tmp = artery_alloc_fromtype(_fileauto_);
	if(0 == tmp)return -1;
	artery_create(tmp, 0, 0, 0);

	struct relation* rel = relationcreate(tmp,0,_art_,_src_, disk,df,_dev_,_dst_);
	if(0 == rel)return -2;
	relationattach((void*)&rel->dst, (void*)&rel->src);

	switch(tmp->type){
	case _vhd_:
	case _vmdk_:
		filemanager_registervdisk(tmp, 0);
		break;
	case _mbr_:
	case _gpt_:
		filemanager_registerptbl(tmp, 0);
		break;
	case _fat_:
	case _ntfs_:
	case _ext_:
	case _hfs_:
		filemanager_registerfsys(tmp, 0);
		break;
	}
	logtoall("type=%.8s\n", &tmp->type);
	return 0;
}


int parse_type(u8* input, int end, u64* type)
{
	if(end>=8)end = 8;

	u8* ptr = (u8*)type;

	int j;
	for(j=0;j<end;j++){
		ptr[j] = input[j];
	}
	for(;j<8;j++)ptr[j] = 0;
	return 0;
}
int parse_type_and_addr(u8* input, u64* type, u64* addr)
{
	int j,at=-1;
	for(j=0;j<64;j++){
		if(input[j] <= 0x20)break;
		if( ('@' == input[j]) | ('.' == input[j]) ){
			at = j;
			break;
		}
	}

	if(at<0){
		*type = 0;
		return 0;
	}
	//logtoall("part2:%s\n", input+at+1);

	*type = 0;
	parse_type(input, at, type);

	*addr = 0;
	hexstr2u64(input+at+1, addr);
	return 0;
}




#define _disk_ hex32('d','i','s','k')
#define _vdisk_ hex64('v','d','i','s','k', 0, 0, 0)
#define _ptbl_ hex32('p','t','b','l')
#define _fsys_ hex32('f','s','y','s')
int filelist(u8* path)
{
	int j;
	u64 type=0, index=0xffff;
	if(path){
		parse_type_and_addr(path, &type, &index);
	}

	if( (0 == type) || (_disk_ == type) || (_raw_ == type) ){
		logtoall("----raw----\n");
		if(diskcount){
			if(index < 0xff && index < diskcount){
				_obj* node = perdisk[index].node;
				void* slot = perdisk[index].slot;
				if(node && node->ontaking){
					int ret = node->ontaking(node, slot, 0,0, 0,_info_, 0,0);
				}
			}
			else{
				for(j=0;j<diskcount;j++){
					logtoall("%d: node=%p,slot=%p\n", j, perdisk[j].node, perdisk[j].slot);
				}
			}
		}
		else{
			logtoall("no disk registered\n");
		}
		logtoall("\n");
	}

	if( (0 == type) || (_vdisk_ == type) ){
		logtoall("----vdisk----\n");
		if(vdcount){
			if(index < 0xff && index < vdcount){
				_obj* node = pervdisk[index].node;
				void* slot = pervdisk[index].slot;
				if(node && node->ontaking){
					int ret = node->ontaking(node, slot, 0,0, 0,_info_, 0,0);
				}
			}
			else{
				for(j=0;j<vdcount;j++){
					logtoall("%d: node=%p,slot=%p\n", j, pervdisk[j].node, pervdisk[j].slot);
				}
			}
		}
		else{
			logtoall("no vdisk registered\n");
		}
		logtoall("\n");
	}

	if( (0 == type) || (_ptbl_ == type) ){
		logtoall("----ptbl----\n");
		if(ptblcount){
			if(index < 0xff && index < vdcount){
				_obj* node = perptbl[index].node;
				void* slot = perptbl[index].slot;
				if(node && node->ontaking){
					int ret = node->ontaking(node, slot, 0,0, 0,_info_, 0,0);
				}
			}
			else{
				for(j=0;j<ptblcount;j++){
					logtoall("%d: node=%p,slot=%p\n", j, perptbl[j].node, perptbl[j].slot);
				}
			}
		}
		else{
			logtoall("no ptbl registered\n");
		}
		logtoall("\n");
	}

	if( (0 == type) || (_fsys_ == type) ){
		logtoall("----fsys----\n");
		if(fsyscount){
			if(index < 0xff && index < fsyscount){
				_obj* node = perfsys[index].node;
				void* slot = perfsys[index].slot;
				if(node && node->ontaking){
					int ret = node->ontaking(node, slot, 0,0, 0,_info_, 0,0);
				}
			}
			else{
				for(j=0;j<fsyscount;j++){
					logtoall("%d: node=%p,slot=%p\n", j, perfsys[j].node, perfsys[j].slot);
				}
			}
		}
		else{
			logtoall("no fsys registered\n");
		}
		logtoall("\n");
	}
	return 0;
}


//file probe "/dev/sda"
//file probe "\\.\physicaldrive0"
int fileprobe(u8* path)
{
	_obj* f = system_alloc_frompath(_file_, path);
	system_create(f, 0, 0, 0);

	filemanager_registerdisk(f, 0);
	return 0;
}


//file mount disk@111 part@222
int filemount(u8* path, u8* slot)
{
	u64 type=0, index=0;
	parse_type_and_addr(path, &type, &index);
	logtoall("type=%.8s, index=%llx\n", &type, index);
/*
	//mount disk@7fff00 part@188000
	u64 type2=0, addr2=0;
	if(slot)parse_type_and_addr(slot, &type2, &addr2);
	if(type2)logtoall("type2=%.8s, addr2=%p\n", &type2, addr2);
*/
	//mount disk@434140
	if( (_disk_ == type) | (_raw_ == type) ){
		if(index < diskcount)file_mount_raw(perdisk[index].node, perdisk[index].slot);
	}

	//mount vdisk@434140
	else if(_vdisk_ == type){
		if(index < vdcount)file_mount_raw(pervdisk[index].node, pervdisk[index].slot);
	}

	//mount ptbl@fff3450.part@0
	else if(_ptbl_ == type){
		if(index < ptblcount)file_mount_part(perptbl[index].node, (u64)perptbl[index].slot);
	}

	//mount fsys#3.inode#875
	else if(_fsys_ == type){
		if(index < fsyscount)file_mount_fsys(perfsys[index].node, (u64)perfsys[index].slot);
	}

	return 0;
}

int filecommand(int argc, u8** argv)
{
	//logtoall("@filecommand: argc=%d\n", argc);
	if(argc > 1){
		if(0 == ncmp(argv[1], "list", 4)){
			filelist((argc>2) ? argv[2] : 0);
			return 0;
		}
		else if(0 == ncmp(argv[1], "probe", 5)){
			fileprobe(argv[2]);
			return 0;
		}
		else if(0 == ncmp(argv[1], "mount", 5)){
			filemount(argv[2], argv[3]);
			return 0;
		}
	}

	logtoall("example:\n");
	logtoall("file list\n");
	logtoall("file probe /dev/sda\n");
	logtoall("file mount disk@111 part@222\n");
	return 0;
}
