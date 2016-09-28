#define u64 unsigned long long
int startfolder(char* p);
int stopfolder();
int readfolder(char* p);
int writefolder(char* p);
int listfolder(char* p);
int choosefolder(char* p);
void say(char*,...);




//
static unsigned char* datahome=0;




static int folder_list(char* p)
{
	return listfolder(p);
}
static int folder_choose(char* p)
{
	return choosefolder(p);
}
static int folder_read(char* p)
{
	int ret=readfolder(datahome);
	if(ret>0)say("%s\n",datahome);
}
static int folder_write(char* p)
{
	return 0;
}




static int folder_start(u64 type,char* p)
{
	return startfolder(p);
}
static int folder_stop()
{
	stopfolder();
}




int folder_create(char* world,unsigned long long* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x6c616963657073;
	p[1]=0x7265646c6f66;
	p[2]=(u64)folder_start;
	p[3]=(u64)folder_stop;
	p[4]=(u64)folder_list;
	p[5]=(u64)folder_choose;
	p[6]=(u64)folder_read;
	p[7]=(u64)folder_write;
	return 0;
}
int folder_delete()
{
	folder_stop();
	return 0;
}
