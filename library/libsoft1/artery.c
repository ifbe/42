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
#define _crawler_ hex64('c','r','a','w','l','e','r', 0)
#define _search_ hex64('s','e','a','r','c','h', 0, 0)
#define _vt100_ hex64('v','t','1','0','0', 0, 0, 0)
//
#define _fftpcm_ hex64('f','f','t','p','c','m',0,0)
#define _fftrgb_ hex64('f','f','t','r','g','b',0,0)
//
#define _img2pbr_ hex64('i','m','g','2','p','b','r',0)
#define _rotate_ hex64('r','o','t','a','t','e',0,0)
#define _picfmt_ hex64('p','i','c','f','m','t',0,0)
#define _sobel_ hex64('s','o','b','e','l',0,0,0)
//
#define _pump_ hex32('p','u','m','p')
#define _stor_ hex32('s','t','o','r')
//
#define _easymux_ hex64('e','a','s','y','m','u','x',0)
#define _mediamux_ hex64('m','e','d','i','a','m','u','x')
#define _echo_ hex32('e','c','h','o')
#define _goslow_ hex64('g','o','s','l','o','w',0,0)
#define _dbgf32_ hex64('d','b','g','f','3','2',0,0)
#define _dbghex_ hex64('d','b','g','h','e','x',0,0)
//
#define _recut_ hex64('r','e','c','u','t', 0, 0, 0)
#define _reline_ hex64('r','e','l','i','n','e', 0, 0)
#define _reorder_ hex64('r','e','o','r','d','e','r', 0)
//
#define _qu2eu_ hex64('q','u','2','e','u',0,0,0)
#define _str2vec_ hex64('s','t','r','2','v','e','c',0)
//
#define _easyag_  hex64('e','a','s','y','a','g', 0 , 0 )
#define _mahony_  hex64('m','a','h','o','n','y', 0 , 0 )
#define _madgwick_ hex64('m','a','d','g','w','i','c','k')
//
int ann_create(_obj* ele, void* arg, int argc, u8** argv);
int ann_delete(_obj* ele, void* arg);
int ann_attach(struct halfrel* self, struct halfrel* peer);
int ann_detach(struct halfrel* self, struct halfrel* peer);
int ann_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int ann_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int cnn_create(_obj* ele, void* arg, int argc, u8** argv);
int cnn_delete(_obj* ele, void* arg);
int cnn_attach(struct halfrel* self, struct halfrel* peer);
int cnn_detach(struct halfrel* self, struct halfrel* peer);
int cnn_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int cnn_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//file
int pump_create(_obj* ele, void* arg, int argc, u8** argv);
int pump_delete(_obj* ele, void* arg);
int pump_detach(struct halfrel* self, struct halfrel* peer);
int pump_attach(struct halfrel* self, struct halfrel* peer);
int pump_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int pump_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int stor_create(_obj* ele, void* arg, int argc, u8** argv);
int stor_delete(_obj* ele, void* arg);
int stor_detach(struct halfrel* self, struct halfrel* peer);
int stor_attach(struct halfrel* self, struct halfrel* peer);
int stor_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int stor_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int filetype_create(_obj* ele, void* arg, int argc, u8** argv);
int filetype_delete(_obj* ele, void* arg);
int filetype_attach(struct halfrel* self, struct halfrel* peer);
int filetype_detach(struct halfrel* self, struct halfrel* peer);
int filetype_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int filetype_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fileauto_create(_obj* ele, void* arg, int argc, u8** argv);
int fileauto_delete(_obj* ele, void* arg);
int fileauto_attach(struct halfrel* self, struct halfrel* peer);
int fileauto_detach(struct halfrel* self, struct halfrel* peer);
int fileauto_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fileauto_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int mbrclient_create(_obj* ele, void* arg, int argc, u8** argv);
int mbrclient_delete(_obj* ele, void* arg);
int mbrclient_attach(struct halfrel* self, struct halfrel* peer);
int mbrclient_detach(struct halfrel* self, struct halfrel* peer);
int mbrclient_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int mbrclient_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int gptclient_create(_obj* ele, void* arg, int argc, u8** argv);
int gptclient_delete(_obj* ele, void* arg);
int gptclient_attach(struct halfrel* self, struct halfrel* peer);
int gptclient_detach(struct halfrel* self, struct halfrel* peer);
int gptclient_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int gptclient_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int fatclient_create(_obj* ele, void* arg, int argc, u8** argv);
int fatclient_delete(_obj* ele, void* arg);
int fatclient_attach(struct halfrel* self, struct halfrel* peer);
int fatclient_detach(struct halfrel* self, struct halfrel* peer);
int fatclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fatclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int ntfsclient_create(_obj* ele, void* arg, int argc, u8** argv);
int ntfsclient_delete(_obj* ele, void* arg);
int ntfsclient_attach(struct halfrel* self, struct halfrel* peer);
int ntfsclient_detach(struct halfrel* self, struct halfrel* peer);
int ntfsclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int ntfsclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int hfsclient_create(_obj* ele, void* arg, int argc, u8** argv);
int hfsclient_delete(_obj* ele, void* arg);
int hfsclient_attach(struct halfrel* self, struct halfrel* peer);
int hfsclient_detach(struct halfrel* self, struct halfrel* peer);
int hfsclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int hfsclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int extclient_create(_obj* ele, void* arg, int argc, u8** argv);
int extclient_delete(_obj* ele, void* arg);
int extclient_attach(struct halfrel* self, struct halfrel* peer);
int extclient_detach(struct halfrel* self, struct halfrel* peer);
int extclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int extclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int h264_create(_obj* ele, void* arg, int argc, u8** argv);
int h264_delete(_obj* ele, void* arg);
int h264_attach(struct halfrel* self, struct halfrel* peer);
int h264_detach(struct halfrel* self, struct halfrel* peer);
int h264_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int h264_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int control_create(_obj* ele, void* arg, int argc, u8** argv);
int control_delete(_obj* ele, void* arg);
int control_attach(struct halfrel* self, struct halfrel* peer);
int control_detach(struct halfrel* self, struct halfrel* peer);
int control_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int control_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int crawler_create(_obj* ele, void* arg, int argc, u8** argv);
int crawler_delete(_obj* ele, void* arg);
int crawler_attach(struct halfrel* self, struct halfrel* peer);
int crawler_detach(struct halfrel* self, struct halfrel* peer);
int crawler_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int crawler_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int search_create(_obj* ele, void* arg, int argc, u8** argv);
int search_delete(_obj* ele, void* arg);
int search_attach(struct halfrel* self, struct halfrel* peer);
int search_detach(struct halfrel* self, struct halfrel* peer);
int search_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int search_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int vt100_create(_obj* ele, void* arg, int argc, u8** argv);
int vt100_delete(_obj* ele, void* arg);
int vt100_attach(struct halfrel* self, struct halfrel* peer);
int vt100_detach(struct halfrel* self, struct halfrel* peer);
int vt100_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int vt100_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int easymux_create(_obj* ele, void* arg, int argc, u8** argv);
int easymux_delete(_obj* ele, void* arg);
int easymux_attach(struct halfrel* self, struct halfrel* peer);
int easymux_detach(struct halfrel* self, struct halfrel* peer);
int easymux_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int easymux_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int mediamux_create(_obj* ele, void* arg, int argc, u8** argv);
int mediamux_delete(_obj* ele, void* arg);
int mediamux_attach(struct halfrel* self, struct halfrel* peer);
int mediamux_detach(struct halfrel* self, struct halfrel* peer);
int mediamux_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int mediamux_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int echo_create(_obj* ele, void* arg, int argc, u8** argv);
int echo_delete(_obj* ele, void* arg);
int echo_attach(struct halfrel* self, struct halfrel* peer);
int echo_detach(struct halfrel* self, struct halfrel* peer);
int echo_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int echo_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int goslow_create(_obj* ele, void* arg, int argc, u8** argv);
int goslow_delete(_obj* ele, void* arg);
int goslow_attach(struct halfrel* self, struct halfrel* peer);
int goslow_detach(struct halfrel* self, struct halfrel* peer);
int goslow_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int goslow_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dbgf32_create(_obj* ele, void* arg, int argc, u8** argv);
int dbgf32_delete(_obj* ele, void* arg);
int dbgf32_attach(struct halfrel* self, struct halfrel* peer);
int dbgf32_detach(struct halfrel* self, struct halfrel* peer);
int dbgf32_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dbgf32_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dbghex_create(_obj* ele, void* arg, int argc, u8** argv);
int dbghex_delete(_obj* ele, void* arg);
int dbghex_attach(struct halfrel* self, struct halfrel* peer);
int dbghex_detach(struct halfrel* self, struct halfrel* peer);
int dbghex_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dbghex_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int fftpcm_create(_obj* ele, void* arg, int argc, u8** argv);
int fftpcm_delete(_obj* ele, void* arg);
int fftpcm_attach(struct halfrel* self, struct halfrel* peer);
int fftpcm_detach(struct halfrel* self, struct halfrel* peer);
int fftpcm_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fftpcm_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fftrgb_create(_obj* ele, void* arg, int argc, u8** argv);
int fftrgb_delete(_obj* ele, void* arg);
int fftrgb_attach(struct halfrel* self, struct halfrel* peer);
int fftrgb_detach(struct halfrel* self, struct halfrel* peer);
int fftrgb_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fftrgb_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int picfmt_create(_obj* ele, void* arg, int argc, u8** argv);
int picfmt_delete(_obj* ele, void* arg);
int picfmt_attach(struct halfrel* self, struct halfrel* peer);
int picfmt_detach(struct halfrel* self, struct halfrel* peer);
int picfmt_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int picfmt_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int sobel_create(_obj* ele, void* arg, int argc, u8** argv);
int sobel_delete(_obj* ele, void* arg);
int sobel_attach(struct halfrel* self, struct halfrel* peer);
int sobel_detach(struct halfrel* self, struct halfrel* peer);
int sobel_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int sobel_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int rotate_create(_obj* ele, void* arg, int argc, u8** argv);
int rotate_delete(_obj* ele, void* arg);
int rotate_attach(struct halfrel* self, struct halfrel* peer);
int rotate_detach(struct halfrel* self, struct halfrel* peer);
int rotate_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int rotate_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int scale_create(_obj* ele, void* arg, int argc, u8** argv);
int scale_delete(_obj* ele, void* arg);
int scale_attach(struct halfrel* self, struct halfrel* peer);
int scale_detach(struct halfrel* self, struct halfrel* peer);
int scale_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int scale_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int recut_create(_obj* ele, void* arg, int argc, u8** argv);
int recut_delete(_obj* ele, void* arg);
int recut_attach(struct halfrel* self, struct halfrel* peer);
int recut_detach(struct halfrel* self, struct halfrel* peer);
int recut_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int recut_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int reline_create(_obj* ele, void* arg, int argc, u8** argv);
int reline_delete(_obj* ele, void* arg);
int reline_attach(struct halfrel* self, struct halfrel* peer);
int reline_detach(struct halfrel* self, struct halfrel* peer);
int reline_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int reline_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int reorder_create(_obj* ele, void* arg, int argc, u8** argv);
int reorder_delete(_obj* ele, void* arg);
int reorder_attach(struct halfrel* self, struct halfrel* peer);
int reorder_detach(struct halfrel* self, struct halfrel* peer);
int reorder_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int reorder_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int qu2eu_create(_obj* ele, void* arg, int argc, u8** argv);
int qu2eu_delete(_obj* ele, void* arg);
int qu2eu_attach(struct halfrel* self, struct halfrel* peer);
int qu2eu_detach(struct halfrel* self, struct halfrel* peer);
int qu2eu_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int qu2eu_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int str2vec_create(_obj* ele, void* arg, int argc, u8** argv);
int str2vec_delete(_obj* ele, void* arg);
int str2vec_attach(struct halfrel* self, struct halfrel* peer);
int str2vec_detach(struct halfrel* self, struct halfrel* peer);
int str2vec_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int str2vec_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int img2pbr_create(_obj* ele, void* arg, int argc, u8** argv);
int img2pbr_delete(_obj* ele, void* arg);
int img2pbr_attach(struct halfrel* self, struct halfrel* peer);
int img2pbr_detach(struct halfrel* self, struct halfrel* peer);
int img2pbr_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int img2pbr_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int easyag_create(_obj* ele, void* arg, int argc, u8** argv);
int easyag_delete(_obj* ele, void* arg);
int easyag_attach(struct halfrel* self, struct halfrel* peer);
int easyag_detach(struct halfrel* self, struct halfrel* peer);
int easyag_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int easyag_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int mahony_create(_obj* ele, void* arg, int argc, u8** argv);
int mahony_delete(_obj* ele, void* arg);
int mahony_attach(struct halfrel* self, struct halfrel* peer);
int mahony_detach(struct halfrel* self, struct halfrel* peer);
int mahony_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int mahony_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int madgwick_create(_obj* ele, void* arg, int argc, u8** argv);
int madgwick_delete(_obj* ele, void* arg);
int madgwick_attach(struct halfrel* self, struct halfrel* peer);
int madgwick_detach(struct halfrel* self, struct halfrel* peer);
int madgwick_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int madgwick_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//uart.gcode
int gcodeclient_create(_obj* ele, void* arg, int argc, u8** argv);
int gcodeclient_delete(_obj* ele, void* arg);
int gcodeclient_attach(struct halfrel* self, struct halfrel* peer);
int gcodeclient_detach(struct halfrel* self, struct halfrel* peer);
int gcodeclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int gcodeclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int gcodeserver_create(_obj* ele, void* arg, int argc, u8** argv);
int gcodeserver_delete(_obj* ele, void* arg);
int gcodeserver_attach(struct halfrel* self, struct halfrel* peer);
int gcodeserver_detach(struct halfrel* self, struct halfrel* peer);
int gcodeserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int gcodeserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int mavlinkclient_create(_obj* ele, void* arg, int argc, u8** argv);
int mavlinkclient_delete(_obj* ele, void* arg);
int mavlinkclient_attach(struct halfrel* self, struct halfrel* peer);
int mavlinkclient_detach(struct halfrel* self, struct halfrel* peer);
int mavlinkclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int mavlinkclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int mavlinkserver_create(_obj* ele, void* arg, int argc, u8** argv);
int mavlinkserver_delete(_obj* ele, void* arg);
int mavlinkserver_attach(struct halfrel* self, struct halfrel* peer);
int mavlinkserver_detach(struct halfrel* self, struct halfrel* peer);
int mavlinkserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int mavlinkserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int nema0183client_create(_obj* ele, void* arg, int argc, u8** argv);
int nema0183client_delete(_obj* ele, void* arg);
int nema0183client_attach(struct halfrel* self, struct halfrel* peer);
int nema0183client_detach(struct halfrel* self, struct halfrel* peer);
int nema0183client_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int nema0183client_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int nema0183server_create(_obj* ele, void* arg, int argc, u8** argv);
int nema0183server_delete(_obj* ele, void* arg);
int nema0183server_attach(struct halfrel* self, struct halfrel* peer);
int nema0183server_detach(struct halfrel* self, struct halfrel* peer);
int nema0183server_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int nema0183server_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//socket
int proxyclient_create(_obj* ele, void* arg, int argc, u8** argv);
int proxyclient_delete(_obj* ele, void* arg);
int proxyclient_attach(struct halfrel* self, struct halfrel* peer);
int proxyclient_detach(struct halfrel* self, struct halfrel* peer);
int proxyclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int proxyclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int proxyserver_create(_obj* ele, void* arg, int argc, u8** argv);
int proxyserver_delete(_obj* ele, void* arg);
int proxyserver_attach(struct halfrel* self, struct halfrel* peer);
int proxyserver_detach(struct halfrel* self, struct halfrel* peer);
int proxyserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int proxyserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int proxymaster_create(_obj* ele, void* arg, int argc, u8** argv);
int proxymaster_delete(_obj* ele, void* arg);
int proxymaster_attach(struct halfrel* self, struct halfrel* peer);
int proxymaster_detach(struct halfrel* self, struct halfrel* peer);
int proxymaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int proxymaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int socksclient_create(_obj* ele, void* arg, int argc, u8** argv);
int socksclient_delete(_obj* ele, void* arg);
int socksclient_attach(struct halfrel* self, struct halfrel* peer);
int socksclient_detach(struct halfrel* self, struct halfrel* peer);
int socksclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int socksclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int socksserver_create(_obj* ele, void* arg, int argc, u8** argv);
int socksserver_delete(_obj* ele, void* arg);
int socksserver_attach(struct halfrel* self, struct halfrel* peer);
int socksserver_detach(struct halfrel* self, struct halfrel* peer);
int socksserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int socksserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int socksmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int socksmaster_delete(_obj* ele, void* arg);
int socksmaster_attach(struct halfrel* self, struct halfrel* peer);
int socksmaster_detach(struct halfrel* self, struct halfrel* peer);
int socksmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int socksmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//udp.dns
int dnsclient_create(_obj* ele, void* arg, int argc, u8** argv);
int dnsclient_delete(_obj* ele, void* arg);
int dnsclient_attach(struct halfrel* self, struct halfrel* peer);
int dnsclient_detach(struct halfrel* self, struct halfrel* peer);
int dnsclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dnsclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dnsserver_create(_obj* ele, void* arg, int argc, u8** argv);
int dnsserver_delete(_obj* ele, void* arg);
int dnsserver_attach(struct halfrel* self, struct halfrel* peer);
int dnsserver_detach(struct halfrel* self, struct halfrel* peer);
int dnsserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dnsserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//udp.ntp
int ntpclient_create(_obj* ele, void* arg, int argc, u8** argv);
int ntpclient_delete(_obj* ele, void* arg);
int ntpclient_attach(struct halfrel* self, struct halfrel* peer);
int ntpclient_detach(struct halfrel* self, struct halfrel* peer);
int ntpclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int ntpclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int ntpserver_create(_obj* ele, void* arg, int argc, u8** argv);
int ntpserver_delete(_obj* ele, void* arg);
int ntpserver_attach(struct halfrel* self, struct halfrel* peer);
int ntpserver_detach(struct halfrel* self, struct halfrel* peer);
int ntpserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int ntpserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//udp.bootp
int bootpclient_create(_obj* ele, void* arg, int argc, u8** argv);
int bootpclient_delete(_obj* ele, void* arg);
int bootpclient_attach(struct halfrel* self, struct halfrel* peer);
int bootpclient_detach(struct halfrel* self, struct halfrel* peer);
int bootpclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int bootpclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int bootpserver_create(_obj* ele, void* arg, int argc, u8** argv);
int bootpserver_delete(_obj* ele, void* arg);
int bootpserver_attach(struct halfrel* self, struct halfrel* peer);
int bootpserver_detach(struct halfrel* self, struct halfrel* peer);
int bootpserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int bootpserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//udp.dhcp
int dhcpclient_create(_obj* ele, void* arg, int argc, u8** argv);
int dhcpclient_delete(_obj* ele, void* arg);
int dhcpclient_attach(struct halfrel* self, struct halfrel* peer);
int dhcpclient_detach(struct halfrel* self, struct halfrel* peer);
int dhcpclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dhcpclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dhcpserver_create(_obj* ele, void* arg, int argc, u8** argv);
int dhcpserver_delete(_obj* ele, void* arg);
int dhcpserver_attach(struct halfrel* self, struct halfrel* peer);
int dhcpserver_detach(struct halfrel* self, struct halfrel* peer);
int dhcpserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int dhcpserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//udp.tftp
int tftpclient_create(_obj* ele, void* arg, int argc, u8** argv);
int tftpclient_delete(_obj* ele, void* arg);
int tftpclient_attach(struct halfrel* self, struct halfrel* peer);
int tftpclient_detach(struct halfrel* self, struct halfrel* peer);
int tftpclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tftpclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tftpserver_create(_obj* ele, void* arg, int argc, u8** argv);
int tftpserver_delete(_obj* ele, void* arg);
int tftpserver_attach(struct halfrel* self, struct halfrel* peer);
int tftpserver_detach(struct halfrel* self, struct halfrel* peer);
int tftpserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tftpserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//udp.quic
int quicclient_create(_obj* ele, void* arg, int argc, u8** argv);
int quicclient_delete(_obj* ele, void* arg);
int quicclient_attach(struct halfrel* self, struct halfrel* peer);
int quicclient_detach(struct halfrel* self, struct halfrel* peer);
int quicclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int quicclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int quicserver_create(_obj* ele, void* arg, int argc, u8** argv);
int quicserver_delete(_obj* ele, void* arg);
int quicserver_attach(struct halfrel* self, struct halfrel* peer);
int quicserver_detach(struct halfrel* self, struct halfrel* peer);
int quicserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int quicserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int quicmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int quicmaster_delete(_obj* ele, void* arg);
int quicmaster_attach(struct halfrel* self, struct halfrel* peer);
int quicmaster_detach(struct halfrel* self, struct halfrel* peer);
int quicmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int quicmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//trav
int udptravclient_create(_obj* ele, void* arg, int argc, u8** argv);
int udptravclient_delete(_obj* ele, void* arg);
int udptravclient_attach(struct halfrel* self, struct halfrel* peer);
int udptravclient_detach(struct halfrel* self, struct halfrel* peer);
int udptravclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int udptravclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int udptravserver_create(_obj* ele, void* arg, int argc, u8** argv);
int udptravserver_delete(_obj* ele, void* arg);
int udptravserver_attach(struct halfrel* self, struct halfrel* peer);
int udptravserver_detach(struct halfrel* self, struct halfrel* peer);
int udptravserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int udptravserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int udptravmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int udptravmaster_delete(_obj* ele, void* arg);
int udptravmaster_attach(struct halfrel* self, struct halfrel* peer);
int udptravmaster_detach(struct halfrel* self, struct halfrel* peer);
int udptravmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int udptravmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tcptravclient_create(_obj* ele, void* arg, int argc, u8** argv);
int tcptravclient_delete(_obj* ele, void* arg);
int tcptravclient_attach(struct halfrel* self, struct halfrel* peer);
int tcptravclient_detach(struct halfrel* self, struct halfrel* peer);
int tcptravclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tcptravclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tcptravserver_create(_obj* ele, void* arg, int argc, u8** argv);
int tcptravserver_delete(_obj* ele, void* arg);
int tcptravserver_attach(struct halfrel* self, struct halfrel* peer);
int tcptravserver_detach(struct halfrel* self, struct halfrel* peer);
int tcptravserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tcptravserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tcptravmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int tcptravmaster_delete(_obj* ele, void* arg);
int tcptravmaster_attach(struct halfrel* self, struct halfrel* peer);
int tcptravmaster_detach(struct halfrel* self, struct halfrel* peer);
int tcptravmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tcptravmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//tcp.ssh
int sshclient_create(_obj* ele, void* arg, int argc, u8** argv);
int sshclient_delete(_obj* ele, void* arg);
int sshclient_attach(struct halfrel* self, struct halfrel* peer);
int sshclient_detach(struct halfrel* self, struct halfrel* peer);
int sshclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int sshclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int sshserver_create(_obj* ele, void* arg, int argc, u8** argv);
int sshserver_delete(_obj* ele, void* arg);
int sshserver_attach(struct halfrel* self, struct halfrel* peer);
int sshserver_detach(struct halfrel* self, struct halfrel* peer);
int sshserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int sshserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int sshmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int sshmaster_delete(_obj* ele, void* arg);
int sshmaster_attach(struct halfrel* self, struct halfrel* peer);
int sshmaster_detach(struct halfrel* self, struct halfrel* peer);
int sshmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int sshmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//tcp.telnet
int telnetclient_create(_obj* ele, void* arg, int argc, u8** argv);
int telnetclient_delete(_obj* ele, void* arg);
int telnetclient_attach(struct halfrel* self, struct halfrel* peer);
int telnetclient_detach(struct halfrel* self, struct halfrel* peer);
int telnetclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int telnetclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int telnetserver_create(_obj* ele, void* arg, int argc, u8** argv);
int telnetserver_delete(_obj* ele, void* arg);
int telnetserver_attach(struct halfrel* self, struct halfrel* peer);
int telnetserver_detach(struct halfrel* self, struct halfrel* peer);
int telnetserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int telnetserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int telnetmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int telnetmaster_delete(_obj* ele, void* arg);
int telnetmaster_attach(struct halfrel* self, struct halfrel* peer);
int telnetmaster_detach(struct halfrel* self, struct halfrel* peer);
int telnetmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int telnetmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//tcp.rdp
int rdpclient_create(_obj* ele, void* arg, int argc, u8** argv);
int rdpclient_delete(_obj* ele, void* arg);
int rdpclient_attach(struct halfrel* self, struct halfrel* peer);
int rdpclient_detach(struct halfrel* self, struct halfrel* peer);
int rdpclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int rdpclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int rdpserver_create(_obj* ele, void* arg, int argc, u8** argv);
int rdpserver_delete(_obj* ele, void* arg);
int rdpserver_attach(struct halfrel* self, struct halfrel* peer);
int rdpserver_detach(struct halfrel* self, struct halfrel* peer);
int rdpserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int rdpserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int rdpmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int rdpmaster_delete(_obj* ele, void* arg);
int rdpmaster_attach(struct halfrel* self, struct halfrel* peer);
int rdpmaster_detach(struct halfrel* self, struct halfrel* peer);
int rdpmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int rdpmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//tcp.vnc
int vncclient_create(_obj* ele, void* arg, int argc, u8** argv);
int vncclient_delete(_obj* ele, void* arg);
int vncclient_attach(struct halfrel* self, struct halfrel* peer);
int vncclient_detach(struct halfrel* self, struct halfrel* peer);
int vncclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int vncclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int vncserver_create(_obj* ele, void* arg, int argc, u8** argv);
int vncserver_delete(_obj* ele, void* arg);
int vncserver_attach(struct halfrel* self, struct halfrel* peer);
int vncserver_detach(struct halfrel* self, struct halfrel* peer);
int vncserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int vncserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int vncmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int vncmaster_delete(_obj* ele, void* arg);
int vncmaster_attach(struct halfrel* self, struct halfrel* peer);
int vncmaster_detach(struct halfrel* self, struct halfrel* peer);
int vncmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int vncmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//http1
int httpclient_create(_obj* ele, void* arg, int argc, u8** argv);
int httpclient_delete(_obj* ele, void* arg);
int httpclient_attach(struct halfrel* self, struct halfrel* peer);
int httpclient_detach(struct halfrel* self, struct halfrel* peer);
int httpclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int httpclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int httpserver_create(_obj* ele, void* arg, int argc, u8** argv);
int httpserver_delete(_obj* ele, void* arg);
int httpserver_attach(struct halfrel* self, struct halfrel* peer);
int httpserver_detach(struct halfrel* self, struct halfrel* peer);
int httpserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int httpserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int httpmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int httpmaster_delete(_obj* ele, void* arg);
int httpmaster_attach(struct halfrel* self, struct halfrel* peer);
int httpmaster_detach(struct halfrel* self, struct halfrel* peer);
int httpmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int httpmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//http3
int http3client_create(_obj* ele, void* arg, int argc, u8** argv);
int http3client_delete(_obj* ele, void* arg);
int http3client_attach(struct halfrel* self, struct halfrel* peer);
int http3client_detach(struct halfrel* self, struct halfrel* peer);
int http3client_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int http3client_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int http3server_create(_obj* ele, void* arg, int argc, u8** argv);
int http3server_delete(_obj* ele, void* arg);
int http3server_attach(struct halfrel* self, struct halfrel* peer);
int http3server_detach(struct halfrel* self, struct halfrel* peer);
int http3server_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int http3server_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int http3master_create(_obj* ele, void* arg, int argc, u8** argv);
int http3master_delete(_obj* ele, void* arg);
int http3master_attach(struct halfrel* self, struct halfrel* peer);
int http3master_detach(struct halfrel* self, struct halfrel* peer);
int http3master_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int http3master_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//tcp.ws
int wsclient_create(_obj* ele, void* arg, int argc, u8** argv);
int wsclient_delete(_obj* ele, void* arg);
int wsclient_attach(struct halfrel* self, struct halfrel* peer);
int wsclient_detach(struct halfrel* self, struct halfrel* peer);
int wsclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int wsclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int wsserver_create(_obj* ele, void* arg, int argc, u8** argv);
int wsserver_delete(_obj* ele, void* arg);
int wsserver_attach(struct halfrel* self, struct halfrel* peer);
int wsserver_detach(struct halfrel* self, struct halfrel* peer);
int wsserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int wsserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int wsmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int wsmaster_delete(_obj* ele, void* arg);
int wsmaster_attach(struct halfrel* self, struct halfrel* peer);
int wsmaster_detach(struct halfrel* self, struct halfrel* peer);
int wsmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int wsmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//tcp.tls1.2
int tls1v2client_create(_obj* ele, void* arg, int argc, u8** argv);
int tls1v2client_delete(_obj* ele, void* arg);
int tls1v2client_attach(struct halfrel* self, struct halfrel* peer);
int tls1v2client_detach(struct halfrel* self, struct halfrel* peer);
int tls1v2client_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v2client_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v2server_create(_obj* ele, void* arg, int argc, u8** argv);
int tls1v2server_delete(_obj* ele, void* arg);
int tls1v2server_attach(struct halfrel* self, struct halfrel* peer);
int tls1v2server_detach(struct halfrel* self, struct halfrel* peer);
int tls1v2server_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v2server_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v2master_create(_obj* ele, void* arg, int argc, u8** argv);
int tls1v2master_delete(_obj* ele, void* arg);
int tls1v2master_attach(struct halfrel* self, struct halfrel* peer);
int tls1v2master_detach(struct halfrel* self, struct halfrel* peer);
int tls1v2master_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v2master_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//tcp.tls1.3
int tls1v3client_create(_obj* ele, void* arg, int argc, u8** argv);
int tls1v3client_delete(_obj* ele, void* arg);
int tls1v3client_attach(struct halfrel* self, struct halfrel* peer);
int tls1v3client_detach(struct halfrel* self, struct halfrel* peer);
int tls1v3client_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v3client_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v3server_create(_obj* ele, void* arg, int argc, u8** argv);
int tls1v3server_delete(_obj* ele, void* arg);
int tls1v3server_attach(struct halfrel* self, struct halfrel* peer);
int tls1v3server_detach(struct halfrel* self, struct halfrel* peer);
int tls1v3server_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v3server_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v3master_create(_obj* ele, void* arg, int argc, u8** argv);
int tls1v3master_delete(_obj* ele, void* arg);
int tls1v3master_attach(struct halfrel* self, struct halfrel* peer);
int tls1v3master_detach(struct halfrel* self, struct halfrel* peer);
int tls1v3master_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int tls1v3master_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//fuckgfw
int fuckgfwclient_create(_obj* ele, void* arg, int argc, u8** argv);
int fuckgfwclient_delete(_obj* ele, void* arg);
int fuckgfwclient_attach(struct halfrel* self, struct halfrel* peer);
int fuckgfwclient_detach(struct halfrel* self, struct halfrel* peer);
int fuckgfwclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fuckgfwclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fuckgfwserver_create(_obj* ele, void* arg, int argc, u8** argv);
int fuckgfwserver_delete(_obj* ele, void* arg);
int fuckgfwserver_attach(struct halfrel* self, struct halfrel* peer);
int fuckgfwserver_detach(struct halfrel* self, struct halfrel* peer);
int fuckgfwserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fuckgfwserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fuckgfwmaster_create(_obj* ele, void* arg, int argc, u8** argv);
int fuckgfwmaster_delete(_obj* ele, void* arg);
int fuckgfwmaster_attach(struct halfrel* self, struct halfrel* peer);
int fuckgfwmaster_detach(struct halfrel* self, struct halfrel* peer);
int fuckgfwmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int fuckgfwmaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//tcp.party
int partyclient_create(_obj* ele, void* arg, int argc, u8** argv);
int partyclient_delete(_obj* ele, void* arg);
int partyclient_attach(struct halfrel* self, struct halfrel* peer);
int partyclient_detach(struct halfrel* self, struct halfrel* peer);
int partyclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int partyclient_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int partyserver_create(_obj* ele, void* arg, int argc, u8** argv);
int partyserver_delete(_obj* ele, void* arg);
int partyserver_attach(struct halfrel* self, struct halfrel* peer);
int partyserver_detach(struct halfrel* self, struct halfrel* peer);
int partyserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int partyserver_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int partymaster_create(_obj* ele, void* arg, int argc, u8** argv);
int partymaster_delete(_obj* ele, void* arg);
int partymaster_attach(struct halfrel* self, struct halfrel* peer);
int partymaster_detach(struct halfrel* self, struct halfrel* peer);
int partymaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
int partymaster_read( _obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);



#define datahome qqq
static struct item* obj = 0;
static void* ppp = 0;
static struct item* ele = 0;
static void* qqq = 0;
static int elelen = 0;
static int qqqlen = 0;




void artery_init(u8* addr)
{
	say("[a,c):artery initing\n");

	int j;
	ele = (void*)(addr+0x000000);
	qqq = (void*)(addr+0x100000);

#define max (0x100000/sizeof(_obj))
	for(j=0;j<0x200000;j++)addr[j] = 0;
	for(j=0;j<max;j++)ele[j].tier = _art_;

	//artery_create(0, (u8*)"HACK://0.0.0.0:2222");
	//artery_create(0, (u8*)"QUIC://0.0.0.0:4444");
	//artery_create(0,  (u8*)"SSH://0.0.0.0:2222");
	//artery_create(0, (u8*)"HTTP://0.0.0.0:4444");

	say("[a,c):artery inited\n");
}
void artery_exit()
{
	say("[a,c):artery exiting\n");

	qqq = 0;
	ele = 0;

	say("[a,c):artery exited\n");
}
void* artery_alloc()
{
	void* addr = &ele[elelen];
	elelen += 1;
	return addr;
}
void artery_recycle()
{
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




void* artery_create(u64 type, void* arg, int argc, u8** argv)
{
	int j,fd,ret,port;
	_obj* e;
	_obj* f;
	u8* url = arg;
	if(0 == type)
	{
		ret = parsetypefromurl(url, (void*)&type);
		if(0 == ret)return 0;	//unknown

		url += ret;
	}

	//
	if(_control_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _control_;
		control_create(e, url, argc, argv);
		return e;
	}
	if(_crawler_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _crawler_;
		crawler_create(e, url, argc, argv);
		return e;
	}
	if(_search_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _search_;
		search_create(e, url, argc, argv);
		return e;
	}
	if(_vt100_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _vt100_;
		vt100_create(e, url, argc, argv);
		return e;
	}

	if(_ann_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _ann_;
		ann_create(e, url, argc, argv);
		return e;
	}

	//
	if(_pump_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _pump_;
		pump_create(e, url, argc, argv);
		return e;
	}
	if(_stor_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _stor_;
		stor_create(e, url, argc, argv);
		return e;
	}
	if(_filetype_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _filetype_;
		filetype_create(e, url, argc, argv);
		return e;
	}
	if(_fileauto_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _fileauto_;
		fileauto_create(e, url, argc, argv);
		return e;
	}

	//part table
	if(_mbr_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _mbr_;
		mbrclient_create(e, url, argc, argv);
		return e;
	}
	if(_gpt_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _gpt_;
		gptclient_create(e, url, argc, argv);
		return e;
	}

	//file system
	if(_fat_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _fat_;
		fatclient_create(e, url, argc, argv);
		return e;
	}
	if(_ntfs_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _ntfs_;
		ntfsclient_create(e, url, argc, argv);
		return e;
	}
	if(_hfs_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _hfs_;
		hfsclient_create(e, url, argc, argv);
		return e;
	}
	if(_ext_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _ext_;
		extclient_create(e, url, argc, argv);
		return e;
	}

	//codec
	if(_h264_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _h264_;
		h264_create(e, url, argc, argv);
		return e;
	}

	//mux
	if(_easymux_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _easymux_;
		easymux_create(e, url, argc, argv);
		return e;
	}
	if(_mediamux_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _mediamux_;
		mediamux_create(e, url, argc, argv);
		return e;
	}

	//test
	if(_echo_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _echo_;
		echo_create(e, url, argc, argv);
		return e;
	}
	if(_dbgf32_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _dbgf32_;
		dbgf32_create(e, url, argc, argv);
		return e;
	}
	if(_dbghex_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _dbghex_;
		dbghex_create(e, url, argc, argv);
		return e;
	}
	if(_goslow_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _goslow_;
		goslow_create(e, url, argc, argv);
		return e;
	}

	//
	if(_fftpcm_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _fftpcm_;
		fftpcm_create(e, url, argc, argv);
		return e;
	}
	if(_fftrgb_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _fftrgb_;
		fftrgb_create(e, url, argc, argv);
		return e;
	}

	//image
	if(_img2pbr_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _img2pbr_;
		img2pbr_create(e, url, argc, argv);
		return e;
	}
	if(_rotate_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _rotate_;
		rotate_create(e, url, argc, argv);
		return e;
	}
	if(_picfmt_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _picfmt_;
		picfmt_create(e, url, argc, argv);
		return e;
	}
	if(_sobel_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _sobel_;
		sobel_create(e, url, argc, argv);
		return e;
	}

	//
	if(_recut_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _recut_;
		recut_create(e, url, argc, argv);
		return e;
	}
	if(_reline_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _reline_;
		reline_create(e, url, argc, argv);
		return e;
	}
	if(_reorder_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _reorder_;
		reorder_create(e, url, argc, argv);
		return e;
	}

	//
	if(_qu2eu_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _qu2eu_;
		qu2eu_create(e, url, argc, argv);
		return e;
	}
	if(_str2vec_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _str2vec_;
		str2vec_create(e, url, argc, argv);
		return e;
	}

	//ahrs
	if(_easyag_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _easyag_;
		easyag_create(e, url, argc, argv);
		return e;
	}
	if(_mahony_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _mahony_;
		mahony_create(e, url, argc, argv);
		return e;
	}
	if(_madgwick_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _madgwick_;
		madgwick_create(e, url, argc, argv);
		return e;
	}

	//
	if(_gcode_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _gcode_;
		gcodeclient_create(e, url, argc, argv);
		return e;
	}
	if(_Gcode_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Gcode_;
		gcodeserver_create(e, url, argc, argv);
		return e;
	}
	if(_mavlink_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _mavlink_;
		mavlinkclient_create(e, url, argc, argv);
		return e;
	}
	if(_Mavlink_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Mavlink_;
		mavlinkserver_create(e, url, argc, argv);
		return e;
	}
	if(_nema0183_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _nema0183_;
		nema0183client_create(e, url, argc, argv);
		return e;
	}
	if(_Nema0183_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Nema0183_;
		nema0183server_create(e, url, argc, argv);
		return e;
	}

	//boop
	if(_BOOTP_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _BOOTP_;
		bootpserver_create(e, url, argc, argv);
		return e;
	}
	if(_bootp_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _bootp_;
		bootpclient_create(e, url, argc, argv);
		return e;
	}

	//dhcp
	if(_DHCP_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _DHCP_;
		dhcpserver_create(e, url, argc, argv);
		return e;
	}
	if(_dhcp_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _dhcp_;
		dhcpclient_create(e, url, argc, argv);
		return e;
	}

	//dns
	if(_DNS_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _DNS_;
		dnsserver_create(e, url, argc, argv);
		return e;
	}
	if(_dns_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _dns_;
		dnsclient_create(e, url, argc, argv);
		return e;
	}

	//ntp
	if(_NTP_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _NTP_;
		ntpserver_create(e, url, argc, argv);
		return e;
	}
	if(_ntp_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _ntp_;
		ntpclient_create(e, url, argc, argv);
		return e;
	}

	//tftp: client,server
	if(_tftp_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _tftp_;
		tftpclient_create(e, url, argc, argv);
		return e;
	}
	if(_Tftp_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Tftp_;
		tftpserver_create(e, url, argc, argv);
		return e;
	}

	//quic: master,server,client
	if(_QUIC_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _QUIC_;
		quicmaster_create(e, url, argc, argv);
		return e;
	}
	if(_Quic_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Quic_;
		quicserver_create(e, url, argc, argv);
		return e;
	}
	if(_quic_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _quic_;
		quicclient_create(e, url, argc, argv);
		return e;
	}

	//udptrav: master,server,client
	if(_UDPTRAV_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _UDPTRAV_;
		udptravmaster_create(e, url, argc, argv);
		return e;
	}
	if(_Udptrav_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Udptrav_;
		udptravserver_create(e, url, argc, argv);
		return e;
	}
	if(_udptrav_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _udptrav_;
		udptravclient_create(e, url, argc, argv);
		return e;
	}

	//tcptrav: master,server,client
	if(_TCPTRAV_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _TCPTRAV_;
		tcptravmaster_create(e, url, argc, argv);
		return e;
	}
	if(_Tcptrav_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Tcptrav_;
		tcptravserver_create(e, url, argc, argv);
		return e;
	}
	if(_tcptrav_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _tcptrav_;
		tcptravclient_create(e, url, argc, argv);
		return e;
	}

	//
	if(_PROXY_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _PROXY_;
		proxymaster_create(e, url, argc, argv);
		return e;
	}
	if(_Proxy_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Proxy_;
		proxyserver_create(e, url, argc, argv);
		return e;
	}
	if(_proxy_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _proxy_;
		proxyclient_create(e, url, argc, argv);
		return e;
	}
	if(_SOCKS_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _SOCKS_;
		socksmaster_create(e, url, argc, argv);
		return e;
	}
	if(_Socks_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Socks_;
		socksserver_create(e, url, argc, argv);
		return e;
	}
	if(_socks_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _socks_;
		socksclient_create(e, url, argc, argv);
		return e;
	}

	//ssh: master,server,client
	if(_SSH_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _SSH_;
		sshmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Ssh_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Ssh_;
		sshserver_create(e, url, argc, argv);

		return e;
	}
	if(_ssh_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _ssh_;
		sshclient_create(e, url, argc, argv);

		return e;
	}

	//telnet: master,server,client
	if(_TELNET_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _TELNET_;
		telnetmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Telnet_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Telnet_;
		telnetserver_create(e, url, argc, argv);

		return e;
	}
	if(_telnet_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _telnet_;
		telnetclient_create(e, url, argc, argv);

		return e;
	}

	//rdp: master,server,client
	if(_RDP_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _RDP_;
		rdpmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Rdp_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Rdp_;
		rdpserver_create(e, url, argc, argv);

		return e;
	}
	if(_rdp_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _rdp_;
		rdpclient_create(e, url, argc, argv);

		return e;
	}

	//vnc: master,server,client
	if(_VNC_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _VNC_;
		vncmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Vnc_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Vnc_;
		vncserver_create(e, url, argc, argv);

		return e;
	}
	if(_vnc_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _vnc_;
		vncclient_create(e, url, argc, argv);

		return e;
	}

	//http: master,server,client
	if(_HTTP_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _HTTP_;
		httpmaster_create(e, url, argc, argv);
		return e;
	}
	if(_Http_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Http_;
		httpserver_create(e, url, argc, argv);
		return e;
	}
	if(_http_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _http_;
		httpclient_create(e, url, argc, argv);

		return e;
	}

	//http: master,server,client
	if(_HTTP3_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _HTTP3_;
		http3master_create(e, url, argc, argv);
		return e;
	}
	if(_Http3_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Http3_;
		http3server_create(e, url, argc, argv);
		return e;
	}
	if(_http3_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _http3_;
		http3client_create(e, url, argc, argv);

		return e;
	}

	//ws: master,server,client
	if(_WS_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _WS_;
		wsmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Ws_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Ws_;
		wsserver_create(e, url, argc, argv);

		return e;
	}
	if(_ws_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _ws_;
		wsclient_create(e, url, argc, argv);

		return e;
	}

	//tls: master,server,client
	if(_TLS1_2_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _TLS1_2_;
		tls1v2master_create(e, url, argc, argv);

		return e;
	}
	if(_Tls1_2_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Tls1_2_;
		tls1v2server_create(e, url, argc, argv);

		return e;
	}
	if(_tls1_2_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _tls1_2_;
		tls1v2client_create(e, url, argc, argv);

		return e;
	}
	if(_TLS1_3_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _TLS1_3_;
		tls1v3master_create(e, url, argc, argv);

		return e;
	}
	if(_Tls1_3_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Tls1_3_;
		tls1v3server_create(e, url, argc, argv);

		return e;
	}
	if(_tls1_3_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _tls1_3_;
		tls1v3client_create(e, url, argc, argv);

		return e;
	}

	//fuckgfw: master,server,client
	if(_FUCKGFW_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _FUCKGFW_;
		fuckgfwmaster_create(e, url, argc, argv);

		return e;
	}
	if(_Fuckgfw_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Fuckgfw_;
		fuckgfwserver_create(e, url, argc, argv);

		return e;
	}
	if(_fuckgfw_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _fuckgfw_;
		fuckgfwclient_create(e, url, argc, argv);

		return e;
	}

	//serve: master,server,client
	if(_PARTY_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _PARTY_;
		partymaster_create(e, url, argc, argv);

		return e;
	}
	if(_Party_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _Party_;
		partyserver_create(e, url, argc, argv);

		return e;
	}
	if(_party_ == type)
	{
		e = artery_alloc();
		if(0 == e)return 0;

		e->type = _party_;
		partyclient_create(e, url, argc, argv);

		return e;
	}

	return 0;
}
int artery_delete(_obj* ele)
{
	return 0;
}
int artery_reader(_obj* art,void* foot, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int artery_writer(_obj* art,void* foot, void* arg, int idx, void* buf, int len)
{
	return 0;
}




int artery_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ele;
	say("@artery_attach\n");

	ele = self->pchip;
	if(0 == ele)return 0;

	switch(ele->type){
	case _ann_:return ann_attach(self, peer);break;
	//case _cnn_:return cnn_attach(self, peer);break;

	case _filetype_:return filetype_attach(self, peer);break;
	case _fileauto_:return fileauto_attach(self, peer);break;

	case _gpt_:return gptclient_attach(self, peer);break;
	case _mbr_:return mbrclient_attach(self, peer);break;

	case _fat_:return fatclient_attach(self, peer);break;
	case _hfs_:return hfsclient_attach(self, peer);break;

	case _picfmt_:return picfmt_attach(self, peer);break;
	case _sobel_:return sobel_attach(self, peer);break;

	case _h264_:return h264_attach(self, peer);break;

	case _dns_:return dnsclient_attach(self, peer);break;
	case _DNS_:return dnsserver_attach(self, peer);break;
	case _ntp_:return ntpclient_attach(self, peer);break;
	case _NTP_:return ntpserver_attach(self, peer);break;

	case _bootp_:return bootpclient_attach(self, peer);break;
	case _BOOTP_:return bootpserver_attach(self, peer);break;
	case _dhcp_:return dhcpclient_attach(self, peer);break;
	case _DHCP_:return dhcpserver_attach(self, peer);break;
	case _tftp_:return tftpclient_attach(self, peer);break;
	case _TFTP_:return tftpserver_attach(self, peer);break;

	case _udptrav_:return udptravclient_attach(self, peer);break;
	case _UDPTRAV_:return udptravmaster_attach(self, peer);break;
	case _tcptrav_:return tcptravclient_attach(self, peer);break;
	case _TCPTRAV_:return tcptravmaster_attach(self, peer);break;

	case _ssh_:return sshclient_attach(self, peer);break;
	case _Ssh_:return sshserver_attach(self, peer);break;
	case _SSH_:return sshmaster_attach(self, peer);break;
	case _telnet_:return telnetclient_attach(self, peer);break;
	case _Telnet_:return telnetserver_attach(self, peer);break;
	case _TELNET_:return telnetmaster_attach(self, peer);break;

	case _rdp_:return rdpclient_attach(self, peer);break;
	case _Rdp_:return rdpserver_attach(self, peer);break;
	case _RDP_:return rdpmaster_attach(self, peer);break;
	case _vnc_:return vncclient_attach(self, peer);break;
	case _Vnc_:return vncserver_attach(self, peer);break;
	case _VNC_:return vncmaster_attach(self, peer);break;

	case _http_:return httpclient_attach(self, peer);break;
	case _Http_:return httpserver_attach(self, peer);break;
	case _HTTP_:return httpmaster_attach(self, peer);break;
	case _http3_:return http3client_attach(self, peer);break;
	case _Http3_:return http3server_attach(self, peer);break;
	case _HTTP3_:return http3master_attach(self, peer);break;
	case _ws_:return wsclient_attach(self, peer);break;
	case _Ws_:return wsserver_attach(self, peer);break;
	case _WS_:return wsmaster_attach(self, peer);break;

	case _tls1_2_:return tls1v2client_attach(self, peer);break;
	case _Tls1_2_:return tls1v2server_attach(self, peer);break;
	case _TLS1_2_:return tls1v2master_attach(self, peer);break;
	case _tls1_3_:return tls1v3client_attach(self, peer);break;
	case _Tls1_3_:return tls1v3server_attach(self, peer);break;
	case _TLS1_3_:return tls1v3master_attach(self, peer);break;

	case _proxy_:return proxyclient_attach(self, peer);break;
	case _Proxy_:return proxyserver_attach(self, peer);break;
	case _PROXY_:return proxymaster_attach(self, peer);break;
	case _socks_:return socksclient_attach(self, peer);break;
	case _Socks_:return socksserver_attach(self, peer);break;
	case _SOCKS_:return socksmaster_attach(self, peer);break;

	case _party_:return partyclient_attach(self, peer);break;
	case _PARTY_:return partymaster_attach(self, peer);break;
	}//switch
	return 0;
}
int artery_detach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ele;
	say("@arterydetach\n");

	ele = self->pchip;
	if(0 == ele)return 0;

	switch(ele->type){
	case _ann_:return ann_detach(self, peer);break;
	//case _cnn_:return cnn_detach(self, peer);break;

	case _filetype_:return filetype_detach(self, peer);break;
	case _fileauto_:return fileauto_detach(self, peer);break;

	case _gpt_:return gptclient_detach(self, peer);break;
	case _mbr_:return mbrclient_detach(self, peer);break;

	case _fat_:return fatclient_detach(self, peer);break;
	case _hfs_:return hfsclient_detach(self, peer);break;

	case _picfmt_:return picfmt_detach(self, peer);break;
	case _sobel_:return sobel_detach(self, peer);break;

	case _h264_:return h264_detach(self, peer);break;

	case _dns_:return dnsclient_detach(self, peer);break;
	case _DNS_:return dnsserver_detach(self, peer);break;
	case _ntp_:return ntpclient_detach(self, peer);break;
	case _NTP_:return ntpserver_detach(self, peer);break;

	case _bootp_:return bootpclient_detach(self, peer);break;
	case _BOOTP_:return bootpserver_detach(self, peer);break;
	case _dhcp_:return dhcpclient_detach(self, peer);break;
	case _DHCP_:return dhcpserver_detach(self, peer);break;
	case _tftp_:return tftpclient_detach(self, peer);break;
	case _TFTP_:return tftpserver_detach(self, peer);break;

	case _udptrav_:return udptravclient_detach(self, peer);break;
	case _UDPTRAV_:return udptravmaster_detach(self, peer);break;
	case _tcptrav_:return tcptravclient_detach(self, peer);break;
	case _TCPTRAV_:return tcptravmaster_detach(self, peer);break;

	case _ssh_:return sshclient_detach(self, peer);break;
	case _Ssh_:return sshserver_detach(self, peer);break;
	case _SSH_:return sshmaster_detach(self, peer);break;
	case _telnet_:return telnetclient_detach(self, peer);break;
	case _Telnet_:return telnetserver_detach(self, peer);break;
	case _TELNET_:return telnetmaster_detach(self, peer);break;

	case _rdp_:return rdpclient_detach(self, peer);break;
	case _Rdp_:return rdpserver_detach(self, peer);break;
	case _RDP_:return rdpmaster_detach(self, peer);break;
	case _vnc_:return vncclient_detach(self, peer);break;
	case _Vnc_:return vncserver_detach(self, peer);break;
	case _VNC_:return vncmaster_detach(self, peer);break;

	case _http_:return httpclient_detach(self, peer);break;
	case _Http_:return httpserver_detach(self, peer);break;
	case _HTTP_:return httpmaster_detach(self, peer);break;
	case _http3_:return http3client_detach(self, peer);break;
	case _Http3_:return http3server_detach(self, peer);break;
	case _HTTP3_:return http3master_detach(self, peer);break;
	case _ws_:return wsclient_detach(self, peer);break;
	case _Ws_:return wsserver_detach(self, peer);break;
	case _WS_:return wsmaster_detach(self, peer);break;

	case _tls1_2_:return tls1v2client_detach(self, peer);break;
	case _Tls1_2_:return tls1v2server_detach(self, peer);break;
	case _TLS1_2_:return tls1v2master_detach(self, peer);break;
	case _tls1_3_:return tls1v3client_detach(self, peer);break;
	case _Tls1_3_:return tls1v3server_detach(self, peer);break;
	case _TLS1_3_:return tls1v3master_detach(self, peer);break;

	case _proxy_:return proxyclient_detach(self, peer);break;
	case _Proxy_:return proxyserver_detach(self, peer);break;
	case _PROXY_:return proxymaster_detach(self, peer);break;
	case _socks_:return socksclient_detach(self, peer);break;
	case _Socks_:return socksserver_detach(self, peer);break;
	case _SOCKS_:return socksmaster_detach(self, peer);break;

	case _party_:return partyclient_detach(self, peer);break;
	case _PARTY_:return partymaster_detach(self, peer);break;
	}//switch
	return 0;
}
int artery_takeby(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	switch(art->type){
	//case _gpt_:return gptclient_take(art,foot, stack,sp, arg,idx, buf,len);break;
	//case _mbr_:return mbrclient_take(art,foot, stack,sp, arg,idx, buf,len);break;

	//case _fat_:return fatclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _hfs_:return hfsclient_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _ann_:ann_read(art,foot, stack,sp, arg,idx, buf,len);break;
	//case _cnn_:cnn_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _control_:control_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _crawler_:crawler_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _search_:search_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _vt100_:vt100_read(art,foot, stack,sp, arg,idx, buf,len);break;

	//case _file_:file_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _pump_:pump_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _stor_:stor_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _h264_:h264_take(art,foot, stack,sp, arg,idx, buf,len);break;

	case _easymux_:easymux_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _mediamux_:mediamux_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _echo_:echo_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _goslow_:goslow_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _dbgf32_:dbgf32_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _dbghex_:dbghex_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _fftpcm_:fftpcm_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _fftrgb_:fftrgb_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _img2pbr_:img2pbr_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _rotate_:rotate_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _picfmt_:picfmt_take(art,foot, stack,sp, arg,idx, buf,len);break;
	case _sobel_:sobel_take(art,foot, stack,sp, arg,idx, buf,len);break;
	//case _scale_: scale_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _recut_:recut_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _reline_:reline_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _reorder_:reorder_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _qu2eu_:qu2eu_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _str2vec_:str2vec_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _easyag_:easyag_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _mahony_:mahony_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _madgwick_:madgwick_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _gcode_:gcodeclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Gcode_:gcodeserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _mavlink_:mavlinkclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Mavlink_:mavlinkserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _nema0183_:nema0183client_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Nema0183_:nema0183server_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _DNS_:dnsserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _dns_:dnsclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _NTP_:ntpserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _ntp_:ntpclient_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _BOOTP_:bootpserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _bootp_:bootpclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _DHCP_:dhcpserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _dhcp_:dhcpclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _TFTP_:tftpserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _tftp_:tftpclient_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _UDPTRAV_:udptravmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _udptrav_:udptravclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _TCPTRAV_:tcptravmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _tcptrav_:tcptravclient_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _FUCKGFW_:fuckgfwmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Fuckgfw_:fuckgfwserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _fuckgfw_:fuckgfwclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _PARTY_:partymaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Party_:partyserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _party_:partyclient_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _PROXY_:proxymaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Proxy_:proxyserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _proxy_:proxyclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _SOCKS_:socksmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Socks_:socksserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _socks_:socksclient_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _SSH_:sshmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Ssh_:sshserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _ssh_:sshclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _TELNET_:telnetmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Telnet_:telnetserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _telnet_:telnetclient_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _RDP_:sshmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Rdp_:sshserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _rdp_:sshclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _VNC_:vncmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Vnc_:vncserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _vnc_:vncclient_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _HTTP_:httpmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Http_:httpserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _http_:httpclient_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _HTTP3_:http3master_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Http3_:http3server_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _http3_:http3client_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _WS_:wsmaster_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Ws_:wsserver_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _ws_:wsclient_read(art,foot, stack,sp, arg,idx, buf,len);break;

	case _TLS1_2_:tls1v2master_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Tls1_2_:tls1v2server_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _tls1_2_:tls1v2client_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _TLS1_3_:tls1v3master_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Tls1_3_:tls1v3server_read(art,foot, stack,sp, arg,idx, buf,len);break;
	case _tls1_3_:tls1v3client_read(art,foot, stack,sp, arg,idx, buf,len);break;
	}//switch
	return 0;
}
int artery_giveby(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	//say("@arterywrite: obj=%.8s,len=%x\n", &ele->type,len);
	switch(art->type){
	//case _gpt_:return gptclient_give(art,foot, stack,sp, arg,idx, buf,len);break;
	//case _mbr_:return mbrclient_give(art,foot, stack,sp, arg,idx, buf,len);break;

	//case _fat_:return fatclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _hfs_:return hfsclient_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _ann_:return ann_write(art,foot, stack,sp, arg,idx, buf,len);break;
	//case _cnn_:return cnn_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _control_:return control_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _crawler_:return crawler_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _search_:return search_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _vt100_:return vt100_write(art,foot, stack,sp, arg,idx, buf,len);break;

	//case _file_:return file_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _pump_:return pump_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _stor_:return stor_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _h264_:return h264_give(art,foot, stack,sp, arg,idx, buf,len);break;

	case _easymux_:return easymux_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _mediamux_:return mediamux_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _echo_:return echo_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _goslow_:return goslow_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _dbgf32_:return dbgf32_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _dbghex_:return dbghex_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _fftpcm_:return fftpcm_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _fftrgb_:return fftrgb_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _img2pbr_:return img2pbr_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _rotate_:return rotate_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _picfmt_:return picfmt_give(art,foot, stack,sp, arg,idx, buf,len);break;
	case _sobel_:return sobel_give(art,foot, stack,sp, arg,idx, buf,len);break;

	case _recut_:return recut_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _reline_:return reline_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _reorder_:return reorder_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _qu2eu_:return qu2eu_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _str2vec_:return str2vec_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _easyag_:return easyag_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _mahony_:return mahony_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _madgwick_:return madgwick_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _Gcode_:return gcodeserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _gcode_:return gcodeclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Mavlink_:return mavlinkserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _mavlink_:return mavlinkclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Nema0183_:return nema0183server_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _nema0183_:return nema0183client_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _DNS_:return dnsserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _dns_:return dnsclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _NTP_:return ntpserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _ntp_:return ntpclient_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _BOOTP_:return bootpserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _bootp_:return bootpclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _DHCP_:return dhcpserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _dhcp_:return dhcpclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _TFTP_:return tftpserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _tftp_:return tftpclient_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _UDPTRAV_:return udptravmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _udptrav_:return udptravclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _TCPTRAV_:return tcptravmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _tcptrav_:return tcptravclient_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _FUCKGFW_:return fuckgfwmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Fuckgfw_:return fuckgfwserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _fuckgfw_:return fuckgfwclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _PARTY_:return partymaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Party_:return partyserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _party_:return partyclient_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _PROXY_:return proxymaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Proxy_:return proxyserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _proxy_:return proxyclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _SOCKS_:return socksmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Socks_:return socksserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _socks_:return socksclient_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _SSH_:return sshmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Ssh_:return sshserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _ssh_:return sshclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _TELNET_:return telnetmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Telnet_:return telnetserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _telnet_:return telnetclient_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _RDP_:return rdpmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Rdp_:return rdpserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _rdp_:return rdpclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _VNC_:return vncmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Vnc_:return vncserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _vnc_:return vncclient_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _HTTP_:return httpmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Http_:return httpserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _http_:return httpclient_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _HTTP3_:return http3master_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Http3_:return http3server_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _http3_:return http3client_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _WS_:return wsmaster_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Ws_:return wsserver_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _ws_:return wsclient_write(art,foot, stack,sp, arg,idx, buf,len);break;

	case _TLS1_2_:return tls1v2master_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Tls1_2_:return tls1v2server_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _tls1_2_:return tls1v2client_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _TLS1_3_:return tls1v3master_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _Tls1_3_:return tls1v3server_write(art,foot, stack,sp, arg,idx, buf,len);break;
	case _tls1_3_:return tls1v3client_write(art,foot, stack,sp, arg,idx, buf,len);break;
	}//switch
	return 0;
}




int artery_insert(u8* buf, int len)
{
	return 0;
}
int artery_remove(u8* buf, int len)
{
	return 0;
}
void* artery_search(u8* buf, int len)
{
	int j,k=0;
	_obj* art;
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
void* artery_modify(int argc, u8** argv)
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
		artery_create(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
