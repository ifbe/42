#include "libsoft.h"




//memory
static u8* fshome;		//fat表
	static u8* pbrbuffer;
	static u8* fatbuffer;
static u8* dirhome;
//
static u64 cache_first;
static int cache_count;
//disk
static int version;
static int byte_per_sec;
static int sec_per_fat;
static int sec_per_clus;
static int sec_of_fat0;
static int sec_of_clus2;		//2号簇所在扇区




struct folder{
	u8 name_this[8];	//0x00-0x07：文件名，不足8个字节0x20补全(短文件名8.3命名规则)
	u8 name_ext[3];		//0x08-0x0A：扩展名
	u8 attr;			//0x0B：文件属性
//0x00=读写
//0x01=只读
//0x02=隐藏
//0x04=系统
//0x08=卷标
//0x10=目录
//0x20=归档
	u8 unused;			//0x0C:
	u8 create_time_ms;	//0x0D：创建时间的10毫秒位
	u16 create_time_s;	//0x0E-0x0F：文件创建时间
	u16 create_date;	//0x10-0x11：文件创建日期
	u16 access_date;	//0x12-0x13：文件最后访问日期
	u16 clus_hi16;		//0x14-0x15：文件起始簇号的高16位 0x0000
	u16 modify_time;	//0x16-0x17：文件最近修改时间
	u16 modify_date;	//0x18-0x19：文件最近修改日期
	u16 clus_lo16;		//0x1A-0x1B：文件起始簇号的地16位 0x0003
	u32 filesize;		//0x1C-0x1F：文件的长度，0x2206=8710bytes=8.5K
};
void fatdate2mydate(int val0, int val1, u8* date)
{
	int tmp;
	date[0] = (val1<< 1)&0x3f;
	date[1] = (val1>> 5)&0x3f;
	date[2] = (val1>>11)&0x1f;
	date[3] = val0 & 0x1f;
	date[4] = (val0>> 5)&0x0f;

	tmp = 1980 + ((val0>>9)&0x7f);
	date[5] = tmp % 100;
	date[6] = tmp / 100;
}
static void parsefolder(struct artery* art, u8* rsi)
{
	int j;
	struct folder* dir;

	for(j=0;j<0x4000;j+=0x20){
		dir = (void*)(rsi+j);
		if(0x0f == rsi[j+0xb])continue;	//longname
		if(0xe5 == rsi[j+0x0])continue;	//deleted
		if(0x00 == rsi[j+0x0])continue;	//have name

		printmemory(rsi+j,0x20);
		say("size=%x\n",dir->filesize);

		int clus = (dir->clus_hi16<<16) + dir->clus_lo16;
		say("clus=%x\n", clus);

		u8 date[8];
		fatdate2mydate(dir->create_date, dir->create_time_s, date);
		say("createtime: %02d%02d, %d:%d, %d:%d:%d\n",
			date[6],date[5], date[4],date[3], date[2],date[1],date[0]
		);
	}

/*
	for(i=0;i<0x4000;i++) rdi[i]=0;
	while(1)
	{
		//datahome-0x20+0x20=datahome
		rsi+=0x20;
		if(rsi >= (u8*)(datahome+0x4000) )break;

		//check
		if( rsi[0xb] ==0xf )continue;	//ignore
		if( rsi[0] ==0xe5 )continue;	//deleted
		if( *(u64*)rsi ==0 )continue;	//named

		//[0x10,0x17]:type
		u64 temp=rsi[0xb];
		if( (temp&0x10) == 0x10 )
		{
			*(u64*)(rdi+0)=0x726964;    //'dir'
		}
		else
		{
			*(u64*)(rdi+0)=0x656c6966+(temp<<32);
		}

		//[0x10,0x17]:cluster
		temp=((u64)(*(u16*)(rsi+0x14)))<<16; //high
		temp+=(u64)(*(u16*)(rsi+0x1a));  //low
		*(u64*)(rdi+0x10)=temp;

		//[0x18,0x1f]:size
		*(u64*)(rdi+0x18)=*(u32*)(rsi+0x1c);

		//[0x20,0x3f]:name
		j=0x20;
		for(i=0;i<8;i++)
		{
			if(rsi[i]!=0x20)
			{
				rdi[j]=rsi[i];
				if( (rdi[j]>='A')&&(rdi[j]<='Z') )rdi[j]+=0x20;
				j++;
			}
		}
		if(rsi[8]!=0x20)
		{
			rdi[j]='.';
			j++;
		}
		for(i=8;i<11;i++)
		{
			if(rsi[i]!=0x20)
			{
				rdi[j]=rsi[i];
				if( (rdi[j]>='A')&&(rdi[j]<='Z') )rdi[j]+=0x20;
				j++;
			}
		}
		//
		rdi+=0x40;
	}*/
}




static u16 fat16_nextclus(struct artery* art, u16 clus)
{
	u16* cache = (void*)fatbuffer;
	return cache[clus];
}
static int fat16_read(struct artery* art, int ign, u32 clus,int offs, u8* buf,int len)
{
	u32 tmp;
	int ret, cnt = 0;
	int byteperclus = byte_per_sec * sec_per_clus;

	while(1){
		if(cnt + byteperclus > len)break;

		//read this cluster
		tmp = byte_per_sec * (sec_of_clus2+sec_per_clus*(clus-2));
		ret = relationread(art,_src_, "",tmp, buf+cnt,byteperclus);
		if(ret < byteperclus)goto retcnt;
		cnt += byteperclus;

		//next clus
		clus = fat16_nextclus(art, clus);
		if(clus < 2)break;
		if(clus >= 0xfff8)break;
		if(clus == 0xfff7){say("bad clus:%x\n",clus);break;}
	}

	tmp = byte_per_sec * (sec_of_clus2+sec_per_clus*(clus-2));
	ret = relationread(art,_src_, "",ret, buf+cnt,len-cnt);
	if(ret < len-cnt)goto retcnt;
	cnt += ret;

retcnt:
	return cnt;
}




static u32 fat32_nextclus(struct artery* art, u32 clus)
{
	u64 byte;
	u32* cache = (void*)fatbuffer;
	u32 remain = clus % cache_count;

	if(cache_first != clus-remain){
		cache_first = clus-remain;

		byte = byte_per_sec*sec_of_fat0 + 4*cache_first;
		relationread(art,_src_, "",byte, cache,4*cache_count);
	}

	return cache[remain];
}
//clus=first clus of file, offs=offset byte of file
static int fat32_read(struct artery* art, int ign, u64 clus,int offs, u8* buf,int len)
{
	u64 tmp;
	int ret, cnt = 0;
	int byteperclus = byte_per_sec * sec_per_clus;

	while(1){
		if(0 == offs)break;
		else if(offs >= byteperclus)offs -= byteperclus;
		else if(offs < byteperclus){
			tmp = byte_per_sec * (sec_of_clus2+sec_per_clus*(clus-2));
			ret = relationread(art,_src_, "",tmp+offs, buf,byteperclus-offs);
			if(ret < byteperclus-offs)goto retcnt;
			cnt += byteperclus-offs;

			offs = 0;
		}

		clus = fat32_nextclus(art, clus);
		if(clus <2)goto retcnt;
		if(clus >= 0x0ffffff8)goto retcnt;
		if(clus == 0x0ffffff7){say("bad clus:%x\n",clus);goto retcnt;}
	}

	while(1){
		if(cnt + byteperclus > len)break;

		//read this cluster
		tmp = byte_per_sec * (sec_of_clus2+sec_per_clus*(clus-2));
		ret = relationread(art,_src_, "",tmp, buf+cnt,byteperclus);
		if(ret < byteperclus)goto retcnt;
		cnt += byteperclus;

		//next clus
		clus = fat32_nextclus(art, clus);
		if(clus <2)goto retcnt;
		if(clus >= 0x0ffffff8)goto retcnt;
		if(clus == 0x0ffffff7){say("bad clus:%x\n",clus);goto retcnt;}
	}

	tmp = byte_per_sec * (sec_of_clus2+sec_per_clus*(clus-2));
	ret = relationread(art,_src_, "",tmp, buf+cnt,len-cnt);
	if(ret < len-cnt)goto retcnt;
	cnt += ret;

retcnt:
	return cnt;
}




int fat_buildcache(struct artery* art)
{
	cache_first = 0;
	cache_count = 0x10000;
	return relationread(art,_src_, "",sec_of_fat0*byte_per_sec, fatbuffer,0x40000);
}
int fat_checkname(char* name, u8* fatname)
{
	int j;
	u8 a,b;
	for(j=0;j<8;j++){
		if(0 == name[j])break;

		a = name[j];
		if(a >= 'a')a -= 0x20;

		b = fatname[j];
		if(b >= 'a')b -= 0x20;

		if(a != b)return j+1;
	}
	return 0;
}
u32 fat_ls(struct artery* art, char* name)
{
	int j;
	struct folder* dir;
	if(0 == name)return 2;

	if(32 == version){
		for(j=0;j<0x1000;j+=0x20){
			dir = (void*)(dirhome+j);
			//printmemory(dir, 0x20);

			if(0x0f == dir->attr)continue;	//longname
			if(0xe5 == dir->name_this[0])continue;	//deleted
			if(0x00 == dir->name_this[0])continue;	//have name
			if(0 == (dir->attr&0x10))continue;

			if(0 == fat_checkname(name, dir->name_this))
			return (dir->clus_hi16<<16) + dir->clus_lo16;
		}
	}
	return 0;
}
int fat_cd(struct artery* art, char* name)
{
	int ret;
	u32 clus;
	if(16 == version){
		if(0 == name){
			ret = fat16_read(art,0, 2,0, dirhome, 0x4000);
			parsefolder(art, dirhome);
		}
		else{
			clus = fat_ls(art, name);
			if(0 == clus)return 0;

			ret = fat16_read(art,0, clus,0, dirhome+0x10000, 0x10000);
			if(ret <= 0)return 0;

			//printmemory(dirhome+0x10000, 0x200);
			parsefolder(art, dirhome+0x10000);
		}
	}
	if(32 == version){
		if(0 == name){
			ret = fat32_read(art,0, 2,0, dirhome, 0x4000);
			parsefolder(art, dirhome);
		}
		else{
			clus = fat_ls(art, name);
			if(0 == clus)return 0;

			ret = fat32_read(art,0, clus,0, dirhome+0x10000, 0x10000);
			if(ret <= 0)return 0;

			//printmemory(dirhome+0x10000, 0x200);
			parsefolder(art, dirhome+0x10000);
		}
	}

	return 0;
}




struct BPB_FAT{
	u8     BS_jmpBoot[3];	//00: 跳转指令
	u8     BS_OEMName[8];	//03: 原始制造商
	u8   byte_per_sec[2];	//0b: 每扇区字节数
	u8      sec_per_clus; 	//0d: 每簇扇区数
	u8    sec_of_fat0[2];	//0e: 保留扇区数目
	u8       BPB_NumFATs; 	//10: 此卷中FAT表数
	u8 BPB_RootEntCnt[2];	//11: FAT32为0
	u8   BPB_TotSec16[2]; 	//13: FAT32为0
	u8         BPB_Media;	//15: 存储介质
	u8    sec_per_fat[2];	//16: FAT32为0
	u8  BPB_SecPerTrk[2];	//18: 磁道扇区数
	u8   BPB_NumHeads[2];	//1a: 磁头数
	u8    BPB_HiddSec[4]; 	//1c: FAT区前隐扇区数
	u8   BPB_TotSec32[4];	//20: 该卷总扇区数
};
struct BPB_FAT16{
	struct BPB_FAT common;
	u8 BS_drvNum;
	u8 BS_Reserved1;
	u8 BS_BootSig;
	u8 BS_VolID[4];
	u8 BS_VolLab[11];
	u8 BS_FileSysType[8];
};
struct BPB_FAT32{
	struct BPB_FAT common;
	u8    sec_per_fat[4]; 	//24: 一个FAT表扇区数
	u8   BPB_ExtFlags[2];	//28: FAT32特有
	u8      BPB_FSVer[2];	//2a: FAT32特有
	u8   BPB_RootClus[4];	//2c: 根目录簇号
	u8         FSInfo[2];	//30: 保留扇区FSINFO扇区数
	u8  BPB_BkBootSec[2];	//32: 通常为6
	u8  BPB_Reserved[12];	//34: 扩展用
	u8         BS_DrvNum;	//40: BIOS驱动器号
	u8      BS_Reserved1;	//41: 未使用
	u8        BS_BootSig;	//42: 扩展引导标记
	u8       BS_VolID[4];	//43: 卷序列号
	u8 BS_FilSysType[11];	//47: offset:71
	u8 BS_FilSysType1[8];	//52: "FAT32   "
};
int fat_check(u8* addr)
{
	int tmp;
	//printmemory(addr,0x200);

	//0x55,0xaa
	if(0x55 != addr[0x1fe])return 0;
	if(0xaa != addr[0x1ff])return 0;

	//totally 2 fat tables
	tmp = addr[0x10];
	if(tmp != 2)return 0;

	//byte per sector
	tmp = addr[0xb] + (addr[0xc]<<8);
	if(tmp < 0x200)return 0;
	if(tmp & 0x1ff)return 0;

	if(addr[0x11])return 16;
	if(addr[0x12])return 16;
	if(addr[0x16])return 16;
	if(addr[0x17])return 16;

	return 32;
}
int fat_parse(struct artery* art, u8* addr)
{
	u8* p;
	struct BPB_FAT* fat = (void*)addr;

	p = fat->byte_per_sec;
	byte_per_sec = p[0] + (p[1]<<8);
	say("byte_per_sec=%x\n", byte_per_sec);

	sec_per_clus = fat->sec_per_clus;
	say("sec_per_clus=%x\n", sec_per_clus);

	p = fat->sec_of_fat0;
	sec_of_fat0 = p[0] + (p[1]<<8);
	say("sec_of_fat0=%x\n", sec_of_fat0);

	int ver = 32;
	if(addr[0x11])ver = 16;
	if(addr[0x12])ver = 16;
	if(addr[0x16])ver = 16;
	if(addr[0x17])ver = 16;

	if(16 == ver)		//这是fat16
	{
		p = fat->sec_per_fat;
		sec_per_fat = *(u16*)(fat->sec_per_fat);
		say("sec_per_fat:%x\n", sec_per_fat);

		sec_of_clus2 = sec_of_fat0 + sec_per_fat*2 + 32;
		say("sec_of_clus2@%x\n", sec_of_clus2);

		version = 16;
	}
	if(32 == ver)		//这是fat32
	{
		struct BPB_FAT32* fat32 = (void*)addr;

		p = fat32->sec_per_fat;
		sec_per_fat = p[0] + (p[1]<<8) + (p[2]<<16) + (p[3]<<24);;
		say("sec_per_fat:%x\n", sec_per_fat);

		sec_of_clus2 = sec_of_fat0 + sec_per_fat*2;
		say("sec_of_clus2@%x\n", sec_of_clus2);

		version = 32;
	}
	return 0;
}




int fatclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int fatclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int fatclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int fatclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	int ret;
	struct artery* art;
	if(_src_ != self->flag)return 0;

	art = self->pchip;
	if(0 == art)return 0;

	ret = relationread(art, _src_, "", 0, pbrbuffer, 0x200);
	if(ret < 0x200){
		return 0;
	}

	ret = fat_check(pbrbuffer);
	if(ret < 12){
		say("wrong fat\n");
		return -1;
	}

	ret = fat_parse(art, pbrbuffer);
	//if(ret < 0)

	ret = fat_buildcache(art);
	//if(ret < 0)

	fat_cd(art, 0);
	fat_cd(art, "efi");
	return 0;
}
int fatclient_delete(struct artery* art)
{
	return 0;
}
int fatclient_create(struct artery* art)
{
	fshome = memorycreate(0x100000, 0);
		pbrbuffer = fshome+0x10000;
		fatbuffer = fshome+0x20000;
	dirhome = memorycreate(0x100000, 0);
	return 0;
}