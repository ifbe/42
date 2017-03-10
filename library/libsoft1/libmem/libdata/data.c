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
static u8* fdhome;
static u8* fshome;
static u8* dirhome;
static u8* datahome;




static int data_ls()
{
	return 0;
}
static int data_cd(u8* p)
{
	int ret;

	//exit?
	stopfile(p);
	if(p == 0)return -3;

	//open
	ret = startfile(p);
	if(ret <= 0)return -2;

	//read
	ret = readfile(0, datahome, 0, 0x8000);

	//
	return 0;
}
static int data_show(u8* addr)
{
	return 0;
}
static int data_edit()
{
	return 0;
}
static int data_start()
{
	return 0;
}
static int data_stop()
{
	return 0;
}
int data_create(void* softaddr, u64* p)
{
	fdhome = softaddr;
	fshome = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x61746164;
	p[2]=(u64)data_start;
	p[3]=(u64)data_stop;
	p[4]=(u64)data_ls;
	p[5]=(u64)data_cd;
	p[6]=(u64)data_show;
	p[7]=(u64)data_edit;

	return 0x100;
}
int data_delete()
{
	return 0;
}
