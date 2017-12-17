#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define __date__ hex32('d','a','t','e')
#define __time__ hex32('t','i','m','e')
void eventwrite(u64,u64,u64,u64);
static u64 date = 0;




void sleep_us(int t)
{
}




void setdate(u64 x)
{
	date = x;
	eventwrite(x, __date__, 0, 0);

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
