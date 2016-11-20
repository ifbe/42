#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
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
void secureshell_create(void*,void*);
void tftp_create(void*,void*);
void websocket_create(void*,void*);

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
void secureshell_delete();
void tftp_delete();
void websocket_delete();
//
void client_create(void*,void*);
void server_create(void*,void*);
void client_delete();
void server_delete();
//
void printmemory(char*, int);
void say(char*, ...);




void net_read(u64* p)
{
}
void net_write()
{
}
void net_list()
{
}
void net_choose()
{
}
void net_start()
{
}
void net_stop(u64 fd)
{
}
int net_create(void* world,u64* p)
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

	return q-(char*)p;
}
int net_delete()
{
	server_delete();
	client_delete();
	return 0;
}
