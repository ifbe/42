#include "artery.h"
//
int ssh_check(  void* p, int fd, void* buf, int len);
int tls_check(  void* p, int fd, void* buf, int len);
int http_check( void* p, int fd, void* buf, int len);
int rtmp_check( void* p, int fd, void* buf, int len);
//phy
int bt_client(   void* p, int fd, void* buf, int len);
int bt_server(   void* p, int fd, void* buf, int len);
int eth_client(  void* p, int fd, void* buf, int len);
int eth_server(  void* p, int fd, void* buf, int len);
int wifi_client( void* p, int fd, void* buf, int len);
int wifi_server( void* p, int fd, void* buf, int len);
//link
int arp_client(  void* p, int fd, void* buf, int len);
int arp_server(  void* p, int fd, void* buf, int len);
int ppp_client(  void* p, int fd, void* buf, int len);
int ppp_server(  void* p, int fd, void* buf, int len);
//ip
int icmp_client( void* p, int fd, void* buf, int len);
int icmp_server( void* p, int fd, void* buf, int len);
int ipv4_client( void* p, int fd, void* buf, int len);
int ipv4_server( void* p, int fd, void* buf, int len);
int ipv6_client( void* p, int fd, void* buf, int len);
int ipv6_server( void* p, int fd, void* buf, int len);
//tran
int udp_client(  void* p, int fd, void* buf, int len);
int udp_server(  void* p, int fd, void* buf, int len);
int tcp_client(  void* p, int fd, void* buf, int len);
int tcp_server(  void* p, int fd, void* buf, int len);
int quic_client( void* p, int fd, void* buf, int len);
int quic_server( void* p, int fd, void* buf, int len);
int ssh_client(  void* p, int fd, void* buf, int len);
int ssh_server(  void* p, int fd, void* buf, int len);
int tls_client(  void* p, int fd, void* buf, int len);
int tls_server(  void* p, int fd, void* buf, int len);
//udp
int dns_client(  void* p, int fd, void* buf, int len);
int dns_server(  void* p, int fd, void* buf, int len);
int hole_client( void* p, int fd, void* buf, int len);
int hole_server( void* p, int fd, void* buf, int len);
int tftp_client( void* p, int fd, void* buf, int len);
int tftp_server( void* p, int fd, void* buf, int len);
//tcp
int http_client( void* p, int fd, void* buf, int len);
int http_server( void* p, int fd, void* buf, int len);
int https_client(void* p, int fd, void* buf, int len);
int https_server(void* p, int fd, void* buf, int len);
int proxy_client(void* p, int fd, void* buf, int len);
int proxy_server(void* p, int fd, void* buf, int len);
int rdp_client(  void* p, int fd, void* buf, int len);
int rdp_server(  void* p, int fd, void* buf, int len);
int rtmp_client( void* p, int fd, void* buf, int len);
int rtmp_server( void* p, int fd, void* buf, int len);
int sql_client(  void* p, int fd, void* buf, int len);
int sql_server(  void* p, int fd, void* buf, int len);
int vnc_client(  void* p, int fd, void* buf, int len);
int vnc_server(  void* p, int fd, void* buf, int len);
int ws_client(   void* p, int fd, void* buf, int len);
int ws_server(   void* p, int fd, void* buf, int len);
int ws_event(struct event* ev, void* buf);
int wss_client(  void* p, int fd, void* buf, int len);
int wss_server(  void* p, int fd, void* buf, int len);
//debug
int chat_client( void* p, int fd, void* buf, int len);
int chat_server( void* p, int fd, void* buf, int len);
int raw_client(  void* p, int fd, void* buf, int len);
int raw_server(  void* p, int fd, void* buf, int len);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port);
int movsb(void*, void*, int);
int decstr2data(void*, void*);
int hexstr2data(void*, void*);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int startsocket(void* addr, int port, int type);
int stopsocket(u64);
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);




static struct object* obj;
static u8* datahome = 0;




void netmgr_bt(int fd, u8* buf, int len)
{
}
void netmgr_eth(int fd, u8* buf, int len)
{
	raw_server(obj, fd, buf, len);
}
void netmgr_wifi(int fd, u8* buf, int len)
{
}
void netmgr_udp(int fd, u8* buf, int len)
{
	u64 name = obj[fd].name;
	if(_dns_ == name)dns_client(obj, fd, buf, len);
	else if(_DNS_ == name)dns_server(obj, fd, buf, len);
	else if(_hole_ == name)hole_client(obj, fd, buf, len);
	else if(_HOLE_ == name)hole_server(obj, fd, buf, len);
	else if(_tftp_ == name)tftp_client(obj, fd, buf, len);
	else if(_TFTP_ == name)tftp_server(obj, fd, buf, len);
	else say("name=%llx\n",name);
}
u64 netmgr_tcp(int fd, u8* buf, int len)
{
	int ret;
	u64 name = obj[fd].name;
	if(name != 0)goto protocol;




handshake:
	name = ssh_check(obj, fd, buf, len);
	if(name != 0)return name;

	name = tls_check(obj, fd, buf, len);
	if(name != 0)goto protocol;

	name = rtmp_check(obj, fd, buf, len);
	if(name != 0)goto protocol;

	name = http_check(obj, fd, buf, len);
	if(name != 0)goto protocol;

	if(name == 0)return 0;




protocol:
	if(_chat_ == name)name = chat_client(obj, fd, buf, len);
	else if(_CHAT_ == name)name = chat_server(obj, fd, buf, len);

	else if(_http_ == name)name = http_client(obj, fd, buf, len);
	else if(_HTTP_ == name)name = http_server(obj, fd, buf, len);

	else if(_ws_ == name)name = ws_client(obj, fd, buf, len);
	else if(_WS_ == name)name = ws_server(obj, fd, buf, len);

	else if(_tls_ == name)name = tls_client(obj, fd, buf, len);
	else if(_TLS_ == name)name = tls_server(obj, fd, buf, len);

	else if(_https_ == name)name = https_client(obj, fd, buf, len);
	else if(_HTTPS_ == name)name = https_server(obj, fd, buf, len);

	else if(_wss_ == name)name = wss_client(obj, fd, buf, len);
	else if(_WSS_ == name)name = wss_server(obj, fd, buf, len);

	else if(_ssh_ == name)name = ssh_client(obj, fd, buf, len);
	else if(_SSH_ == name)name = ssh_server(obj, fd, buf, len);

	else if(_proxy_ == name)name = proxy_client(obj, fd, buf, len);
	else if(_PROXY_ == name)name = proxy_server(obj, fd, buf, len);

	else if(_rdp_ == name)name = rdp_client(obj, fd, buf, len);
	else if(_RDP_ == name)name = rdp_server(obj, fd, buf, len);

	else if(_vnc_ == name)name = vnc_client(obj, fd, buf, len);
	else if(_VNC_ == name)name = vnc_server(obj, fd, buf, len);

	else if(_rtmp_ == name)name = rtmp_client(obj, fd, buf, len);
	else if(_RTMP_ == name)name = rtmp_server(obj, fd, buf, len);

	else if(_sql_ == name)name = sql_client(obj, fd, buf, len);
	else if(_SQL_ == name)name = sql_server(obj, fd, buf, len);

	else printmemory(buf, len);
	return name;
}




int netmgr_read()
{
	return 0;
}
int netmgr_write(struct event* ev)
{
	int len;
	u64 type;

	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;
	//u64 when = p[3];

	if(why == '+'){}
	else if(why == '-'){}
	else if(why == '@')
	{
		type = obj[where].type;
		//say("%x\n",type);
/*
		//bluetooth
		if(type == 'B')
		{
			netmgr_bt(where, datahome, len);
			return 0;
		}
		//wifi
		if(type == 'W')
		{
			netmgr_wifi(where, datahome, len);
			return 0;
		}
*/
		//raw
		if(type == 'R')
		{
			len = readsocket(where, datahome, 0, 0x100000);
			if(len == 0)return 0;		//sticky
			if(len < 0)goto fail;		//wrong

			netmgr_eth(where, datahome, len);
			return 0;
		}

		//udp
		if( (type == 'U')|(type == 'u') )
		{
		while(1)
		{
			len = readsocket(where, datahome, 0, 0x100000);
			if(len <= 0)return 0;		//sticky

			netmgr_udp(where, datahome, len);
		}
		}

		//read socket
		len = readsocket(where, datahome, 0, 0x100000);
		if(len == 0)return 0;		//sticky
		if(len < 0)goto fail;		//wrong

		//serve socket
		what = netmgr_tcp(where, datahome, len);
		if(what == 0)goto fail;

		//change event
		obj[where].name = what;
		if(_WS_ == what)
		{
			ev->why = len;
			ev->what = hex32('w','@',0,0);
			return 42;
		}
		else if(_http_ == what)
		{
			ev->why = len;
			ev->what = _http_;
		}
	}
	return 0;

fail:
	stopsocket(where);
	return 0;
}
int netmgr_list()
{
	return 0;
}
int netmgr_cd()
{
	return 0;
}
int netmgr_stop(u8* p)
{
	int j;
	u64 fd=0;
	if(p==0)
	{
		for(j=6;j<1024;j++)
		{
			stopsocket(j);
			return 0;
		}
	}

	decstr2data(p, &fd);
	stopsocket(fd);

	//say("[%d]out\n",fd);
	return 0;
}
int netmgr_start()
{
	return 0;
}
void netmgr_delete()
{
}
void netmgr_create(void* w)
{
	obj = w + 0x000000;
	datahome = w + 0x300000;
}
