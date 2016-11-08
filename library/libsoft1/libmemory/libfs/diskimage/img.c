#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
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




int diskimage_read(u8* mem, u8* file, u64 offset, u64 count)
{
	//return guys[which].read(mem, file, offset, count);
}
int diskimage_write(u8* mem, u8* file, u64 offset, u64 count)
{
	//return guys[which].write(mem, file, offset, count);
}




//just command
static int diskimage_ls()
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
static int diskimage_cd(u8* p)
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

	//list
	diskimage_ls();
	return 0;
}
static int diskimage_show(u8* addr)
{
	return 0;
}
static int diskimage_edit()
{
	return 0;
}
static int diskimage_start()
{
	return 0;
}
static int diskimage_stop()
{
	return 0;
}
void diskimage_create(void* softaddr, u64* p)
{
	guys = softaddr;
	fshome = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x6b736964;
	p[10]=(u64)diskimage_start;
	p[11]=(u64)diskimage_stop;
	p[12]=(u64)diskimage_ls;
	p[13]=(u64)diskimage_cd;
	p[14]=(u64)diskimage_show;
	p[15]=(u64)diskimage_edit;
}
void diskimage_delete()
{
}
