#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void arp_create(void*,void*);
void eth_create(void*,void*);
void ftp_create(void*,void*);
void http_create(void*,void*);
void icmp_create(void*,void*);
void ipv4_create(void*,void*);
void ipv6_create(void*,void*);
void ppp_create(void*,void*);
void quic_create(void*,void*);
void slip_create(void*,void*);
void ssh_create(void*,void*);
void tftp_create(void*,void*);
void client_delete();
void server_delete();
void arp_delete();
void eth_delete();
void ftp_delete();
void http_delete();
void icmp_delete();
void ipv4_delete();
void ipv6_delete();
void ppp_delete();
void quic_delete();
void slip_delete();
void ssh_delete();
void tftp_delete();
void client_create(void*,void*);
void server_create(void*,void*);
int net_stop(u64 fd);
//
int readserver(u64 fd, u8* addr, u64 offset, u64 count);
int writeserver(u64 fd, u8* addr, u64 offset, u64 count);
int startserver(u8* addr, int port, u8* dir, int opt);
int stopserver(u64 x);
//
int cmp(u8*,u8*);
int ncmp(u8*,u8*,int);
//
void printmemory(char*, int);
void say(char*, ...);




//
struct node
{
	//8*4=32
	u64 fd;
	u64 time;
	u64 type;
	u64 usage;

	//32
	u8 addr[32];
};
static struct node known[16];
static int max = 0;
//
static char* GET = 0;
static char* Connection = 0;
static char* Upgrade = 0;
static char* Sec_WebSocket_Key = 0;
//
static u8* datahome;




static void explainstr(char* buf, int max)
{
	int flag;
	int linehead;

	GET = 0;
	Connection = 0;
	Upgrade = 0;
	Sec_WebSocket_Key = 0;

	linehead = 0;
	while(1)
	{
		if(ncmp(buf+linehead, "GET ", 4) == 0)GET = buf+linehead+4;
		else if(ncmp(buf+linehead, "Connection: ", 12) == 0)Connection = buf+linehead+12;
		else if(ncmp(buf+linehead, "Upgrade: ", 9) == 0)Upgrade = buf+linehead+9;
		else if(ncmp(buf+linehead, "Sec-WebSocket-Key: ", 19) == 0)Sec_WebSocket_Key = buf+linehead+19;

		//eat until next character
		flag=0;
		while(1)
		{
			if(buf[linehead] == 0)
			{
				//say("[0x0@(%d,%d)]\n",linehead,max);
			}
			else if(buf[linehead] == 0xd)
			{
				flag=1;
				//say("[0xd@(%d,%d)]\n",linehead,max);
			}
			else if(buf[linehead] == 0xa)
			{
				flag=1;
				//say("[0xa@(%d,%d)]",linehead,max);
				say("\n");
			}
			else
			{
				if(flag==0)
				{
					say("%c", buf[linehead]);
				}
				else break;
			}

			linehead++;
			if(linehead >= max)break;
		}

		if(linehead >= max)break;
	}
	say("GET@%llx,Connection@%llx,Upgrade@%llx,Sec-WebSocket-Key@%llx\n",
		(u64)GET,
		(u64)Connection,
		(u64)Upgrade,
		(u64)Sec_WebSocket_Key
	);
}




void printknown()
{
	int k;
	for(k=0;k<max;k++)
	{
		say("%d ",known[k].fd);
	}
	say("\n");
}
void handle_delete(u64 fd)
{
	int j;
	say("[%d]leave\n", fd);

	for(j=0;j<max;j++)
	{
		if(known[j].fd == fd)
		{
			for(;j<max-1;j++)
			{
				known[j].fd = known[j+1].fd;
				known[j].type = known[j+1].type;
			}
			known[j].fd = 0;
			max--;
printknown();
			break;
		}
	}
}
void handle_create(u64* p)
{
	int j,k;
	u64 str = p[0];
	u64 fd = p[2];

	//user max
	if(max >= 8)
	{
		net_stop(fd);
		return;
	}

	//
	for(j=0;j<max+1;j++)
	{
		if(known[j].fd == fd)
		{
			known[j].type = 0;
			break;
		}

		if(known[j].fd < fd)
		{
			for(k=max;k>j;k--)
			{
				known[k].fd = known[k-1].fd;
				known[k].type = known[k-1].type;
			}

			known[j].fd = fd;
			known[j].type = 0;
			max++;
printknown();
			break;
		}
	}

	say("[%d]%s\n", fd, (void*)str);
}
void handle_read(u64* p)
{
	int ret;
	u64 fd = p[2];

	ret = readserver(fd, datahome, 0, 0x100000);
	if(ret <= 0)
	{
		net_stop(fd);
	}
	datahome[ret] = 0;

/*
	if(type == 1)
	{
		//普通socket，而且不是GET请求，丢弃
		explainstr(datahome, nread);
		if(GET == 0)return;

		//可能是http，websocket
		else
		{
			//这是个websocket请求
			if( (Upgrade != 0) && (Sec_WebSocket_Key != 0) )
			{
				handshake_websocket(fd);
			}

			//http请求根
			else if(GET != 0)
			{
				handshake_http(fd);
			}

			return;
		}
	}
*/
	net_stop(fd);
}




void net_read(u64* p)
{
	int ret = p[1] & 0xffff;

	if(ret == 0x406e) handle_read(p);
	else if(ret == 0x2b6e) handle_create(p);
	else if(ret == 0x2d6e) handle_delete(p[2]);
}
int net_write()
{
	return 0;
}
int net_list()
{
	return 0;
}
int net_choose()
{
	return 0;
}
int net_start()
{
	return 0;
}
int net_stop(u64 fd)
{
	handle_delete(fd);
	stopserver(fd);
	return 0;
}
int net_create(void* world,u64* p)
{
	//
	int j;
	char* q = (void*)known;
	for(j=0;j<64*16;j++)
	{
		q[j] = 0;
	}
	datahome = world + 0x300000;

	//
	p[0]=0;			//type
	p[1]=0x74656e;		//id

	p[10]=(u64)net_start;
	p[11]=(u64)net_stop;
	p[12]=(u64)net_list;
	p[13]=(u64)net_choose;
	p[14]=(u64)net_read;
	p[15]=(u64)net_write;

	//
	q=(char*)p+0x80;

	client_create(world,q);
	q+=0x80;

	server_create(world,q);
	q+=0x80;

	arp_create(world,q);
	eth_create(world,q);
	ftp_create(world,q);
	http_create(world,q);
	icmp_create(world,q);
	ipv4_create(world,q);
	ipv6_create(world,q);
	ppp_create(world,q);
	quic_create(world,q);
	slip_create(world,q);
	ssh_create(world,q);
	tftp_create(world,q);

	return q-(char*)p;
}
int net_delete()
{
	tftp_delete();
	ssh_delete();
	slip_delete();
	quic_delete();
	ppp_delete();
	ipv6_delete();
	ipv4_delete();
	icmp_delete();
	http_delete();
	ftp_delete();
	eth_delete();
	arp_delete();

	server_delete();
	client_delete();
	return 0;
}