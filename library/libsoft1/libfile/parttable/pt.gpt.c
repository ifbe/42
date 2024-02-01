#include "libsoft.h"
#define _EFI_PART_ hex64('E','F','I',' ','P','A','R','T')
u32 crc32(u32 crc, u8* buf, u32 len);




static u8 guid_bbp[16] = {0x48,0x61,0x68,0x21,0x49,0x64,0x6F,0x6E,0x74,0x4E,0x65,0x65,0x64,0x45,0x46,0x49};
static u8 guid_efi[16] = {0x45,0x46,0x49,0x20,0x50,0x41,0x52,0x54,0x00,0x00,0x01,0x00,0x5C,0x00,0x00,0x00};
//win
static u8 guid_fat[16] = {0x28,0x73,0x2a,0xC1,0x1F,0xF8,0xD2,0x11,0xBA,0x4B,0x00,0xA0,0xC9,0x3E,0xC9,0x3B};
static u8 guid_ntfs[16]= {0xA2,0xA0,0xD0,0xEB,0xE5,0xB9,0x33,0x44,0x87,0xC0,0x68,0xB6,0xB7,0x26,0x99,0xC7};
static u8 guid_rsvd[16]= {0x16,0xe3,0xc9,0xe3,0x5c,0x0b,0xb8,0x4d,0x81,0x7d,0xf9,0x2d,0xf0,0x02,0x15,0xae};
//mac
static u8 guid_hfs[16] = {0x00,0x53,0x46,0x48,0x00,0x00,0xaa,0x11,0xaa,0x11,0x00,0x30,0x65,0x43,0xec,0xac};
static u8 guid_apfs[16]= {0xEF,0x57,0x34,0x7C,0x00,0x00,0xAA,0x11,0xAA,0x11,0x00,0x30,0x65,0x43,0xEC,0xAC};
//linux
static u8 guid_ext4[16]= {0xaf,0x3d,0xc6,0x0f,0x83,0x84,0x72,0x47,0x8e,0x79,0x3d,0x69,0xd8,0x47,0x7d,0xe4};
static u8 guid_zfs[16] = {};
//[+0x400,+0x4400],每个0x80,总共0x80个
struct gptpart{
	u8 guid_type[16];	//[+0,+0xf]:类型guid
	u8 guid_part[16];	//[+0x10,+0x1f]:分区guid
	u64 lba_start;		//[+0x20,+0x27]:起始lba
	u64 lba_end;		//[+0x28,+0x2f]:末尾lba
	u64 flag;			//[+0x30,+0x37]:属性标签
	char name[0x48];		//[+0x38,+0x7f]:名字
}__attribute__((packed));
struct parsed{
	u64 type;
	u64 name;
	u64 start;
	u64 count;
}__attribute__((packed));




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




int parse_gpt_one(struct gptpart* part, struct parsed* out)
{
	if(0 == part->lba_start)return 0;
	//printmemory(part, 0x80);

	u64 type;
	if(0 == ncmp(part->guid_type, guid_bbp, 16))type = _bbp_;
	else if(0 == ncmp(part->guid_type, guid_efi, 16))type = _efi_;
	else if(0 == ncmp(part->guid_type, guid_fat, 16))type = _fat_;
	else if(0 == ncmp(part->guid_type, guid_ntfs,16))type = _ntfs_;
	else if(0 == ncmp(part->guid_type, guid_rsvd,16))type = _rsvd_;
	else if(0 == ncmp(part->guid_type, guid_hfs, 16))type = _hfs_;
	else if(0 == ncmp(part->guid_type, guid_apfs,16))type = _apfs_;
	else if(0 == ncmp(part->guid_type, guid_ext4,16))type = _ext_;
	else type = hex32('?','?','?','?');

	if(out){
		out->type = type;
		out->name = 0;
		out->start = part->lba_start;
		out->count = part->lba_end - part->lba_start+1;
	}
	else{
		logtoall("[%016llx,%016llx]:	type=%.8s, name=%s\n", part->lba_start, part->lba_end, &type, part->name);
	}
	return 1;
}
int parse_gpt(u8* src, struct parsed* out)
{
	int j;
	int ret,cnt;
	u32 hcrc,bcrc;
	u8* tmp;
	logtoall("@parse_gpt\n");

	j = src[0x20c];
	if(j >= 0x14)j -= 0x14;
	hcrc = crc32(0, src + 0x200, 0x10);
	hcrc = crc32(hcrc, src + 0x214, 4);
	hcrc = crc32(hcrc, src + 0x214, j);
	logtoall("head crc:	%x, %x\n", *(u32*)(src+0x210), hcrc);
	logtoall("body crc:	%x, %x\n", *(u32*)(src+0x258), crc32(0, src+0x400, 0x4000));

	cnt = 0;
	for(j=0;j<0x80;j++){
		tmp = src + 0x400 + 0x80*j;
		ret = parse_gpt_one((void*)tmp, out ? &out[cnt] : 0);
		if(ret)cnt += 1;
	}
	return cnt;
}



/*
void mount_gpt_one(_obj* art, struct gptpart* part)
{
	if(0 == part->lba_start)return;
	printmemory(part, 0x80);

	u64 type;
	if(0 == ncmp(part->guid_type, guid_bbp, 16))type = _bbp_;
	else if(0 == ncmp(part->guid_type, guid_efi, 16))type = _efi_;
	else if(0 == ncmp(part->guid_type, guid_fat, 16))type = _fat_;
	else if(0 == ncmp(part->guid_type, guid_ntfs,16))type = _ntfs_;
	else if(0 == ncmp(part->guid_type, guid_hfs, 16))type = _hfs_;
	else if(0 == ncmp(part->guid_type, guid_apfs,16))type = _apfs_;
	else if(0 == ncmp(part->guid_type, guid_ext4,16))type = _ext_;
	else type = hex32('?','?','?','?');

	int j;
	char* name = part->name;
	for(j=0;j<0x40;j++)name[j] = part->name[j*2];

	//logtoall("[%016llx,%016llx]:	type=%.8s, name=%s\n", part->lba_start, part->lba_end, &type, name);
	if((_efi_ == type)|(_fat_ == type)){
		_obj* tmp = artery_alloc_prepobj_create(_art_, _fat_,0,0,0, 0);
		if(0 == tmp)return;
		struct relation* rel = relationcreate(tmp,0,_art_,_src_, art,(void*)(part->lba_start<<9),_art_,_dst_);
		if(0 == rel)return;
		artery_attach((void*)&rel->dst, (void*)&rel->src);
	}
}
void mount_gpt(_obj* art, u8* src)
{
	int j,k;
	u32 crc;
	logtoall("@mount_gpt\n");

	j = src[0x20c];
	if(j >= 0x14)j -= 0x14;
	crc = crc32(0, src + 0x200, 0x10);
	crc = crc32(crc, src + 0x214, 4);
	crc = crc32(crc, src + 0x214, j);
	logtoall("head crc:	%x, %x\n", *(u32*)(src+0x210), crc);
	logtoall("body crc:	%x, %x\n", *(u32*)(src+0x258), crc32(0, src+0x400, 0x4000));

	for(j=0;j<0x80;j++){
		mount_gpt_one(art, (void*)(src + 0x400 + 0x80*j));
	}
}*/




static int gptclient_showinfo(_obj* art)
{
	u8* gpt = art->listptr.buf0;
	return parse_gpt(gpt, 0);
}
static int gptclient_showpart(_obj* art,void* foot, void* buf,int len)
{
	u8* gpt = art->listptr.buf0;
	return parse_gpt(gpt, buf);
}




int gptclient_ontake(_obj* art,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	//logtoall("@gptclient_take\n");

	switch(idx){
	case _info_:return gptclient_showinfo(art);
	case _part_:return gptclient_showpart(art,foot, buf,len);
	}

	//logtoall("foot=%x\n",foot);
	u64 offs = (u64)foot + arg;		//foot->offs + arg		//partoffs + dataoffs
	int ret = take_data_from_peer(art,_src_, stack,sp+2, offs,_pos_, buf,len);
	//logtoall("gpt.ret=%x\n",ret);
	return ret;
}
int gptclient_ongive(_obj* art,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int gptclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gptclient_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ele = self->pchip;
	if(0 == ele)return 0;
	void* buf = ele->listptr.buf0;
	if(0 == buf)return 0;

	//read
	if(_src_ == self->foottype){
		int ret = take_data_from_peer(ele,_src_, 0,0, 0,_pos_, buf,0x4800);
		if(ret != 0x4800)return 0;
	}

	//check self type, parse or mount
	parse_gpt(buf, 0);
	return 0;
}




int gptclient_reader(_obj* art,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int gptclient_writer(_obj* art,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int gptclient_delete(_obj* ele)
{
	if(ele->listptr.buf0){
		memoryfree(ele->listptr.buf0);
		ele->listptr.buf0 = 0;
	}
	return 0;
}
int gptclient_create(_obj* art, u8* url)
{
	logtoall("@gptclient_create\n");
	art->listptr.buf0 = memoryalloc(0x10000, 0);

	art->ongiving = (void*)gptclient_ongive;
	art->ontaking = (void*)gptclient_ontake;
	return 0;
}
