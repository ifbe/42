#define u64 unsigned long long




static void arp_list()
{
}
static void arp_choose()
{
}
static void arp_read()
{
}
static void arp_write()
{
}
static void arp_start(u64 type,char* p)
{
}
static void arp_stop()
{
}
void arp_create(char* world,u64* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x707261;		//id
	p[2]=(u64)arp_start;
	p[3]=(u64)arp_stop;
	p[4]=(u64)arp_list;
	p[5]=(u64)arp_choose;
	p[6]=(u64)arp_read;
	p[7]=(u64)arp_write;
}
void arp_delete()
{
	arp_stop();
}
