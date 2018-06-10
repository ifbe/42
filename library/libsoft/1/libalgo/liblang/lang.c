#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int purec_read(void*, int, void*, int);
int purec_start();
int ncmp(void*,void*,int);
int cmp(void*,void*);
int hexstr2data(void*, void*);
int decstr2data(void*, void*);
//
int openreadclose(void*, u64, void*, u64);
int openwriteclose(void*, u64, void*, u64);
//
int printmemory(void* addr, int count);
int say(void* str, ...);




//
static u8* guys;
static u8* fshome;
static u8* dirhome;
static u8* datahome;




static int lang_read()
{
	return 0;
}
static int lang_write(u8* p)
{
	int ret;

	ret = openreadclose(p, 0, datahome, 0x100000);
	if(ret<=0)return 0;

	purec_start();
	ret = purec_read(datahome, ret, 0, 0);

	return ret;
}
static int lang_list()
{
	return 0;
}
static int lang_choose()
{
	return 0;
}
static int lang_start()
{
	return 0;
}
static int lang_stop()
{
	return 0;
}
void lang_create(u8* softaddr,u64* p)
{
	guys = softaddr;
	fshome = softaddr+0x100000;
	dirhome = softaddr+0x200000;
	datahome = softaddr+0x300000;
}
void lang_delete()
{
}
