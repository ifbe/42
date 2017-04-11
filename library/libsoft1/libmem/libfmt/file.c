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
int check_ext(void*);
int parse_ext(void*, void*);
int check_fat(void*);
int parse_fat(void*, void*);
int check_hfs(void*);
int parse_hfs(void*, void*);
int check_ntfs(void*);
int parse_ntfs(void*, void*);
//parttable
int check_applept(void*);
int parse_applept(void*, void*);
int check_gpt(void*);
int parse_gpt(void*, void*);
int check_mbr(void*);
int parse_mbr(void*, void*);
//diskimage
int check_dmg(void*);
int check_qcow2(void*);
int check_vdi(void*);
int check_vhd(void*);
int check_vmdk(void*);
//
int startfile(u8*);
int stopfile(int);
int readfile(u64,u8*,u64,u64);
int writefile(u64,u8*,u64,u64);
//
void printmemory(void*, int);
void say(void*, ...);




struct object
{
	u64 type0;
	u64 stage0;
	u64 type1;
	u64 stage1;
	u64 type2;
	u64 stage2;
	u64 type3;
	u64 stage3;

	u8 data[0xc0];
};
struct filesystem
{
	//
	u64 magic;	//mnt
	u64 parent;	//fs[fs[this].parent].read?
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	//
	u8 data[0xc0];
};
static struct object* obj;
static struct filesystem* fs;
static u8* dirhome;
static u8* datahome;
//jpg in ext4 in gpt in vhd in zip ......
static int root;
static int curr;




#define ext 0x747865
#define fat 0x746166
#define hfs 0x736668
#define ntfs 0x7366746e
int mount_tree(int parent, u64 type)
{
	int j;
	void* p;
/*
	//search
	for(j=0;j<0x1000;j++)
	{
		if(fs[j].magic == 0)break;
	}
	if(j >= 0x1000)return 0;

	//create
	fs[j].magic = 0x746e6d;
	fs[j].parent = parent;
	if(type == ext)ext_create(obj, &(fs[j]));
	else if(type == fat)fat_create(obj, &(fs[j]));
	else if(type == hfs)hfs_create(obj, &(fs[j]));
	else if(type == ntfs)ntfs_create(obj, &(fs[j]));

	//start
	fs[j].start();
*/
	return j;
}
int mount_what(u8* buf, int len)
{
	//picture
	if(check_bmp(buf) != 0)parse_bmp(buf, len);
	else if(check_flif(buf) != 0)parse_flif(buf, len);
	else if(check_jpg(buf) != 0)parse_jpg(buf, len);
	else if(check_png(buf) != 0)parse_png(buf, len);
	else if(check_webp(buf) != 0)parse_webp(buf, len);

	//compress
	else if(check_7z(buf) != 0)parse_7z(buf);
	else if(check_cpio(buf) != 0)parse_cpio(buf);
	else if(check_gz(buf) != 0)parse_gz(buf);
	else if(check_tar(buf) != 0)parse_tar(buf);
	else if(check_zip(buf) != 0)parse_zip(buf);

	//executable
	else if(check_elf(buf) != 0)parse_elf(buf);
	else if(check_macho(buf) != 0)parse_macho(buf);
	else if(check_pe(buf) != 0)parse_pe(buf);

	//filesystem
	else if(check_ext(buf) != 0)say("ext\n");
	else if(check_fat(buf) != 0)say("fat\n");
	else if(check_hfs(buf) != 0)say("hfs\n");
	else if(check_ntfs(buf) != 0)say("ntfs\n");

	//parttable
	else if(check_gpt(buf) != 0)parse_gpt(buf, dirhome);
	else if(check_mbr(buf) != 0)parse_mbr(buf, dirhome);

	//unknown
	else say("unknown\n");
	return 0;
}




static int file_read(u8* addr)
{
	printmemory(datahome, 0x200);
	return 0;
}
static int file_write(u8* p)
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
	ret = mount_what(datahome, 0x8000);
	return 0;
}
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
static int file_cd()
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
	obj = softaddr;
	fs = softaddr + 0x100000;
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

	return 0x100;
}
int file_delete()
{
	return 0;
}
