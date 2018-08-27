#include "libsoft.h"
int httpclient_create(struct element* ele, void* url, void* buf, int len);
int httpclient_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int httpserver_create(struct element* ele, void* url, void* buf, int len);
int httpserver_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int httpmaster_create(struct element* ele, void* url, void* buf, int len);
int httpmaster_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int sshclient_create( struct element* ele, void* url, void* buf, int len);
int sshclient_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int sshserver_create( struct element* ele, void* url, void* buf, int len);
int sshserver_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int sshmaster_create( struct element* ele, void* url, void* buf, int len);
int sshmaster_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int tlsclient_create( struct element* ele, void* url, void* buf, int len);
int tlsclient_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int tlsserver_create( struct element* ele, void* url, void* buf, int len);
int tlsserver_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int tlsmaster_create( struct element* ele, void* url, void* buf, int len);
int tlsmaster_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
//
int wsclient_create(  struct element* ele, void* url, void* buf, int len);
int wsclient_write(struct element* ele, void* sty, struct object* obj, void* pin, u8* buf, int len);
int wsserver_create(  struct element* ele, void* url, void* buf, int len);
int wsserver_write(struct element* ele, void* sty, struct object* obj, void* pin, void* buf, int len);
int wsmaster_create(  struct element* ele, void* url, void* buf, int len);
int wsmaster_write(struct element* ele, void* sty, struct object* obj, void* pin, void* buf, int len);
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
	if('?' == type)say("?????????????@arterywrite\n");
	else if(_HTTP_ == type)httpmaster_write(dc, df, sc, sf, buf, len);
	else if(_Http_ == type)httpserver_write(dc, df, sc, sf, buf, len);
	else if(_http_ == type)httpclient_write(dc, df, sc, sf, buf, len);
	else if(_SSH_  == type)sshmaster_write(dc, df, sc, sf, buf, len);
	else if(_Ssh_  == type)sshserver_write(dc, df, sc, sf, buf, len);
	else if(_ssh_  == type)sshclient_write(dc, df, sc, sf, buf, len);
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
void* arterycreate(u64 type, u8* url)
{
	int j,fd,ret,port;
	struct element* e;
	if(0 == type)
	{
		ret = parsetypefromurl(url, (void*)&type);
		if(0 == ret)return 0;	//unknown

		url += ret;
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

	//ssh: master,server,client
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
/*
	u8 host[0x100];	//127.0.0.1
	u8* url;	//dir/file.html
	u8* t;		//http

	if(0 == type)
	{
		for(j=0;j<0x1000;j++)
		{
			if(0 == ncmp(name+j, "://", 3))
			{
				t = (u8*)&type;
				for(ret=0;ret<j;ret++)
				{
					if(ret >= 8)break;
					t[ret] = name[ret];
				}
				name += j+3;
				break;
			}
		}
	}
	if(0 == type)return 0;
	if(_uart_ == type)return systemcreate(_uart_, name);
	else if(_FILE_ == type)return systemcreate(_FILE_, name);
	else if(_file_ == type)return systemcreate(_file_, name);
	else if(_RAW_ == type)return systemcreate(_RAW_, name);
	else if(_raw_ == type)return systemcreate(_raw_, name);
	else if(_UDP_ == type)return systemcreate(_UDP_, name);
	else if(_udp_ == type)return systemcreate(_udp_, name);
	else if(_TCP_ == type)return systemcreate(_TCP_, name);
	else if(_tcp_ == type)return systemcreate(_tcp_, name);

	//decode ipaddr
	port = 80;
	ret = parseurl(name, 0x100, host, &port);

	mysnprintf(host, 80, "%.*s", ret, name);
	say("artery: host=%s\n", host);

	url = name + ret;
	say("artery: url=%s\n", url);

	if(_http_ == type)	//http client
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _http_;
		httpclient_create(e, datahome, host, url);
		return e;
	}
	else if(_ws_ == type)	//ws client
	{
		e = allocelement();
		if(0 == e)return 0;

		e->type = _ws_;
		wsclient_create(e, datahome, host, url);
		return e;
	}

	//raw family
	if(_ICMP_ == type)
	{
	}
	else if(_icmp_ == type)
	{
	}

	//udp family
	else if(_DNS_ == type)	//DNS server
	{
		fd = startsocket(host, port, 'U');
		if(0 >= fd)return 0;

		obj[fd].name = _DNS_;
	}
	else if(_dns_ == type)	//DNS client
	{
		fd = startsocket(host, port, 'u');
		if(0 >= fd)return 0;

		obj[fd].name = _dns_;
		ret = dns_write_query(datahome, 666, url+1, 666);
		writesocket(fd, 0, datahome, ret);
	}
	else if(_HOLE_ == type)	//p2p server
	{
		fd = startsocket(host, port, 'U');
		if(0 >= fd)return 0;

		obj[fd].name = _HOLE_;
	}
	else if(_hole_ == type)	//p2p client
	{
		fd = startsocket(host, port, 'u');
		if(0 >= fd)return 0;

		obj[fd].name = _hole_;
		writesocket(fd, 0, url+1, 16);
	}
	else if(_TFTP_ == type)	//tftp server
	{
		fd = startsocket(host, port, 'U');
		if(0 >= fd)return 0;

		obj[fd].name = _TFTP_;
	}
	else if(_tftp_ == type)	//tftp client
	{
		fd = startsocket(host, port, 'u');
		if(0 >= fd)return 0;

		obj[fd].name = _tftp_;
		ret = tftp_write(datahome, 0x100000);
		ret = writesocket(fd, 0, datahome, ret);
	}

	//tcp family
	else if(_SSH_ == type)	//ssh server
	{
		ssh_start();
		fd = startsocket(host, port, 'T');
		if(0 >= fd)return 0;

		obj[fd].name = _SSH_;
	}
	else if(_ssh_ == type)	//ssh client
	{
		fd = startsocket(host, port, 't');
		if(0 >= fd)return 0;

		obj[fd].name = _ssh_;
		ret = secureshell_write_handshake(datahome, 0x100000);
		ret = writesocket(fd, 0, datahome, ret);
	}
	else if(_TLS_ == type)	//tls server
	{
		tls_start();
		fd = startsocket(host, port, 'T');
		if(0 >= fd)return 0;

		obj[fd].name = _TLS_;
	}
	else if(_tls_ == type)	//tls client
	{
		fd = startsocket(host, port, 't');
		if(0 >= fd)return 0;

		obj[fd].name = _tls_;
		ret = tls_write_client_hello(datahome, 0x100000);
		ret = writesocket(fd, 0, datahome, ret);
	}
	else if(_sql_ == type)	//sql client
	{
		fd = startsocket(host, port, 't');
		if(0 >= fd)return 0;

		obj[fd].name = _sql_;
	}

	return 0;
}*/
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

	arterycreate(0, (u8*)"SSH://127.0.0.1:2222");
	arterycreate(0, (u8*)"HTTP://127.0.0.1:4444");
	//say("[8,c):inited artery\n");
}
