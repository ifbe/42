#define u64 unsigned long long 
//
int createsocket();
int deletesocket();
int startsocket(char*,int);
int stopsocket();
int readsocket(char*,int);
int writesocket(char*,int);
int listsocket(char*);
int choosesocket(char*);
//
void buf2addrport(unsigned char* p,int max,unsigned char* addr,int* port);
void printmemory(char*,int);
void say(char*,...);




//
static unsigned char* datahome=0;




static void tcp_list(char* p)
{
	listsocket(p);
}
static void tcp_choose(char* p)
{
	choosesocket(p);
}
static void tcp_read()
{
	int ret;
	ret=readsocket(datahome,0x1000);
	printmemory(datahome,ret);
}
static void tcp_write(char* p)
{
	int i=0;
	if(p==0)return;

	while(p[i]!=0)i++;
	writesocket(p,i);
}




static int tcp_start(u64 type,char* p)
{
	unsigned char addr[16];
	int port;

	//
	buf2addrport(p,64,addr,&port);
//say("addr=%s,port=%d\n",addr,port);

	//
	if(addr[0]!=0)startsocket(addr,port);
	else startsocket("0.0.0.0",-port);
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
	p[2]=(u64)tcp_start;
	p[3]=(u64)tcp_stop;
	p[4]=(u64)tcp_list;
	p[5]=(u64)tcp_choose;
	p[6]=(u64)tcp_read;
	p[7]=(u64)tcp_write;
}
int tcp_delete()
{
	tcp_stop();
}
