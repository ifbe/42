#define QWORD unsigned long long




static void ipv4_list()
{
}
static void ipv4_choose()
{
}
static void ipv4_read()
{
}
static void ipv4_write()
{
}
static void ipv4_start(QWORD type,char* p)
{
}
static void ipv4_stop()
{
}
void ipv4_create(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x34767069;	//id
	p[2]=(QWORD)ipv4_start;
	p[3]=(QWORD)ipv4_stop;
	p[4]=(QWORD)ipv4_list;
	p[5]=(QWORD)ipv4_choose;
	p[6]=(QWORD)ipv4_read;
	p[7]=(QWORD)ipv4_write;
}
void ipv4_delete()
{
	ipv4_stop();
}
