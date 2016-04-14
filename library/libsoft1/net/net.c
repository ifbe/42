#define QWORD unsigned long long




static void net_list()
{
}
static void net_switch()
{
}
static void net_read()
{
}
static void net_write()
{
}
static void net_start(QWORD type,char* p)
{
}
static void net_stop()
{
}
void net_init(char* world,QWORD* p)
{
	//
	p[0]=0;			//type
	p[1]=0x74656e;		//id
	p[2]=(QWORD)net_start;
	p[3]=(QWORD)net_stop;
	p[4]=(QWORD)net_list;
	p[5]=(QWORD)net_switch;
	p[6]=(QWORD)net_read;
	p[7]=(QWORD)net_write;
}
void net_kill()
{
	net_stop();
}
