#define QWORD unsigned long long




static void ethernet_list()
{
}
static void ethernet_switch()
{
}
static void ethernet_read()
{
}
static void ethernet_write()
{
}
static void ethernet_start(QWORD type,char* p)
{
}
static void ethernet_stop()
{
}
void ethernet_init(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x687465;		//id
	p[2]=(QWORD)ethernet_start;
	p[3]=(QWORD)ethernet_stop;
	p[4]=(QWORD)ethernet_list;
	p[5]=(QWORD)ethernet_switch;
	p[6]=(QWORD)ethernet_read;
	p[7]=(QWORD)ethernet_write;
}
void ethernet_kill()
{
	ethernet_stop();
}
