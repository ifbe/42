#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
int startfolder(u8* p);
int stopfolder();
int readfolder(u8* p);
int writefolder(u8* p);
int listfolder(u8* p);
int choosefolder(u8* p);
void say(void*, ...);




//
static u8* datahome=0;




static int folder_list(u8* p)
{
	return listfolder(p);
}
static int folder_choose(u8* p)
{
	return choosefolder(p);
}
static int folder_read(u8* p)
{
	int ret=readfolder(datahome);
	if(ret>0)say("%s\n",datahome);
	return ret;
}
static int folder_write(u8* p)
{
	return 0;
}




static int folder_start(u64 type, u8* p)
{
	return startfolder(p);
}
static int folder_stop()
{
	return stopfolder();
}




int folder_create(u8* world, u64* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x6c616963657073;
	p[1]=0x726964;

	p[10]=(u64)folder_start;
	p[11]=(u64)folder_stop;
	p[12]=(u64)folder_list;
	p[13]=(u64)folder_choose;
	p[14]=(u64)folder_read;
	p[15]=(u64)folder_write;
	return 0;
}
int folder_delete()
{
	folder_stop();
	return 0;
}
