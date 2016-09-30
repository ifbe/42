#define u64 unsigned long long 
void buf2addrport(unsigned char* p,int max,unsigned char* addr,int* port);




static void udp_list()
{
}
static void udp_choose()
{
}
static void udp_read()
{
}
static void udp_write()
{
}




static int udp_start(u64 type,char* p)
{
}
static int udp_stop()
{
}
int udp_create(char* world,unsigned long long* p)
{
	p[0]=0x74656e;
	p[1]=0x706475;

	p[10]=(u64)udp_start;
	p[11]=(u64)udp_stop;
	p[12]=(u64)udp_list;
	p[13]=(u64)udp_choose;
	p[14]=(u64)udp_read;
	p[15]=(u64)udp_write;
}
int udp_delete()
{
}
