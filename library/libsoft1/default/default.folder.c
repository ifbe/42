#define QWORD unsigned long long
int systemstart(int type,char* p);
int systemstop();
int systemread(char* p,QWORD,QWORD);
int systemwrite(char* p,QWORD,QWORD);
int systemlist(char* p);
int systemswitch(char* p);
static unsigned char* datahome=0;




static int folder_list(char* p)
{
	return systemlist(p);
}
static int folder_switch(char* p)
{
	return systemswitch(p);
}
static int folder_read(char* p)
{
	int ret=systemread(datahome,0,0);
	if(ret>0)say("%s\n",datahome);
}
static int folder_write()
{
	return 0;
}




static int folder_start(QWORD type,char* p)
{
	return systemstart(0,p);
}
static int folder_stop()
{
	systemstop();
}




void folder_init(char* world,unsigned long long* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x7265646c6f66;
	p[1]=0;
	p[2]=(QWORD)folder_start;
	p[3]=(QWORD)folder_stop;
	p[4]=(QWORD)folder_list;
	p[5]=(QWORD)folder_switch;
	p[6]=(QWORD)folder_read;
	p[7]=(QWORD)folder_write;
}
void folder_kill()
{
	folder_stop();
}
