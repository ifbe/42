#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void md5sum(void*, void*, int);
void sha1sum(void*, void*, int);
//
int startfile(u8*);
int stopfile(u64);
int readfile(u64, u8*, u64, u64);
int writefile(u64, u8*, u64, u64);
//
void printmemory(void*, int);
void say(void*, ...);




//
static u8* fdhome;
static u8* fshome;
static u8* dirhome;
static u8* datahome;
static int algorithm = 0;
void explain_data(u8* buf, int len)
{
	char res[0x40];
	md5sum(res, buf, len);
	printmemory(res, 16);
}




static int data_ls()
{
	return 0;
}
static int data_cd(u8* p)
{
	int fd;
	int ret;

	//open
	fd = startfile(p);
	if(fd <= 0)return -1;

	//read
	ret = readfile(fd, datahome, 0, 0x100000);
	if(ret < 0)return 0;

	//close
	stopfile(fd);

	//.......
	explain_data(datahome, ret);

	//
	return 0;
}
static int data_read(u8* addr)
{
	return 0;
}
static int data_write()
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
	p[6]=(u64)data_read;
	p[7]=(u64)data_write;

	return 0x100;
}
int data_delete()
{
	return 0;
}
