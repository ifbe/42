#define u64 unsigned long long
void client_create(void*,void*);
void server_create(void*,void*);
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
void tftp_create(void*,void*);
void client_delete();
void server_delete();
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
void tftp_delete();




int net_list()
{
	return 0;
}
int net_choose()
{
	return 0;
}
int net_read()
{
	return 0;
}
int net_write()
{
	return 0;
}
int net_start(u64 type,char* p)
{
	return 0;
}
int net_stop()
{
	return 0;
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

	client_create(world,q);
	q+=0x80;

	server_create(world,q);
	q+=0x80;

	arp_create(world,q);
	eth_create(world,q);
	ftp_create(world,q);
	http_create(world,q);
	icmp_create(world,q);
	ipv4_create(world,q);
	ipv6_create(world,q);
	ppp_create(world,q);
	quic_create(world,q);
	slip_create(world,q);
	ssh_create(world,q);
	tftp_create(world,q);

	return q-(char*)p;
}
int net_delete()
{
	tftp_delete();
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
	server_delete();
	client_delete();
	return 0;
}
