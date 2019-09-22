#include "libsoft.h"
int findzero(void*);
int findhead(void*);
int findtail(void*);
int openreadclose(void* name, u64 off, void* mem, u64 len);
int openwriteclose(void* name, u64 off, void* mem, u64 len);
int wsserver_read(void*, void*, void*, void*, void* buf, int len);
int wsserver_write(void*, void*, void*, void*, void* buf, int len);
int tlsserver_write(void*, void*, void*, void*, void* buf, int len);




struct httpparsed{
	u8* GET;
	u8* POST;
	u8* Host;
	u8* Connection;
	u8* Upgrade;
	u8* Content_Length;
	u8* Content_Type;
	u8* Content;
	u8* End;
};
void httpparser(u8* buf, int len, struct httpparsed* p)
{
	int j,k;
	p->GET = 0;
	p->POST = 0;
	p->Host = 0;
	p->Connection = 0;
	p->Upgrade = 0;
	p->Content_Length = 0;
	p->Content_Type = 0;
	p->Content = 0;
	p->End = buf+len;

	k = 0;
	for(j=0;j<len;j++){
		if((0xd == buf[j])&&(0xa == buf[j+1])){
			if(0xd == buf[k]){p->Content = buf+k+2;break;}
			else if(ncmp(buf+k, "GET ", 4) == 0)p->GET = buf+k+4;
			else if(ncmp(buf+k, "POST ", 5) == 0)p->POST = buf+k+5;
			else if(ncmp(buf+k, "Host: ", 6) == 0)p->Host = buf+k+6;
			else if(ncmp(buf+k, "Upgrade: ", 9) == 0)p->Upgrade = buf+k+9;
			else if(ncmp(buf+k, "Connection: ", 12) == 0)p->Connection = buf+k+12;
			else if(ncmp(buf+k, "Content-Length: ", 16) == 0)p->Content_Length = buf+k+16;
			
			k = j+2;
		}
	}
	//say("Content@%llx\nEnd@%llx\n", p->Content, p->End);
}



/*
void httpserver_sendback(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int ret;
	//printmemory(buf, len);

	//response
	ret = mysnprintf(buf+len, 0x1000,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/plain\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		len
	);

	//send head, send ctx
	system_leafwrite(obj, pin, ele, sty, buf+len, ret);
	system_leafwrite(obj, pin, ele, sty, buf, len);
}
int httpserver_nullpost(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	struct httpparsed* p)
{
	return 0;
}
int httpserver_nullget(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	struct httpparsed* p)
{
	int ctxlen, tmplen, ret;
	void* ctxbuf;
	void* tmpbuf;
	//printmemory(buf, len);
	if(0 == ncmp(p->GET, "/favicon.ico", 12))return 1;

	//write ctx
	ctxbuf = buf+len+1;
	if(0 == ncmp(p->GET, "/ ", 2)){
		ctxlen = mysnprintf(ctxbuf, 0x1000, "<center>root</center><hr>");
	}
	else if(0 == ncmp(p->GET, "/arena ", 7)){
		ctxlen = mysnprintf(ctxbuf, 0x1000, "<center>arena</center><hr>");
	}
	else if(0 == ncmp(p->GET, "/actor ", 7)){
		ctxlen = mysnprintf(ctxbuf, 0x1000, "<center>actor</center><hr>");
	}
	else{
		ret = 0;
		while(p->GET[ret] > 0x20)ret++;

		ctxbuf = p->GET;
		ctxlen = ret;
	}

	//write head
	tmpbuf = ctxbuf+ctxlen+1;
	tmplen = mysnprintf(tmpbuf, 0x1000,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		ctxlen
	);

	//send head, send ctx
	system_leafwrite(obj, pin, ele, sty, tmpbuf, tmplen);
	system_leafwrite(obj, pin, ele, sty, ctxbuf, ctxlen);
	return 1;
}
int httpserver_orelpost(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	struct httpparsed* p)
{
	int t;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;
	say("@httpserver_orelpost\n");

	rel = ele->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			t = p->End - p->Content;
			arenawrite(self, peer, p->Content, t);
			break;
		}

		rel = samesrcnextdst(rel);
	}

	return 0;
}
int httpserver_orelget(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	struct httpparsed* p)
{
	int ret;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

	int ctxlen;
	void* ctxbuf;
	int tmplen;
	void* tmpbuf;

	//printmemory(buf, len);
	if(0 == ncmp(p->GET, "/favicon.ico", 12))return 0;

	//write ctx
	ctxbuf = buf+len+1;
	ctxlen = 0;

	rel = ele->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			ctxlen = arenaread(self, peer, ctxbuf, 0);
			break;
		}

		rel = samesrcnextdst(rel);
	}
	//say("ctxlen=%d\n",ctxlen);
	//printmemory(ctxbuf, ctxlen);

	//write head
	tmpbuf = ctxbuf+ctxlen+1;
	tmplen = mysnprintf(tmpbuf, 0x1000,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		ctxlen
	);

	//send head, send ctx
	system_leafwrite(obj, pin, ele, sty, tmpbuf, tmplen);
	system_leafwrite(obj, pin, ele, sty, ctxbuf, ctxlen);
	return 1;
}*/




int httpclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int j,k;
	struct element* ele;
	say("@httpclient_write: %x\n", len);
	//printmemory(buf, 256);

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	if(ele->stage1 > 0){
		relationwrite(ele, _dst_, 0, 0, buf, len);
	}
	else{
		k = 0;
		for(j=0;j<len/2;j++){
			//say("%d,%x\n",j,buf[j]);
			if(	(buf[j+0] == 0xd) && (buf[j+1] == 0xa) ) {
				if(buf[k] != 0xd){
					say("%.*s\n", j-k, buf+k);
					k = j+2;
				}
				else{
					relationwrite(ele, _dst_, 0, 0, buf+j+2, len-j-2);
					break;
				}
				j++;
			}
		}
		ele->stage1 = 1;
	}

	return 0;
}
int httpclient_stop(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpclient_start(struct halfrel* self, struct halfrel* peer)
{
	struct element* ele = (void*)(self->chip);
	if(_src_ == self->flag){
		relationwrite(ele, _src_, 0, 0, ele->buf, ele->len);
		ele->stage1 = 0;
	}
	return 0;
}
int httpclient_delete(struct element* ele)
{
	return 0;
}
int httpclient_create(struct element* ele, u8* url)
{
	int j;
	u8* buf;
	int hlen;
	u8* host;
	int clen;
	u8* ctxt;
	if(0 == url)return 0;
	if(0 == url[0])return 0;
	//printmemory(url, 32);

	//get host
	buf = url;
	for(j=0;j<32;j++){
		if('/' == buf[j])break;
	}
	if(buf[j] != '/')return 0;
	host = buf;
	hlen = j;

	//get ctx
	buf = host + j;
	for(j=0;j<256;j++){
		if(buf[j] <= 0x20)break;
	}
	if(buf[j] > 0x20)return 0;
	ctxt = buf;
	clen = j;

	buf = ele->buf = memorycreate(0x1000, 0);
	if(0 == buf)return 0;

	ele->len = mysnprintf(buf, 0x1000,
		"GET %.*s HTTP/1.1\r\n"
		"Host: %.*s\r\n"
		"\r\n",
		clen, ctxt,
		hlen, host
	);
	say("%.*s\n", ele->len, buf);
	return 0;
}




int httpserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{/*
	u8 tmp[0x1000];
	if(0 == buf)
	{
		len = mysnprintf(tmp, 0x1000,
			"HTTP/1.1 200 OK\r\n"
			"Content-type: text/html\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			len
		);
		buf = tmp;
	}

	system_leafwrite(ele->obj, 0, ele, sty, buf, len);*/
	return 0;
}
int httpserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpserver_delete(struct element* ele)
{
	return 0;
}
int httpserver_create(struct element* ele, u8* url)
{
	return 0;
}




int httpmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int j,k,ret;
	u8 tmp[0x1000];
	struct httpparsed p;
	struct element* e = (void*)(self->chip);
	if(0 == e)return 0;
/*
	//https
	if(0x16 == buf[0])
	{
		e = arterycreate(_Tls_, 0);
		if(e)
		{
			relationcreate(e, 0, _art_, 0, obj, 0, _fd_, 0);
			tlsserver_write(e, sty, obj, pin, buf, len);
		}
		return 0;
	}
*/
	//parse
	httpparser(buf, len, &p);
/*
	//websocket
	if((0 != p.Connection)&&(0 != p.Upgrade))
	{
		e = arterycreate(_Ws_, 0);
		if(e)
		{
			relationcreate(e, 0, _art_, _src_, obj, 0, _fd_, 0);
			wsserver_write(e, sty, obj, pin, buf, len);
		}
		return 0;
	}
*/
	if(p.GET){
		ret = mysnprintf(tmp, 0x1000,
			"HTTP/1.1 200 OK\r\n"
			"Content-type: text/plain\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			len
		);
		relationwrite(e, _src_, 0, 0, tmp, ret);
		relationwrite(e, _src_, 0, 0, buf, len);
	}

	//close or not
	if(0 != p.Connection)
	{
		if(0 == ncmp(p.Connection, "keep-alive", 10))return 0;
	}
	//systemdelete(obj);
	return 0;
}
int httpmaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpmaster_delete(struct element* ele)
{
	return 0;
}
int httpmaster_create(struct element* ele, u8* url)
{
	return 0;
}
