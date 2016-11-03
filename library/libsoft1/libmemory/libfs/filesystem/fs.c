#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int ext_create(void*,void*);
int ext_delete();
int fat_create(void*,void*);
int fat_delete();
int hfs_create(void*,void*);
int hfs_delete();
int ntfs_create(void*,void*);
int ntfs_delete();
//
int startfile(u8*);
int stopfile(u8*);
int readfile(u8*,u8*,u64,u64);
int writefile(u8*,u8*,u64,u64);
//
void printmemory(u64 start,u64 count);
void say(void*, ...);




//
static u8* fshome;
static u8* dirhome;
static u8* datahome;




static void filesystem_list()
{
}
static int filesystem_choose(u8* p)
{
	int ret;

	//exit?
	stopfile(p);
	if(p == 0)return 0;

	//open
	ret = startfile(p);
	if(ret <= 0)return -1;

	//read
	readfile(datahome, 0, 0, 0x8000);

	//explain

	//list
	filesystem_list();
	return 0;
}
static void filesystem_read()
{
}
static void filesystem_write()
{
}
static void filesystem_start()
{
}
static void filesystem_stop()
{
}
void filesystem_create(void* world, u64* p)
{
	fshome = world+0x100000;
	dirhome = world+0x200000;
	datahome = world+0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x7366;

	p[10]=(u64)filesystem_start;
	p[11]=(u64)filesystem_stop;
	p[12]=(u64)filesystem_list;
	p[13]=(u64)filesystem_choose;
	p[14]=(u64)filesystem_read;
	p[15]=(u64)filesystem_write;

	//
	ext_create(world,0);
	fat_create(world,0);
	hfs_create(world,0);
	ntfs_create(world,0);
}
void filesystem_delete()
{
	ext_delete();
	fat_delete();
	hfs_delete();
	ntfs_delete();
}
