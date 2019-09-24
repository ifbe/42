#include "libsoft.h"
#define _fat_ hex32('f','a','t',0)
#define _ntfs_ hex32('n','t','f','s')
#define _hfs_ hex32('h','f','s',0)
#define _ext_ hex32('e','x','t',0)
//
#define _echo_ hex32('e','c','h','o')
#define _pump_ hex32('p','u','m','p')
#define _dbgf32_ hex64('d','b','g','f','3','2',0,0)
#define _dbghex_ hex64('d','b','g','h','e','x',0,0)
#define _fftpcm_ hex64('f','f','t','p','c','m',0,0)
//
#define _recut_ hex64('r','e','c','u','t', 0, 0, 0)
#define _reline_ hex64('r','e','l','i','n','e', 0, 0)
#define _reorder_ hex64('r','e','o','r','d','e','r', 0)
//
#define _str2fv_ hex64('s','t','r','2','f','v',0,0)
#define _fv2str_ hex64('f','v','2','s','t','r',0,0)
#define _qu2eu_ hex64('q','u','2','e','u',0,0,0)
//
#define _easyag_  hex64('e','a','s','y','a','g', 0 , 0 )
#define _mahony_  hex64('m','a','h','o','n','y', 0 , 0 )
#define _madgwick_ hex64('m','a','d','g','w','i','c','k')
//file
int fileclient_create(struct element* ele, void* url);
int fileclient_delete(struct element* ele, void* url);
int fileclient_start(struct halfrel* self, struct halfrel* peer);
int fileclient_stop( struct halfrel* self, struct halfrel* peer);
int fileclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fileclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mbrclient_create(struct element* ele, void* url);
int mbrclient_delete(struct element* ele, void* url);
int mbrclient_start(struct halfrel* self, struct halfrel* peer);
int mbrclient_stop( struct halfrel* self, struct halfrel* peer);
int mbrclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mbrclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gptclient_create(struct element* ele, void* url);
int gptclient_delete(struct element* ele, void* url);
int gptclient_start(struct halfrel* self, struct halfrel* peer);
int gptclient_stop( struct halfrel* self, struct halfrel* peer);
int gptclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gptclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int fatclient_create(struct element* ele, void* url);
int fatclient_delete(struct element* ele, void* url);
int fatclient_start(struct halfrel* self, struct halfrel* peer);
int fatclient_stop( struct halfrel* self, struct halfrel* peer);
int fatclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fatclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int ntfsclient_create(struct element* ele, void* url);
int ntfsclient_delete(struct element* ele, void* url);
int ntfsclient_start(struct halfrel* self, struct halfrel* peer);
int ntfsclient_stop( struct halfrel* self, struct halfrel* peer);
int ntfsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int ntfsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hfsclient_create(struct element* ele, void* url);
int hfsclient_delete(struct element* ele, void* url);
int hfsclient_start(struct halfrel* self, struct halfrel* peer);
int hfsclient_stop( struct halfrel* self, struct halfrel* peer);
int hfsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hfsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int extclient_create(struct element* ele, void* url);
int extclient_delete(struct element* ele, void* url);
int extclient_start(struct halfrel* self, struct halfrel* peer);
int extclient_stop( struct halfrel* self, struct halfrel* peer);
int extclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int extclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int echo_create(struct element* ele, void* url);
int echo_delete(struct element* ele, void* url);
int echo_start(struct halfrel* self, struct halfrel* peer);
int echo_stop( struct halfrel* self, struct halfrel* peer);
int echo_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int echo_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int pump_create(struct element* ele, void* url);
int pump_delete(struct element* ele, void* url);
int pump_stop( struct halfrel* self, struct halfrel* peer);
int pump_start(struct halfrel* self, struct halfrel* peer);
int pump_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int pump_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbgf32_create(struct element* ele, void* url);
int dbgf32_delete(struct element* ele, void* url);
int dbgf32_start(struct halfrel* self, struct halfrel* peer);
int dbgf32_stop( struct halfrel* self, struct halfrel* peer);
int dbgf32_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbgf32_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbghex_create(struct element* ele, void* url);
int dbghex_delete(struct element* ele, void* url);
int dbghex_start(struct halfrel* self, struct halfrel* peer);
int dbghex_stop( struct halfrel* self, struct halfrel* peer);
int dbghex_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbghex_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fftpcm_create(struct element* ele, void* url);
int fftpcm_delete(struct element* ele, void* url);
int fftpcm_start(struct halfrel* self, struct halfrel* peer);
int fftpcm_stop( struct halfrel* self, struct halfrel* peer);
int fftpcm_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fftpcm_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int recut_create(struct element* ele, void* url);
int recut_delete(struct element* ele, void* url);
int recut_start(struct halfrel* self, struct halfrel* peer);
int recut_stop( struct halfrel* self, struct halfrel* peer);
int recut_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int recut_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int reline_create(struct element* ele, void* url);
int reline_delete(struct element* ele, void* url);
int reline_start(struct halfrel* self, struct halfrel* peer);
int reline_stop( struct halfrel* self, struct halfrel* peer);
int reline_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int reline_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int reorder_create(struct element* ele, void* url);
int reorder_delete(struct element* ele, void* url);
int reorder_start(struct halfrel* self, struct halfrel* peer);
int reorder_stop( struct halfrel* self, struct halfrel* peer);
int reorder_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int reorder_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int qu2eu_create(struct element* ele, void* url);
int qu2eu_delete(struct element* ele, void* url);
int qu2eu_start(struct halfrel* self, struct halfrel* peer);
int qu2eu_stop( struct halfrel* self, struct halfrel* peer);
int qu2eu_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int qu2eu_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int str2fv_create(struct element* ele, void* url);
int str2fv_delete(struct element* ele, void* url);
int str2fv_start(struct halfrel* self, struct halfrel* peer);
int str2fv_stop( struct halfrel* self, struct halfrel* peer);
int str2fv_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int str2fv_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fv2str_create(struct element* ele, void* url);
int fv2str_delete(struct element* ele, void* url);
int fv2str_start(struct halfrel* self, struct halfrel* peer);
int fv2str_stop( struct halfrel* self, struct halfrel* peer);
int fv2str_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fv2str_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int easyag_create(struct element* ele, void* url);
int easyag_delete(struct element* ele, void* url);
int easyag_start(struct halfrel* self, struct halfrel* peer);
int easyag_stop( struct halfrel* self, struct halfrel* peer);
int easyag_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int easyag_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mahony_create(struct element* ele, void* url);
int mahony_delete(struct element* ele, void* url);
int mahony_start(struct halfrel* self, struct halfrel* peer);
int mahony_stop( struct halfrel* self, struct halfrel* peer);
int mahony_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mahony_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int madgwick_create(struct element* ele, void* url);
int madgwick_delete(struct element* ele, void* url);
int madgwick_start(struct halfrel* self, struct halfrel* peer);
int madgwick_stop( struct halfrel* self, struct halfrel* peer);
int madgwick_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int madgwick_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//uart.gcode
int gcodeclient_create(struct element* ele, void* url);
int gcodeclient_delete(struct element* ele, void* url);
int gcodeclient_start(struct halfrel* self, struct halfrel* peer);
int gcodeclient_stop( struct halfrel* self, struct halfrel* peer);
int gcodeclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gcodeclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gcodeserver_create(struct element* ele, void* url);
int gcodeserver_delete(struct element* ele, void* url);
int gcodeserver_start(struct halfrel* self, struct halfrel* peer);
int gcodeserver_stop( struct halfrel* self, struct halfrel* peer);
int gcodeserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gcodeserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//uart.gps
int gpsclient_create(struct element* ele, void* url);
int gpsclient_delete(struct element* ele, void* url);
int gpsclient_start(struct halfrel* self, struct halfrel* peer);
int gpsclient_stop( struct halfrel* self, struct halfrel* peer);
int gpsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gpsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gpsserver_create(struct element* ele, void* url);
int gpsserver_delete(struct element* ele, void* url);
int gpsserver_start(struct halfrel* self, struct halfrel* peer);
int gpsserver_stop( struct halfrel* self, struct halfrel* peer);
int gpsserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gpsserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//socket
int hackclient_create(struct element* ele, void* url);
int hackclient_delete(struct element* ele, void* url);
int hackclient_start(struct halfrel* self, struct halfrel* peer);
int hackclient_stop( struct halfrel* self, struct halfrel* peer);
int hackclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hackclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hackserver_create(struct element* ele, void* url);
int hackserver_delete(struct element* ele, void* url);
int hackserver_start(struct halfrel* self, struct halfrel* peer);
int hackserver_stop( struct halfrel* self, struct halfrel* peer);
int hackserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hackserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//udp.dns
int dnsclient_create(struct element* ele, void* url);
int dnsclient_delete(struct element* ele, void* url);
int dnsclient_start(struct halfrel* self, struct halfrel* peer);
int dnsclient_stop( struct halfrel* self, struct halfrel* peer);
int dnsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dnsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dnsserver_create(struct element* ele, void* url);
int dnsserver_delete(struct element* ele, void* url);
int dnsserver_start(struct halfrel* self, struct halfrel* peer);
int dnsserver_stop( struct halfrel* self, struct halfrel* peer);
int dnsserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dnsserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//udp.tftp
int tftpclient_create(struct element* ele, void* url);
int tftpclient_delete(struct element* ele, void* url);
int tftpclient_start(struct halfrel* self, struct halfrel* peer);
int tftpclient_stop( struct halfrel* self, struct halfrel* peer);
int tftpclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tftpclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tftpserver_create(struct element* ele, void* url);
int tftpserver_delete(struct element* ele, void* url);
int tftpserver_start(struct halfrel* self, struct halfrel* peer);
int tftpserver_stop( struct halfrel* self, struct halfrel* peer);
int tftpserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tftpserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//udp.quic
int quicclient_create(struct element* ele, void* url);
int quicclient_delete(struct element* ele, void* url);
int quicclient_start(struct halfrel* self, struct halfrel* peer);
int quicclient_stop( struct halfrel* self, struct halfrel* peer);
int quicclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicserver_create(struct element* ele, void* url);
int quicserver_delete(struct element* ele, void* url);
int quicserver_start(struct halfrel* self, struct halfrel* peer);
int quicserver_stop( struct halfrel* self, struct halfrel* peer);
int quicserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicmaster_create(struct element* ele, void* url);
int quicmaster_delete(struct element* ele, void* url);
int quicmaster_start(struct halfrel* self, struct halfrel* peer);
int quicmaster_stop( struct halfrel* self, struct halfrel* peer);
int quicmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.ssh
int sshclient_create(struct element* ele, void* url);
int sshclient_delete(struct element* ele, void* url);
int sshclient_start(struct halfrel* self, struct halfrel* peer);
int sshclient_stop( struct halfrel* self, struct halfrel* peer);
int sshclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshserver_create(struct element* ele, void* url);
int sshserver_delete(struct element* ele, void* url);
int sshserver_start(struct halfrel* self, struct halfrel* peer);
int sshserver_stop( struct halfrel* self, struct halfrel* peer);
int sshserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshmaster_create(struct element* ele, void* url);
int sshmaster_delete(struct element* ele, void* url);
int sshmaster_start(struct halfrel* self, struct halfrel* peer);
int sshmaster_stop( struct halfrel* self, struct halfrel* peer);
int sshmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.telnet
int telnetclient_create(struct element* ele, void* url);
int telnetclient_delete(struct element* ele, void* url);
int telnetclient_start(struct halfrel* self, struct halfrel* peer);
int telnetclient_stop( struct halfrel* self, struct halfrel* peer);
int telnetclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetserver_create(struct element* ele, void* url);
int telnetserver_delete(struct element* ele, void* url);
int telnetserver_start(struct halfrel* self, struct halfrel* peer);
int telnetserver_stop( struct halfrel* self, struct halfrel* peer);
int telnetserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetmaster_create(struct element* ele, void* url);
int telnetmaster_delete(struct element* ele, void* url);
int telnetmaster_start(struct halfrel* self, struct halfrel* peer);
int telnetmaster_stop( struct halfrel* self, struct halfrel* peer);
int telnetmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.http
int httpclient_create(struct element* ele, void* url);
int httpclient_delete(struct element* ele, void* url);
int httpclient_start(struct halfrel* self, struct halfrel* peer);
int httpclient_stop( struct halfrel* self, struct halfrel* peer);
int httpclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpserver_create(struct element* ele, void* url);
int httpserver_delete(struct element* ele, void* url);
int httpserver_start(struct halfrel* self, struct halfrel* peer);
int httpserver_stop( struct halfrel* self, struct halfrel* peer);
int httpserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpmaster_create(struct element* ele, void* url);
int httpmaster_delete(struct element* ele, void* url);
int httpmaster_start(struct halfrel* self, struct halfrel* peer);
int httpmaster_stop( struct halfrel* self, struct halfrel* peer);
int httpmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.ws
int wsclient_create(struct element* ele, void* url);
int wsclient_delete(struct element* ele, void* url);
int wsclient_start(struct halfrel* self, struct halfrel* peer);
int wsclient_stop( struct halfrel* self, struct halfrel* peer);
int wsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsserver_create(struct element* ele, void* url);
int wsserver_delete(struct element* ele, void* url);
int wsserver_start(struct halfrel* self, struct halfrel* peer);
int wsserver_stop( struct halfrel* self, struct halfrel* peer);
int wsserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsmaster_create(struct element* ele, void* url);
int wsmaster_delete(struct element* ele, void* url);
int wsmaster_start(struct halfrel* self, struct halfrel* peer);
int wsmaster_stop( struct halfrel* self, struct halfrel* peer);
int wsmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.tls
int tlsclient_create(struct element* ele, void* url);
int tlsclient_delete(struct element* ele, void* url);
int tlsclient_start(struct halfrel* self, struct halfrel* peer);
int tlsclient_stop( struct halfrel* self, struct halfrel* peer);
int tlsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsserver_create(struct element* ele, void* url);
int tlsserver_delete(struct element* ele, void* url);
int tlsserver_start(struct halfrel* self, struct halfrel* peer);
int tlsserver_stop( struct halfrel* self, struct halfrel* peer);
int tlsserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsmaster_create(struct element* ele, void* url);
int tlsmaster_delete(struct element* ele, void* url);
int tlsmaster_start(struct halfrel* self, struct halfrel* peer);
int tlsmaster_stop( struct halfrel* self, struct halfrel* peer);
int tlsmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.serve
int serveclient_create(struct element* ele, void* url);
int serveclient_delete(struct element* ele, void* url);
int serveclient_start(struct halfrel* self, struct halfrel* peer);
int serveclient_stop( struct halfrel* self, struct halfrel* peer);
int serveclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int serveclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int serveserver_create(struct element* ele, void* url);
int serveserver_delete(struct element* ele, void* url);
int serveserver_start(struct halfrel* self, struct halfrel* peer);
int serveserver_stop( struct halfrel* self, struct halfrel* peer);
int serveserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int serveserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int servemaster_create(struct element* ele, void* url);
int servemaster_delete(struct element* ele, void* url);
int servemaster_start(struct halfrel* self, struct halfrel* peer);
int servemaster_stop( struct halfrel* self, struct halfrel* peer);
int servemaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int servemaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int parseurl(u8* buf, int len, u8* addr, int* port);
int ncmp(void*, void*, int);
int cmp(void*, void*);



#define datahome qqq
static struct object* obj = 0;
static void* ppp = 0;
static struct element* ele = 0;
static void* qqq = 0;
static int elelen = 0;
static int qqqlen = 0;
void* allocelement()
{
	void* addr = &ele[elelen];
	elelen += 1;
	return addr;
}
int parsetypefromurl(u8* url, u8* type)
{
	int j,k;
	for(k=0;k<8;k++)type[k] = 0;
	for(j=0;j<16;j++)
	{
		if(0 == ncmp(url+j, "://", 3))
		{
			for(k=0;k<j;k++)type[k] = url[k];
			return j+3;
		}
	}
	return 0;
}




int arteryread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct element* ele = (void*)(self->chip);
	switch(ele->type){
		case _echo_:echo_read(self, peer, arg, idx, buf, len);break;
		case _pump_:pump_read(self, peer, arg, idx, buf, len);break;
		case _dbgf32_:dbgf32_read(self, peer, arg, idx, buf, len);break;
		case _dbghex_:dbghex_read(self, peer, arg, idx, buf, len);break;
		case _fftpcm_:fftpcm_read(self, peer, arg, idx, buf, len);break;

		case _recut_:recut_read(self, peer, arg, idx, buf, len);break;
		case _reline_:reline_read(self, peer, arg, idx, buf, len);break;
		case _reorder_:reorder_read(self, peer, arg, idx, buf, len);break;

		case _qu2eu_:qu2eu_read(self, peer, arg, idx, buf, len);break;
		case _str2fv_:str2fv_read(self, peer, arg, idx, buf, len);break;
		case _fv2str_:fv2str_read(self, peer, arg, idx, buf, len);break;

		case _easyag_:easyag_read(self, peer, arg, idx, buf, len);break;
		case _mahony_:mahony_read(self, peer, arg, idx, buf, len);break;
		case _madgwick_:madgwick_read(self, peer, arg, idx, buf, len);break;

		case _gps_:gpsclient_read(self, peer, arg, idx, buf, len);break;

		case _HTTP_:httpmaster_read(self, peer, arg, idx, buf, len);break;
		case _Http_:httpserver_read(self, peer, arg, idx, buf, len);break;
		case _http_:httpclient_read(self, peer, arg, idx, buf, len);break;
		case _WS_:wsmaster_read(self, peer, arg, idx, buf, len);break;
		case _Ws_:wsserver_read(self, peer, arg, idx, buf, len);break;
		case _ws_:wsclient_read(self, peer, arg, idx, buf, len);break;
	}
	return 0;
}
int arterywrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct element* ele = (void*)(self->chip);
	//say("@arterywrite\n");
	switch(ele->type){
		case _echo_:return echo_write(self, peer, arg, idx, buf, len);break;
		case _pump_:return pump_write(self, peer, arg, idx, buf, len);break;
		case _dbgf32_:return dbgf32_write(self, peer, arg, idx, buf, len);break;
		case _dbghex_:return dbghex_write(self, peer, arg, idx, buf, len);break;
		case _fftpcm_:return fftpcm_write(self, peer, arg, idx, buf, len);break;

		case _recut_:return recut_write(self, peer, arg, idx, buf, len);break;
		case _reline_:return reline_write(self, peer, arg, idx, buf, len);break;
		case _reorder_:return reorder_write(self, peer, arg, idx, buf, len);break;

		case _qu2eu_:return qu2eu_write(self, peer, arg, idx, buf, len);break;
		case _str2fv_:return str2fv_write(self, peer, arg, idx, buf, len);break;
		case _fv2str_:return fv2str_write(self, peer, arg, idx, buf, len);break;

		case _easyag_:return easyag_write(self, peer, arg, idx, buf, len);break;
		case _mahony_:return mahony_write(self, peer, arg, idx, buf, len);break;
		case _madgwick_:return madgwick_write(self, peer, arg, idx, buf, len);break;

		case _gps_:return gpsclient_write(self, peer, arg, idx, buf, len);break;

		case _HTTP_:return httpmaster_write(self, peer, arg, idx, buf, len);break;
		case _Http_:return httpserver_write(self, peer, arg, idx, buf, len);break;
		case _http_:return httpclient_write(self, peer, arg, idx, buf, len);break;
		case _WS_:return wsmaster_write(self, peer, arg, idx, buf, len);break;
		case _Ws_:return wsserver_write(self, peer, arg, idx, buf, len);break;
		case _ws_:return wsclient_write(self, peer, arg, idx, buf, len);break;
	}
	return 0;
}
int arterystop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int arterystart(struct halfrel* self, struct halfrel* peer)
{
	struct element* ele;
	say("@arterystart\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	switch(ele->type){
		case _http_:return httpclient_start(self, peer);break;
	}
	return 0;
}




int arterydelete(void* ele)
{
	return 0;
}
void* arterycreate(u64 type, void* argstr)
{
	int j,fd,ret,port;
	struct element* e;
	struct element* f;
	u8* url = argstr;
	if(0 == type)
	{
		ret = parsetypefromurl(url, (void*)&type);
		if(0 == ret)return 0;	//unknown

		url += ret;
	}

	//file system
	if(_file_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		fileclient_create(e, url);
		return e;
	}
	if(_fat_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _fat_;
		fatclient_create(e, url);
		return e;
	}
	if(_ntfs_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ntfs_;
		ntfsclient_create(e, url);
		return e;
	}
	if(_hfs_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _hfs_;
		hfsclient_create(e, url);
		return e;
	}
	if(_ext_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ext_;
		extclient_create(e, url);
		return e;
	}

	//test
	if(_echo_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _echo_;
		echo_create(e, url);
		return e;
	}
	if(_pump_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _pump_;
		pump_create(e, url);
		return e;
	}
	if(_dbgf32_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _dbgf32_;
		dbgf32_create(e, url);
		return e;
	}
	if(_dbghex_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _dbghex_;
		dbghex_create(e, url);
		return e;
	}
	if(_fftpcm_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _fftpcm_;
		fftpcm_create(e, url);
		return e;
	}

	if(_recut_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _recut_;
		recut_create(e, url);
		return e;
	}
	if(_reline_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _reline_;
		reline_create(e, url);
		return e;
	}
	if(_reorder_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _reorder_;
		reorder_create(e, url);
		return e;
	}

	//
	if(_qu2eu_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _qu2eu_;
		qu2eu_create(e, url);
		return e;
	}
	if(_str2fv_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _str2fv_;
		str2fv_create(e, url);
		return e;
	}
	if(_fv2str_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _fv2str_;
		fv2str_create(e, url);
		return e;
	}

	//ahrs
	if(_easyag_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _easyag_;
		easyag_create(e, url);
		return e;
	}
	if(_mahony_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _mahony_;
		mahony_create(e, url);
		return e;
	}
	if(_madgwick_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _madgwick_;
		madgwick_create(e, url);
		return e;
	}

	//gcode: client,server
	if(_gcode_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _gcode_;
		gcodeclient_create(e, url);
		return e;
	}
	if(_Gcode_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Gcode_;
		gcodeserver_create(e, url);
		return e;
	}

	//gps: client,server
	if(_gps_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _gps_;
		gpsclient_create(e, url);
		return e;
	}
	if(_Gps_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Gps_;
		gpsserver_create(e, url);
		return e;
	}

	//simple control over message
	if(_HACK_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _HACK_;
		if(url)hackserver_create(e, url);
		return e;
	}
	if(_hack_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _hack_;
		if(url)hackclient_create(e, url);
		return e;
	}

	//dns
	if(_Dns_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Dns_;
		if(url)dnsserver_create(e, url);
		return e;
	}
	if(_dns_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _dns_;
		if(url)dnsclient_create(e, url);
		return e;
	}

	//tftp: client,server
	if(_tftp_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _tftp_;
		if(url)tftpclient_create(e, url);
		return e;
	}
	if(_Tftp_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Tftp_;
		if(url)tftpserver_create(e, url);
		return e;
	}

	//quic: master,server,client
	if(_QUIC_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _QUIC_;
		if(url)quicmaster_create(e, url);
		return e;
	}
	if(_Quic_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Quic_;
		if(url)quicserver_create(e, url);
		return e;
	}
	if(_quic_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _quic_;
		if(url)quicclient_create(e, url);
		return e;
	}

	//serve: master,server,client
	if(_SERVE_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _SERVE_;
		if(url)servemaster_create(e, url);

		return e;
	}
	if(_Serve_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Serve_;
		if(url)serveserver_create(e, url);

		return e;
	}
	if(_serve_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _serve_;
		if(url)serveclient_create(e, url);

		return e;
	}

	//ssh: master,server,client
	if(_SSH_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _SSH_;
		if(url)sshmaster_create(e, url);

		return e;
	}
	if(_Ssh_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Ssh_;
		if(url)sshserver_create(e, url);

		return e;
	}
	if(_ssh_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ssh_;
		if(url)sshclient_create(e, url);

		return e;
	}

	//telnet: master,server,client
	if(_TELNET_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _TELNET_;
		if(url)telnetmaster_create(e, url);

		return e;
	}
	if(_Telnet_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Telnet_;
		if(url)telnetserver_create(e, url);

		return e;
	}
	if(_telnet_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _telnet_;
		if(url)telnetclient_create(e, url);

		return e;
	}

	//http: master,server,client
	if(_HTTP_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _HTTP_;
		if(url)httpmaster_create(e, url);
		return e;
	}
	if(_Http_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Http_;
		if(url)httpserver_create(e, url);
		return e;
	}
	if(_http_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _http_;
		if(url)httpclient_create(e, url);

		return e;
	}

	//ws: master,server,client
	if(_WS_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _WS_;
		if(url)wsmaster_create(e, url);

		return e;
	}
	if(_Ws_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Ws_;
		if(url)wsserver_create(e, url);

		return e;
	}
	if(_ws_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ws_;
		if(url)wsclient_create(e, url);

		return e;
	}

	//tls: master,server,client
	if(_TLS_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _TLS_;
		if(url)tlsmaster_create(e, url);

		return e;
	}
	if(_Tls_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Tls_;
		if(url)tlsserver_create(e, url);

		return e;
	}
	if(_tls_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _tls_;
		if(url)tlsclient_create(e, url);

		return e;
	}

	if(_https_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _http_;
		if(url)
		{
			f = arterycreate(_tls_, url);
			relationcreate(e, 0, _art_, 0, f, 0, _art_, 0);
		}

		return e;
	}
	return 0;
}
void* arterymodify(int argc, char** argv)
{
	int j;
	u64 name = 0;
	u8* tmp = (u8*)&name;
	if(argc < 2)return 0;
//say("%s,%s,%s,%s\n",argv[0],argv[1],argv[2],argv[3]);
	if(0 == ncmp(argv[1], "create", 6))
	{
		for(j=0;j<8;j++)
		{
			if(argv[2][j] <= 0x20)break;
			tmp[j] = argv[2][j];
		}
		say("%llx,%llx\n",name, argv[3]);
		arterycreate(name, argv[3]);
	}
	return 0;
}
void* arterysearch(u8* buf, int len)
{
	int j,k=0;
	struct element* art;
	for(j=0;j<0x1000;j++)
	{
		art = &ele[j];
		if(0 == art->type)continue;

		k++;
		say("[%04x]: %.8s, %.8s\n", j,
			&art->tier, &art->type);
	}

	if(0 == k)say("empth artery\n");
	return 0;
}




int arteryevent(struct event* ev)
{
	return 0;
}
void freeartery()
{
	//say("[8,c):freeing artery\n");

	qqq = 0;
	ele = 0;
	ppp = 0;
	obj = 0;
}
void initartery(void* addr)
{
	int j;
	obj = addr+0x000000;
	ele = addr+0x100000;
	ppp = addr+0x200000;
	qqq = addr+0x300000;

#define max (0x100000/sizeof(struct element))
	for(j=0;j<max;j++)ele[j].tier = _art_;

	//arterycreate(0, (u8*)"HACK://0.0.0.0:2222");
	//arterycreate(0, (u8*)"QUIC://0.0.0.0:4444");
	//arterycreate(0,  (u8*)"SSH://0.0.0.0:2222");
	//arterycreate(0, (u8*)"HTTP://0.0.0.0:4444");
	//say("[8,c):inited artery\n");
}
