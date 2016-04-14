#define QWORD unsigned long long
void arp_init(void*,void*);
void eth_init(void*,void*);
void ftp_init(void*,void*);
void http_init(void*,void*);
void icmp_init(void*,void*);
void ipv4_init(void*,void*);
void ipv6_init(void*,void*);
void ppp_init(void*,void*);
void quic_init(void*,void*);
void slip_init(void*,void*);
void ssh_init(void*,void*);
void tcp_init(void*,void*);
void tftp_init(void*,void*);
void udp_init(void*,void*);
void arp_kill();
void eth_kill();
void ftp_kill();
void http_kill();
void icmp_kill();
void ipv4_kill();
void ipv6_kill();
void ppp_kill();
void quic_kill();
void slip_kill();
void ssh_kill();
void tcp_kill();
void tftp_kill();
void udp_kill();




static int net_list()
{
}
static int net_switch()
{
}
static int net_read()
{
}
static int net_write()
{
}
static int net_start(QWORD type,char* p)
{
}
static int net_stop()
{
}
int net_init(char* world,QWORD* p)
{
	//
	p[0]=0;			//type
	p[1]=0x74656e;		//id
	p[2]=(QWORD)net_start;
	p[3]=(QWORD)net_stop;
	p[4]=(QWORD)net_list;
	p[5]=(QWORD)net_switch;
	p[6]=(QWORD)net_read;
	p[7]=(QWORD)net_write;

	//
	char* q=(char*)p+0x40;

	arp_init(world,q);
	q+=0x40;

	eth_init(world,q);
	q+=0x40;

	ftp_init(world,q);
	q+=0x40;

	http_init(world,q);
	q+=0x40;

	icmp_init(world,q);
	q+=0x40;

	ipv4_init(world,q);
	q+=0x40;

	ipv6_init(world,q);
	q+=0x40;

	ppp_init(world,q);
	q+=0x40;

	quic_init(world,q);
	q+=0x40;

	slip_init(world,q);
	q+=0x40;

	ssh_init(world,q);
	q+=0x40;

	tcp_init(world,q);
	q+=0x40;

	tftp_init(world,q);
	q+=0x40;

	udp_init(world,q);
	q+=0x40;

	return q-(char*)p;
}
int net_kill()
{
	udp_kill();
	tftp_kill();
	tcp_kill();
	ssh_kill();
	slip_kill();
	quic_kill();
	ppp_kill();
	ipv6_kill();
	ipv4_kill();
	icmp_kill();
	http_kill();
	ftp_kill();
	eth_kill();
	arp_kill();
}
