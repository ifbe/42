#define QWORD unsigned long long
int systemstart(int type,char* p);
int systemstop();
int systemread(char* p,QWORD,QWORD);
int systemwrite(char* p,QWORD,QWORD);
int systemlist(char* p);
int systemchoose(char* p);
void say(char*,...);




//
static unsigned char* datahome=0;




static int process_list(char* p)
{
	return systemlist(p);
}
static int process_choose(char* p)
{
	return systemchoose(p);
}
static int process_read(char* p)
{
	int ret=systemread(datahome,0,0);
	if(ret>0)say("%s\n",datahome);
}
static int process_write()
{
	return 0;
}




static int process_start(QWORD type,char* p)
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
	p[2]=(QWORD)process_start;
	p[3]=(QWORD)process_stop;
	p[4]=(QWORD)process_list;
	p[5]=(QWORD)process_choose;
	p[6]=(QWORD)process_read;
	p[7]=(QWORD)process_write;
	return 0;
}
int process_delete()
{
	return 0;
}
