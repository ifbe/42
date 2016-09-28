#define u64 unsigned long long
int startprocess(int type,char* p);
int stopprocess();
int readprocess(char* p,u64,u64);
int writeprocess(char* p,u64,u64);
int listprocess(char* p);
int chooseprocess(char* p);
void say(char*,...);




//
static unsigned char* datahome=0;




static int process_list(char* p)
{
	return listprocess(p);
}
static int process_choose(char* p)
{
	return chooseprocess(p);
}
static int process_read(char* p)
{
	int ret=readprocess(datahome,0,0);
	if(ret>0)say("%s\n",datahome);
}
static int process_write()
{
	return 0;
}




static int process_start(u64 type,char* p)
{
}
static int process_stop()
{
}




int process_create(char* world,unsigned long long* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x6c616963657073;
	p[1]=0x737365636f7270;
	p[2]=(u64)process_start;
	p[3]=(u64)process_stop;
	p[4]=(u64)process_list;
	p[5]=(u64)process_choose;
	p[6]=(u64)process_read;
	p[7]=(u64)process_write;
	return 0;
}
int process_delete()
{
	return 0;
}
