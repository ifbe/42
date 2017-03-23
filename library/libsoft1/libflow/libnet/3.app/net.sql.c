#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void printmemory(void*, int);
void say(void*, ...);




#define SQL 0x4c5153
#define sql 0x6c7173
int serve_sql(void* p, int fd, char* buf, int len)
{
	printmemory(buf, len);
	return sql;
}
