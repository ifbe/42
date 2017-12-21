#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




void chemequa_list()
{
}
void chemequa_choose()
{
}
void chemequa_read()
{
}
void chemequa_write()
{
}
void chemequa_start()
{
}
void chemequa_stop()
{
}
void chemequa_create(void* base, u64* p)
{
	p[0]=0x6d656863;
	p[1]=0x61757165;
	p[2]=(u64)chemequa_start;
	p[3]=(u64)chemequa_stop;
	p[4]=(u64)chemequa_list;
	p[5]=(u64)chemequa_choose;
	p[6]=(u64)chemequa_read;
	p[7]=(u64)chemequa_write;
}
void chemequa_delete()
{
}
