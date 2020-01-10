#include "libsoft.h"
#define _EFI_PART_ hex64('E','F','I',' ','P','A','R','T')
#define _ext_ hex32('e','x','t',0)
#define _fat_ hex32('f','a','t',0)
#define _hfs_ hex32('h','f','s',0)
#define _ntfs_ hex32('n','t','f','s')
u32 crc32(u32 crc, u8* buf, u32 len);




int check_gpt(u8* addr)
{
	//[1fe, 1ff] = (0xaa，0x55)
	u64 temp = *(u16*)(addr+0x1fe);
	if(temp != 0xaa55)return 0;

	//[200, 207] = "EFI PART"
	temp = *(u64*)(addr+0x200);
	if(temp != _EFI_PART_)return 0;

	//检查crc32校验正确还是错误
	//

	//最终确定
	return _gpt_;	//'gpt'
}




//[+0x400,+0x4400],每个0x80,总共0x80个
//[+0,+0xf]:类型guid
//[+0x10,+0x1f]:分区guid
//[+0x20,+0x27]:起始lba
//[+0x28,+0x2f]:末尾lba
//[+0x30,+0x37]:属性标签
//[+0x38,+0x7f]:名字
void parse_gpt(u8* src, u8* dst)
{
	int j=0, k=0;
	u32 crc;
	u64* srcqword;
	u64* dstqword;

	j = src[0x20c];
	if(j >= 0x14)j -= 0x14;
	crc = crc32(0, src + 0x200, 0x10);
	crc = crc32(crc, src + 0x214, 4);
	crc = crc32(crc, src + 0x214, j);
	say("head crc:	%x, %x\n", *(u32*)(src+0x210), crc);
	say("body crc:	%x, %x\n", *(u32*)(src+0x258), crc32(0, src+0x400, 0x4000));

	src += 0x400;
	for(k=0;k<0x10000;k++)dst[k] = 0;
	for(j=0;j<0x80;j++)	//0x80 partitions per disk
	{
		//先取数字出来
		srcqword = (u64*)src;
		dstqword = (u64*)dst;
		if(srcqword[0] == 0)
		{
			src += 0x80;
			continue;
		}

		//类型，子类型，开始，结束
		u64 firsthalf = srcqword[0];
		u64 secondhalf = srcqword[1];
		u64 startlba = srcqword[4];
		u64 endlba = srcqword[5];

		//不同分区类型
		if(firsthalf == 0x477284830fc63daf)
		{
			if(secondhalf == 0xe47d47d8693d798e)
			{
				dstqword[2] = _ext_;
				//say("ext\n");
			}
		}
		else if(firsthalf == 0x11d2f81fc12a7328)
		{
			if(secondhalf == 0x3bc93ec9a0004bba)
			{
				dstqword[2] = _fat_;
				//say("fat\n");
			}
		}
		else if(firsthalf == 0x11aa000048465300)
		{
			if(secondhalf == 0xacec4365300011aa)
			{
				dstqword[2] = _hfs_;
				//say("hfs\n");
			}
		}
		else if(firsthalf == 0x4433b9e5ebd0a0a2)
		{
			if(secondhalf == 0xc79926b7b668c087)
			{
				dstqword[2] = _ntfs_;
				//say("ntfs\n");
			}
		}
		else
		{
			dstqword[2]=0x3f;
			//say("unknown\n");
		}

		dstqword[0] = startlba;
		dstqword[1] = endlba;
		for(k=0;k<0x40;k++)
		{
			dst[0x40 + k] = src[0x38 + k*2];
		}
		say("[%012x,%012x]:	%8.8s	%s\n",
			dstqword[0], dstqword[1],
			&dstqword[2], dst+0x40
		);

		//pointer++
		src += 0x80;
		dst += 0x80;
	}
}




int gptclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int gptclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int gptclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gptclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	int ret;
	struct object* obj;
	struct artery* ele;
	u8* src;
	u8* dst;

	obj = peer->pchip;
	ele = self->pchip;
	src = ele->buf0;
	dst = ele->buf1;

	ret = readfile(obj, obj->selffd, "", 0, src, 0x4800);
	if(ret != 0x4800)return 0;

	parse_gpt(src, dst);
	return 0;
}
int gptclient_delete(struct artery* ele)
{
	if(ele->buf0){
		memorydelete(ele->buf0);
		ele->buf0 = 0;
	}
	if(ele->buf1){
		memorydelete(ele->buf1);
		ele->buf1 = 0;
	}
	return 0;
}
int gptclient_create(struct artery* ele, u8* url)
{
	ele->buf0 = memorycreate(0x10000, 0);
	ele->buf1 = memorycreate(0x10000, 0);
	return 0;
}
