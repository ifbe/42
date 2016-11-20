#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int diary(char*, int, char*, ...);
void say(char*, ...);




//why,what,where,when
int serve_chat(u64* p, char* buf, int len)
{
	say("[%llx]%s",p[2], buf);
	return 1;
}
