#define QWORD unsigned long long




static void interface_list()
{
}
static void interface_switch()
{
}
static void interface_read()
{
}
static void interface_write()
{
}
static void interface_start(QWORD type,char* p)
{
}
static void interface_stop()
{
}
void interface_init(char* world,QWORD* p)
{
	//
	p[0]=0;			//type
	p[1]=0x6563616669;	//id
	p[2]=(QWORD)interface_start;
	p[3]=(QWORD)interface_stop;
	p[4]=(QWORD)interface_list;
	p[5]=(QWORD)interface_switch;
	p[6]=(QWORD)interface_read;
	p[7]=(QWORD)interface_write;
}
void interface_kill()
{
	interface_stop();
}
