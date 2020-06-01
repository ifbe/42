#include "libsoft.h"
#define _EFI_PART_ hex64('E','F','I',' ','P','A','R','T')
u32 crc32(u32 crc, u8* buf, u32 len);




int check_gpt(u8* addr)
{
	//[1fe, 1ff] = (0x55，0xaa)
	if(0x55 != addr[0x1fe])return 0;
	if(0xaa != addr[0x1ff])return 0;

	//[200, 207] = "EFI PART"
	if(*(u64*)(addr+0x200) != _EFI_PART_)return 0;

	//检查crc32校验正确还是错误
	//

	//最终确定
	return _gpt_;	//'gpt'
}




//[+0x400,+0x4400],每个0x80,总共0x80个
struct gptpart{
	u8 guid_type[16];	//[+0,+0xf]:类型guid
	u8 guid_part[16];	//[+0x10,+0x1f]:分区guid
	u64 lba_start;		//[+0x20,+0x27]:起始lba
	u64 lba_end;		//[+0x28,+0x2f]:末尾lba
	u64 flag;			//[+0x30,+0x37]:属性标签
	u8 name[0x48];		//[+0x38,+0x7f]:名字
};
static u8 guid_bbp[16] = {0x48,0x61,0x68,0x21,0x49,0x64,0x6F,0x6E,0x74,0x4E,0x65,0x65,0x64,0x45,0x46,0x49};
static u8 guid_efi[16] = {0x45,0x46,0x49,0x20,0x50,0x41,0x52,0x54,0x00,0x00,0x01,0x00,0x5C,0x00,0x00,0x00};
static u8 guid_fat[16] = {0x28,0x73,0x2a,0xC1,0x1F,0xF8,0xD2,0x11,0xBA,0x4B,0x00,0xA0,0xC9,0x3E,0xC9,0x3B};
static u8 guid_hfs[16] = {0x00,0x53,0x46,0x48,0x00,0x00,0xaa,0x11,0xaa,0x11,0x00,0x30,0x65,0x43,0xec,0xac};
static u8 guid_apfs[16]= {0xEF,0x57,0x34,0x7C,0x00,0x00,0xAA,0x11,0xAA,0x11,0x00,0x30,0x65,0x43,0xEC,0xAC};
static u8 guid_ntfs[16]= {0xA2,0xA0,0xD0,0xEB,0xE5,0xB9,0x33,0x44,0x87,0xC0,0x68,0xB6,0xB7,0x26,0x99,0xC7};
void parse_gpt(u8* src)
{
	int j,k;
	u32 crc;
	struct gptpart* part;

	j = src[0x20c];
	if(j >= 0x14)j -= 0x14;
	crc = crc32(0, src + 0x200, 0x10);
	crc = crc32(crc, src + 0x214, 4);
	crc = crc32(crc, src + 0x214, j);
	say("head crc:	%x, %x\n", *(u32*)(src+0x210), crc);
	say("body crc:	%x, %x\n", *(u32*)(src+0x258), crc32(0, src+0x400, 0x4000));

	part = (void*)(src+0x400);
	for(j=0;j<0x80;j++)
	{
		if(0 == part[j].lba_start)continue;
/*
		else if(firsthalf == )
		{
			if(secondhalf == )
			{
				dstqword[2] = _hfs_;
				//say("hfs\n");
			}
		}
*/
		for(k=0;k<0x40;k++)part[j].name[k] = part[j].name[k*2];
		say("[%012x,%012x]:	%s\n", part[j].lba_start, part[j].lba_end, part[j].name);

		if(0)say("???\n");
		else if(0 == ncmp(part[j].guid_type, guid_bbp, 16))say("bbp!\n");
		else if(0 == ncmp(part[j].guid_type, guid_efi, 16))say("efi!\n");
		else if(0 == ncmp(part[j].guid_type, guid_fat, 16))say("fat!\n");
		else if(0 == ncmp(part[j].guid_type, guid_hfs, 16))say("hfs!\n");
		else if(0 == ncmp(part[j].guid_type, guid_apfs, 16))say("apfs!\n");
		else if(0 == ncmp(part[j].guid_type, guid_ntfs, 16))say("ntfs!\n");
		else printmemory(part[j].guid_type, 0x10);

		printmemory(part[j].guid_part, 0x10);
	}
}




int gptclient_read(_art* art,int foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int gptclient_write(_art* art,int foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int gptclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gptclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	if(_src_ == self->flag){
		struct artery* ele = self->pchip;
		int ret = take_data_from_peer(ele,_src_, 0,0, "",0, ele->buf0,0x4800);
		if(ret != 0x4800)return 0;

		parse_gpt(ele->buf0);
	}
	return 0;
}
int gptclient_delete(struct artery* ele)
{
	if(ele->buf0){
		memorydelete(ele->buf0);
		ele->buf0 = 0;
	}
	return 0;
}
int gptclient_create(struct artery* ele, u8* url)
{
	ele->buf0 = memorycreate(0x10000, 0);
	return 0;
}
