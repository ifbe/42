#define u64 unsigned long long
void arp_create(void*,void*);
void eth_create(void*,void*);
void ftp_create(void*,void*);
void http_create(void*,void*);
void icmp_create(void*,void*);
void ipv4_create(void*,void*);
void ipv6_create(void*,void*);
void ppp_create(void*,void*);
void quic_create(void*,void*);
void slip_create(void*,void*);
void ssh_create(void*,void*);
void tcp_create(void*,void*);
void tftp_create(void*,void*);
void udp_create(void*,void*);
void arp_delete();
void eth_delete();
void ftp_delete();
void http_delete();
void icmp_delete();
void ipv4_delete();
void ipv6_delete();
void ppp_delete();
void quic_delete();
void slip_delete();
void ssh_delete();
void tcp_delete();
void tftp_delete();
void udp_delete();




static int net_list()
{
}
static int net_choose()
{
}
static int net_read()
{
}
static int net_write()
{
}
static int net_start(u64 type,char* p)
{
}
static int net_stop()
{
}
int net_create(char* world,u64* p)
{
	//
	p[0]=0;			//type
	p[1]=0x74656e;		//id
	p[2]=(u64)net_start;
	p[3]=(u64)net_stop;
	p[4]=(u64)net_list;
	p[5]=(u64)net_choose;
	p[6]=(u64)net_read;
	p[7]=(u64)net_write;

	//
	char* q=(char*)p+0x40;

	arp_create(world,q);
	q+=0x40;

	eth_create(world,q);
	q+=0x40;

	ftp_create(world,q);
	q+=0x40;

	http_create(world,q);
	q+=0x40;

	icmp_create(world,q);
	q+=0x40;

	ipv4_create(world,q);
	q+=0x40;

	ipv6_create(world,q);
	q+=0x40;

	ppp_create(world,q);
	q+=0x40;

	quic_create(world,q);
	q+=0x40;

	slip_create(world,q);
	q+=0x40;

	ssh_create(world,q);
	q+=0x40;

	tcp_create(world,q);
	q+=0x40;

	tftp_create(world,q);
	q+=0x40;

	udp_create(world,q);
	q+=0x40;

	return q-(char*)p;
}
int net_delete()
{
	udp_delete();
	tftp_delete();
	tcp_delete();
	ssh_delete();
	slip_delete();
	quic_delete();
	ppp_delete();
	ipv6_delete();
	ipv4_delete();
	icmp_delete();
	http_delete();
	ftp_delete();
	eth_delete();
	arp_delete();
}
