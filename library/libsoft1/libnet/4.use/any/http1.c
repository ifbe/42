#include "libsoft.h"
#define BUF buflen.buf
#define LEN buflen.len
int findzero(void*);
int findhead(void*);
int findtail(void*);
int openreadclose(void* name, u64 off, void* mem, u64 len);
int openwriteclose(void* name, u64 off, void* mem, u64 len);




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




int httpclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int j,k;
	say("@httpclient_write:%p,%p\n", art, foot);
	if(len>0)printmemory(buf, len<16?len:16);

	switch(stack[sp-1].foottype){
	case _dst_:{
		break;
	}
	case _src_:{
		if(art->vfmt >= 2){
            //src to dst
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);
			return 0;
		}

		if(art->vfmt == 1){
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
						give_data_into_peer(art, _dst_, stack,sp, 0,0, buf+j+2,len-j-2);
						break;
					}
					j++;
				}
			}
			art->vfmt = 2;
		}

	}//src
	}//switch

	return 0;
}
int httpclient_detach(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@httpclient_detach: %.4s\n", &self->foottype);
	return 0;
}
int httpclient_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* art;
	_obj* obj;
	say("@httpclient_attach: %.4s\n", &self->foottype);

	art = self->pchip;
	if(_src_ == self->foottype){
		give_data_into_peer(art,_src_, 0,0, 0,0, art->BUF,art->LEN);
		art->vfmt = 1;
	}
	return 0;
}
int httpclient_delete(_obj* ele)
{
	return 0;
}
int httpclient_create(_obj* ele, u8* url)
{
	int j;
	ele->vfmt = 0;
	if(0 == url)return 0;
	if(0 == url[0])return 0;
	//printmemory(url, 32);

	//get host
	int hlen;
	void* host;
	for(j=0;j<32;j++){
		if(url[j] <= 0x20)break;
		if('/' == url[j])break;
	}
	host = url;
	hlen = j;

	//get ctx
	int clen;
	u8* ctxt;
	if('/' != url[j]){
		ctxt = (u8*)"/";
		clen = 1;
	}
	else{
		ctxt = url + j;
		for(j=0;j<256;j++){
			if(ctxt[j] <= 0x20)break;
		}
		if(ctxt[j] > 0x20)return 0;
		clen = j;
	}

	//http req0
	ele->LEN = mysnprintf(ele->BUF, 0xf0,
		"GET %.*s HTTP/1.1\r\n"
		"Host: %.*s\r\n"
		"\r\n",
		clen, ctxt,
		hlen, host
	);
	say("%.*s\n", ele->LEN, ele->BUF);
	return 0;
}




int httpserver_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
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
int httpserver_detach(struct halfrel* self, struct halfrel* peer)
{
	say("@httpserver_detach: %.4s\n", &self->foottype);
	return 0;
}
int httpserver_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@httpserver_attach: %.4s\n", &self->foottype);
	return 0;
}
int httpserver_delete(_obj* ele)
{
	return 0;
}
int httpserver_create(_obj* ele, u8* url)
{
	return 0;
}




int httpmaster_write_bydst(_obj* art,void* foot, _syn* stack,int sp, struct httpparsed* arg, int idx, u8* buf, int len)
{
	int ret;
	u8 tmp[0x400];

if(0 == idx){
	char* type = "text/plain";
	if(arg->Content_Type)type = (char*)arg->Content_Type;

	ret = mysnprintf(tmp, 0x1000,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: %s\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		type, arg->Content_Length
	);
	give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
}
	give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);
	return 0;
}
int httpmaster_write_bysrc(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	struct httpparsed p;
	httpparser(buf, len, &p);

	//websocket
	if((0 != p.Connection)&&(0 != p.Upgrade))
	{
		if(0==stack|sp<2)return 0;
		_obj* TCP = stack[sp-2].pchip;
		if(0 == TCP)return 0;
		_obj* Tcp = TCP->sockinfo.child;
		if(0 == Tcp)return 0;
		_obj* Ws = artery_create(_Ws_, 0, 0, 0);
		if(0 == Ws)return 0;
		struct relation* rel = relationcreate(Ws, 0, _art_, _src_, Tcp, 0, _sys_, _dst_);
		if(0 == rel)return 0;
		stack[sp-2].pchip = Tcp;
		stack[sp-1].pchip = Ws;
		stack[sp-1].foottype = _src_;
		artery_giveby(Ws,0, stack,sp, 0,0, buf,len);
		return 0;
	}

	int j,ret;
	u8 tmp[0x400];
	if(art->orel0){
/*		struct str* s[16];
		for(j=0;j<4;j++)s[j] = 0;
		take_data_from_peer(art,_dst_, stack,sp, p.GET,0, s,4);

		len = 0;
		for(j=0;j<4;j++){
			if(s[j])len += s[j]->len;
		}
		if(0 == len)return 0;

		ret = mysnprintf(tmp, 0x1000,
			"HTTP/1.1 200 OK\r\n"
			"Content-type: %s\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			"text/html", len
		);
		give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);

		for(j=0;j<4;j++){
			if(s[j])give_data_into_peer(art,_src_, stack,sp, 0,0, s[j]->buf,s[j]->len);
		}
*/
		give_data_into_peer(art,_dst_, stack,sp, &p,0, p.Content,p.End-p.Content);
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
			give_data_into_peer(art, _src_, stack,sp, 0,0, tmp,ret);
			give_data_into_peer(art, _src_, stack,sp, 0,0, buf,len);
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
	//system_delete(obj);
	return 0;
}
int httpmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	say("@httpmaster_write:%p,%p\n", art, foot);
	switch(stack[sp-1].foottype){
	case _dst_:return httpmaster_write_bydst(art,foot, stack,sp, arg,idx, buf,len);
	default:return httpmaster_write_bysrc(art,foot, stack,sp, arg,idx, buf,len);
	}
	return 0;
}
int httpmaster_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int httpmaster_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int httpmaster_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int httpmaster_delete(_obj* ele)
{
	return 0;
}
int httpmaster_create(_obj* ele, u8* url)
{
	return 0;
}
