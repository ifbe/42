#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//multimedia
int jpg_yes(void*);
int jpg_explain(void*, int);
//compress
int tar_create(void*,void*);
int tar_delete();
int tar_yes(u8*);
int zip_create(void*,void*);
int zip_delete();
int zip_yes();
//filesystem
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
//parttable
int gpt_create(void*,void*);
int gpt_delete();
int gpt_yes(u8*);
void gpt_explain(u8*, u8*);
int mbr_create(void*,void*);
int mbr_delete();
int mbr_yes(u8*);
void mbr_explain(u8*, u8*);
//diskimage
int vhd_create(void*,void*);
int vhd_delete();
int vhd_yes(u8*);
int vmdk_create(void*,void*);
int vmdk_delete();
int vmdk_yes(u8*);
//
int startfile(u8*);
int stopfile(int);
int readfile(u64,u8*,u64,u64);
int writefile(u64,u8*,u64,u64);
//
void printmemory(void*, int);
void say(void*, ...);




//
static u8* fdhome;
static u8* fshome;
static u8* dirhome;
static u8* datahome;
//
static int which = 0;




//
int file_explain(u8* p)
{
	if(jpg_yes(p) > 0)
	{
		jpg_explain(p, 0x100000);
	}

	//filesystem
	else if(ext_yes(p) > 0)
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

	//parttable
	else if(gpt_yes(p) > 0)
	{
		say("gpt\n");
		gpt_explain(p, dirhome);
	}
	else if(mbr_yes(p) > 0)
	{
		say("mbr\n");
		mbr_explain(p, dirhome);
	}

	//unknown
	else
	{
		say("unknown\n");
		return -1;
	}
	return 0;
}
int file_mount(u8* addr)
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
		if(id == fdhome[j].id)
		{
			which = id;
			break;
		}
	}
*/
	return 0;
}




//just command
static int file_ls()
{
	int j;
	u8* p8;
	u64* p64;
	for(j=0;j<0x10;j++)
	{
		p8 = dirhome + j*0x80;
		p64 = (void*)p8;
		if(p64[0] == 0)break;

		say("%-16s%-16s%s\n", p8, p8+8, p8+0x40);
	}
	return 0;
}
static int file_cd(u8* p)
{
	int fd;
	int ret;

	//open
	fd = startfile(p);
	if(fd <= 0)return -1;

	//read
	ret = readfile(fd, datahome, 0, 0x8000);
	if(ret <= 0)return 0;

	//close
	stopfile(fd);

	//11111111
	ret = file_explain(datahome);
	return 0;
}
static int file_read(u8* addr)
{
	printmemory(datahome, 0x200);
	return 0;
}
static int file_write()
{
	return 0;
}
static int file_start()
{
	return 0;
}
static int file_stop()
{
	return 0;
}
int file_create(void* softaddr, u64* p)
{
	u8* q;
	fdhome = softaddr;
	fshome = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x656c6966;
	p[2]=(u64)file_start;
	p[3]=(u64)file_stop;
	p[4]=(u64)file_ls;
	p[5]=(u64)file_cd;
	p[6]=(u64)file_read;
	p[7]=(u64)file_write;
	q = (u8*)p;
	q += 0x100;

	ext_create(softaddr, q);
	q += 0x100;

	fat_create(softaddr, q);
	q += 0x100;

	hfs_create(softaddr, q);
	q += 0x100;

	ntfs_create(softaddr, q);
	q += 0x100;

	return q-(u8*)p;
}
int file_delete()
{
	ext_delete();
	fat_delete();
	hfs_delete();
	ntfs_delete();
	return 0;
}
