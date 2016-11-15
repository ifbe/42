#define u64 unsigned long long




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
static void icmp_start(u64 type,char* p)
{
}
static void icmp_stop()
{
}
void icmp_create(char* world,u64* p)
{
/*
	p[0]=0x74656e;		//type
	p[1]=0x706d6369;	//id

	p[10]=(u64)icmp_start;
	p[11]=(u64)icmp_stop;
	p[12]=(u64)icmp_list;
	p[13]=(u64)icmp_choose;
	p[14]=(u64)icmp_read;
	p[15]=(u64)icmp_write;
*/
}
void icmp_delete()
{
	icmp_stop();
}
