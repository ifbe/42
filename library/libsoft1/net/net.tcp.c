#define QWORD unsigned long long 
//
int initsocket();
int killsocket();
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




static int tcp_start(QWORD type,char* p)
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
int tcp_init(char* world,unsigned long long* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x74656e;
	p[1]=0x706374;
	p[2]=(QWORD)tcp_start;
	p[3]=(QWORD)tcp_stop;
	p[4]=(QWORD)tcp_list;
	p[5]=(QWORD)tcp_choose;
	p[6]=(QWORD)tcp_read;
	p[7]=(QWORD)tcp_write;
}
int tcp_kill()
{
	tcp_stop();
}
