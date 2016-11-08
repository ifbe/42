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




int compress_read(u8* mem, u8* file, u64 offset, u64 count)
{
	//return guys[which].read(mem, file, offset, count);
}
int compress_write(u8* mem, u8* file, u64 offset, u64 count)
{
	//return guys[which].write(mem, file, offset, count);
}




//just command
static int compress_ls()
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
static int compress_cd(u8* p)
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
	compress_ls();
	return 0;
}
static int compress_show(u8* addr)
{
	return 0;
}
static int compress_edit()
{
	return 0;
}
static int compress_start()
{
	return 0;
}
static int compress_stop()
{
	return 0;
}
void compress_create(void* softaddr, u64* p)
{
	guys = softaddr;
	fshome = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x6d6f6f7a;
	p[10]=(u64)compress_start;
	p[11]=(u64)compress_stop;
	p[12]=(u64)compress_ls;
	p[13]=(u64)compress_cd;
	p[14]=(u64)compress_show;
	p[15]=(u64)compress_edit;
}
void compress_delete()
{
}
