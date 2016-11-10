#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int tar_create(void*,void*);
int tar_delete();
int tar_yes(u8*);
int zip_create(void*,void*);
int zip_delete();
int zip_yes();
//
int ext_create(void*,void*);
int ext_delete();
int ext_yes(u8*);
int fat_create(void*,void*);
int fat_delete();
int fat_yes(u8*);
int hfs_create(void*,void*);
int hfs_delete();
int hfs_yes(u8*);
int ntfs_create(void*,void*);
int ntfs_delete();
int ntfs_yes(u8*);
//
int gpt_create(void*,void*);
int gpt_delete();
int gpt_yes(u8*);
void gpt_explain(u8*, u8*);
int mbr_create(void*,void*);
int mbr_delete();
int mbr_yes(u8*);
void mbr_explain(u8*, u8*);
//
int vhd_create(void*,void*);
int vhd_delete();
int vhd_yes(u8*);
int vmdk_create(void*,void*);
int vmdk_delete();
int vmdk_yes(u8*);
//
int startfile(u8*);
int stopfile(u8*);
int readfile(u8*,u8*,u64,u64);
int writefile(u8*,u8*,u64,u64);
//
void printmemory(u64 start,u64 count);
void say(void*, ...);




//
static u8* guys;
static u8* fshome;
static u8* dirhome;
static u8* datahome;
//
static int which = 0;




//
int filesystem_explain(u8* p)
{
	//explain
	if(ext_yes(p) > 0)
	{
		say("ext\n");
	}
	else if(fat_yes(p) > 0)
	{
		say("fat\n");
	}
	else if(hfs_yes(p) > 0)
	{
		say("hfs\n");
	}
	else if(ntfs_yes(p) > 0)
	{
		say("ntfs\n");
	}
	else if(gpt_yes(p) > 0)
	{
		say("gpt\n");
		gpt_explain(p, fshome);
	}
	else if(mbr_yes(p) > 0)
	{
		say("mbr\n");
		mbr_explain(p, fshome);
	}
	else
	{
		say("parttable notfound\n");
		return -1;
	}
	return 0;
}
int filesystem_mount(u8* addr)
{
/*
	int j;
	u64 id = 0;
	if(isext(addr) > 0)
	{
		id = ext
	}
	else if(isfat(addr) > 0)
	{
		id = fat
	}
	else if(ishfs(addr) > 0)
	{
		id = hfs
	}
	else if(isntfs(addr) > 0)
	{
		id = ntfs
	}
	else
	{
		return -1;
	}

	for(j=0;j<0x10000/0x80;j++)
	{
		if(id == guys[j].id)
		{
			which = id;
			break;
		}
	}
	return 0;
*/
}




//just command
static int filesystem_ls()
{
	int j;
	for(j=0;j<0x80*0x80;j+=0x80)
	{
		if(*(u64*)(fshome+j) == 0)break;

		say("(%8s,%8s)  [%08llx,%08llx] %s\n",
			fshome+j, fshome+j+8,
			*(u64*)(fshome+j+0x10), *(u64*)(fshome+j+0x18),
			fshome+j+0x40
		);
	}
	return j / 0x80;
}
static int filesystem_cd(u8* p)
{
	int ret;

	//exit?
	stopfile(p);
	if(p == 0)return -3;

	//open
	ret = startfile(p);
	if(ret <= 0)return -2;

	//read
	ret = readfile(datahome, 0, 0, 0x8000);

	//11111111
	ret = filesystem_explain(datahome);

	//list
	filesystem_ls();
	return 0;
}
static int filesystem_show(u8* addr)
{
	return 0;
}
static int filesystem_edit()
{
	return 0;
}
static int filesystem_start()
{
	return 0;
}
static int filesystem_stop()
{
	return 0;
}
void filesystem_create(void* softaddr, u64* p)
{
	guys = softaddr;
	fshome = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x7366;
	p[10]=(u64)filesystem_start;
	p[11]=(u64)filesystem_stop;
	p[12]=(u64)filesystem_ls;
	p[13]=(u64)filesystem_cd;
	p[14]=(u64)filesystem_show;
	p[15]=(u64)filesystem_edit;

	//
	ext_create(softaddr,0);
	fat_create(softaddr,0);
	hfs_create(softaddr,0);
	ntfs_create(softaddr,0);
}
void filesystem_delete()
{
	ext_delete();
	fat_delete();
	hfs_delete();
	ntfs_delete();
}
