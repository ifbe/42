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
int tftp_write(void*, int);
int tls_write_client_hello(void*, int);
int secureshell_write_handshake(void*, int);
int websocket_write_handshake(void*, int);
int http_write_request(void*, int, void*, void*);
int dns_write_query(void*, int, void*, int);
void quic_start();
void quic_stop();
void ssh_start();
void ssh_stop();
void tls_start();
void tls_stop();
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




//----------------uppercase = server, lowercase = client----------------
//link
#define ARP 0x505241		//s
#define arp 0x707261		//c
#define EAP 0x504146		//s
#define eap 0x706166		//c
#define IPX 0x585049		//s
#define ipx 0x787069		//c
#define PPPOE 0x454f505050	//s
#define pppoe 0x656f707070	//c
#define WOL 0x4c4f57		//s
#define wol 0x6c6f77		//c
//ip
#define ICMP 0x504d4349		//s
#define icmp 0x706d6369		//c
#define IGMP 0x504d4749		//s
#define igmp 0x706d6769		//c
//tran
#define QUIC 0x43495551		//s
#define quic 0x63697571		//c
#define SOCKS5 0x35534b434f53	//s
#define socks5 0x35736b636f73	//c
#define SSH 0x485353		//s
#define ssh 0x687373		//c
#define TLS 0x534c54		//s
#define tls 0x736c74		//c
//udp
#define BOOTP 0x50544f4f42	//s
#define bootp 0x70746f6f62	//c
#define DNS 0x534e44		//s
#define dns 0x736e64		//c
#define TFTP 0x50544654		//s
#define tftp 0x70746674		//c
#define WEBRTC 0x435452424557	//s
#define webrtc 0x637472626577	//c
//hole
#define HOLE 0x454c4f48		//s
#define hole 0x656c6f68		//c
#define STUN 0x4e555453		//s
#define stun 0x6e757473		//c
#define TURN 0x4e525554		//s
#define turn 0x6e727574		//c
#define ICE 0x454349		//s
#define ice 0x656369		//c
//http family
#define HTTP 0x50545448		//parse, reply
#define http 0x70747468		//req url, get reply
#define HTTPS 0x5350545448	//parse, reply
#define https 0x7370747468	//req url, get reply
#define WS 0x5357		//s 
#define ws 0x7377		//c
#define WSS 0x535357		//s
#define wss 0x737377		//c
//live stream
#define FLV 0x564c46		//s
#define flv 0x766c66		//c
#define RTMP 0x504d5452		//s
#define rtmp 0x706d7472		//c
//remote desktop
#define RDP 0x504452		//s
#define rdp 0x706472		//c
#define VNC 0x434e56		//s
#define vnc 0x636e76		//c
#define SPICE 0x4543495053	//s
#define spice 0x6563697073	//c
//download
#define ED2K 0x4b324445		//s
#define ed2k 0x6b326465		//c
#define MAGNET 0x54454e47414d	//s
#define magnet 0x74656e67616d	//c
#define TORRENT 0x544e4552524f54	//s
#define torrent 0x746e6572726f74	//c
//
#define PROXY 0x59584f5250	//s
#define proxy 0x79786f7270	//c
#define SQL 0x4c5153		//s
#define sql 0x6c7173		//c
//application
#define CHAT 0x54414843		//check, broadcast
#define chat 0x74616863		//send user msg
#define RAW 0x574152		//parse, print
#define raw 0x776172		//send raw packet
//----------------uppercase = server, lowercase = client----------------




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
	u64 type = obj[fd].type;
	if(dns == type)dns_client(obj, fd, buf, len);
	else if(DNS == type)dns_server(obj, fd, buf, len);
	else if(type == hole)hole_client(obj, fd, buf, len);
	else if(type == HOLE)hole_server(obj, fd, buf, len);
	else if(type == tftp)tftp_client(obj, fd, buf, len);
	else if(type == TFTP)tftp_server(obj, fd, buf, len);
	else say("type=%llx\n",type);
}
u64 netmgr_tcp(int fd, u8* buf, int len)
{
	int ret;
	u64 type = obj[fd].type;
	if(type != 0)goto protocol;




handshake:
	obj[fd].stage1 = 0;

	type = ssh_check(obj, fd, buf, len);
	if(type != 0)return type;

	type = tls_check(obj, fd, buf, len);
	if(type != 0)goto protocol;

	type = rtmp_check(obj, fd, buf, len);
	if(type != 0)goto protocol;

	type = http_check(obj, fd, buf, len);
	if(type != 0)goto protocol;

	if(type == 0)return 0;




protocol:
	if(chat == type)type = chat_client(obj, fd, buf, len);
	else if(CHAT == type)type = chat_server(obj, fd, buf, len);

	else if(http == type)type = http_client(obj, fd, buf, len);
	else if(HTTP == type)type = http_server(obj, fd, buf, len);

	else if(ws == type)type = ws_client(obj, fd, buf, len);
	else if(WS == type)type = ws_server(obj, fd, buf, len);

	else if(tls == type)type = tls_client(obj, fd, buf, len);
	else if(TLS == type)type = tls_server(obj, fd, buf, len);

	else if(https == type)type = https_client(obj, fd, buf, len);
	else if(HTTPS == type)type = https_server(obj, fd, buf, len);

	else if(wss == type)type = wss_client(obj, fd, buf, len);
	else if(WSS == type)type = wss_server(obj, fd, buf, len);

	else if(ssh == type)type = ssh_client(obj, fd, buf, len);
	else if(SSH == type)type = ssh_server(obj, fd, buf, len);

	else if(proxy == type)type = proxy_client(obj, fd, buf, len);
	else if(PROXY == type)type = proxy_server(obj, fd, buf, len);

	else if(rdp == type)type = rdp_client(obj, fd, buf, len);
	else if(RDP == type)type = rdp_server(obj, fd, buf, len);

	else if(vnc == type)type = vnc_client(obj, fd, buf, len);
	else if(VNC == type)type = vnc_server(obj, fd, buf, len);

	else if(rtmp == type)type = rtmp_client(obj, fd, buf, len);
	else if(RTMP == type)type = rtmp_server(obj, fd, buf, len);

	else if(sql == type)type = sql_client(obj, fd, buf, len);
	else if(SQL == type)type = sql_server(obj, fd, buf, len);

	else printmemory(buf, len);
	return type;
}
int network_explain(struct event* ev)
{
	int len;
	u64 sock;

	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;
	//u64 when = p[3];

	if(why == '+'){}
	else if(why == '-'){}
	else if(why == '@')
	{
		sock = obj[where].sock;
		//say("%x\n",sock);
/*
		//bluetooth
		if(sock == 'B')
		{
			netmgr_bt(where, datahome, len);
			return 0;
		}
		//wifi
		if(sock == 'W')
		{
			netmgr_wifi(where, datahome, len);
			return 0;
		}
*/
		//raw
		if(sock == 'R')
		{
			len = readsocket(where, datahome, 0, 0x100000);
			if(len == 0)return 0;		//sticky
			if(len < 0)goto fail;		//wrong

			netmgr_eth(where, datahome, len);
			return 0;
		}

		//udp
		if( (sock == 'U')|(sock == 'u') )
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
		obj[where].type = what;
		if((what == WS) && (obj[where].stage1 >= 4))
		{
			ws_event(ev, datahome);
			return 42;
		}
		else if(what == http)
		{
			ev->why = len;
			ev->what = http;
		}
	}
	return 0;

fail:
	stopsocket(where);
	return 0;
}








int netmgr_read()
{
	return 0;
}
int netmgr_write(u8* p)
{
	//
	int ret;
	u64 fd=0;
	u64 tmp=CHAT;

	u8 type[16];
	u8 addr[16];
	u8* url;
	int port;

	if(p == 0)return 0;
	type[0] = addr[0] = 0;
	port = 0;


	//parse
	ret = buf2net(p, 256, type, addr, &port);
	if(ret <= 0)
	{
		say("%s\n", p);
		return 0;
	}
	url = p+ret;


	//
	if(addr[0] == 0)mysnprintf(addr, 10, "0.0.0.0");
	if(port == 0)port = 2222;
	//say("%s:%d\n",addr, port);

	if(ncmp(type, "RAW", 3) == 0)		//raw server
	{
		fd = startsocket(addr, port, 'R');
		if(fd == 0)return 0;

		tmp = RAW;
	}
	else if(ncmp(type, "raw", 3) == 0)	//raw client
	{
		fd = startsocket(addr, port, 'r');
		if(fd == 0)return 0;

		tmp = raw;
	}


	else if(ncmp(type, "UDP", 3) == 0)	//udp server
	{
		fd = startsocket(addr, port, 'U');
		if(fd == 0)return 0;
	}
	else if(ncmp(type, "udp", 3) == 0)	//udp client
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;
	}
	else if(ncmp(type, "DNS", 3) == 0)	//DNS server
	{
		fd = startsocket(addr, port, 'U');
		if(fd == 0)return 0;

		tmp = DNS;
	}
	else if(ncmp(type, "dns", 3) == 0)	//DNS client
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;

		ret = dns_write_query(datahome, 666, url+1, 666);
		writesocket(fd, datahome, 0, ret);
		tmp = dns;
	}
	else if(ncmp(type, "HOLE", 4) == 0)	//p2p server
	{
		fd = startsocket(addr, port, 'U');
		if(fd == 0)return 0;

		tmp = HOLE;
	}
	else if(ncmp(type, "hole", 4) == 0)	//p2p client
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;

		writesocket(fd, url+1, 0, 16);
		tmp = hole;
	}
	else if(ncmp(type, "TFTP", 3) == 0)	//tftp server
	{
		fd = startsocket(addr, port, 'U');
		if(fd == 0)return 0;
	}
	else if(ncmp(type, "tftp", 4) == 0)	//tftp client
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;

		ret = tftp_write(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);

		tmp = CHAT;
	}


	else if(ncmp(type, "TCP", 3) == 0)	//tcp server
	{
		fd = startsocket(addr, port, 'T');
		if(fd == 0)return 0;
	}
	else if(ncmp(type, "tcp", 3) == 0)	//tcp client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;
	}
	else if(ncmp(type, "SSH", 3) == 0)	//ssh server
	{
		ssh_start();
		fd = startsocket(addr, port, 'T');
		if(fd == 0)return 0;
	}
	else if(ncmp(type, "ssh", 3) == 0)	//ssh client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		ret = secureshell_write_handshake(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);

		tmp = ssh;
	}
	else if(ncmp(type, "TLS", 3) == 0)	//tls server
	{
		tls_start();
		fd = startsocket(addr, port, 'T');
		if(fd == 0)return 0;
	}
	else if(ncmp(type, "tls", 3) == 0)	//tls client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		ret = tls_write_client_hello(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);

		tmp = tls;
	}
	else if(ncmp(type, "sql", 3) == 0)	//sql client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		tmp = sql;
	}
	else if(ncmp(type, "http", 4) == 0)	//http client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		ret = http_write_request(datahome, 0x100000, url, addr);
		ret = writesocket(fd, datahome, 0, ret);

		tmp = http;
	}
	else if(ncmp(type, "ws", 2) == 0)	//ws client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		ret = websocket_write_handshake(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);

		tmp = ws;
	}


	obj[fd].type = tmp;
	obj[fd].stage1 = 0;
	return fd;
}
int netmgr_list(u8* p)
{
	int j;
	u64 fd=0;
	if(p==0)
	{
		for(j=0;j<4096;j++)
		{
			if(obj[j].sock == 0)continue;

			say("%d:	%llx,%llx\n",
				j, obj[j].sock, obj[j].type);
		}
		return 0;
	}

	decstr2data(p, &fd);
	say("%llx,%llx\n", obj[fd].sock, obj[fd].type);
	return 0;
}
int netmgr_choose(u8* p)
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
int netmgr_start(u64 fd)
{
	//say("[%d]in\n",fd);
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
