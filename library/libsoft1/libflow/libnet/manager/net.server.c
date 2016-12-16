#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int selfname(u64, void*);
int peername(u64, void*);
int createserver();
int deleteserver();
int startserver(void* addr, int port, void* dir, int opt);
int stopserver(u64 x);
int listserver(u8*);
int chooseserver(u8*, u8*, int, u8*);
int readserver(u64 fd, void* addr, u64 offset, u64 count);
int writeserver(u64 fd, void* addr, u64 offset, u64 count);
//
void notify_create(u64* p);
void notify_delete(u64 fd);
int serve_first(void* p, u8* buf, u64 len);
int serve_chat(void* p, u8* buf, u64 len);
int serve_http(void* p, u8* buf, u64 len);
int serve_ws(void* p, u8* buf, u64 len);
int serve_https(void* p, u8* buf, u64 len);
int serve_wss(void* p, u8* buf, u64 len);
int serve_secureshell(void* p, u8* buf, u64 len);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
int movsb(void*,void*,int);
int copy(u8*,u8*);
int ncopy(u8*,u8*,int);
//
void printmemory(void*, int);
void say(void*, ...);




//
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




void known_print()
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
void known_delete(u64 fd)
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
			known_print();
			break;
		}
	}
}
void known_create(u64* p)
{
	int j,k;
	u64 fd = p[2];

	//user max
	if(max >= 0x10000/0x40)
	{
		writeserver(fd, "too many", 0, 8);
		notify_delete(fd);
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
			known_print();
			break;
		}
	}
}
void known_read(u64* p)
{
	int count;
	int index;
	u64 temp;

	//读
	temp = p[2];
	count = readserver(temp, datahome, 0, 0x100000);
	if(count <= 0)
	{
		notify_delete(temp);
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
server:
{
	0000000?	first, chat(serve)
	0000001?	http, ws(serve)
	0000002?	https, wss(serve)
	0000003?	ssh(serve)
	0000004?	socks(serve)
}
client:
{
	8000000?	first, chat(client)
	8000001?	http, ws(client)
	8000002?	http, wss(client)
	8000003?	ssh(client)
	8000004?	socks(client)
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
	if(temp <= 0x0f)
	{
		serve_chat(&known[index], datahome, count);
	}

	//http
	else if(temp == 0x10)
	{
		serve_http(&known[index], datahome, count);
	}

	//ws
	else if(temp <= 0x1f)
	{
		serve_ws(&known[index], datahome, count);
	}

	//https
	else if(temp == 0x20)
	{
		serve_https(&known[index], datahome, count);
	}

	//wss
	else if(temp <= 0x2f)
	{
		serve_wss(&known[index], datahome, count);
	}

	//ssh
	else if(temp <= 0x3f)
	{
		serve_secureshell(&known[index], datahome, count);
	}

	//socks
	else if(temp <= 0x4f)
	{
		//serve_socks(&known[index], datahome, count);
	}

	//error
	else goto forceclose;

	//最后判断关不关fd
	if(known[index].type != 0)return;

forceclose:
	notify_delete(known[index].fd);
	return;
}




void notify_create(u64* p)
{
	int ret = p[1] & 0xffff;

	if(ret == 0x406e) known_read(p);
	else if(ret == 0x2b6e) known_create(p);
	else if(ret == 0x2d6e) known_delete(p[2]);

	writeserver(p[2], 0, 0, 0);
}
void notify_delete(u64 fd)
{
	known_delete(fd);
	stopserver(fd);
}




static int server_read(u64* p)
{
	return 0;
}
static int server_write(u8* p)
{
	return 0;
}
static int server_list(u8* p)
{
	return 0;
}
static int server_choose(u8* p)
{
	return 0;
}
static int server_start(u8* p)
{
	//0.0.0.0:2222/sdcard
	return startserver("0,0,0,0", 2222, "", 0);
}
static int server_stop()
{
	int j;
	for(j=max-1;j>=0;j--)
	{
		notify_delete(known[j].fd);
	}
	return stopserver(0);
}
int server_create(void* softhome, u64* p)
{
	//
	int j;
	char* q;

	//
	p[0]=0x74656e;
	p[1]=0x726576726573;
	p[10]=(u64)server_start;
	p[11]=(u64)server_stop;
	p[12]=(u64)server_list;
	p[13]=(u64)server_choose;
	p[14]=(u64)server_read;
	p[15]=(u64)server_write;

	known = softhome + 0x100000 + 0xf0000;
	dirhome = softhome + 0x200000;
	datahome = softhome + 0x300000;

	q = (void*)known;
	for(j=0;j<0x10000;j++)q[j] = 0;

	return 0;
}
int server_delete()
{
	return 0;
}
