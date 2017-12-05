#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
u64 startthread(void*, void*);
void stopthread();
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




struct uartinfo
{
	char* buf;
	int len;
	int enq;
	int deq;
};
static struct uartinfo info;
static u64 thread;
static int master;




void systemshell_process(char* p)
{
}
void* systemshell_thread(void* p)
{
}




int systemshell_read(char* buf, int len)
{
	return 0;
}
int systemshell_write(char* buf, int len)
{
	return 0;
}
int systemshell_list(char* p)
{
	return 0;
}
int systemshell_choose(char* p)
{
	return 0;
}




int systemshell_stop()
{
	return 0;
}
int systemshell_start(char* p)
{
	return 0;
}
void systemshell_create()
{
}
void systemshell_delete()
{
}
