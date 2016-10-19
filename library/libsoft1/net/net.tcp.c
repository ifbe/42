#define u64 unsigned long long 
//
int createsocket();
int deletesocket();
int startsocket(char*,int);
int stopsocket();
int readsocket(char*,int);
int writesocket(char*,int);
int listsocket(char*);
int choosesocket(char*,int);
//
void buf2addrport(unsigned char* p,int max,unsigned char* addr,int* port);
void printmemory(char*,int);
void say(char*,...);




//
static unsigned char* datahome=0;




static int tcp_list(char* p)
{
	listsocket(p);
	return 1;
}
static int tcp_choose(char* p)
{
	char addr[64];
	int port;
	int ret;

	buf2addrport(p, 64, addr, &port);
	ret = choosesocket(addr, port);
	return 1;
}
static int tcp_read()
{
}
static int tcp_write(char* p)
{
	int i=0;
	if(p==0)return 0;

	while(p[i]!=0)i++;
	return writesocket(p,i);
}




static int tcp_start(u64 type,char* p)
{
}
static int tcp_stop()
{
	stopsocket();
}
int tcp_create(char* world,unsigned long long* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x74656e;
	p[1]=0x706374;

	p[10]=(u64)tcp_start;
	p[11]=(u64)tcp_stop;
	p[12]=(u64)tcp_list;
	p[13]=(u64)tcp_choose;
	p[14]=(u64)tcp_read;
	p[15]=(u64)tcp_write;
}
int tcp_delete()
{
	tcp_stop();
}
