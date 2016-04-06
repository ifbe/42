#define QWORD unsigned long long 
void buf2addrport(unsigned char* p,int max,unsigned char* addr,int* port);




static void tcp_list()
{
}
static void tcp_switch()
{
}
static void tcp_read()
{
}
static void tcp_write()
{
}




static int tcp_start(char* p)
{
	unsigned char addr[16];
	int port;

	//
	buf2addrport(p,64,addr,&port);

	//
	startsocket(addr,port);
}
static int tcp_stop()
{
	stopsocket();
}
int tcp_init(char* world,unsigned long long* p)
{
	p[0]=0x706374;
	p[1]=0;
	p[2]=(QWORD)tcp_start;
	p[3]=(QWORD)tcp_stop;
	p[4]=(QWORD)tcp_list;
	p[5]=(QWORD)tcp_switch;
	p[6]=(QWORD)tcp_read;
	p[7]=(QWORD)tcp_write;
}
int tcp_kill()
{
}
