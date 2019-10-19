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
	if(0x81 != buf[0])say("@wsclient_clientread: type=%x\n",buf[0]);

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




int wsclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int wsclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];
	struct relation* orel;
	struct element* ele = self->pchip;
	say("@wsclient_write: %llx, %.4s, %d\n", self->pchip, &self->flag, len);
    printmemory(buf, len<16?len:16);

	switch(self->flag){
		case _dst_:{
			ret = websocket_clientwrite(buf, len, tmp, 0x1000);
			//printmemory(buf, len);
			//printmemory(tmp, ret);
			relationwrite(ele, _src_, 0, 0, tmp, ret);
			break;
		}
		case _src_:{
			if(0 == ele->stage1)
			{
			/*	say("ws.serverhello={\n"
					"%.*s"
					"}=ws.serverhello\n",
					len, buf);*/

				//parse serverhello
				//websocket_clientread_handshake();

				ele->stage1 = 1;
			}
			else{
				ret = websocket_clientread(buf, len, 0, 0);
				buf += ret;
				len -= ret;

				relationwrite(ele, _dst_, 0, 0, buf, len);
			}
			break;
		}
	}
	return 0;
}
int wsclient_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsclient_start(struct halfrel* self, struct halfrel* peer)
{
	int ret;
	u8 tmp[0x100];

	if(_src_ == self->flag){
		ret = websocket_clientwrite_handshake(0, 0, tmp, 0);
		relationwrite(self->pchip, _src_, 0, 0, tmp, ret);
	}
	return 0;
}
int wsclient_delete(struct element* ele)
{
	//unlink
	//delete
	return 0;
}
int wsclient_create(struct element* ele, u8* url)
{
	ele->stage1 = 0;
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
	say("Sec_WebSocket_Key=%s\n", Sec_WebSocket_Key);

	//对这个字符串做一次sha1
	sha1sum(sha1buf, Sec_WebSocket_Key, j);
	say("sha1=");
	for(j=0;j<20;j++)say("%02x", sha1buf[j]);
	say("\n");

	//把sha1的结果以base64格式编码
	base64_encode(base64buf, sha1buf, 20);
	say("base64=%s\n", base64buf);

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




int wsserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int wsserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];
	struct element* ele = self->pchip;
	say("@wsserver_write: %llx, %.4s, %d\n", self->pchip, &self->flag, len);
    printmemory(buf, len<16?len:16);

	switch(self->flag){
		case _dst_:{
			ret = websocket_serverwrite_head(buf, len, tmp, 0x100);
			//printmemory(tmp, ret);
			//printmemory(buf, len);
			relationwrite(ele, _src_, 0, 0, tmp, ret);
			relationwrite(ele, _src_, 0, 0, buf, len);
			break;
		}
		case _src_:{
			if(0 == ele->stage1){
				ele->stage1 = 1;
			/*	say("ws.clienthello={\n"
					"%.*s"
					"}=ws.clienthello\n",
					len, buf);*/

				//parse clienthello
				ret = websocket_serverread_handshake(buf, len, tmp, 256);
				ret = relationwrite(ele, _src_, 0, 0, tmp, ret);

				//on clienthello do something
				//blen = mysnprintf(tmp, 0x1000, "Who dare summon me ?!");

				ele->stage1 = 1;
			}
			else{
				ret = websocket_serverread_head(buf, len, tmp, 0x1000);
				say("sending:\n");
				printmemory(tmp, ret<16?ret:16);
				relationwrite(ele, _dst_, 0, 0, tmp, ret);
			}
			break;
		}
	}
	return 0;
}
int wsserver_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsserver_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsserver_delete(struct element* ele)
{
	return 0;
}
int wsserver_create(struct element* ele, u8* url)
{
	ele->stage1 = 0;
	return 0;
}




int wsmaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int wsmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct object* obj;		//parent
	struct object* Tcp;		//child

	struct element* ele;	//master
	struct element* Ws;		//server

	struct relation* rel;
	struct object* ptmx;
	struct element* echo;
	say("@wsserver_write: %llx, %.4s, %d\n", self->pchip, &self->flag, len);
    printmemory(buf, len<16?len:16);

	//parent, child
	obj = peer->pchip;
	if(0 == obj)return 0;
	Tcp = obj->tempobj;
	if(0 == Tcp)return 0;

	//master, server
	ele = self->pchip;
	if(0 == ele)return 0;
	Ws = arterycreate(_Ws_, 0, 0, 0);
	if(0 == Ws)return 0;

	//child -> server
	rel = relationcreate(Ws, 0, _art_, _src_, Tcp, 0, _sys_, _dst_);
	//relationstart(&rel->srcchip, &rel->dstchip);
	self = (void*)&rel->dstchip;
	peer = (void*)&rel->srcchip;
	arterywrite(self, peer, 0, 0, buf, len);

	//server -> ???
	switch(ele->name){
	case _echo_:{
		say("1111111\n");
		echo = arterycreate(_echo_, 0, 0, 0);
		relationcreate(Ws, 0, _art_, _dst_, echo, 0, _art_, _src_);
		//relationstart(&rel->srcchip, &rel->dstchip);
		break;
	}//echo
	case _ptmx_:{
		say("22222222\n");
		ptmx = systemcreate(_ptmx_, "/dev/ptmx", 0, 0);
		relationcreate(Ws, 0, _art_, _dst_, ptmx, 0, _sys_, _dst_);
		//relationstart(&rel->srcchip, &rel->dstchip);
		break;
	}//ptmx
	default:{
		say("who can serve it?\n");
	}
	}//switch
	return 0;
}
int wsmaster_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsmaster_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int wsmaster_delete(struct element* ele)
{
	return 0;
}
int wsmaster_create(struct element* ele, u8* url)
{
	if(0 == url)goto none;
	if(0 == ncmp(url, "echo", 4)){
		ele->name = _echo_;
		return 0;
	}
	if(0 == ncmp(url, "ptmx", 4)){
		ele->name = _ptmx_;
		return 0;
	}

none:
	ele->name = 0;
	return 0;
}
