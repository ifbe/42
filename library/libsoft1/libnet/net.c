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
int decstr2data(void*, void*);
int hexstr2data(void*, void*);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int startsocket(void* addr, int port, int type);
int stopsocket(u64);
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);




void netmgr_bt(struct object* obj, int fd, u8* buf, int len)
{
}
void netmgr_eth(struct object* obj, int fd, u8* buf, int len)
{
	raw_server(obj, fd, buf, len);
}
void netmgr_wifi(struct object* obj, int fd, u8* buf, int len)
{
}
void netmgr_udp(struct object* obj, int fd, u8* buf, int len)
{
	u64 name = obj[fd].name;
	printmemory(buf, len);

	if(_dns_ == name)dns_client(obj, fd, buf, len);
	else if(_DNS_ == name)dns_server(obj, fd, buf, len);
	else if(_hole_ == name)hole_client(obj, fd, buf, len);
	else if(_HOLE_ == name)hole_server(obj, fd, buf, len);
	else if(_tftp_ == name)tftp_client(obj, fd, buf, len);
	else if(_TFTP_ == name)tftp_server(obj, fd, buf, len);
	else say("name=%llx\n",name);
}
u64 netmgr_tcp(struct object* obj, int fd, u8* buf, int len)
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