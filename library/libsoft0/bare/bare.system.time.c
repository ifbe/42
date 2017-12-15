#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
static u64 date = 0;




void sleep_us(int t)
{
}




void setdate(u64 x)
{
	date = x;
}
u64 getdate()
{
	return date;
}




void settime(u64 time)
{
}
u64 gettime()
{
	return 0;
}
