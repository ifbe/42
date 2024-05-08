#include "libsoft.h"


struct SparseExtentHeader  
{  
	u32 magicNumber; // 0x00 - 0x03  
	u32 version; // 0x04 - 0x07  
	u32 flags; // 0x08 - 0x0B  
	u64 capacity; // 0x0C - 0x13  
	u64 grainSize; // 0x14 - 0x1B  
	u64 descriptorOffset; // 0x1C - 0x23  
	u64 descriptorSize; // 0x24 - 0x2B  
	u32 numGTEsPerGT; // 0x2C - 0x2F  
	u64 rgdOffset; // 0x30 - 0x37  
	u64 gdOffset; // 0x38 - 0x3F  
	u64 overHead; // 0x40 - 0x47  
	char uncleanShutdown; // 0x48  
	char singleEndLineChar; // 0x49 - 0x4C  
	char nonEndLineChar; // 0x49  
	char doubleEndLineChar1; // 0x4A  
	char doubleEndLineChar2; // 0x4B  
	u16 compressAlgorithm; // 0x4C  
	u8 pad[433];
}__attribute__((packed));

#define cacheblocksize_gd 0x10000
#define cacheblocksize_gt 0x10000
struct pervd{
	u8 cache_gd[cacheblocksize_gd];
	u8 cache_gt[cacheblocksize_gt];

	struct SparseExtentHeader h;

	u64 cachepos_gd;
	u64 cachepos_gt;
}__attribute__((packed));


int check_vmdk(u8* addr)
{
	if('V' != addr[3])return 0;
	if('M' != addr[2])return 0;
	if('D' != addr[1])return 0;
	if('K' != addr[0])return 0;

	return _vmdk_;
}
int parse_vmdk(struct SparseExtentHeader* h)
{
	printmemory(h, 0x100);
	logtoall(
		"magicNumber=%x\n"
		"version=%x\n"
		"flags=%x\n",
		h->magicNumber, h->version, h->flags);
	logtoall(
		"capacity = 0x%llx sector = 0x%llx MB\n"
		"grainSize = 0x%llx sector = 0x%llx byte\n",
		h->capacity, (h->capacity<<9)>>20,
		h->grainSize, h->grainSize<<9);
	logtoall(
		"descriptorOffset = 0x%llx sector = 0x%llx byte\n"
		"descriptorSize = 0x%llx sector = 0x%llx byte\n",
		h->descriptorOffset, h->descriptorOffset<<9,
		h->descriptorSize, h->descriptorSize<<9);
	logtoall(
		"numGTEsPerGT=%x\n"
		"rgdOffset = 0x%llx sector = 0x%llx byte\n"
		"gdOffset = 0x%llx sector = 0x%llx byte\n"
		"overHead = 0x%llx sector = 0x%llx byte\n",
		h->numGTEsPerGT,
		h->rgdOffset, h->rgdOffset<<9,
		h->gdOffset, h->gdOffset<<9,
		h->overHead, h->overHead<<9);
	logtoall(
		"uncleanShutdown=%x\n"
		"singleEndLineChar=%x\n"
		"nonEndLineChar=%x\n"
		"doubleEndLineChar1=%x\n"
		"doubleEndLineChar2=%x\n"
		"compressAlgorithm=%x\n",
		h->uncleanShutdown, h->singleEndLineChar, h->nonEndLineChar, h->doubleEndLineChar1, h->doubleEndLineChar2, h->compressAlgorithm
	);
	return 0;
}


static void* ensure_gd_in_cache(_obj* art, u64 id)
{
	u64 cacheblock_id = (id*4)/cacheblocksize_gd;
	u64 offs_in_gd = cacheblock_id * cacheblocksize_gd;

	struct pervd* vd = art->priv_ptr;
	if(vd->cachepos_gd != offs_in_gd){
		struct SparseExtentHeader* h = &vd->h;
		u64 toreadpos = offs_in_gd + (h->gdOffset<<9);
		int ret = take_data_from_peer(art,_src_, 0,0, toreadpos,_pos_, vd->cache_gd, cacheblocksize_gd);
		vd->cachepos_gd = offs_in_gd;
	}
	return vd->cache_gd + (id*4)%cacheblocksize_gd;
}
static void* ensure_gt_in_cache(_obj* art, u64 gt, u64 id)
{
	struct pervd* vd = art->priv_ptr;
	if(gt != vd->cachepos_gt){
		struct SparseExtentHeader* h = &vd->h;
		if(h->numGTEsPerGT*4 > cacheblocksize_gd){
			logtoall("error: bytepergt> cacheblocksize\n");
		}
		u64 toreadpos = gt<<9;
		int ret = take_data_from_peer(art,_src_, 0,0, toreadpos,_pos_, vd->cache_gt, h->numGTEsPerGT*4);
		vd->cachepos_gt = gt;
	}
	return vd->cache_gt + id*4;
}


static int vmdk_addr_to_grain(_obj* art, u64 addr, u64* grain)
{
	struct pervd* vd = art->priv_ptr;
	struct SparseExtentHeader* h = &vd->h;

	u64 logicsector = addr>>9;
	*grain = logicsector / h->grainSize;
	return 0;
}
static int vmdk_grain_to_fileoffs(_obj* art, u64 grain, u64* fileoffs)
{
	struct pervd* vd = art->priv_ptr;
	struct SparseExtentHeader* h = &vd->h;
	u64 id_in_gd = grain / h->numGTEsPerGT;
	u64 id_in_gt = grain % h->numGTEsPerGT;
	logtoall("id_in_gd=%llx, id_in_gt=%llx\n", id_in_gd, id_in_gt);

	//1.read gd, find gt sector
	u32* gd_entry = ensure_gd_in_cache(art, id_in_gd);
	u64 gt = *gd_entry;

	//2.read gt, find grain sector
	u32* gt_entry = ensure_gt_in_cache(art, gt, id_in_gt);
	*fileoffs = (*gt_entry) << 9;
	return 0;
}
static int vmdk_readonegrain(_obj* art, u64 addr, u8* buf, int len){
	u64 grain = 0;
	vmdk_addr_to_grain(art, addr, &grain);
	logtoall("grain=%llx\n", grain);

	u64 fileoffs = 0;
	vmdk_grain_to_fileoffs(art, grain, &fileoffs);
	logtoall("grain=%llx\n", fileoffs);

	struct pervd* vd = art->priv_ptr;
	struct SparseExtentHeader* h = &vd->h;
	u64 bytepergrain = h->grainSize<<9;
	u64 offs_in_grain = addr % bytepergrain;
	if(len > bytepergrain-offs_in_grain)len = bytepergrain-offs_in_grain;
	logtoall("offs_in_grain=%llx, lenafter=%x\n", offs_in_grain, len);
	int ret = take_data_from_peer(art,_src_, 0,0, fileoffs+offs_in_grain,_pos_, buf, len);
	return ret;
}


static int vmdk_ontake(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int cmd, u8* buf, int len)
{
	logtoall("@vmdk_ontake:obj=%p,slot=%p,arg=%llx,cmd=%x,buf=%p,len=%x\n", art,foot, arg,cmd, buf,len);
	vmdk_readonegrain(art, arg, buf, len);
	return 0;
}
static int vmdk_ongive(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int vmdk_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vmdk_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@vmdk_attach:%x\n",self->foottype);
	_obj* ele = self->pchip;
	if(0 == ele)return 0;
	struct pervd* vd = ele->priv_ptr;
	if(0 == vd)return 0;

	if(_src_ == self->foottype){
		int ret;
		struct item* xxx = peer->pchip;
		if((_sys_ == xxx->tier)|(_art_ == xxx->tier)){
			_obj* obj = peer->pchip;
			ret = file_reader(obj,0, 0,_pos_, &vd->h,0x200);
			if(0x200 != ret)return -1;
		}
		else{
			if(xxx->ontaking){
				ret = xxx->ontaking(xxx,peer->pfoot, 0,0, 0,_pos_, &vd->h,0x200);
			}
			else{
				ret = take_data_from_peer(ele,_src_, 0,0, 0,_pos_, (void*)&vd->h,0x200);
			}
			if(0x200 != ret)return -1;
		}

		//check self type, parse or mount
		parse_vmdk(&vd->h);
	}
	return 0;
}




int vmdk_delete(_obj* art)
{
	if(art->priv_ptr){
		memoryfree(art->priv_ptr);
		art->priv_ptr = 0;
	}
	return 0;
}
int vmdk_create(_obj* art)
{
	logtoall("@vmdk_create\n");
	art->ongiving = (void*)vmdk_ongive;
	art->ontaking = (void*)vmdk_ontake;

	struct pervd* vd = art->priv_ptr = memoryalloc(0x100000, 0);
	vd->cachepos_gd = 0xffffffff;
	vd->cachepos_gt = 0xffffffff;
	return 0;
}
