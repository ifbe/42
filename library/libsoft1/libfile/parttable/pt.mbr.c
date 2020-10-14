#include "libsoft.h"




int check_mbr(u8* addr)
{
	if(0x55 != addr[0x1fe])return 0;
	if(0xaa != addr[0x1ff])return 0;
	return _mbr_;
}




//[+0x1be,+0x1fd],每个0x10,总共4个
struct mbrpart{
	u8     bootflag;	//[+0]:活动标记
	u8 chs_start[3];	//[+0x1,+0x3]:开始磁头柱面扇区
	u8     parttype;	//[+0x4]:分区类型
	u8   chs_end[3];	//[+0x5,+0x7]:结束磁头柱面扇区
	u8 lba_start[4];	//[+0x8,+0xb]:起始lba
	u8 lba_count[4];	//[+0xc,+0xf]:大小
};
u32 hackforarmalign4(u8* p)
{
	return p[0] + (p[1]<<8) + (p[2]<<16) + (p[3]<<24);
}
void parse_mbr_one(struct mbrpart* part)
{
	u32 start,count;
	if(0 == part->parttype)return;

	//count-1 = bus error, gcc bug ?
	start = hackforarmalign4(part->lba_start);
	count = hackforarmalign4(part->lba_count);
	say("[%08x,%08x]:\n", start, start+count-1);

	switch(part->parttype){
	case 0x05:
	case 0x15:
	{
		say("extend-chs\n");
		break;
	}
	case 0x0f:
	case 0x1f:
	{
		say("extend-lba\n");
		break;
	}
	case 0x04:
	case 0x06:
	case 0x0e:
	case 0x14:
	case 0x16:
	case 0x1e:
	{
		say("fat16\n");
		break;
	}
	case 0x0b:
	case 0x1b:
	{
		say("fat32-chs\n");
		break;
	}
	case 0x0c:
	case 0x1c:
	{
		say("fat32-lba\n");
		break;
	}
	case 0x07:
	case 0x17:
	{
		say("ntfs\n");
		break;
	}
	case 0x83:
	{
		say("ext\n");
		break;
	}
	default:{
		say("type=%02x\n",part->parttype);
	}
	}//switch
}
void parse_mbr(u8* src)
{
	int j;
	src += 0x1be;
	for(j=0;j<0x40;j+=0x10)parse_mbr_one((void*)(src+j));
}




int mbrclient_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int mbrclient_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
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
	if(_src_ == self->flag){
		struct artery* ele = self->pchip;
		int ret = take_data_from_peer(ele,_src_, 0,0, "",0, ele->buf0,0x1000);
		if(ret != 0x1000)return 0;

		parse_mbr(ele->buf0);
	}
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
	art->buf0 = memorycreate(0x1000, 0);
	return 0;
}
