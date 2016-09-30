#define u64 unsigned long long




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
static void ipv6_start(u64 type,char* p)
{
}
static void ipv6_stop()
{
}
void ipv6_create(char* world,u64* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x36767069;	//id

	p[10]=(u64)ipv6_start;
	p[11]=(u64)ipv6_stop;
	p[12]=(u64)ipv6_list;
	p[13]=(u64)ipv6_choose;
	p[14]=(u64)ipv6_read;
	p[15]=(u64)ipv6_write;
}
void ipv6_delete()
{
	ipv6_stop();
}
