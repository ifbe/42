#include "artery.h"
void eventwrite(u64,u64,u64,u64);
void sha1sum(u8* out, u8* in, int len);
void base64_encode(u8* out,u8* in, int len);
void datastr2hexstr(void* o, void* i, int len);
void decstr2data(void* i, void* o);
//
int findzero(void* p);
int findhead(void* p);
int findtail(void* p);
u8* findstr(void* src, int max, void* target, int tarlen);
//
int readsocket(int fd, u8* addr, int offset, int count);
int writesocket(int fd, u8* addr, int offset, int count);
int ncmp(void*, void*, int);
int cmp(void*, void*);




int websocket_read_handshake(u64 fd, u8* buf, int len)
{
	int j;
	u8* Sec_WebSocket_Key;
	u8* sha1buf = buf+0x1000;
	u8* base64buf = buf+0x2000;

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
	return mysnprintf(buf, 256,
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: %s\r\n"
		"\r\n",
		base64buf
	);
}
int websocket_read(u8* buf, int len)
{
//#define dbg say
#define dbg(fmt,...) do{}while(0)
	int i,j,k;
	int type,masked;
	unsigned char key[4];
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
		count	= ((u64)buf[2]<<56)
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
		count	= (buf[2]<<8)
			+ (buf[3]);
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
			buf[0] &= 0x8f;
			buf[1] &= 0x7f;
			for(i=0;i<count;i++)
			{
				buf[i] = buf[i+k] ^ key[i%4];
				//dbg("%c",buf[j+i]);
			}
			buf[count] = 0;
			//dbg("\n");
		}//type=ascii
	}//masked=1

	dbg("%s\n",buf);
	return count;
}




int websocket_write_handshake(u8* buf, int len)
{
	return mysnprintf(buf, len,
		"GET / HTTP/1.1\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n"
		"\r\n"
	);
}
int websocket_write(u64 fd, void* buf, int len)
{
	u8 headbuf[16];
	int headlen;
	int ret;

	//len
	headbuf[0] = 0x81;
	if(len<=125)
	{
		headlen = 2;
		headbuf[1] = len;
	}
	else if(len<0xffff)
	{
		headlen = 4;
		headbuf[1] = 126;
		headbuf[2] = (len>>8)&0xff;
		headbuf[3] = len&0xff;
	}
	else
	{
		headlen = 10;
		headbuf[1] = 127;
		headbuf[2] = 0;
		headbuf[3] = 0;
		headbuf[4] = 0;
		headbuf[5] = 0;
		headbuf[6] = (len>>24)&0xff;
		headbuf[7] = (len>>16)&0xff;
		headbuf[8] = (len>>8)&0xff;
		headbuf[9] = (len)&0xff;
	}

	//write
	ret = writesocket(fd, headbuf, 0, headlen);
	ret = writesocket(fd, buf, 0, len);
	return ret;
}




#define ws 0x7377
#define WS 0x5357
int serve_ws_client_hello(struct object* obj, int fd, u8* buf, int len)
{
	printmemory(buf, len);
	obj[fd].stage1 = 1;
	return ws;
}
int serve_ws_client_data(struct object* obj, int fd, u8* buf, int len)
{
	printmemory(buf, len);
	return ws;
}
int serve_ws_server_hello(struct object* obj, int fd, u8* buf, int len)
{
	int ret = websocket_read_handshake(fd, buf, len);
	if(ret <= 0)goto theend;

	ret = writesocket(fd, buf, 0, ret);
	if(ret <= 0)goto theend;

	obj[fd].stage1 = 1;

theend:
	return WS;
}
int serve_ws_server_data(struct object* obj, int fd, u8* buf, int len)
{
	u64 temp;
	len = websocket_read(buf, len);
	if(len < 0)goto theend;

	temp = obj[fd].stage1;
	if(temp == 1)
	{
		say("%.*s\n", len, buf);

		websocket_write(fd, "four two", 8);
		obj[fd].stage1 = 2;
	}
	else if(temp == 2)
	{
		say("%.*s\n", len, buf);

		websocket_write(fd, "haha@2", 6);
		obj[fd].stage1 = 3;
	}
	else if(temp == 3)
	{
		printmemory(buf, len);
		websocket_write(fd, "success", 7);

		obj[fd].stage1 = 4;

		obj[fd].type_data = 0x2b77;
		obj[fd].stage3 = WS;
	}
	else
	{
		if(ncmp(buf, "kbd ", 4) == 0)
		{
			obj[fd].type_data = 0x64626b;
			decstr2data(buf+4, &(obj[fd].stage3));
		}
		else if(ncmp(buf, "char ", 5) == 0)
		{
			obj[fd].type_data = 0x72616863;
			decstr2data(buf+5, &(obj[fd].stage3));
		}
		else
		{
			obj[fd].type_data = 0;
		}
	}

theend:
	return WS;
}
int serve_ws(struct object* obj, int fd, u8* buf, int len)
{
	u64 type = obj[fd].type_road;
	u64 stage = obj[fd].stage1;
	if(type == ws)
	{
		if(stage == 0)
		{
			return serve_ws_client_hello(obj, fd, buf, len);
		}
		else
		{
			return serve_ws_client_data(obj, fd, buf, len);
		}
	}
	else
	{
		if(stage == 0)
		{
			return serve_ws_server_hello(obj, fd, buf, len);
		}
		else
		{
			return serve_ws_server_data(obj, fd, buf, len);
		}
	}
}
int serve_wss(void* p, int fd, u8* buf, int len)
{
	return 0;
}
