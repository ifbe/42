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




#define dbg logtoall
//#define dbg(fmt,...) do{}while(0)


#define WS_OPCODE_CONTINUATION          0x0
#define WS_OPCODE_TEXT                  0x1
#define WS_OPCODE_BINARY                0x2
#define WS_OPCODE_CLOSE                 0x8
#define WS_OPCODE_PING                  0x9
#define WS_OPCODE_PONG                  0xA
struct clientframe_1byte {	// 1字节负载长度版本 (PayloadLen <= 125)
    u8 opcode:4;      // [0,3]  操作码: 0x1=文本, 0x2=二进制, 0x8=关闭, 0x9=ping, 0xa=pong
    u8 rsvd:3;
    u8 fin:1;         // 7      是否最后一帧

    u8 payloadlen:7;  // [8,14] 负载长度 (7位)
    u8 mask:1;        // 15     是否掩码

    u8 maskkey[4];    // [16,47] 掩码密钥 (如果mask=1)

    u8 data[];
} __attribute__((packed));
struct clientframe_2byte {	// 2字节负载长度版本 (PayloadLen = 126)
    u8 opcode:4;      // [0,3]  操作码: 0x1=文本, 0x2=二进制, 0x8=关闭, 0x9=ping, 0xa=pong
    u8 rsvd:3;
    u8 fin:1;         // 7      是否最后一帧

    u8 payloadlen:7;  // [8,14] 负载长度 (7位)
    u8 mask:1;        // 15     是否掩码

    u8 extlen[2];     // [16,31] 16位扩展长度

    u8 maskkey[4];    // [32,63] 掩码密钥 (如果mask=1)

    u8 data[];
} __attribute__((packed));
struct clientframe_8byte {	// 8字节负载长度版本 (PayloadLen = 127)
    u8 opcode:4;      // [0,3]  操作码: 0x1=文本, 0x2=二进制, 0x8=关闭, 0x9=ping, 0xa=pong
    u8 rsvd:3;
    u8 fin:1;         // 7      是否最后一帧

    u8 payloadlen:7;  // [8,14] 负载长度 (7位)
    u8 mask:1;        // 15     是否掩码

    u8 extlen[8];     // [16,79] 64位扩展长度

    u8 maskkey[4];    // [80,111] 掩码密钥 (如果mask=1)

    u8 data[];
} __attribute__((packed));

//
struct perserver{
	u8* recvcache;
	u8* decryptcache;
	int recvcount;
	int decryptcount;
} __attribute__((packed));




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
int wsserver_parse_handshake(u8* buf, int len, u8* dst, int max)
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
int wsserver_onrecv_handshake(_obj* art,void* foot, _syn* stack,int sp, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

/*	logtoall("ws.clienthello={\n"
		"%.*s"
		"}=ws.clienthello\n",
		len, buf);*/

	//parse clienthello
	ret = wsserver_parse_handshake(buf, len, tmp, 256);
	ret = give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);

	return 0;
}




int wsserver_parse_packet_old(u8* buf, int len, u8* dst, int max)
{
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
		return -99;
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

	*(u32*)key = *(u32*)(buf + k);
	j = k;
	k += 4;
	dbg("key=%x\n",*(u32*)key);

	switch(type){
	case 1:
		dst[0] = buf[0]&0x8f;
		dst[1] = buf[1]&0x7f;
		for(i=0;i<count;i++)
		{
			dst[i] = buf[i+k] ^ key[i%4];
			//dbg("%c",buf[j+i]);
		}
		dst[count] = 0;
		break;
	case 2:
		dbg("binary\n");
		break;
	}

	dbg("%s\n",dst);
	return count;
}
int wsserver_onrecv_pkt(_obj* art,void* foot, _syn* stack,int sp, void* buf, int len)
{
	if(len < 6){
		logtoall("%s warn: recvcount=%d\n", __FUNCTION__, len);
		return 0;
	}

	struct clientframe_1byte* pkt1 = (void*)buf;
	struct clientframe_2byte* pkt2 = (void*)buf;
	struct clientframe_8byte* pkt8 = (void*)buf;
	if(pkt1->mask != 1){
		logtoall("%s error: mask=%d\n", __FUNCTION__, pkt1->mask);
		return -2;
	}

	int j;
	int headlen = 2+4;
	u64 datalen = 0;
	u8* dataptr = 0;
	u8* maskkey = 0;
	if(pkt1->payloadlen == 127){
		headlen = 2+8+4;
		datalen = 0;
		for(j=0;j<8;j++)datalen = (datalen<<8) | pkt8->extlen[j];
		//
		maskkey = pkt8->maskkey;
		dataptr = pkt8->data;
	}
	else if(pkt1->payloadlen == 126){
		headlen = 2+2+4;
		datalen = (pkt2->extlen[0]<<8) | pkt2->extlen[1];
		//
		maskkey = pkt2->maskkey;
		dataptr = pkt2->data;
	}
	else{
		datalen = pkt1->payloadlen;
		//
		maskkey = pkt1->maskkey;
		dataptr = pkt1->data;
	}

	if(headlen+datalen > len){
		logtoall("%s warn: headlen+datalen(%x+%x)>len(%x)\n", __FUNCTION__, headlen, datalen, len);
		return 0;
	}

	logtoall("%s: opcode=%d fin=%d headlen=%x datalen=%x\n", __FUNCTION__, pkt1->opcode, pkt1->fin, headlen, datalen);
	switch(pkt1->opcode){
	case WS_OPCODE_CONTINUATION:
		break;
	case WS_OPCODE_TEXT:
		break;
	case WS_OPCODE_BINARY:
		break;
	case WS_OPCODE_CLOSE:
		return headlen+datalen;
	case WS_OPCODE_PING:
		return headlen+datalen;
	case WS_OPCODE_PONG:
		return headlen+datalen;
	default:
		return headlen+datalen;
	}

	struct perserver* per = (void*)art->priv_256b;
	if( (datalen < 0x1000) && (per->decryptcount==0) && (pkt1->fin == 1) ){
		int j;
		u8 tmp[0x1000];
		for(j=0;j<datalen;j++)
		{
			tmp[j] = dataptr[j] ^ maskkey[j%4];
			//dbg("%c",buf[j+i]);
		}
		give_data_into_peer(art,_dst_, stack,sp, 0,0, tmp,datalen);
	}
	else{
		if(0 == per->decryptcache){
			per->decryptcount = 0;
			per->decryptcache = memoryalloc(0x100000, 0);
		}

		for(j=0;j<datalen;j++){
			//dbg("%c",buf[j+i]);
			per->decryptcache[per->decryptcount + j] = dataptr[j] ^ maskkey[j%4];
		}
		per->decryptcount += datalen;

		if(pkt1->fin){
			give_data_into_peer(art,_dst_, stack,sp, 0,0, per->decryptcache,per->decryptcount);
			per->decryptcount = 0;
		}
	}

	return headlen+datalen;
}



int wsserver_onrecv_cache(struct perserver* per, u8* buf, int len)
{
	if(0 == per->recvcache){
		per->recvcache = memoryalloc(0x100000, 0);
		if(0 == per->recvcache)return -1;
	}

	int j;
	for(j=0;j<len;j++)per->recvcache[per->recvcount + j] = buf[j];
	per->recvcount += len;
	return 0;
}
int wsserver_onrecv_raw(_obj* art,void* foot, _syn* stack,int sp, void* buf, int len)
{
	struct perserver* per = (void*)art->priv_256b;

	int usecache = 0;
	if( (len < 6) || (per->recvcount > 0) ){
		wsserver_onrecv_cache(per, buf, len);
		usecache = 1;
	}

	int j;
	int ret;
	int offs = 0;
	while(1){
		if(usecache){
			logtoall("%s usecache: %x/%x\n", __FUNCTION__, offs, len);
			ret = wsserver_onrecv_pkt(art,foot, stack,sp, per->recvcache+offs,per->recvcount-offs);
			//ret is how many byte consumed
			if(ret < 0){
				logtoall("%s error: ret=%d\n", __FUNCTION__, ret);
				per->recvcount = 0;
				//todo: close socket
				break;
			}
			else if(ret == 0){
				//buffer not consumed, means pkt not complete, move remain to head and return
				if(offs > 0){
					for(j=0;j<per->recvcount - offs;j++){
						per->recvcache[0+j] = per->recvcache[offs+j];
					}
					per->recvcount -= offs;
				}
				break;
			}
			else if(offs+ret < per->recvcount){
				offs += ret;
				continue;
			}
			else if(offs+ret == per->recvcount){
				//normal finish
				per->recvcount = 0;
				break;
			}
			else{
				logtoall("%s error: offs+ret(%x+%x) > recvcount(%x)\n", __FUNCTION__, offs, ret, per->recvcount);
				break;
			}
		}
		else{
			logtoall("%s usebuf: %x/%x\n", __FUNCTION__, offs, len);
			ret = wsserver_onrecv_pkt(art,foot, stack,sp, buf+offs,len-offs);

			//ret is how many byte consumed
			if(ret < 0){
				logtoall("%s error: ret=%d\n", __FUNCTION__, ret);
				break;
			}
			else if(ret == 0){
				//buffer not consumed, means pkt not complete, wait next buffer
				wsserver_onrecv_cache(per, buf+offs, len-offs);
				break;
			}
			else if(offs+ret < len){
				offs += ret;
				continue;
			}
			else if(offs+ret == len){
				//normal finish
				break;
			}
			else{
				logtoall("%s error: offs+ret(%x+%x) > len(%x)\n", __FUNCTION__, offs, ret, len);
				break;
			}
		}//while
	}

	return 0;
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
int wsserver_respond_text(_obj* art,void* foot, _syn* stack,int sp, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

	ret = websocket_serverwrite_head(buf, len, tmp, 0x100);

	//printmemory(tmp, ret);
	give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);

	//printmemory(buf, len);
	give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);
	return 0;
}
int wsserver_respond_blob(_obj* art,void* foot, _syn* stack,int sp, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

	ret = websocket_serverwrite_head(buf, len, tmp, 0x100);
	tmp[0] = 0x82;  // FIN=1, opcode=2 (binary)

	//printmemory(tmp, ret);
	give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);

	//printmemory(buf, len);
	give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);
	return 0;
}



int wsserver_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int wsserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	logtoall("@wsserver_write: obj=%p, len=%x\n", art, len);
    printmemory(buf, len<16?len:16);

	switch(stack[sp-1].foottype){
	case _dst_:
		logtoall("from dst\n");
		if(0){		//text
			wsserver_respond_text(art, foot, stack,sp, buf, len);
		}
		else{		//blob
			wsserver_respond_blob(art, foot, stack,sp, buf, len);
		}
		break;
	case _src_:
		logtoall("from src: %d\n", art->vfmt);
		if(0 == art->vfmt){
			wsserver_onrecv_handshake(art,foot, stack,sp, buf, len);
			art->vfmt = 1;
		}
		else{
			wsserver_onrecv_raw(art,foot, stack,sp, buf, len);
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
	struct perserver* per = (void*)ele->priv_256b;
	per->recvcache = 0;
	per->recvcount = 0;
	per->decryptcache = 0;
	per->decryptcount = 0;

	ele->vfmt = 0;
	return 0;
}




int wsmaster_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int wsmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	logtoall("@wsmaster_write: %llx, %.4s, %d\n", art, &foot, len);
	printmemory(buf, len<16?len:16);

	if(_src_ != stack[sp-1].foottype){
		logtoall("%s error: reject not from src\n", __FUNCTION__);
		return 0;
	}

	//server
	_obj* Ws = artery_alloc_fromtype(_Ws_);
	if(0 == Ws)return 0;
	artery_create(Ws, 0, 0, 0);

	//socket: parent,child
	_obj* obj = stack[sp-2].pchip;
	if(0 == obj)return 0;
	_obj* Tcp = obj->sockinfo.child;
	if(0 == Tcp)return 0;

	//socket -> server
	relationcreate(Tcp, 0, _sys_, _dst_, Ws, 0, _art_, _src_);
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
		relationcreate(echo, 0, _art_, _src_, Ws, 0, _art_, _dst_);
		//relationstart(&rel->srcchip, &rel->dstchip);
		break;
	}//echo
	case _ptmx_:{
		_obj* ptmx = system_alloc_frompath(_ptmx_, (u8*)"/dev/ptmx");
		if(0 == ptmx)break;
		system_create(ptmx, "/dev/ptmx", 0, 0);
		relationcreate(ptmx, 0, _sys_, _dst_, Ws, 0, _art_, _dst_);
		//relationstart(&rel->srcchip, &rel->dstchip);
		break;
	}//ptmx
	default:{
		logtoall("who can serve it?\n");

		struct halfrel* pair[2];
		int ret = relationsearch(art, _tar_, &pair[0], &pair[1]);
		if(ret <= 0)break;

		relationcreate(Ws, 0, _art_, _dst_, pair[1]->pchip, 0, pair[1]->nodetype, pair[1]->foottype);
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
