#define QWORD unsigned long long




static void ipv6_list()
{
}
static void ipv6_choose()
{
}
static void ipv6_read()
{
}
static void ipv6_write()
{
}
static void ipv6_start(QWORD type,char* p)
{
}
static void ipv6_stop()
{
}
void ipv6_create(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x36767069;	//id
	p[2]=(QWORD)ipv6_start;
	p[3]=(QWORD)ipv6_stop;
	p[4]=(QWORD)ipv6_list;
	p[5]=(QWORD)ipv6_choose;
	p[6]=(QWORD)ipv6_read;
	p[7]=(QWORD)ipv6_write;
}
void ipv6_delete()
{
	ipv6_stop();
}
