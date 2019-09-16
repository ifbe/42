#include "libsoft.h"
#define _fat_ hex32('f','a','t',0)
#define _ntfs_ hex32('n','t','f','s')
#define _hfs_ hex32('h','f','s',0)
#define _ext_ hex32('e','x','t',0)
//
#define _dbgf32_ hex64('d','b','g','f','3','2',0,0)
#define _dbghex_ hex64('d','b','g','h','e','x',0,0)
#define _echo_ hex32('e','c','h','o')
#define _pump_ hex32('p','u','m','p')
//
#define _mpu9250_ hex64('m','p','u','9','2','5','0',0)
#define _lsm9ds1_ hex64('l','s','m','9','d','s','1',0)
//
#define _reline_ hex64('r','e','l','i','n','e',0,0)
#define _str2fv_ hex64('s','t','r','2','f','v',0,0)
#define _fv2str_ hex64('f','v','2','s','t','r',0,0)
#define _qu2eu_ hex64('q','u','2','e','u',0,0,0)
//
#define _easyag_  hex64('e','a','s','y','a','g', 0 , 0 )
#define _mahony_  hex64('m','a','h','o','n','y', 0 , 0 )
#define _madgwick_ hex64('m','a','d','g','w','i','c','k')
//file
int fileclient_create(struct element* ele, void* url);
int mbrclient_create(struct element* ele, void* url);
int gptclient_create(struct element* ele, void* url);
int fatclient_create(struct element* ele, void* url);
int ntfsclient_create(struct element* ele, void* url);
int hfsclient_create(struct element* ele, void* url);
int extclient_create(struct element* ele, void* url);
//
int dbgf32_create(struct element* ele, void* url);
int dbgf32_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbgf32_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbghex_create(struct element* ele, void* url);
int dbghex_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int dbghex_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int echo_create(struct element* ele, void* url);
int echo_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int echo_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int pump_create(struct element* ele, void* url);
int pump_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int pump_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int mpu9250_create(struct element* ele, void* url);
int mpu9250_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mpu9250_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int lsm9ds1_create(struct element* ele, void* url);
int lsm9ds1_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int lsm9ds1_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int reline_create(struct element* ele, void* url);
int reline_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int reline_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int qu2eu_create(struct element* ele, void* url);
int qu2eu_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int qu2eu_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int str2fv_create(struct element* ele, void* url);
int str2fv_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int str2fv_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fv2str_create(struct element* ele, void* url);
int fv2str_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int fv2str_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//
int easyag_create(struct element* ele, void* url);
int easyag_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int easyag_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mahony_create(struct element* ele, void* url);
int mahony_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int mahony_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int madgwick_create(struct element* ele, void* url);
int madgwick_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int madgwick_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//uart.gcode
int gcodeclient_create(struct element* ele, void* url);
int gcodeclient_write( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gcodeserver_create(struct element* ele, void* url);
int gcodeserver_write( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//uart.gps
int gpsclient_create(struct element* ele, void* url);
int gpsclient_write( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
int gpsserver_create(struct element* ele, void* url);
int gpsserver_write( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len);
//socket
int hackclient_create(struct element* ele, void* url);
int hackclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int hackserver_create(struct element* ele, void* url);
int hackserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//udp.dns
int dnsclient_create(struct element* ele, void* url);
int dnsclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int dnsserver_create(struct element* ele, void* url);
int dnsserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//udp.tftp
int tftpclient_create(struct element* ele, void* url);
int tftpclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int tftpserver_create(struct element* ele, void* url);
int tftpserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//udp.quic
int quicclient_create(struct element* ele, void* url);
int quicclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int quicserver_create(struct element* ele, void* url);
int quicserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int quicmaster_create(struct element* ele, void* url);
int quicmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//tcp.ssh
int sshclient_create(struct element* ele, void* url);
int sshclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int sshserver_create(struct element* ele, void* url);
int sshserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int sshmaster_create(struct element* ele, void* url);
int sshmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//tcp.telnet
int telnetclient_create(struct element* ele, void* url);
int telnetclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int telnetserver_create(struct element* ele, void* url);
int telnetserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int telnetmaster_create(struct element* ele, void* url);
int telnetmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//tcp.http
int httpclient_create(struct element* ele, void* url);
int httpclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int httpserver_create(struct element* ele, void* url);
int httpserver_read( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int httpserver_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int httpmaster_create(struct element* ele, void* url);
int httpmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//tcp.ws
int wsclient_create(struct element* ele, void* url);
int wsclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int wsserver_create(struct element* ele, void* url);
int wsserver_read( struct element* ele, void* sty, struct object* obj, void* pin, void* buf, int len);
int wsserver_write(struct element* ele, void* sty, struct object* obj, void* pin, void* buf, int len);
int wsmaster_create(struct element* ele, void* url);
int wsmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, void* buf, int len);
//tcp.tls
int tlsclient_create(struct element* ele, void* url);
int tlsclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int tlsserver_create(struct element* ele, void* url);
int tlsserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int tlsmaster_create(struct element* ele, void* url);
int tlsmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//tcp.serve
int serveclient_create(struct element* ele, void* url);
int serveclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int serveserver_create(struct element* ele, void* url);
int serveserver_read( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int serveserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int servemaster_create(struct element* ele, void* url);
int servemaster_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
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



/*
int artery_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct element* ele = dc;
	if(0 == ele)return 0;

//say("arterywrite@{\n");
	switch(ele->type)
	{
		case _gps_: gpsclient_write(dc, df, sc, sf, buf, len);break;

		case _HACK_:hackserver_write(dc, df, sc, sf, buf, len);break;
		case _hack_:hackclient_write(dc, df, sc, sf, buf, len);break;

		case _Dns_:dnsserver_rootwrite(dc, df, sc, sf, buf, len);break;
		case _dns_:dnsclient_rootwrite(dc, df, sc, sf, buf, len);break;

		case _QUIC_:quicmaster_write(dc, df, sc, sf, buf, len);break;
		case _Quic_:quicserver_write(dc, df, sc, sf, buf, len);break;
		case _quic_:quicclient_write(dc, df, sc, sf, buf, len);break;

		case _SSH_:sshmaster_write(dc, df, sc, sf, buf, len);break;
		case _Ssh_:sshserver_write(dc, df, sc, sf, buf, len);break;
		case _ssh_:sshclient_write(dc, df, sc, sf, buf, len);break;

		case _TELNET_:telnetmaster_write(dc, df, sc, sf, buf, len);break;
		case _Telnet_:telnetserver_write(dc, df, sc, sf, buf, len);break;
		case _telnet_:telnetclient_write(dc, df, sc, sf, buf, len);break;

		case _HTTP_:httpmaster_write(dc, df, sc, sf, buf, len);break;
		case _Http_:httpserver_rootwrite(dc, df, sc, sf, buf, len);break;
		case _http_:httpclient_write(dc, df, sc, sf, buf, len);break;

		case _WS_:wsmaster_write(dc, df, sc, sf, buf, len);break;
		case _Ws_:wsserver_rootwrite(dc, df, sc, sf, buf, len);break;
		case _ws_:wsclient_write(dc, df, sc, sf, buf, len);break;

		case _TLS_:tlsmaster_write(dc, df, sc, sf, buf, len);break;
		case _Tls_:tlsserver_write(dc, df, sc, sf, buf, len);break;
		case _tls_:tlsclient_write(dc, df, sc, sf, buf, len);break;

		case _SERVE_:servemaster_write(dc, df, sc, sf, buf, len);break;
		case _Serve_:serveserver_rootwrite(dc, df, sc, sf, buf, len);break;
		case _serve_:serveclient_write(dc, df, sc, sf, buf, len);break;

		dafault: printmemory(buf, len);
	}
//say("}@arterywrite\n");

	return 0;
}
int artery_rootread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}
int artery_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct element* ele = dc;
	if(0 == ele)return 0;

	switch(ele->type)
	{
		case _HTTP_:
		case _Http_:httpserver_leafwrite(dc, df, sc, sf, buf, len);break;
		case _WS_:
		case _Ws_:wsserver_leafwrite(dc, df, sc, sf, buf, len);break;

		default: say("%llx\n", ele->type);
	}
	return 0;
}
int artery_leafread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}*/




int arteryread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct element* ele = (void*)(self->chip);
	switch(ele->type){
		case _dbgf32_:dbgf32_read(self, peer, arg, idx, buf, len);break;
		case _dbghex_:dbghex_read(self, peer, arg, idx, buf, len);break;
		case _echo_:echo_read(self, peer, arg, idx, buf, len);break;
		case _pump_:pump_read(self, peer, arg, idx, buf, len);break;

		case _mpu9250_:mpu9250_read(self, peer, arg, idx, buf, len);break;
		case _lsm9ds1_:lsm9ds1_read(self, peer, arg, idx, buf, len);break;

		case _reline_:reline_read(self, peer, arg, idx, buf, len);break;
		case _qu2eu_:qu2eu_read(self, peer, arg, idx, buf, len);break;
		case _str2fv_:str2fv_read(self, peer, arg, idx, buf, len);break;
		case _fv2str_:fv2str_read(self, peer, arg, idx, buf, len);break;

		case _easyag_:easyag_read(self, peer, arg, idx, buf, len);break;
		case _mahony_:mahony_read(self, peer, arg, idx, buf, len);break;
		case _madgwick_:madgwick_read(self, peer, arg, idx, buf, len);break;
	}
	return 0;
}
int arterywrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct element* ele = (void*)(self->chip);
	//say("@arterywrite\n");
	switch(ele->type){
		case _dbgf32_:return dbgf32_write(self, peer, arg, idx, buf, len);break;
		case _dbghex_:return dbghex_write(self, peer, arg, idx, buf, len);break;
		case _echo_:return echo_write(self, peer, arg, idx, buf, len);break;
		case _pump_:return pump_write(self, peer, arg, idx, buf, len);break;

		case _mpu9250_:return mpu9250_write(self, peer, arg, idx, buf, len);break;
		case _lsm9ds1_:return lsm9ds1_write(self, peer, arg, idx, buf, len);break;

		case _qu2eu_:return qu2eu_write(self, peer, arg, idx, buf, len);break;
		case _str2fv_:return str2fv_write(self, peer, arg, idx, buf, len);break;
		case _fv2str_:return fv2str_write(self, peer, arg, idx, buf, len);break;

		case _easyag_:return easyag_write(self, peer, arg, idx, buf, len);break;
		case _mahony_:return mahony_write(self, peer, arg, idx, buf, len);break;
		case _madgwick_:return madgwick_write(self, peer, arg, idx, buf, len);break;

		case _gps_:return gpsclient_write(self, peer, arg, idx, buf, len);break;
	}
	return 0;
}
int arterystop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int arterystart(struct halfrel* self, struct halfrel* peer)
{
	say("@arterystart\n");
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

	//gyro
	if(_mpu9250_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _mpu9250_;
		mpu9250_create(e, url);
		return e;
	}
	if(_lsm9ds1_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _lsm9ds1_;
		lsm9ds1_create(e, url);
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
	if(_reline_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _reline_;
		reline_create(e, url);
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
