#include "libsoft.h"
#define _HACK_ hex32('H','A','C','K')
#define _gcode_ hex64('g','c','o','d','e',0,0,0)
#define _gps_ hex32('g','p','s',0)
#define _mpu_ hex32('m','p','u',0)
#define _ahrs_ hex32('a','h','r','s')
#define _fat_ hex32('f','a','t',0)
#define _ntfs_ hex32('n','t','f','s')
#define _hfs_ hex32('h','f','s',0)
#define _ext_ hex32('e','x','t',0)
int gpsclient_create(struct element* ele, void* url, void* buf, int len);
int gpsclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int mpuclient_create(struct element* ele, void* url, void* buf, int len);
int mpuclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int ahrsclient_create(struct element* ele, void* url, void* buf, int len);
int ahrsclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int gcodeclient_create(struct element* ele, void* url, void* buf, int len);
int gcodeclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int chatserver_create(struct element* ele, void* url, void* buf, int len);
int chatserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int hackserver_create(struct element* ele, void* url, void* buf, int len);
int hackserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int httpclient_create(struct element* ele, void* url, void* buf, int len);
int httpclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int httpserver_create(struct element* ele, void* url, void* buf, int len);
int httpserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int httpmaster_create(struct element* ele, void* url, void* buf, int len);
int httpmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int sshclient_create(struct element* ele, void* url, void* buf, int len);
int sshclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int sshserver_create(struct element* ele, void* url, void* buf, int len);
int sshserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int sshmaster_create(struct element* ele, void* url, void* buf, int len);
int sshmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int tlsclient_create(struct element* ele, void* url, void* buf, int len);
int tlsclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int tlsserver_create(struct element* ele, void* url, void* buf, int len);
int tlsserver_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int tlsmaster_create(struct element* ele, void* url, void* buf, int len);
int tlsmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int wsclient_create(struct element* ele, void* url, void* buf, int len);
int wsclient_write( struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int wsserver_create(struct element* ele, void* url, void* buf, int len);
int wsserver_write( struct element* ele, void* sty, struct object* obj, void* pin, void* buf, int len);
int wsmaster_create(struct element* ele, void* url, void* buf, int len);
int wsmaster_write( struct element* ele, void* sty, struct object* obj, void* pin, void* buf, int len);
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




int arterywrite_ev(struct event* ev)
{
	return 0;
}
int arteryread_all()
{
	return 0;
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




int arteryread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == sc)return arteryread_all();
	return 0;
}
int arterywrite(void* dc,void* df,void* sc,void* sf,void* buf, int len)
{
	u64 type;
	struct element* ele;
	if(0 == dc)return arterywrite_ev(buf);
say("arterywrite@{\n");

	ele = dc;
	type = ele->type;
	if(_gps_ == type)gpsclient_write(dc, df, sc, sf, buf, len);
	else if(_mpu_ == type)mpuclient_write(dc, df, sc, sf, buf, len);
	else if(_ahrs_ == type)ahrsclient_write(dc, df, sc, sf, buf, len);
	else if(_CHAT_ == type)chatserver_write(dc, df, sc, sf, buf, len);
	else if(_HACK_ == type)hackserver_write(dc, df, sc, sf, buf, len);
	else if(_HTTP_ == type)httpmaster_write(dc, df, sc, sf, buf, len);
	else if(_Http_ == type)httpserver_write(dc, df, sc, sf, buf, len);
	else if(_http_ == type)httpclient_write(dc, df, sc, sf, buf, len);
	else if(_SSH_  == type)sshmaster_write(dc, df, sc, sf, buf, len);
	else if(_Ssh_  == type)sshserver_write(dc, df, sc, sf, buf, len);
	else if(_ssh_  == type)sshclient_write(dc, df, sc, sf, buf, len);
	else if(_TLS_  == type)tlsmaster_write(dc, df, sc, sf, buf, len);
	else if(_Tls_  == type)tlsserver_write(dc, df, sc, sf, buf, len);
	else if(_tls_  == type)tlsclient_write(dc, df, sc, sf, buf, len);
	else if(_WS_   == type)wsmaster_write(dc, df, sc, sf, buf, len);
	else if(_Ws_   == type)wsserver_write(dc, df, sc, sf, buf, len);
	else if(_ws_   == type)wsclient_write(dc, df, sc, sf, buf, len);
	else printmemory(buf, len);

say("}@arterywrite\n");
	return 0;
}
int arterystop()
{
	return 0;
}
int arterystart()
{
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
	if(_CHAT_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _CHAT_;
		if(url)chatserver_create(e, url, datahome, 0x100000);
		return e;
	}
	if(_HACK_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _HACK_;
		if(url)hackserver_create(e, url, datahome, 0x100000);
		return e;
	}
	if(_gcode_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _gcode_;
		gcodeclient_create(e, url, datahome, 0x100000);
		return e;
	}

	//ahrs
	if(_gps_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _gps_;
		gpsclient_create(e, url, datahome, 0x100000);
		return e;
	}
	if(_mpu_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _mpu_;
		mpuclient_create(e, url, datahome, 0x100000);
		return e;
	}
	if(_ahrs_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ahrs_;
		ahrsclient_create(e, url, datahome, 0x100000);
		return e;
	}

	//filesystem
	if(_fat_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _fat_;
		fatclient_create(e, url, datahome, 0x100000);
		return e;
	}
	if(_ntfs_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ntfs_;
		ntfsclient_create(e, url, datahome, 0x100000);
		return e;
	}
	if(_hfs_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _hfs_;
		hfsclient_create(e, url, datahome, 0x100000);
		return e;
	}
	if(_ext_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ext_;
		extclient_create(e, url, datahome, 0x100000);
		return e;
	}

	//http: master,server,client
	if(_HTTP_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _HTTP_;
		if(url)httpmaster_create(e, url, datahome, 0x100000);
		return e;
	}
	if(_Http_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Http_;
		if(url)httpserver_create(e, url, datahome, 0x100000);
		return e;
	}
	if(_http_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _http_;
		if(url)httpclient_create(e, url, datahome, 0x100000);

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
			relationcreate(e, 0, _art_, f, 0, _art_);
		}

		return e;
	}

	//ssh: master,server,client
	if(_SSH_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _SSH_;
		if(url)sshmaster_create(e, url, datahome, 0x100000);

		return e;
	}
	if(_Ssh_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Ssh_;
		if(url)sshserver_create(e, url, datahome, 0x100000);

		return e;
	}
	if(_ssh_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ssh_;
		if(url)sshclient_create(e, url, datahome, 0x100000);

		return e;
	}

	//tls: master,server,client
	if(_TLS_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _TLS_;
		if(url)tlsmaster_create(e, url, datahome, 0x100000);

		return e;
	}
	if(_Tls_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Tls_;
		if(url)tlsserver_create(e, url, datahome, 0x100000);

		return e;
	}
	if(_tls_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _tls_;
		if(url)tlsclient_create(e, url, datahome, 0x100000);

		return e;
	}

	//ws: master,server,client
	if(_WS_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _WS_;
		if(url)wsmaster_create(e, url, datahome, 0x100000);

		return e;
	}
	if(_Ws_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _Ws_;
		if(url)wsserver_create(e, url, datahome, 0x100000);

		return e;
	}
	if(_ws_ == type)
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ws_;
		if(url)wsclient_create(e, url, datahome, 0x100000);

		return e;
	}
	return 0;
}
int arterychoose(u8* buf, int len)
{
	int j;
	u8 data[0x1000];
	if(0 == len)
	{
		arterycreate(0, buf);
	}
	else
	{
		for(j=0;j<len;j++)
		{
			if(0 == buf[j])break;
			data[j] = buf[j];
		}
		data[j] = 0;

		arterycreate(0, data);
	}
	return 0;
}
int arterylist(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<0x1000;j++)
	{
		if(0 == ele[j].type)continue;

		k++;
		say("[%03x]: %.8s\n", j, &ele[j].type);
	}

	if(0 == k)say("empth artery\n");
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

	arterycreate(0, (u8*)"HACK://127.0.0.1:2222");
	arterycreate(0, (u8*)"CHAT://127.0.0.1:4444");
	arterycreate(0, (u8*)"SSH://127.0.0.1:2222");
	arterycreate(0, (u8*)"HTTP://127.0.0.1:4444");
	//say("[8,c):inited artery\n");
}
