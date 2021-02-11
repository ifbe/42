#include "libsoft.h"




//[+0x1be,+0x1fd],每个0x10,总共4个
struct mbrpart{
	u8     bootflag;	//[+0]:活动标记
	u8 chs_start[3];	//[+0x1,+0x3]:开始磁头柱面扇区
	u8     parttype;	//[+0x4]:分区类型
	u8   chs_end[3];	//[+0x5,+0x7]:结束磁头柱面扇区
	u8 lba_start[4];	//[+0x8,+0xb]:起始lba
	u8 lba_count[4];	//[+0xc,+0xf]:大小
}__attribute__((packed));




int check_mbr(u8* addr)
{
	if(0x55 != addr[0x1fe])return 0;
	if(0xaa != addr[0x1ff])return 0;
	return _mbr_;
}
u32 hackforarmalign4(u8* p)
{
	return p[0] + (p[1]<<8) + (p[2]<<16) + (p[3]<<24);
}




void parse_mbr_one(struct mbrpart* part)
{
	u32 start,count;
	u8* type;
	if(0 == part->parttype)return;

	//count-1 = bus error, gcc bug ?
	start = hackforarmalign4(part->lba_start);
	count = hackforarmalign4(part->lba_count);

	type = "?";
	switch(part->parttype){
	case 0x05:
	case 0x15:
	{
		type = "extend-chs";
		break;
	}
	case 0x0f:
	case 0x1f:
	{
		type = "extend-lba";
		break;
	}
	case 0x04:
	case 0x06:
	case 0x0e:
	case 0x14:
	case 0x16:
	case 0x1e:
	{
		type = "fat16";
		break;
	}
	case 0x0b:
	case 0x1b:
	{
		type = "fat32-chs";
		break;
	}
	case 0x0c:
	case 0x1c:
	{
		type = "fat32-lba";
		break;
	}
	case 0x07:
	case 0x17:
	{
		type = "ntfs";
		break;
	}
	case 0x83:
	{
		type = "ext";
		break;
	}
	}//switch

	say("[%08x,%08x]:type=%02x(%s)\n", start, start+count-1, part->parttype, type);
}
void parse_mbr(u8* src)
{
	int j;
	src += 0x1be;
	for(j=0;j<0x40;j+=0x10){
		parse_mbr_one((void*)(src+j));
	}
}




int mount_mbr_one(_art* art, struct mbrpart* part)
{
	u64 start,count;
	if(0 == part->parttype)return 0;

	//count-1 = bus error, gcc bug ?
	start = hackforarmalign4(part->lba_start);
	count = hackforarmalign4(part->lba_count);
	say("[%08x,%08x]:type=%x\n", start, start+count-1, part->parttype);

	switch(part->parttype){
	case 0x0b:
	case 0x1b:
	case 0x0c:
	case 0x1c:
	{
		struct artery* tmp = arterycreate(_fat_,0,0,0);
		if(0 == tmp)return 0;
		struct relation* rel = relationcreate(tmp,0,_art_,_src_, art,(void*)(start<<9),_art_,_dst_);
		if(0 == rel)return 0;
		arterylinkup((void*)&rel->dst, (void*)&rel->src);
		break;
	}
	}//switch

	return 0;
}
int mount_mbr(_art* art, u8* src)
{
	int j;
	src += 0x1be;
	for(j=0;j<0x40;j+=0x10){
		mount_mbr_one(art, (void*)(src+j));
	}
	return 0;
}




int mbrclient_showmount(_art* art)
{
	struct relation* rel = art->orel0;
	while(1){
		if(0 == rel)break;
		say("%llx,%llx -> %llx,%llx\n",rel->srcchip,rel->srcfoot,rel->dstchip,rel->dstfoot);
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int mbrclient_showinfo(_art* art)
{
	u8* mbr = art->buf0;
	parse_mbr(mbr);
	return 0;
}




static int mbrclient_ontake(_art* art,void* foot, _syn* stack,int sp, u8* arg, int off, u8* buf, int len)
{
	//say("@mbrclient_ontake\n");
	if(arg){
		//info
		if('i' == arg[0])return mbrclient_showinfo(art);
		//rel
		if('r' == arg[0])return mbrclient_showmount(art);
	}

takedata:
	return take_data_from_peer(art,_src_, stack,sp+2, arg,(u64)foot+off, buf,len);
}
static int mbrclient_ongive(_art* art,void* foot, _syn* stack,int sp, u8* arg, int idx, u8* buf, int len)
{
	return 0;
}




int mbrclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mbrclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@mbrclient_linkup:%x\n",self->flag);
	struct artery* ele = self->pchip;
	if(0 == ele)return 0;
	void* buf = ele->buf0;
	if(0 == buf)return 0;

	//read
	int ret;
	struct item* xxx = peer->pchip;
	if((_sys_ == xxx->tier)|(_art_ == xxx->tier)){
		struct sysobj* obj = peer->pchip;
		ret = readfile(obj, obj->selffd, "", 0, buf, 0x10000);
		if(0x10000 != ret)return -1;
	}
	else{
		if(xxx->ontaking){
			ret = xxx->ontaking(xxx,peer->pfoot, 0,0, 0,0, buf, 0x10000);
		}
		else{
			ret = take_data_from_peer(ele,_src_, 0,0, "",0, ele->buf0,0x1000);
		}
		if(0x10000 != ret)return -1;
	}

	//check self type, parse or mount
	mount_mbr(ele, buf);
	return 0;
}
int mbrclient_delete(struct artery* art)
{
	if(art->buf0){
		memorydelete(art->buf0);
		art->buf0 = 0;
	}
	return 0;
}
int mbrclient_create(struct artery* art)
{
	say("@mbrclient_create\n");
	art->buf0 = memorycreate(0x100000, 0);

	art->ongiving = (void*)mbrclient_ongive;
	art->ontaking = (void*)mbrclient_ontake;
	return 0;
}
