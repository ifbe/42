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




static int monitor_list(u8* p)
{
	return listprocess(p);
}
static int monitor_choose(u8* p)
{
	return chooseprocess(p);
}
static int monitor_read(u8* p)
{
	int ret=readprocess(datahome,0,0);
	if(ret>0)say("%s\n",datahome);
	return ret;
}
static int monitor_write()
{
	return 0;
}




static int monitor_start(u64 type,u8* p)
{
	return 0;
}
static int monitor_stop()
{
	return 0;
}




int monitor_create(u8* world,u64* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x6c616963657073;
	p[1]=0x636f7270;
	p[2]=(u64)monitor_start;
	p[3]=(u64)monitor_stop;
	p[4]=(u64)monitor_list;
	p[5]=(u64)monitor_choose;
	p[6]=(u64)monitor_read;
	p[7]=(u64)monitor_write;
	return 0x100;
}
int monitor_delete()
{
	return 0;
}
