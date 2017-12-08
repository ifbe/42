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




void systemshell_process(char* p)
{
}
int readshell(int fd, char* buf, int off, int len)
{
	return 0;
}
int writeshell(int fd, char* buf, int off, int len)
{
	return 0;
}
int listshell(char* p)
{
	return 0;
}
int chooseshell(char* p)
{
	return 0;
}
int stopshell()
{
	return 0;
}
int startshell(char* p)
{
	return 0;
}
void deleteshell()
{
}
void createshell(void* addr)
{
}
