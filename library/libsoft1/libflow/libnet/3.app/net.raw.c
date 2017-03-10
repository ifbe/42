#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
u64 check_http(void*, int);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void say(void*, ...);




#define raw 0x776172
u64 serve_raw(u64 fd, u64 type, u8* buf, int len)
{
	int proto;

	//
	say(	"%02x:%02x:%02x:%02x:%02x:%02x->",
		buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
	say(	"%02x:%02x:%02x:%02x:%02x:%02x, ",
		buf[6], buf[7], buf[8], buf[9], buf[10], buf[11]);

	//
	proto = (buf[12]<<8)+buf[13];
	if(proto == 0x0800)
	{
		say("ipv%d, len=%d, ", buf[14]>>4, (buf[14]&0xf)<<2);
		if(buf[0x17] == 0x1)say("icmp\n");
		else if(buf[0x17] == 0x6)say("tcp\n");
		else if(buf[0x17] == 0x11)say("udp\n");
		else say("%02x\n", buf[0x17]);

		say("%d.%d.%d.%d:%d->%d.%d.%d.%d:%d\n",
		buf[26], buf[27], buf[28], buf[29], *(u16*)(buf+0x22),
		buf[30], buf[31], buf[32], buf[33], *(u16*)(buf+0x24)
		);
	}
	else say("%04x\n", proto);

	//
	say("\n");
	return raw;
}




#define chat 0x74616863
#define TLS 0x534c54
#define SSH 0x485353
#define SOCKS 0x534b434f53
u64 serve_first(u64 fd, u64 type, char* buf, int len)
{
	u64 ret;

	//tls server's connection
	if(buf[0] == 0x16)
	{
		return TLS;
	}

	//ssh server's connection
	ret = ncmp(buf, "SSH-2.0-", 8);
	if(ret == 0)
	{
		return SSH;
	}

	//socks server's connection
	if(0)
	{
		return SOCKS;
	}

	//1?:   http or ws
	ret = check_http(buf, len);
	if(ret > 0)
	{
		return ret;
	}

	//default: debug
	return chat;
}
