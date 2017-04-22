#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
u32 getrandom();
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
//
void printmemory(void*, int);
void say(void*, ...);




struct object
{
	//[0x00,0x0f]
	u64 type_sock;  //raw, bt, udp, tcp?
	u64 stage0;
	u64 type_road;  //ssh, tls?
	u64 stage1;
	u64 type_app;   //http2, ws, rdp, vnc?
	u64 stage2;
	u64 type_data;  //html, rgb?
	u64 stage3;

	//[0x40,0x7f]
	u8 addr_src[0x20];
	u8 addr_dst[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};




struct dnshead
{
	u16 tran;
	u16 flag;
	u16 q;
	u16 a;
	u16 auth;
	u16 addi;
};
int dns_read_query(u8* buf, int len)
{
	printmemory(buf, len);
	return len;
}
int dns_read_answer(u8* buf, int len)
{
	printmemory(buf, len);
	return len;
}
int dns_write_query(u8* buf, int len, u8* domain, int count)
{
	int i,j,k;
	struct dnshead* h;

	//head
	h = (void*)buf;
	h->tran = getrandom()&0xffff;
	h->flag = 1;
	h->q = 0x100;
	h->a = 0;
	h->auth = 0;
	h->addi = 0;

	//question
	i = 0;		//seg len
	j = 0;		//src len
	k = 12;		//dst len
	while(1)
	{
		if(domain[j] <= 0x20)
		{
			buf[k] = i;
			buf[k+i+1] = 0;
			k += i+2;
			break;
		}
		else if(domain[j] == '.')
		{
			buf[k] = i;
			k += i+1;
			i = 0;
		}
		else
		{
			buf[k+1+i] = domain[j];
			i++;
		}

		j++;
	}

	//tail
	buf[k+0] = 0;
	buf[k+1] = 1;
	buf[k+2] = 0;
	buf[k+3] = 1;

	return k+4;
}
int dns_write_answer(u8* buf, int len)
{
	//name
	buf[len] = 0xc0;
	buf[len+1] = 0xc0;

	//type
	buf[len+2] = 0;
	buf[len+3] = 1;

	//class
	buf[len+4] = 0;
	buf[len+5] = 1;

	//ttl
	buf[len+6] = 0;
	buf[len+7] = 0;
	buf[len+8] = 2;
	buf[len+9] = 8;

	//4
	buf[len+10] = 0;
	buf[len+11] = 4;

	//ip
	buf[len+12] = 127;
	buf[len+13] = 0;
	buf[len+14] = 0;
	buf[len+15] = 1;

	return len+16;
}




#define DNS 0x534e44
#define dns 0x736e64
int serve_dns(struct object* obj, int fd, u8* buf, int len)
{
	int ret;
	u64 type = obj[fd].type_road;
	if(type == DNS)
	{
		ret = dns_write_answer(buf, len);
		writesocket(fd, buf, 0, len+16);
	}
	if(type == dns)
	{
		dns_read_answer(buf, len);
	}
	return 0;
}
