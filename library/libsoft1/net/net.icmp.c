#define QWORD unsigned long long




static void icmp_list()
{
}
static void icmp_choose()
{
}
static void icmp_read()
{
}
static void icmp_write()
{
}
static void icmp_start(QWORD type,char* p)
{
}
static void icmp_stop()
{
}
void icmp_create(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x706d6369;	//id
	p[2]=(QWORD)icmp_start;
	p[3]=(QWORD)icmp_stop;
	p[4]=(QWORD)icmp_list;
	p[5]=(QWORD)icmp_choose;
	p[6]=(QWORD)icmp_read;
	p[7]=(QWORD)icmp_write;
}
void icmp_delete()
{
	icmp_stop();
}
