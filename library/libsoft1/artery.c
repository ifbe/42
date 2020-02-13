#include "libsoft.h"
#define _ann_ hex32('a','n','n',0)
//
#define _fat_ hex32('f','a','t',0)
#define _ntfs_ hex32('n','t','f','s')
#define _hfs_ hex32('h','f','s',0)
#define _ext_ hex32('e','x','t',0)
//
#define _gcode_ hex64('g','c','o','d','e',0,0,0)
#define _Gcode_ hex64('G','c','o','d','e',0,0,0)
#define _modbus_ hex64('m','o','d','b','u','s',0,0)
#define _Modbus_ hex64('M','o','d','b','u','s',0,0)
#define _mavlink_ hex64('m','a','v','l','i','n','k',0)
#define _Mavlink_ hex64('M','a','v','l','i','n','k',0)
#define _nema0183_ hex64('n','e','m','a','0','1','8','3')
#define _Nema0183_ hex64('N','e','m','a','0','1','8','3')
//
#define _control_ hex64('c','o','n','t','r','o','l', 0)
#define _search_ hex64('s','e','a','r','c','h', 0, 0)
#define _vt100_ hex64('v','t','1','0','0', 0, 0, 0)
//
#define _fftpcm_ hex64('f','f','t','p','c','m',0,0)
#define _fftrgb_ hex64('f','f','t','r','g','b',0,0)
#define _rotate_ hex64('r','o','t','a','t','e',0,0)
//
#define _echo_ hex32('e','c','h','o')
#define _pump_ hex32('p','u','m','p')
#define _goslow_ hex64('g','o','s','l','o','w',0,0)
#define _dbgf32_ hex64('d','b','g','f','3','2',0,0)
#define _dbghex_ hex64('d','b','g','h','e','x',0,0)
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
int ann_create(struct artery* ele, void* url, int argc, u8** argv);
int ann_delete(struct artery* ele, void* url);
int ann_linkup(struct halfrel* self, struct halfrel* peer);
int ann_discon( struct halfrel* self, struct halfrel* peer);
int ann_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int ann_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//file
int fileclient_create(struct artery* ele, void* url, int argc, u8** argv);
int fileclient_delete(struct artery* ele, void* url);
int fileclient_linkup(struct halfrel* self, struct halfrel* peer);
int fileclient_discon( struct halfrel* self, struct halfrel* peer);
int fileclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fileclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mbrclient_create(struct artery* ele, void* url, int argc, u8** argv);
int mbrclient_delete(struct artery* ele, void* url);
int mbrclient_linkup(struct halfrel* self, struct halfrel* peer);
int mbrclient_discon( struct halfrel* self, struct halfrel* peer);
int mbrclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mbrclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gptclient_create(struct artery* ele, void* url, int argc, u8** argv);
int gptclient_delete(struct artery* ele, void* url);
int gptclient_linkup(struct halfrel* self, struct halfrel* peer);
int gptclient_discon( struct halfrel* self, struct halfrel* peer);
int gptclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gptclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int fatclient_create(struct artery* ele, void* url, int argc, u8** argv);
int fatclient_delete(struct artery* ele, void* url);
int fatclient_linkup(struct halfrel* self, struct halfrel* peer);
int fatclient_discon( struct halfrel* self, struct halfrel* peer);
int fatclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fatclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int ntfsclient_create(struct artery* ele, void* url, int argc, u8** argv);
int ntfsclient_delete(struct artery* ele, void* url);
int ntfsclient_linkup(struct halfrel* self, struct halfrel* peer);
int ntfsclient_discon( struct halfrel* self, struct halfrel* peer);
int ntfsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int ntfsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hfsclient_create(struct artery* ele, void* url, int argc, u8** argv);
int hfsclient_delete(struct artery* ele, void* url);
int hfsclient_linkup(struct halfrel* self, struct halfrel* peer);
int hfsclient_discon( struct halfrel* self, struct halfrel* peer);
int hfsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hfsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int extclient_create(struct artery* ele, void* url, int argc, u8** argv);
int extclient_delete(struct artery* ele, void* url);
int extclient_linkup(struct halfrel* self, struct halfrel* peer);
int extclient_discon( struct halfrel* self, struct halfrel* peer);
int extclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int extclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int control_create(struct artery* ele, void* url, int argc, u8** argv);
int control_delete(struct artery* ele, void* url);
int control_linkup(struct halfrel* self, struct halfrel* peer);
int control_discon( struct halfrel* self, struct halfrel* peer);
int control_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int control_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int search_create(struct artery* ele, void* url, int argc, u8** argv);
int search_delete(struct artery* ele, void* url);
int search_linkup(struct halfrel* self, struct halfrel* peer);
int search_discon( struct halfrel* self, struct halfrel* peer);
int search_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int search_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int vt100_create(struct artery* ele, void* url, int argc, u8** argv);
int vt100_delete(struct artery* ele, void* url);
int vt100_linkup(struct halfrel* self, struct halfrel* peer);
int vt100_discon( struct halfrel* self, struct halfrel* peer);
int vt100_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int vt100_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int echo_create(struct artery* ele, void* url, int argc, u8** argv);
int echo_delete(struct artery* ele, void* url);
int echo_linkup(struct halfrel* self, struct halfrel* peer);
int echo_discon( struct halfrel* self, struct halfrel* peer);
int echo_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int echo_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int pump_create(struct artery* ele, void* url, int argc, u8** argv);
int pump_delete(struct artery* ele, void* url);
int pump_discon( struct halfrel* self, struct halfrel* peer);
int pump_linkup(struct halfrel* self, struct halfrel* peer);
int pump_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int pump_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int goslow_create(struct artery* ele, void* url, int argc, u8** argv);
int goslow_delete(struct artery* ele, void* url);
int goslow_linkup(struct halfrel* self, struct halfrel* peer);
int goslow_discon( struct halfrel* self, struct halfrel* peer);
int goslow_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int goslow_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbgf32_create(struct artery* ele, void* url, int argc, u8** argv);
int dbgf32_delete(struct artery* ele, void* url);
int dbgf32_linkup(struct halfrel* self, struct halfrel* peer);
int dbgf32_discon( struct halfrel* self, struct halfrel* peer);
int dbgf32_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbgf32_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbghex_create(struct artery* ele, void* url, int argc, u8** argv);
int dbghex_delete(struct artery* ele, void* url);
int dbghex_linkup(struct halfrel* self, struct halfrel* peer);
int dbghex_discon( struct halfrel* self, struct halfrel* peer);
int dbghex_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbghex_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int fftpcm_create(struct artery* ele, void* url, int argc, u8** argv);
int fftpcm_delete(struct artery* ele, void* url);
int fftpcm_linkup(struct halfrel* self, struct halfrel* peer);
int fftpcm_discon( struct halfrel* self, struct halfrel* peer);
int fftpcm_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fftpcm_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fftrgb_create(struct artery* ele, void* url, int argc, u8** argv);
int fftrgb_delete(struct artery* ele, void* url);
int fftrgb_linkup(struct halfrel* self, struct halfrel* peer);
int fftrgb_discon( struct halfrel* self, struct halfrel* peer);
int fftrgb_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fftrgb_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int rotate_create(struct artery* ele, void* url, int argc, u8** argv);
int rotate_delete(struct artery* ele, void* url);
int rotate_linkup(struct halfrel* self, struct halfrel* peer);
int rotate_discon( struct halfrel* self, struct halfrel* peer);
int rotate_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int rotate_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int recut_create(struct artery* ele, void* url, int argc, u8** argv);
int recut_delete(struct artery* ele, void* url);
int recut_linkup(struct halfrel* self, struct halfrel* peer);
int recut_discon( struct halfrel* self, struct halfrel* peer);
int recut_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int recut_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int reline_create(struct artery* ele, void* url, int argc, u8** argv);
int reline_delete(struct artery* ele, void* url);
int reline_linkup(struct halfrel* self, struct halfrel* peer);
int reline_discon( struct halfrel* self, struct halfrel* peer);
int reline_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int reline_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int reorder_create(struct artery* ele, void* url, int argc, u8** argv);
int reorder_delete(struct artery* ele, void* url);
int reorder_linkup(struct halfrel* self, struct halfrel* peer);
int reorder_discon( struct halfrel* self, struct halfrel* peer);
int reorder_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int reorder_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int qu2eu_create(struct artery* ele, void* url, int argc, u8** argv);
int qu2eu_delete(struct artery* ele, void* url);
int qu2eu_linkup(struct halfrel* self, struct halfrel* peer);
int qu2eu_discon( struct halfrel* self, struct halfrel* peer);
int qu2eu_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int qu2eu_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int str2fv_create(struct artery* ele, void* url, int argc, u8** argv);
int str2fv_delete(struct artery* ele, void* url);
int str2fv_linkup(struct halfrel* self, struct halfrel* peer);
int str2fv_discon( struct halfrel* self, struct halfrel* peer);
int str2fv_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int str2fv_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fv2str_create(struct artery* ele, void* url, int argc, u8** argv);
int fv2str_delete(struct artery* ele, void* url);
int fv2str_linkup(struct halfrel* self, struct halfrel* peer);
int fv2str_discon( struct halfrel* self, struct halfrel* peer);
int fv2str_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fv2str_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int easyag_create(struct artery* ele, void* url, int argc, u8** argv);
int easyag_delete(struct artery* ele, void* url);
int easyag_linkup(struct halfrel* self, struct halfrel* peer);
int easyag_discon( struct halfrel* self, struct halfrel* peer);
int easyag_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int easyag_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mahony_create(struct artery* ele, void* url, int argc, u8** argv);
int mahony_delete(struct artery* ele, void* url);
int mahony_linkup(struct halfrel* self, struct halfrel* peer);
int mahony_discon( struct halfrel* self, struct halfrel* peer);
int mahony_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mahony_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int madgwick_create(struct artery* ele, void* url, int argc, u8** argv);
int madgwick_delete(struct artery* ele, void* url);
int madgwick_linkup(struct halfrel* self, struct halfrel* peer);
int madgwick_discon( struct halfrel* self, struct halfrel* peer);
int madgwick_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int madgwick_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//uart.gcode
int gcodeclient_create(struct artery* ele, void* url, int argc, u8** argv);
int gcodeclient_delete(struct artery* ele, void* url);
int gcodeclient_linkup(struct halfrel* self, struct halfrel* peer);
int gcodeclient_discon( struct halfrel* self, struct halfrel* peer);
int gcodeclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gcodeclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gcodeserver_create(struct artery* ele, void* url, int argc, u8** argv);
int gcodeserver_delete(struct artery* ele, void* url);
int gcodeserver_linkup(struct halfrel* self, struct halfrel* peer);
int gcodeserver_discon( struct halfrel* self, struct halfrel* peer);
int gcodeserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gcodeserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mavlinkclient_create(struct artery* ele, void* url, int argc, u8** argv);
int mavlinkclient_delete(struct artery* ele, void* url);
int mavlinkclient_linkup(struct halfrel* self, struct halfrel* peer);
int mavlinkclient_discon( struct halfrel* self, struct halfrel* peer);
int mavlinkclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mavlinkclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mavlinkserver_create(struct artery* ele, void* url, int argc, u8** argv);
int mavlinkserver_delete(struct artery* ele, void* url);
int mavlinkserver_linkup(struct halfrel* self, struct halfrel* peer);
int mavlinkserver_discon( struct halfrel* self, struct halfrel* peer);
int mavlinkserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mavlinkserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int nema0183client_create(struct artery* ele, void* url, int argc, u8** argv);
int nema0183client_delete(struct artery* ele, void* url);
int nema0183client_linkup(struct halfrel* self, struct halfrel* peer);
int nema0183client_discon( struct halfrel* self, struct halfrel* peer);
int nema0183client_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int nema0183client_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int nema0183server_create(struct artery* ele, void* url, int argc, u8** argv);
int nema0183server_delete(struct artery* ele, void* url);
int nema0183server_linkup(struct halfrel* self, struct halfrel* peer);
int nema0183server_discon( struct halfrel* self, struct halfrel* peer);
int nema0183server_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int nema0183server_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//socket
int hackclient_create(struct artery* ele, void* url, int argc, u8** argv);
int hackclient_delete(struct artery* ele, void* url);
int hackclient_linkup(struct halfrel* self, struct halfrel* peer);
int hackclient_discon( struct halfrel* self, struct halfrel* peer);
int hackclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hackclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hackserver_create(struct artery* ele, void* url, int argc, u8** argv);
int hackserver_delete(struct artery* ele, void* url);
int hackserver_linkup(struct halfrel* self, struct halfrel* peer);
int hackserver_discon( struct halfrel* self, struct halfrel* peer);
int hackserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int hackserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int proxyclient_create(struct artery* ele, void* url, int argc, u8** argv);
int proxyclient_delete(struct artery* ele, void* url);
int proxyclient_linkup(struct halfrel* self, struct halfrel* peer);
int proxyclient_discon( struct halfrel* self, struct halfrel* peer);
int proxyclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int proxyclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int proxyserver_create(struct artery* ele, void* url, int argc, u8** argv);
int proxyserver_delete(struct artery* ele, void* url);
int proxyserver_linkup(struct halfrel* self, struct halfrel* peer);
int proxyserver_discon( struct halfrel* self, struct halfrel* peer);
int proxyserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int proxyserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int proxymaster_create(struct artery* ele, void* url, int argc, u8** argv);
int proxymaster_delete(struct artery* ele, void* url);
int proxymaster_linkup(struct halfrel* self, struct halfrel* peer);
int proxymaster_discon( struct halfrel* self, struct halfrel* peer);
int proxymaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int proxymaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int socksclient_create(struct artery* ele, void* url, int argc, u8** argv);
int socksclient_delete(struct artery* ele, void* url);
int socksclient_linkup(struct halfrel* self, struct halfrel* peer);
int socksclient_discon( struct halfrel* self, struct halfrel* peer);
int socksclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int socksclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int socksserver_create(struct artery* ele, void* url, int argc, u8** argv);
int socksserver_delete(struct artery* ele, void* url);
int socksserver_linkup(struct halfrel* self, struct halfrel* peer);
int socksserver_discon( struct halfrel* self, struct halfrel* peer);
int socksserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int socksserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int socksmaster_create(struct artery* ele, void* url, int argc, u8** argv);
int socksmaster_delete(struct artery* ele, void* url);
int socksmaster_linkup(struct halfrel* self, struct halfrel* peer);
int socksmaster_discon( struct halfrel* self, struct halfrel* peer);
int socksmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int socksmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//udp.dns
int dnsclient_create(struct artery* ele, void* url, int argc, u8** argv);
int dnsclient_delete(struct artery* ele, void* url);
int dnsclient_linkup(struct halfrel* self, struct halfrel* peer);
int dnsclient_discon( struct halfrel* self, struct halfrel* peer);
int dnsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dnsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dnsserver_create(struct artery* ele, void* url, int argc, u8** argv);
int dnsserver_delete(struct artery* ele, void* url);
int dnsserver_linkup(struct halfrel* self, struct halfrel* peer);
int dnsserver_discon( struct halfrel* self, struct halfrel* peer);
int dnsserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dnsserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//udp.tftp
int tftpclient_create(struct artery* ele, void* url, int argc, u8** argv);
int tftpclient_delete(struct artery* ele, void* url);
int tftpclient_linkup(struct halfrel* self, struct halfrel* peer);
int tftpclient_discon( struct halfrel* self, struct halfrel* peer);
int tftpclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tftpclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tftpserver_create(struct artery* ele, void* url, int argc, u8** argv);
int tftpserver_delete(struct artery* ele, void* url);
int tftpserver_linkup(struct halfrel* self, struct halfrel* peer);
int tftpserver_discon( struct halfrel* self, struct halfrel* peer);
int tftpserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tftpserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//udp.quic
int quicclient_create(struct artery* ele, void* url, int argc, u8** argv);
int quicclient_delete(struct artery* ele, void* url);
int quicclient_linkup(struct halfrel* self, struct halfrel* peer);
int quicclient_discon( struct halfrel* self, struct halfrel* peer);
int quicclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicserver_create(struct artery* ele, void* url, int argc, u8** argv);
int quicserver_delete(struct artery* ele, void* url);
int quicserver_linkup(struct halfrel* self, struct halfrel* peer);
int quicserver_discon( struct halfrel* self, struct halfrel* peer);
int quicserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicmaster_create(struct artery* ele, void* url, int argc, u8** argv);
int quicmaster_delete(struct artery* ele, void* url);
int quicmaster_linkup(struct halfrel* self, struct halfrel* peer);
int quicmaster_discon( struct halfrel* self, struct halfrel* peer);
int quicmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int quicmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.ssh
int sshclient_create(struct artery* ele, void* url, int argc, u8** argv);
int sshclient_delete(struct artery* ele, void* url);
int sshclient_linkup(struct halfrel* self, struct halfrel* peer);
int sshclient_discon( struct halfrel* self, struct halfrel* peer);
int sshclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshserver_create(struct artery* ele, void* url, int argc, u8** argv);
int sshserver_delete(struct artery* ele, void* url);
int sshserver_linkup(struct halfrel* self, struct halfrel* peer);
int sshserver_discon( struct halfrel* self, struct halfrel* peer);
int sshserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshmaster_create(struct artery* ele, void* url, int argc, u8** argv);
int sshmaster_delete(struct artery* ele, void* url);
int sshmaster_linkup(struct halfrel* self, struct halfrel* peer);
int sshmaster_discon( struct halfrel* self, struct halfrel* peer);
int sshmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int sshmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.telnet
int telnetclient_create(struct artery* ele, void* url, int argc, u8** argv);
int telnetclient_delete(struct artery* ele, void* url);
int telnetclient_linkup(struct halfrel* self, struct halfrel* peer);
int telnetclient_discon( struct halfrel* self, struct halfrel* peer);
int telnetclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetserver_create(struct artery* ele, void* url, int argc, u8** argv);
int telnetserver_delete(struct artery* ele, void* url);
int telnetserver_linkup(struct halfrel* self, struct halfrel* peer);
int telnetserver_discon( struct halfrel* self, struct halfrel* peer);
int telnetserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetmaster_create(struct artery* ele, void* url, int argc, u8** argv);
int telnetmaster_delete(struct artery* ele, void* url);
int telnetmaster_linkup(struct halfrel* self, struct halfrel* peer);
int telnetmaster_discon( struct halfrel* self, struct halfrel* peer);
int telnetmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int telnetmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.http
int httpclient_create(struct artery* ele, void* url, int argc, u8** argv);
int httpclient_delete(struct artery* ele, void* url);
int httpclient_linkup(struct halfrel* self, struct halfrel* peer);
int httpclient_discon( struct halfrel* self, struct halfrel* peer);
int httpclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpserver_create(struct artery* ele, void* url, int argc, u8** argv);
int httpserver_delete(struct artery* ele, void* url);
int httpserver_linkup(struct halfrel* self, struct halfrel* peer);
int httpserver_discon( struct halfrel* self, struct halfrel* peer);
int httpserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpmaster_create(struct artery* ele, void* url, int argc, u8** argv);
int httpmaster_delete(struct artery* ele, void* url);
int httpmaster_linkup(struct halfrel* self, struct halfrel* peer);
int httpmaster_discon( struct halfrel* self, struct halfrel* peer);
int httpmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int httpmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.ws
int wsclient_create(struct artery* ele, void* url, int argc, u8** argv);
int wsclient_delete(struct artery* ele, void* url);
int wsclient_linkup(struct halfrel* self, struct halfrel* peer);
int wsclient_discon( struct halfrel* self, struct halfrel* peer);
int wsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsserver_create(struct artery* ele, void* url, int argc, u8** argv);
int wsserver_delete(struct artery* ele, void* url);
int wsserver_linkup(struct halfrel* self, struct halfrel* peer);
int wsserver_discon( struct halfrel* self, struct halfrel* peer);
int wsserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsmaster_create(struct artery* ele, void* url, int argc, u8** argv);
int wsmaster_delete(struct artery* ele, void* url);
int wsmaster_linkup(struct halfrel* self, struct halfrel* peer);
int wsmaster_discon( struct halfrel* self, struct halfrel* peer);
int wsmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int wsmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.tls
int tlsclient_create(struct artery* ele, void* url, int argc, u8** argv);
int tlsclient_delete(struct artery* ele, void* url);
int tlsclient_linkup(struct halfrel* self, struct halfrel* peer);
int tlsclient_discon( struct halfrel* self, struct halfrel* peer);
int tlsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsserver_create(struct artery* ele, void* url, int argc, u8** argv);
int tlsserver_delete(struct artery* ele, void* url);
int tlsserver_linkup(struct halfrel* self, struct halfrel* peer);
int tlsserver_discon( struct halfrel* self, struct halfrel* peer);
int tlsserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsmaster_create(struct artery* ele, void* url, int argc, u8** argv);
int tlsmaster_delete(struct artery* ele, void* url);
int tlsmaster_linkup(struct halfrel* self, struct halfrel* peer);
int tlsmaster_discon( struct halfrel* self, struct halfrel* peer);
int tlsmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int tlsmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//fuckgfw
int fuckgfwclient_create(struct artery* ele, void* url, int argc, u8** argv);
int fuckgfwclient_delete(struct artery* ele, void* url);
int fuckgfwclient_linkup(struct halfrel* self, struct halfrel* peer);
int fuckgfwclient_discon( struct halfrel* self, struct halfrel* peer);
int fuckgfwclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fuckgfwclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fuckgfwserver_create(struct artery* ele, void* url, int argc, u8** argv);
int fuckgfwserver_delete(struct artery* ele, void* url);
int fuckgfwserver_linkup(struct halfrel* self, struct halfrel* peer);
int fuckgfwserver_discon( struct halfrel* self, struct halfrel* peer);
int fuckgfwserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fuckgfwserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fuckgfwmaster_create(struct artery* ele, void* url, int argc, u8** argv);
int fuckgfwmaster_delete(struct artery* ele, void* url);
int fuckgfwmaster_linkup(struct halfrel* self, struct halfrel* peer);
int fuckgfwmaster_discon( struct halfrel* self, struct halfrel* peer);
int fuckgfwmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fuckgfwmaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//tcp.party
int partyclient_create(struct artery* ele, void* url, int argc, u8** argv);
int partyclient_delete(struct artery* ele, void* url);
int partyclient_linkup(struct halfrel* self, struct halfrel* peer);
int partyclient_discon( struct halfrel* self, struct halfrel* peer);
int partyclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int partyclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int partyserver_create(struct artery* ele, void* url, int argc, u8** argv);
int partyserver_delete(struct artery* ele, void* url);
int partyserver_linkup(struct halfrel* self, struct halfrel* peer);
int partyserver_discon( struct halfrel* self, struct halfrel* peer);
int partyserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int partyserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int partymaster_create(struct artery* ele, void* url, int argc, u8** argv);
int partymaster_delete(struct artery* ele, void* url);
int partymaster_linkup(struct halfrel* self, struct halfrel* peer);
int partymaster_discon( struct halfrel* self, struct halfrel* peer);
int partymaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int partymaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int parseurl(u8* buf, int len, u8* addr, int* port);
int ncmp(void*, void*, int);
int cmp(void*, void*);



#define datahome qqq
static struct object* obj = 0;
static void* ppp = 0;
static struct artery* ele = 0;
static void* qqq = 0;
static int elelen = 0;
static int qqqlen = 0;
void* allocartery()
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
	struct artery* ele = (void*)(self->chip);
	switch(ele->type){
		case _ann_:ann_read(self, peer, arg, idx, buf, len);break;

		case _control_:control_read(self, peer, arg, idx, buf, len);break;
		case _search_:search_read(self, peer, arg, idx, buf, len);break;
		case _vt100_:vt100_read(self, peer, arg, idx, buf, len);break;

		case _echo_:echo_read(self, peer, arg, idx, buf, len);break;
		case _pump_:pump_read(self, peer, arg, idx, buf, len);break;
		case _goslow_:goslow_read(self, peer, arg, idx, buf, len);break;
		case _dbgf32_:dbgf32_read(self, peer, arg, idx, buf, len);break;
		case _dbghex_:dbghex_read(self, peer, arg, idx, buf, len);break;

		case _fftpcm_:fftpcm_read(self, peer, arg, idx, buf, len);break;
		case _fftrgb_:fftrgb_read(self, peer, arg, idx, buf, len);break;
		case _rotate_:rotate_read(self, peer, arg, idx, buf, len);break;

		case _recut_:recut_read(self, peer, arg, idx, buf, len);break;
		case _reline_:reline_read(self, peer, arg, idx, buf, len);break;
		case _reorder_:reorder_read(self, peer, arg, idx, buf, len);break;

		case _qu2eu_:qu2eu_read(self, peer, arg, idx, buf, len);break;
		case _str2fv_:str2fv_read(self, peer, arg, idx, buf, len);break;
		case _fv2str_:fv2str_read(self, peer, arg, idx, buf, len);break;

		case _easyag_:easyag_read(self, peer, arg, idx, buf, len);break;
		case _mahony_:mahony_read(self, peer, arg, idx, buf, len);break;
		case _madgwick_:madgwick_read(self, peer, arg, idx, buf, len);break;

		case _gcode_:gcodeclient_read(self, peer, arg, idx, buf, len);break;
		case _Gcode_:gcodeserver_read(self, peer, arg, idx, buf, len);break;
		case _mavlink_:mavlinkclient_read(self, peer, arg, idx, buf, len);break;
		case _Mavlink_:mavlinkserver_read(self, peer, arg, idx, buf, len);break;
		case _nema0183_:nema0183client_read(self, peer, arg, idx, buf, len);break;
		case _Nema0183_:nema0183server_read(self, peer, arg, idx, buf, len);break;

		case _PROXY_:proxymaster_read(self, peer, arg, idx, buf, len);break;
		case _Proxy_:proxyserver_read(self, peer, arg, idx, buf, len);break;
		case _proxy_:proxyclient_read(self, peer, arg, idx, buf, len);break;
		case _SOCKS_:socksmaster_read(self, peer, arg, idx, buf, len);break;
		case _Socks_:socksserver_read(self, peer, arg, idx, buf, len);break;
		case _socks_:socksclient_read(self, peer, arg, idx, buf, len);break;
		case _FUCKGFW_:fuckgfwmaster_read(self, peer, arg, idx, buf, len);break;
		case _Fuckgfw_:fuckgfwserver_read(self, peer, arg, idx, buf, len);break;
		case _fuckgfw_:fuckgfwclient_read(self, peer, arg, idx, buf, len);break;

		case _HTTP_:httpmaster_read(self, peer, arg, idx, buf, len);break;
		case _Http_:httpserver_read(self, peer, arg, idx, buf, len);break;
		case _http_:httpclient_read(self, peer, arg, idx, buf, len);break;
		case _WS_:wsmaster_read(self, peer, arg, idx, buf, len);break;
		case _Ws_:wsserver_read(self, peer, arg, idx, buf, len);break;
		case _ws_:wsclient_read(self, peer, arg, idx, buf, len);break;

		case _SSH_:sshmaster_read(self, peer, arg, idx, buf, len);break;
		case _Ssh_:sshserver_read(self, peer, arg, idx, buf, len);break;
		case _ssh_:sshclient_read(self, peer, arg, idx, buf, len);break;
		case _TLS_:tlsmaster_read(self, peer, arg, idx, buf, len);break;
		case _Tls_:tlsserver_read(self, peer, arg, idx, buf, len);break;
		case _tls_:tlsclient_read(self, peer, arg, idx, buf, len);break;
	}
	return 0;
}
int arterywrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct artery* ele = (void*)(self->chip);
	//say("@arterywrite: %.8s\n", &ele->type);
	switch(ele->type){
		case _ann_:return ann_write(self, peer, arg, idx, buf, len);break;

		case _control_:return control_write(self, peer, arg, idx, buf, len);break;
		case _search_:return search_write(self, peer, arg, idx, buf, len);break;
		case _vt100_:return vt100_write(self, peer, arg, idx, buf, len);break;

		case _echo_:return echo_write(self, peer, arg, idx, buf, len);break;
		case _pump_:return pump_write(self, peer, arg, idx, buf, len);break;
		case _goslow_:return goslow_write(self, peer, arg, idx, buf, len);break;
		case _dbgf32_:return dbgf32_write(self, peer, arg, idx, buf, len);break;
		case _dbghex_:return dbghex_write(self, peer, arg, idx, buf, len);break;

		case _fftpcm_:return fftpcm_write(self, peer, arg, idx, buf, len);break;
		case _fftrgb_:return fftrgb_write(self, peer, arg, idx, buf, len);break;
		case _rotate_:return rotate_write(self, peer, arg, idx, buf, len);break;

		case _recut_:return recut_write(self, peer, arg, idx, buf, len);break;
		case _reline_:return reline_write(self, peer, arg, idx, buf, len);break;
		case _reorder_:return reorder_write(self, peer, arg, idx, buf, len);break;

		case _qu2eu_:return qu2eu_write(self, peer, arg, idx, buf, len);break;
		case _str2fv_:return str2fv_write(self, peer, arg, idx, buf, len);break;
		case _fv2str_:return fv2str_write(self, peer, arg, idx, buf, len);break;

		case _easyag_:return easyag_write(self, peer, arg, idx, buf, len);break;
		case _mahony_:return mahony_write(self, peer, arg, idx, buf, len);break;
		case _madgwick_:return madgwick_write(self, peer, arg, idx, buf, len);break;

		case _Gcode_:return gcodeserver_write(self, peer, arg, idx, buf, len);break;
		case _gcode_:return gcodeclient_write(self, peer, arg, idx, buf, len);break;
		case _Mavlink_:return mavlinkserver_write(self, peer, arg, idx, buf, len);break;
		case _mavlink_:return mavlinkclient_write(self, peer, arg, idx, buf, len);break;
		case _Nema0183_:return nema0183server_write(self, peer, arg, idx, buf, len);break;
		case _nema0183_:return nema0183client_write(self, peer, arg, idx, buf, len);break;

		case _PROXY_:return proxymaster_write(self, peer, arg, idx, buf, len);break;
		case _Proxy_:return proxyserver_write(self, peer, arg, idx, buf, len);break;
		case _proxy_:return proxyclient_write(self, peer, arg, idx, buf, len);break;
		case _SOCKS_:return socksmaster_write(self, peer, arg, idx, buf, len);break;
		case _Socks_:return socksserver_write(self, peer, arg, idx, buf, len);break;
		case _socks_:return socksclient_write(self, peer, arg, idx, buf, len);break;
		case _FUCKGFW_:return fuckgfwmaster_write(self, peer, arg, idx, buf, len);break;
		case _Fuckgfw_:return fuckgfwserver_write(self, peer, arg, idx, buf, len);break;
		case _fuckgfw_:return fuckgfwclient_write(self, peer, arg, idx, buf, len);break;

		case _HTTP_:return httpmaster_write(self, peer, arg, idx, buf, len);break;
		case _Http_:return httpserver_write(self, peer, arg, idx, buf, len);break;
		case _http_:return httpclient_write(self, peer, arg, idx, buf, len);break;
		case _WS_:return wsmaster_write(self, peer, arg, idx, buf, len);break;
		case _Ws_:return wsserver_write(self, peer, arg, idx, buf, len);break;
		case _ws_:return wsclient_write(self, peer, arg, idx, buf, len);break;

		case _SSH_:return sshmaster_write(self, peer, arg, idx, buf, len);break;
		case _Ssh_:return sshserver_write(self, peer, arg, idx, buf, len);break;
		case _ssh_:return sshclient_write(self, peer, arg, idx, buf, len);break;
		case _TLS_:return tlsmaster_write(self, peer, arg, idx, buf, len);break;
		case _Tls_:return tlsserver_write(self, peer, arg, idx, buf, len);break;
		case _tls_:return tlsclient_write(self, peer, arg, idx, buf, len);break;
	}
	return 0;
}
int arterydiscon(struct halfrel* self, struct halfrel* peer)
{
	struct artery* ele;
	say("@arterydiscon\n");

	ele = self->pchip;
	if(0 == ele)return 0;

	switch(ele->type){
		case _ann_:return ann_discon(self, peer);break;
		case _http_:return httpclient_discon(self, peer);break;
		case _ws_:return wsclient_discon(self, peer);break;

		case _ssh_:return sshclient_discon(self, peer);break;
		case _tls_:return tlsclient_discon(self, peer);break;

		case _proxy_:return proxyclient_discon(self, peer);break;
		case _Proxy_:return proxyserver_discon(self, peer);break;
		case _socks_:return socksclient_discon(self, peer);break;
		case _Socks_:return socksserver_discon(self, peer);break;
	}
	return 0;
}
int arterylinkup(struct halfrel* self, struct halfrel* peer)
{
	struct artery* ele;
	say("@arterylinkup\n");

	ele = self->pchip;
	if(0 == ele)return 0;

	switch(ele->type){
		case _ann_:return ann_linkup(self, peer);break;
		case _http_:return httpclient_linkup(self, peer);break;
		case _ws_:return wsclient_linkup(self, peer);break;

		case _ssh_:return sshclient_linkup(self, peer);break;
		case _tls_:return tlsclient_linkup(self, peer);break;

		case _proxy_:return proxyclient_linkup(self, peer);break;
		case _Proxy_:return proxyserver_linkup(self, peer);break;
		case _socks_:return socksclient_linkup(self, peer);break;
		case _Socks_:return socksserver_linkup(self, peer);break;
	}
	return 0;
}




int arterydelete(void* ele)
{
	return 0;
}
void* arterycreate(u64 type, void* argstr, int argc, u8** argv)
{
	int j,fd,ret,port;
	struct artery* e;
	struct artery* f;
	u8* url = argstr;
	if(0 == type)
	{
		ret = parsetypefromurl(url, (void*)&type);
		if(0 == ret)return 0;	//unknown

		url += ret;
	}

	//
	if(_control_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _control_;
		control_create(e, url, argc, argv);
		return e;
	}
	if(_search_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _search_;
		search_create(e, url, argc, argv);
		return e;
	}
	if(_vt100_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _vt100_;
		vt100_create(e, url, argc, argv);
		return e;
	}

	if(_ann_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _ann_;
		ann_create(e, url, argc, argv);
		return e;
	}

	//
	if(_file_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _file_;
		fileclient_create(e, url, argc, argv);
		return e;
	}
	if(_mbr_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _mbr_;
		mbrclient_create(e, url, argc, argv);
		return e;
	}
	if(_gpt_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _gpt_;
		gptclient_create(e, url, argc, argv);
		return e;
	}

	//file system
	if(_fat_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _fat_;
		fatclient_create(e, url, argc, argv);
		return e;
	}
	if(_ntfs_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _ntfs_;
		ntfsclient_create(e, url, argc, argv);
		return e;
	}
	if(_hfs_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _hfs_;
		hfsclient_create(e, url, argc, argv);
		return e;
	}
	if(_ext_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _ext_;
		extclient_create(e, url, argc, argv);
		return e;
	}

	//test
	if(_echo_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _echo_;
		echo_create(e, url, argc, argv);
		return e;
	}
	if(_pump_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _pump_;
		pump_create(e, url, argc, argv);
		return e;
	}
	if(_dbgf32_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _dbgf32_;
		dbgf32_create(e, url, argc, argv);
		return e;
	}
	if(_dbghex_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _dbghex_;
		dbghex_create(e, url, argc, argv);
		return e;
	}
	if(_goslow_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _goslow_;
		goslow_create(e, url, argc, argv);
		return e;
	}

	//
	if(_fftpcm_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _fftpcm_;
		fftpcm_create(e, url, argc, argv);
		return e;
	}
	if(_fftrgb_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _fftrgb_;
		fftrgb_create(e, url, argc, argv);
		return e;
	}
	if(_rotate_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _rotate_;
		rotate_create(e, url, argc, argv);
		return e;
	}

	//
	if(_recut_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _recut_;
		recut_create(e, url, argc, argv);
		return e;
	}
	if(_reline_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _reline_;
		reline_create(e, url, argc, argv);
		return e;
	}
	if(_reorder_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _reorder_;
		reorder_create(e, url, argc, argv);
		return e;
	}

	//
	if(_qu2eu_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _qu2eu_;
		qu2eu_create(e, url, argc, argv);
		return e;
	}
	if(_str2fv_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _str2fv_;
		str2fv_create(e, url, argc, argv);
		return e;
	}
	if(_fv2str_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _fv2str_;
		fv2str_create(e, url, argc, argv);
		return e;
	}

	//ahrs
	if(_easyag_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _easyag_;
		easyag_create(e, url, argc, argv);
		return e;
	}
	if(_mahony_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _mahony_;
		mahony_create(e, url, argc, argv);
		return e;
	}
	if(_madgwick_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _madgwick_;
		madgwick_create(e, url, argc, argv);
		return e;
	}

	//
	if(_gcode_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _gcode_;
		gcodeclient_create(e, url, argc, argv);
		return e;
	}
	if(_Gcode_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Gcode_;
		gcodeserver_create(e, url, argc, argv);
		return e;
	}
	if(_mavlink_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _mavlink_;
		mavlinkclient_create(e, url, argc, argv);
		return e;
	}
	if(_Mavlink_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Mavlink_;
		mavlinkserver_create(e, url, argc, argv);
		return e;
	}
	if(_nema0183_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _nema0183_;
		nema0183client_create(e, url, argc, argv);
		return e;
	}
	if(_Nema0183_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Nema0183_;
		nema0183server_create(e, url, argc, argv);
		return e;
	}

	//simple control over message
	if(_HACK_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _HACK_;
		hackserver_create(e, url, argc, argv);
		return e;
	}
	if(_hack_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _hack_;
		hackclient_create(e, url, argc, argv);
		return e;
	}
	if(_PROXY_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _PROXY_;
		proxymaster_create(e, url, argc, argv);
		return e;
	}
	if(_Proxy_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Proxy_;
		proxyserver_create(e, url, argc, argv);
		return e;
	}
	if(_proxy_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _proxy_;
		proxyclient_create(e, url, argc, argv);
		return e;
	}
	if(_SOCKS_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _SOCKS_;
		socksmaster_create(e, url, argc, argv);
		return e;
	}
	if(_Socks_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Socks_;
		socksserver_create(e, url, argc, argv);
		return e;
	}
	if(_socks_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _socks_;
		socksclient_create(e, url, argc, argv);
		return e;
	}

	//dns
	if(_Dns_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Dns_;
		dnsserver_create(e, url, argc, argv);
		return e;
	}
	if(_dns_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _dns_;
		dnsclient_create(e, url, argc, argv);
		return e;
	}

	//tftp: client,server
	if(_tftp_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _tftp_;
		tftpclient_create(e, url, argc, argv);
		return e;
	}
	if(_Tftp_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Tftp_;
		tftpserver_create(e, url, argc, argv);
		return e;
	}

	//quic: master,server,client
	if(_QUIC_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _QUIC_;
		quicmaster_create(e, url, argc, argv);
		return e;
	}
	if(_Quic_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Quic_;
		quicserver_create(e, url, argc, argv);
		return e;
	}
	if(_quic_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _quic_;
		quicclient_create(e, url, argc, argv);
		return e;
	}

	//fuckgfw: master,server,client
	if(_FUCKGFW_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _FUCKGFW_;
		fuckgfwmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Fuckgfw_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Fuckgfw_;
		fuckgfwserver_create(e, url, argc, argv);

		return e;
	}
	if(_fuckgfw_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _fuckgfw_;
		fuckgfwclient_create(e, url, argc, argv);

		return e;
	}

	//serve: master,server,client
	if(_PARTY_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _PARTY_;
		partymaster_create(e, url, argc, argv);

		return e;
	}
	if(_Party_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Party_;
		partyserver_create(e, url, argc, argv);

		return e;
	}
	if(_party_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _party_;
		partyclient_create(e, url, argc, argv);

		return e;
	}

	//ssh: master,server,client
	if(_SSH_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _SSH_;
		sshmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Ssh_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Ssh_;
		sshserver_create(e, url, argc, argv);

		return e;
	}
	if(_ssh_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _ssh_;
		sshclient_create(e, url, argc, argv);

		return e;
	}

	//telnet: master,server,client
	if(_TELNET_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _TELNET_;
		telnetmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Telnet_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Telnet_;
		telnetserver_create(e, url, argc, argv);

		return e;
	}
	if(_telnet_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _telnet_;
		telnetclient_create(e, url, argc, argv);

		return e;
	}

	//http: master,server,client
	if(_HTTP_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _HTTP_;
		httpmaster_create(e, url, argc, argv);
		return e;
	}
	if(_Http_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Http_;
		httpserver_create(e, url, argc, argv);
		return e;
	}
	if(_http_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _http_;
		httpclient_create(e, url, argc, argv);

		return e;
	}

	//ws: master,server,client
	if(_WS_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _WS_;
		wsmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Ws_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Ws_;
		wsserver_create(e, url, argc, argv);

		return e;
	}
	if(_ws_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _ws_;
		wsclient_create(e, url, argc, argv);

		return e;
	}

	//tls: master,server,client
	if(_TLS_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _TLS_;
		tlsmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Tls_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _Tls_;
		tlsserver_create(e, url, argc, argv);

		return e;
	}
	if(_tls_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _tls_;
		tlsclient_create(e, url, argc, argv);

		return e;
	}

	if(_https_ == type)
	{
		e = allocartery();
		if(0 == e)return 0;

		e->type = _http_;

		return e;
	}
	return 0;
}
void* arterymodify(int argc, u8** argv)
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
		arterycreate(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
void* arterysearch(u8* buf, int len)
{
	int j,k=0;
	struct artery* art;
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




void freeartery()
{
	//say("[a,c):freeing artery\n");

	qqq = 0;
	ele = 0;
}
void initartery(u8* addr)
{
	int j;
	ele = (void*)(addr+0x000000);
	qqq = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct artery))
	for(j=0;j<0x200000;j++)addr[j] = 0;
	for(j=0;j<max;j++)ele[j].tier = _art_;

	//arterycreate(0, (u8*)"HACK://0.0.0.0:2222");
	//arterycreate(0, (u8*)"QUIC://0.0.0.0:4444");
	//arterycreate(0,  (u8*)"SSH://0.0.0.0:2222");
	//arterycreate(0, (u8*)"HTTP://0.0.0.0:4444");
	//say("[a,c):inited artery\n");
}
