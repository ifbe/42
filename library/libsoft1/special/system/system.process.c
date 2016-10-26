#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
int startprocess(int type,u8* p);
int stopprocess();
int readprocess(u8*, u64, u64);
int writeprocess(u8*, u64, u64);
int listprocess(u8*);
int chooseprocess(u8*);
void say(void*, ...);




//
static u8* datahome=0;




static int process_list(u8* p)
{
	return listprocess(p);
}
static int process_choose(u8* p)
{
	return chooseprocess(p);
}
static int process_read(u8* p)
{
	int ret=readprocess(datahome,0,0);
	if(ret>0)say("%s\n",datahome);
	return ret;
}
static int process_write()
{
	return 0;
}




static int process_start(u64 type,u8* p)
{
	return 0;
}
static int process_stop()
{
	return 0;
}




int process_create(u8* world,u64* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x6c616963657073;
	p[1]=0x636f7270;

	p[10]=(u64)process_start;
	p[11]=(u64)process_stop;
	p[12]=(u64)process_list;
	p[13]=(u64)process_choose;
	p[14]=(u64)process_read;
	p[15]=(u64)process_write;
	return 0;
}
int process_delete()
{
	return 0;
}
