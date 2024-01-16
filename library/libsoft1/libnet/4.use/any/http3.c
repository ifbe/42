#include "libsoft.h"
#define BUF buflen.buf
#define LEN buflen.len
int findzero(void*);
int findhead(void*);
int findtail(void*);
int openreadclose(void* name, u64 off, void* mem, u64 len);
int openwriteclose(void* name, u64 off, void* mem, u64 len);




struct http3parsed{
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
void http3parser(u8* buf, int len, struct http3parsed* p)
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
	//logtoall("Content@%llx\nEnd@%llx\n", p->Content, p->End);
}




int http3client_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int http3client_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	logtoall("@http3client_write:%p,%p\n", art, foot);
	return 0;
}
int http3client_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@http3client_detach: %.4s\n", &self->foottype);
	return 0;
}
int http3client_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@http3client_attach: %.4s\n", &self->foottype);
	return 0;
}
int http3client_delete(_obj* ele)
{
	return 0;
}
int http3client_create(_obj* ele, u8* url)
{
	return 0;
}




int http3server_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int http3server_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int http3server_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@http3server_detach: %.4s\n", &self->foottype);
	return 0;
}
int http3server_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@http3server_attach: %.4s\n", &self->foottype);
	return 0;
}
int http3server_delete(_obj* ele)
{
	return 0;
}
int http3server_create(_obj* ele, u8* url)
{
	return 0;
}




int http3master_write_byudp(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	logtoall("@%s\n", __FUNCTION__);
	return 0;
}
int http3master_write_bysrc(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	struct http3parsed p;
	http3parser(buf, len, &p);

	u8 tmp[0x400];
	int ret = mysnprintf(tmp, 0x1000,
"HTTP/1.1 200 OK\r\n"
"Content-type: text/plain\r\n"
"Content-Length: %d\r\n"
"Alt-Svc: h3=\":443\"\r\n"
"\r\n",
	len
	);
	give_data_into_peer(art, _src_, stack,sp, 0,0, tmp,ret);
	give_data_into_peer(art, _src_, stack,sp, 0,0, buf,len);
	return 0;
}
int http3master_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	//logtoall("@http3master_write:%p,%p\n", art, foot);
	switch(stack[sp-1].foottype){
	case _quic_:return http3master_write_byudp(art,foot, stack,sp, arg,idx, buf,len);
	default:return http3master_write_bysrc(art,foot, stack,sp, arg,idx, buf,len);
	}
	return 0;
}
int http3master_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int http3master_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int http3master_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int http3master_delete(_obj* ele)
{
	return 0;
}
int http3master_create(_obj* ele, u8* url)
{
	return 0;
}
