#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int serve_first(void* p, u8* buf, u64 len);
int serve_chat(void* p, u8* buf, u64 len);
int serve_http(void* p, u8* buf, u64 len);
int serve_ws(void* p, u8* buf, u64 len);
int serve_https(void* p, u8* buf, u64 len);
int serve_wss(void* p, u8* buf, u64 len);
int serve_secureshell(void* p, u8* buf, u64 len);
//
int tls_start();
int tls_stop();
int ssh_start();
int ssh_stop();
int movsb(void*,void*,int);
//
int startserver(void* addr, int port, void* dir, int opt);
int stopserver(u64 x);
int readserver(u64 fd, void* addr, u64 offset, u64 count);
int writeserver(u64 fd, void* addr, u64 offset, u64 count);
//
void printmemory(void*, int);
void say(void*, ...);




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
	tls_start();
	ssh_start();
	return startserver("0,0,0,0", 2222, "", 0);
}
static int server_stop()
{
/*
	int j;
	for(j=max-1;j>=0;j--)
	{
		notify_delete(known[j].fd);
	}
	return stopserver(0);
*/
	return 0;
}
int server_create(void* softhome, u64* p)
{
	p[0]=0x74656e;
	p[1]=0x726576726573;
	p[10]=(u64)server_start;
	p[11]=(u64)server_stop;
	p[12]=(u64)server_list;
	p[13]=(u64)server_choose;
	p[14]=(u64)server_read;
	p[15]=(u64)server_write;

	return 0;
}
int server_delete()
{
	return 0;
}
