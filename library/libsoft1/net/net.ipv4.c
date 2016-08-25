#define u64 unsigned long long




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
static void ipv4_start(u64 type,char* p)
{
}
static void ipv4_stop()
{
}
void ipv4_create(char* world,u64* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x34767069;	//id
	p[2]=(u64)ipv4_start;
	p[3]=(u64)ipv4_stop;
	p[4]=(u64)ipv4_list;
	p[5]=(u64)ipv4_choose;
	p[6]=(u64)ipv4_read;
	p[7]=(u64)ipv4_write;
}
void ipv4_delete()
{
	ipv4_stop();
}
