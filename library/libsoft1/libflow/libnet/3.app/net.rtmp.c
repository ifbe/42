#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void say(void*, ...);




#define RTMP 0x504d5452
#define rtmp 0x706d7472
u64 check_rtmp(u64 fd, u64 type, u8* buf, int len)
{
	if(buf[0] == 0x3)return RTMP;
	return 0;
}
u64 serve_rtmp(u64 fd, u64 type, u8* buf, int len)
{
	return 0;
}
