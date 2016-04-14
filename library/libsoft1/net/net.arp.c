#define QWORD unsigned long long




static void arp_list()
{
}
static void arp_switch()
{
}
static void arp_read()
{
}
static void arp_write()
{
}
static void arp_start(QWORD type,char* p)
{
}
static void arp_stop()
{
}
void arp_init(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x707261;		//id
	p[2]=(QWORD)arp_start;
	p[3]=(QWORD)arp_stop;
	p[4]=(QWORD)arp_list;
	p[5]=(QWORD)arp_switch;
	p[6]=(QWORD)arp_read;
	p[7]=(QWORD)arp_write;
}
void arp_kill()
{
	arp_stop();
}
