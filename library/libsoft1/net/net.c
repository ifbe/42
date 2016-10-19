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
void sock_create(void*,void*);
void tftp_create(void*,void*);
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
void sock_delete();
void tftp_delete();




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

	p[10]=(u64)net_start;
	p[11]=(u64)net_stop;
	p[12]=(u64)net_list;
	p[13]=(u64)net_choose;
	p[14]=(u64)net_read;
	p[15]=(u64)net_write;

	//
	char* q=(char*)p+0x80;

	arp_create(world,q);
	q+=0x80;

	eth_create(world,q);
	q+=0x80;

	ftp_create(world,q);
	q+=0x80;

	http_create(world,q);
	q+=0x80;

	icmp_create(world,q);
	q+=0x80;

	ipv4_create(world,q);
	q+=0x80;

	ipv6_create(world,q);
	q+=0x80;

	ppp_create(world,q);
	q+=0x80;

	quic_create(world,q);
	q+=0x80;

	slip_create(world,q);
	q+=0x80;

	ssh_create(world,q);
	q+=0x80;

	sock_create(world,q);
	q+=0x80;

	tftp_create(world,q);
	q+=0x80;

	return q-(char*)p;
}
int net_delete()
{
	tftp_delete();
	ssh_delete();
	sock_delete();
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
