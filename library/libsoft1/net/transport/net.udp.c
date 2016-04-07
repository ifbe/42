#define QWORD unsigned long long 
void buf2addrport(unsigned char* p,int max,unsigned char* addr,int* port);




static void udp_list()
{
}
static void udp_switch()
{
}
static void udp_read()
{
}
static void udp_write()
{
}




static int udp_start(QWORD type,char* p)
{
}
static int udp_stop()
{
}
int udp_init(char* world,unsigned long long* p)
{
	p[0]=0x706475;
	p[1]=0;
	p[2]=(QWORD)udp_start;
	p[3]=(QWORD)udp_stop;
	p[4]=(QWORD)udp_list;
	p[5]=(QWORD)udp_switch;
	p[6]=(QWORD)udp_read;
	p[7]=(QWORD)udp_write;
}
int udp_kill()
{
}
