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
struct parsed{
	u64 type;
	u64 name;
	u64 start;
	u64 count;
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




int parse_mbr_one(struct mbrpart* part, struct parsed* out)
{
	u32 start,count;
	u64 type;
	char* str;
	if(0 == part->parttype)return 0;

	//count-1 = bus error, gcc bug ?
	start = hackforarmalign4(part->lba_start);
	count = hackforarmalign4(part->lba_count);

	type = 0;
	str = "?";
	switch(part->parttype){
	case 0x05:
	case 0x15:
	{
		str = "extend-chs";
		break;
	}
	case 0x0f:
	case 0x1f:
	{
		str = "extend-lba";
		break;
	}
	case 0x04:
	case 0x06:
	case 0x0e:
	case 0x14:
	case 0x16:
	case 0x1e:
	{
		str = "fat16";
		type = _fat_;
		break;
	}
	case 0x0b:
	case 0x1b:
	{
		str = "fat32-chs";
		type = _fat_;
		break;
	}
	case 0x0c:
	case 0x1c:
	{
		str = "fat32-lba";
		type = _fat_;
		break;
	}
	case 0x07:
	case 0x17:
	{
		str = "ntfs";
		type = _ntfs_;
		break;
	}
	case 0x83:
	{
		str = "ext";
		type = _ext_;
		break;
	}
	}//switch

	say("[%08x,%08x]:type=%02x(%s)\n", start, start+count-1, part->parttype, str);
	if(out){
		out->type = type;
		out->name = 0;
		out->start = start;
		out->count = count;
	}
	return 1;
}
int parse_mbr(u8* src, struct parsed* out)
{
	int j;
	int ret,cnt;
	u8* tmp;

	cnt = 0;
	for(j=0;j<4;j++){
		tmp = src + 0x1be + 0x10*j;
		ret = parse_mbr_one((void*)tmp, out ? &out[cnt] : 0);
		if(ret)cnt += 1;
	}
	return cnt;
}




int mount_mbr_one(_obj* art, struct mbrpart* part)
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
		_obj* tmp = arterycreate(_fat_,0,0,0);
		if(0 == tmp)return 0;
		struct relation* rel = relationcreate(tmp,0,_art_,_src_, art,(void*)(start<<9),_art_,_dst_);
		if(0 == rel)return 0;
		arteryattach((void*)&rel->dst, (void*)&rel->src);
		break;
	}
	}//switch

	return 0;
}
int mount_mbr(_obj* art, u8* src)
{
	int j;
	src += 0x1be;
	for(j=0;j<0x40;j+=0x10){
		mount_mbr_one(art, (void*)(src+j));
	}
	return 0;
}




static int mbrclient_showinfo(_obj* art)
{
	u8* mbr = art->listptr.buf0;
	return parse_mbr(mbr, 0);
}
static int mbrclient_showpart(_obj* art,void* foot, void* buf,int len)
{
	u8* mbr = art->listptr.buf0;
	return parse_mbr(mbr, buf);
}




static int mbrclient_ontake(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int off, u8* buf, int len)
{
	u64 offs;
	//say("@mbrclient_ontake\n");

	if(arg){
		//info
		if('i' == arg[0])return mbrclient_showinfo(art);
		//part
		if('p' == arg[0])return mbrclient_showpart(art,foot, buf,len);
	}

takedata:
	offs = (u64)foot + off;		//foot->offs + idx		//partoffs + dataoffs
	return take_data_from_peer(art,_src_, stack,sp+2, arg,offs, buf,len);
}
static int mbrclient_ongive(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, u8* buf, int len)
{
	return 0;
}




int mbrclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mbrclient_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@mbrclient_attach:%x\n",self->foottype);
	_obj* ele = self->pchip;
	if(0 == ele)return 0;
	void* buf = ele->listptr.buf0;
	if(0 == buf)return 0;

	//read
	int ret;
	struct item* xxx = peer->pchip;
	if((_sys_ == xxx->tier)|(_art_ == xxx->tier)){
		_obj* obj = peer->pchip;
		ret = file_take(obj,0, "",0, buf,0x10000);
		if(0x10000 != ret)return -1;
	}
	else{
		if(xxx->ontaking){
			ret = xxx->ontaking(xxx,peer->pfoot, 0,0, 0,0, buf, 0x10000);
		}
		else{
			ret = take_data_from_peer(ele,_src_, 0,0, "",0, ele->listptr.buf0,0x1000);
		}
		if(0x10000 != ret)return -1;
	}

	//check self type, parse or mount
	parse_mbr(buf, 0);
	return 0;
}
int mbrclient_delete(_obj* art)
{
	if(art->listptr.buf0){
		memorydelete(art->listptr.buf0);
		art->listptr.buf0 = 0;
	}
	return 0;
}
int mbrclient_create(_obj* art)
{
	say("@mbrclient_create\n");
	art->listptr.buf0 = memorycreate(0x100000, 0);

	art->ongiving = (void*)mbrclient_ongive;
	art->ontaking = (void*)mbrclient_ontake;
	return 0;
}
