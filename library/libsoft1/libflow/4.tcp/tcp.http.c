#include "libsoft.h"
#define _ok_ hex16('o','k')
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




int httpclient_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpclient_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int j,k;
	say("@httpclient_write: %llx, %.4s, %x\n", art, &foot, len);
	if(len>0)printmemory(buf, len<16?len:16);

	switch(foot){
	case _dst_:{
		break;
	}
	case _src_:{
		if(art->stage1 >= 2){
            //src to dst
			relationwrite(art,_dst_, stack,sp, 0,0, buf,len);
			return 0;
		}

		if(art->stage1 == 1){
            //recv: http reply0
            //send: http data
			k = 0;
			for(j=0;j<len-2;j++){
				//say("%d,%x\n",j,buf[j]);
				if(	(buf[j+0] == 0xd) && (buf[j+1] == 0xa) ) {
					if(buf[k] != 0xd){
						say("(((%.*s)))\n", j-k, buf+k);
						k = j+2;
					}
					else{
						//say("dbg:%d\n",len-j-2);
						//printmemory(buf+j+2, len-j-2);
						relationwrite(art, _dst_, stack,sp, 0,0, buf+j+2,len-j-2);
						break;
					}
					j++;
				}
			}
			art->stage1 = 2;
		}

	}//src
	}//switch

	return 0;
}
int httpclient_discon(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@httpclient_discon: %.4s\n", &self->flag);
	return 0;
}
int httpclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct artery* art;
	struct sysobj* obj;
	say("@httpclient_linkup: %.4s\n", &self->flag);

	art = self->pchip;
	if(_src_ == self->flag){
		relationwrite(art,_src_, 0,0, 0,0, art->buf0,art->len);
		art->stage1 = 1;
	}
	return 0;
}
int httpclient_delete(struct artery* ele)
{
	return 0;
}
int httpclient_create(struct artery* ele, u8* url)
{
	int j;
	u8* buf;
	int hlen;
	void* host;
	int clen;
	void* ctxt;
	ele->stage1 = 0;
	if(0 == url)return 0;
	if(0 == url[0])return 0;
	//printmemory(url, 32);

	//get host
	for(j=0;j<32;j++){
		if(url[j] <= 0x20)break;
		if('/' == url[j])break;
	}
	host = url;
	hlen = j;

	//get ctx
	if('/' != url[j]){
		ctxt = "/";
		clen = 1;
	}
	else{
		buf = url + j;
		for(j=0;j<256;j++){
			if(buf[j] <= 0x20)break;
		}
		if(buf[j] > 0x20)return 0;
		ctxt = buf;
		clen = j;
	}

	//http req0
	buf = ele->buf0 = ele->data;
	if(0 == buf)return 0;

	ele->len = mysnprintf(buf, 0x80,
		"GET %.*s HTTP/1.1\r\n"
		"Host: %.*s\r\n"
		"\r\n",
		clen, ctxt,
		hlen, host
	);
	say("%.*s\n", ele->len, buf);
	return 0;
}




int httpserver_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpserver_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
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
int httpserver_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@httpserver_discon: %.4s\n", &self->flag);
	return 0;
}
int httpserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@httpserver_linkup: %.4s\n", &self->flag);
	return 0;
}
int httpserver_delete(struct artery* ele)
{
	return 0;
}
int httpserver_create(struct artery* ele, u8* url)
{
	return 0;
}




int httpmaster_write_bydst(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int ret;
	u8 tmp[0x400];

	//if stillgoing, store

	//if finished, send
	if(0 == arg)arg = "text/plain";
	ret = mysnprintf(tmp, 0x1000,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: %s\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		arg, len
	);
	relationwrite(art,_src_, stack,sp, 0,0, tmp,ret);
	relationwrite(art,_src_, stack,sp, 0,0, buf,len);
	return 0;
}
int httpmaster_write_bysrc(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	struct httpparsed p;
	httpparser(buf, len, &p);

	//websocket
	if((0 != p.Connection)&&(0 != p.Upgrade))
	{
		if(0==stack|sp<2)return 0;
		struct sysobj* TCP = stack[sp-2].pchip;
		if(0 == TCP)return 0;
		struct sysobj* Tcp = TCP->tempobj;
		if(0 == Tcp)return 0;
		struct artery* Ws = arterycreate(_Ws_, 0, 0, 0);
		if(0 == Ws)return 0;
		struct relation* rel = relationcreate(Ws, 0, _art_, _src_, Tcp, 0, _sys_, _dst_);
		if(0 == rel)return 0;
		stack[sp-2].pchip = Tcp;
		stack[sp-1].pchip = Ws;
		arterywrite(Ws,_src_, stack,sp, 0,0, buf,len);
		return 0;
	}

	int j,ret;
	u8 tmp[0x400];
	if(art->orel0){
		struct str* s[16];
		for(j=0;j<4;j++)s[j] = 0;
		relationread(art,_dst_, stack,sp, p.GET,0, s,4);

		len = 0;
		for(j=0;j<4;j++){
			if(s[j])len += s[j]->len;
		}
		ret = mysnprintf(tmp, 0x1000,
			"HTTP/1.1 200 OK\r\n"
			"Content-type: %s\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			"text/html", len
		);
		relationwrite(art,_src_, stack,sp, 0,0, tmp,ret);

		for(j=0;j<4;j++){
			if(s[j])relationwrite(art,_src_, stack,sp, 0,0, s[j]->buf,s[j]->len);
		}
		return 0;
	}
	else{
		if(p.GET){
			ret = mysnprintf(tmp, 0x1000,
				"HTTP/1.1 200 OK\r\n"
				"Content-type: text/plain\r\n"
				"Content-Length: %d\r\n"
				"\r\n",
				len
			);
			relationwrite(art, _src_, stack,sp, 0,0, tmp,ret);
			relationwrite(art, _src_, stack,sp, 0,0, buf,len);
		}
		else{
			say("unknown: POST\n");
		}
	}

	//close or not
	if(0 != p.Connection)
	{
		if(0 == ncmp(p.Connection, "keep-alive", 10))return 0;
	}
	//systemdelete(obj);
	return 0;
}
int httpmaster_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	say("foot=%.4s\n", &foot);
	if(_dst_ == foot)return httpmaster_write_bydst(art,foot, stack,sp, arg,idx, buf,len);
	else return httpmaster_write_bysrc(art,foot, stack,sp, arg,idx, buf,len);
	return 0;
}
int httpmaster_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int httpmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int httpmaster_delete(struct artery* ele)
{
	return 0;
}
int httpmaster_create(struct artery* ele, u8* url)
{
	return 0;
}
