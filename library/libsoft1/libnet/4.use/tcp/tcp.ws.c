#include "libsoft.h"
#define _echo_ hex32('e','c','h','o')
int findzero(void* p);
int findhead(void* p);
int findtail(void* p);
u8* findstr(void* src, int max, void* target, int tarlen);
//
void sha1sum(u8* out, u8* in, int len);
void base64_encode(u8* out,u8* in, int len);
void datastr2hexstr(void* o, void* i, int len);
void decstr2data(void* i, void* o);








//client behavior
int websocket_clientread_handshake(u8* buf, int len, u8* dst, int max)
{
	return 0;
}
int websocket_clientwrite_handshake(u8* buf, int len, u8* dst, int max)
{
	return mysnprintf(dst, 0x1000,
		"GET / HTTP/1.1\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n"
		"\r\n"
	);
}
int websocket_clientread(u8* buf, int len, u8* dst, int max)
{
	if(0x81 != buf[0])logtoall("@wsclient_clientread: type=%x\n",buf[0]);

	if(126 > buf[1])return 2;
	if(126 == buf[1])return 4;
	if(127 == buf[1])return 10;
	return 0;
}
int websocket_clientwrite(u8* buf, int len, u8* dst, int max)
{
	int j,ret;
	u8 key[4];

	dst[0] = 0x81;
	if(len <= 125)
	{
		ret = 2;
		dst[1] = len;
	}
	else if(len < 0xffff)
	{
		ret = 4;
		dst[1] = 126;
		dst[2] = (len>>8)&0xff;
		dst[3] = len&0xff;
	}
	else
	{
		ret = 10;
		dst[1] = 127;
		dst[2] = 0;
		dst[3] = 0;
		dst[4] = 0;
		dst[5] = 0;
		dst[6] = (len>>24)&0xff;
		dst[7] = (len>>16)&0xff;
		dst[8] = (len>>8)&0xff;
		dst[9] = (len)&0xff;
	}

	dst[1] |= 0x80;
	*(u32*)key = 0x6d85f927;
	*(u32*)(dst+ret) = *(u32*)key;
	ret += 4;

	for(j=0;j<len;j++){
		dst[ret+j] = buf[j] ^ key[j%4];
	}
	return ret+len;
}




int wsclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int wsclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];
	logtoall("@wsclient_write: %llx, %.4s, %d\n", art, &foot, len);
    printmemory(buf, len<16?len:16);

	switch(stack[sp-1].foottype){
	case _dst_:
		ret = websocket_clientwrite(buf, len, tmp, 0x1000);
		//printmemory(buf, len);
		//printmemory(tmp, ret);
		give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
		break;
	case _src_:
		if(0 == art->vfmt)
		{
		/*	logtoall("ws.serverhello={\n"
				"%.*s"
				"}=ws.serverhello\n",
				len, buf);*/

			//parse serverhello
			//websocket_clientread_handshake();

			art->vfmt = 1;
		}
		else{
			ret = websocket_clientread(buf, len, 0, 0);
			buf += ret;
			len -= ret;

			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);
		}
		break;
	}
	return 0;
}
int wsclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsclient_attach(struct halfrel* self, struct halfrel* peer)
{
	int ret;
	u8 tmp[0x100];

	if(_src_ == self->foottype){
		ret = websocket_clientwrite_handshake(0, 0, tmp, 0);
		give_data_into_peer(self->pchip,_src_, 0,0, 0,0, tmp,ret);
	}
	return 0;
}
int wsclient_delete(_obj* ele)
{
	//unlink
	//delete
	return 0;
}
int wsclient_create(_obj* ele, u8* url)
{
	ele->vfmt = 0;
	return 1;
}








//server behavior
int websocket_serverread_handshake(u8* buf, int len, u8* dst, int max)
{
	int j;
	u8 sha1buf[0x100];
	u8 base64buf[0x100];
	u8* Sec_WebSocket_Key;

	//
	Sec_WebSocket_Key = findstr(buf, len, "Sec-WebSocket-Key", 17);
	if(Sec_WebSocket_Key == 0)return 0;
	Sec_WebSocket_Key += 19;

	//在Sec_WebSocket_Key尾巴上添加一个固定的字符串
	j = findtail(Sec_WebSocket_Key);
	j += mysnprintf(Sec_WebSocket_Key + j, 256,
		"258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	logtoall("Sec_WebSocket_Key=%s\n", Sec_WebSocket_Key);

	//对这个字符串做一次sha1
	sha1sum(sha1buf, Sec_WebSocket_Key, j);
	logtoall("sha1=");
	for(j=0;j<20;j++)logtoall("%02x", sha1buf[j]);
	logtoall("\n");

	//把sha1的结果以base64格式编码
	base64_encode(base64buf, sha1buf, 20);
	logtoall("base64=%s\n", base64buf);

	//把base64的结果作为accept密钥
	return mysnprintf(dst, 256,
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: %s\r\n"
		"\r\n",
		base64buf
	);
}
int websocket_serverwrite_handshake(u8* buf, int len, u8* dst, int max)
{
	//already replyed in websocket_serverread_handshake()
	return 0;
}
int websocket_serverread_head(u8* buf, int len, u8* dst, int max)
{
//#define dbg say
#define dbg(fmt,...) do{}while(0)
	int i,j,k;
	int type,masked;
	u8 key[4];
	u64 count;

	//byte0.bit7
	if((buf[0]&0x80)==0x80)dbg("tail,");
	else dbg("part,");

	//byte0.[3,0]
	k = buf[0]&0xf;
	if(k==0)
	{
		type=0;
		dbg("external,");
	}
	else if(k==1)
	{
		type=1;
		dbg("text,");
	}
	else if(k==2)
	{
		type=2;
		dbg("binary,");
	}
	else if(k==9)
	{
		dbg("ping\n");
		return 0;
	}
	else if(k==0xa)
	{
		dbg("pong\n");
		return 0;
	}
	else if(k==8)
	{
		//dbg("[%d]type8->close\n",fd);
		//epoll_del(fd);
		return -1;
	}
	else
	{
		//dbg("[%d]known->close\n",fd);
		//epoll_del(fd);
		return -2;
	}

	//byte1.bit7
	if( (buf[1]>>7) == 1)
	{
		masked=1;
		dbg("masked,");
	}
	else
	{
		masked=0;
		dbg("unmasked,");
	}

	//
	k = buf[1]&0x7f;
	if(k==127)
	{
		count = ((u64)buf[2]<<56)
			+ ((u64)buf[3]<<48)
			+ ((u64)buf[4]<<40)
			+ ((u64)buf[5]<<32)
			+ ((u64)buf[6]<<24)
			+ ((u64)buf[7]<<16)
			+ ((u64)buf[8]<<8)
			+ buf[9];
		k = 10;
	}
	else if(k==126)
	{
		count = (buf[2]<<8) + (buf[3]);
		k = 4;
	}
	else
	{
		count = k;
		k = 2;
	}
	dbg("count=%llx,", count);

	if(masked != 1)dbg("\n");
	else
	{
		*(u32*)key = *(u32*)(buf + k);
		j = k;
		k += 4;
		dbg("key=%x\n",*(u32*)key);

		if(type==1)
		{
			dst[0] = buf[0]&0x8f;
			dst[1] = buf[1]&0x7f;
			for(i=0;i<count;i++)
			{
				dst[i] = buf[i+k] ^ key[i%4];
				//dbg("%c",buf[j+i]);
			}
			dst[count] = 0;
			//dbg("\n");
		}//type=ascii
	}//masked=1

	dbg("%s\n",dst);
	return count;
}
int websocket_serverwrite_head(u8* buf, int len, u8* dst, int max)
{
	int ret;

	//len
	dst[0] = 0x81;
	if(len <= 125)
	{
		ret = 2;
		dst[1] = len;
	}
	else if(len < 0xffff)
	{
		ret = 4;
		dst[1] = 126;
		dst[2] = (len>>8)&0xff;
		dst[3] = len&0xff;
	}
	else
	{
		ret = 10;
		dst[1] = 127;
		dst[2] = 0;
		dst[3] = 0;
		dst[4] = 0;
		dst[5] = 0;
		dst[6] = (len>>24)&0xff;
		dst[7] = (len>>16)&0xff;
		dst[8] = (len>>8)&0xff;
		dst[9] = (len)&0xff;
	}

	return ret;
}




int wsserver_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int wsserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];
	logtoall("@wsserver_write: %llx, %.4s, %d\n", art, &foot, len);
    printmemory(buf, len<16?len:16);

	switch(stack[sp-1].foottype){
	case _dst_:
		ret = websocket_serverwrite_head(buf, len, tmp, 0x100);
		//printmemory(tmp, ret);
		//printmemory(buf, len);
		give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
		give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);
		break;
	case _src_:
		if(0 == art->vfmt){
			art->vfmt = 1;
		/*	logtoall("ws.clienthello={\n"
				"%.*s"
				"}=ws.clienthello\n",
				len, buf);*/

			//parse clienthello
			ret = websocket_serverread_handshake(buf, len, tmp, 256);
			ret = give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);

			//on clienthello do something
			//blen = mysnprintf(tmp, 0x1000, "Who dare summon me ?!");

			art->vfmt = 1;
		}
		else{
			ret = websocket_serverread_head(buf, len, tmp, 0x1000);
			logtoall("sending:\n");
			printmemory(tmp, ret<16?ret:16);
			give_data_into_peer(art,_dst_, stack,sp, 0,0, tmp,ret);
		}
		break;
	}
	return 0;
}
int wsserver_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsserver_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsserver_delete(_obj* ele)
{
	return 0;
}
int wsserver_create(_obj* ele, u8* url)
{
	ele->vfmt = 0;
	return 0;
}




int wsmaster_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int wsmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	logtoall("@wsserver_write: %llx, %.4s, %d\n", art, &foot, len);
    printmemory(buf, len<16?len:16);

	//socket: parent,child
	_obj* obj = stack[sp-2].pchip;
	if(0 == obj)return 0;
	_obj* Tcp = obj->sockinfo.child;
	if(0 == Tcp)return 0;

	//server
	_obj* Ws = artery_alloc_fromtype(_Ws_);
	if(0 == Ws)return 0;
	artery_create(Ws, 0, 0, 0);

	//socket -> server
	relationcreate(Ws, 0, _art_, _src_, Tcp, 0, _sys_, _dst_);
	stack[sp-2].pchip = Tcp;
	stack[sp-1].pchip = Ws;
	stack[sp-1].foottype = _src_;
	wsserver_write(Ws,0, stack,sp, 0,0, buf,len);

	//server -> ???
	switch(art->vfmt){
	case _echo_:{
		_obj* echo = artery_alloc_fromtype(_echo_);
		if(0 == echo)break;
		artery_create(echo, 0, 0, 0);
		relationcreate(Ws, 0, _art_, _dst_, echo, 0, _art_, _src_);
		//relationstart(&rel->srcchip, &rel->dstchip);
		break;
	}//echo
	case _ptmx_:{
		_obj* ptmx = system_alloc_frompath(_ptmx_, (u8*)"/dev/ptmx");
		if(0 == ptmx)break;
		system_create(ptmx, "/dev/ptmx", 0, 0);
		relationcreate(Ws, 0, _art_, _dst_, ptmx, 0, _sys_, _dst_);
		//relationstart(&rel->srcchip, &rel->dstchip);
		break;
	}//ptmx
	default:{
		logtoall("who can serve it?\n");
	}
	}//switch
	return 0;
}
int wsmaster_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsmaster_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsmaster_delete(_obj* ele)
{
	return 0;
}
int wsmaster_create(_obj* ele, u8* url)
{
	if(0 == url)goto none;
	if(0 == ncmp(url, "echo", 4)){
		ele->vfmt = _echo_;
		return 0;
	}
	if(0 == ncmp(url, "ptmx", 4)){
		ele->vfmt = _ptmx_;
		return 0;
	}

none:
	ele->vfmt = 0;
	return 0;
}
