#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int serve_first(void* p, u8* buf, u64 len);
int serve_chat(void* p, u8* buf, u64 len);
int serve_http(void* p, u8* buf, u64 len);
int serve_ws(void* p, u8* buf, u64 len);
int serve_https(void* p, u8* buf, u64 len);
int serve_wss(void* p, u8* buf, u64 len);
int serve_secureshell(void* p, u8* buf, u64 len);
//
int selfname(u64, void*);
int peername(u64, void*);
void client_create(void*,void*);
void server_create(void*,void*);
void client_delete();
void server_delete();
//
void printmemory(char*, int);
void say(char*, ...);




struct node
{
	//why
	u64 detail;

	//what
	u64 type;       //[0,f]none, [0x10,0x1f]tcp, [0x20,0x2f]ws

	//where
	u64 fd;

	//when
	u64 time;

	//32
	u8 addr[32];
};
static int max = 0;

//
static struct node* known = 0;
static u8* dirhome = 0;
static u8* datahome = 0;




void net_print()
{
	int k;
	u64 fd;
	u8* p;
	u32* q;

	say("\nknown{\n");
	for(k=0;k<max;k++)
	{
		fd = known[k].fd;
		p = known[k].addr;
		q = (u32*)(p+4);

		say("   [%lld]%d.%d.%d.%d:%d\n",
			fd,
			p[0],p[1],p[2],p[3],
			q[0]
		);
	}
	say("}%d\n",max);
}




int net_delete()
{
	server_delete();
	client_delete();
	return 0;
}
int net_create(void* world,u64* p)
{
	int j;
	char* tmp;
	known = world + 0x100000 + 0xf0000;
	dirhome = world + 0x200000;
	datahome = world + 0x300000;

	tmp = (void*)known;
	for(j=0;j<0x10000;j++)tmp[j] = 0;

	tmp=(char*)p;
	client_create(world, tmp);
	tmp+=0x80;

	server_create(world, tmp);
	tmp+=0x80;

	return tmp-(char*)p;
}
void net_stop(u64 fd)
{
	int j;
	//say("[%d]leave\n", fd);

	for(j=0;j<max;j++)
	{
		if(known[j].fd == fd)
		{
			//左移
			movsb(&known[j], &known[j+1], (max-1-j)*sizeof(struct node));
			max--;

			//
			known[max].fd = 0;

			//
			net_print();
			break;
		}
	}
}
void net_start(u64 fd)
{
	int j,k;
	if(max >= 0x10000/0x40)
	{
		writeserver(fd, "too many", 0, 8);
		net_delete(fd);
		stopserver(fd);
		return;
	}

	//say("[%d]%s\n", fd, (void*)str);
	for(j=0;j<=max;j++)
	{
		if(known[j].fd == fd)
		{
			known[j].type = 0;
			break;
		}

		if(known[j].fd < fd)
		{
			//右移
			movsb(&known[j+1], &known[j], (max-j)*sizeof(struct node));
			max++;

			//known[j].time =
			known[j].fd = fd;
			known[j].type = 0;
			//known[j].detail =
			peername(fd, known[j].addr);

			//
			net_print();
			break;
		}
	}
}
void net_read(u64 temp)
{
	int count;
	int index;

	//读
	count = readserver(temp, datahome, 0, 0x100000);
	if(count <= 0)
	{
		net_delete(temp);
		stopserver(temp);
		return;
	}
	datahome[count] = 0;

	//找
	for(index=0;index<max;index++)
	{
		if(temp == known[index].fd)break;
	}
	if(index >= max)return;

//--------------------------------------------------------
/*
server:bit31=0, client:bit31=1
{
	00000000	default, chat
	00000100	http
	000002??	ws
	000003??	https
	000004??	wss
	000005??	ssh
	000006??	socks?
	000007??	rdp
	000008??	vnc
}
*/
//--------------------------------------------------------

	//first
	temp = known[index].type;
	if(temp == 0)
	{
		//
		serve_first(&known[index], datahome, count);

		//
		temp = known[index].type;
		if(temp == 0)goto forceclose;
	}

	//chat
	if(temp <= 0xff)
	{
		serve_chat(&known[index], datahome, count);
	}

	//http
	else if(temp <= 0x1ff)
	{
		serve_http(&known[index], datahome, count);
	}

	//ws
	else if(temp <= 0x2ff)
	{
		serve_ws(&known[index], datahome, count);
	}

	//https
	else if(temp <= 0x3ff)
	{
		serve_https(&known[index], datahome, count);
	}

	//wss
	else if(temp <= 0x4ff)
	{
		serve_wss(&known[index], datahome, count);
	}

	//ssh
	else if(temp <= 0x5ff)
	{
		serve_secureshell(&known[index], datahome, count);
	}

	//socks
	else if(temp <= 0x6ff)
	{
		//serve_socks(&known[index], datahome, count);
	}

	//rdp
	else if(temp <= 0x7ff)
	{
		//serve_rdp(&known[index], datahome, count);
	}

	//vnc
	else if(temp <= 0x8ff)
	{
		//serve_vnc(&known[index], datahome, count);
	}

	//error
	else goto forceclose;

	//最后判断关不关fd
	if(known[index].type != 0)return;

forceclose:
	net_delete(known[index].fd);
	stopserver(known[index].fd);
	return;
}
void net_write()
{
}
void net_list()
{
}
void net_choose()
{
}








void network_explain(u64* p)
{
	int type = p[1] & 0xffff;
	u64 fd = p[2];

	if(type == 0x406e) net_read(fd);
	else if(type == 0x2b6e) net_start(fd);
	else if(type == 0x2d6e) net_stop(fd);

	writeserver(p[2], 0, 0, 0);
}