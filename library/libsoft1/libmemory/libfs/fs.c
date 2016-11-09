#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int parttable_create(void*,void*);
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
int parttable_explain(u8*);
int parttable_focus(u64);
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
int filesystem_read(u8* mem, u8* file, u64 offset, u64 count)
{
	if(which <= 0)return -1;

	//return guys[which].read(mem, file, offset, count);
}
int filesystem_write(u8* mem, u8* file, u64 offset, u64 count)
{
	if(which <= 0)return -1;

	//return guys[which].write(mem, file, offset, count);
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

	//
	ret = parttable_explain(datahome);

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
	parttable_create(softaddr,0);

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
