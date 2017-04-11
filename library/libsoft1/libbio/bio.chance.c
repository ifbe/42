#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




void chance_list()
{
}
void chance_choose()
{
}
void chance_read()
{
}
void chance_write()
{
}
void chance_start()
{
}
void chance_stop()
{
}
void chance_create(void* base, u64* p)
{
	p[0]=0x6f6962;
	p[1]=0x65636e616863;
	p[2]=(u64)chance_start;
	p[3]=(u64)chance_stop;
	p[4]=(u64)chance_list;
	p[5]=(u64)chance_choose;
	p[6]=(u64)chance_read;
	p[7]=(u64)chance_write;
}
void chance_delete()
{
}
