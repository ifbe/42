#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//multimedia
int check_bmp(void*);
int parse_bmp(void*, int);
int check_flif(void*);
int parse_flif(void*, int);
int check_jpg(void*);
int parse_jpg(void*, int);
int check_png(void*);
int parse_png(void*, int);
int check_webp(void*);
int parse_webp(void*, int);
//compress
int check_7z(void*);
int parse_7z(void*);
int check_cpio(void*);
int parse_cpio(void*);
int check_gz(void*);
int parse_gz(void*);
int check_tar(void*);
int parse_tar(void*);
int check_zip(void*);
int parse_zip(void*);
//executable
int check_elf(void*);
int parse_elf(void*);
int check_macho(void*);
int parse_macho(void*);
int check_pe(void*);
int parse_pe(void*);
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
int file_explain(u8* buf, int len)
{
	if(check_bmp(buf) > 0)parse_bmp(buf, len);
	else if(check_flif(buf) > 0)parse_flif(buf, len);
	else if(check_jpg(buf) > 0)parse_jpg(buf, len);
	else if(check_png(buf) > 0)parse_png(buf, len);
	else if(check_webp(buf) > 0)parse_webp(buf, len);

	//compress
	else if(check_7z(buf) > 0)parse_7z(buf);
	else if(check_cpio(buf) > 0)parse_cpio(buf);
	else if(check_gz(buf) > 0)parse_gz(buf);
	else if(check_tar(buf) > 0)parse_tar(buf);
	else if(check_zip(buf) > 0)parse_zip(buf);

	//executable
	else if(check_elf(buf) > 0)parse_elf(buf);
	else if(check_macho(buf) > 0)parse_macho(buf);
	else if(check_pe(buf) > 0)parse_pe(buf);

	//filesystem
	else if(ext_yes(buf) > 0)
	{
		say("ext\n");
	}
	else if(fat_yes(buf) > 0)
	{
		say("fat\n");
	}
	else if(hfs_yes(buf) > 0)
	{
		say("hfs\n");
	}
	else if(ntfs_yes(buf) > 0)
	{
		say("ntfs\n");
	}

	//parttable
	else if(gpt_yes(buf) > 0)
	{
		say("gpt\n");
		gpt_explain(buf, dirhome);
	}
	else if(mbr_yes(buf) > 0)
	{
		say("mbr\n");
		mbr_explain(buf, dirhome);
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
	ret = file_explain(datahome, 0x8000);
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
