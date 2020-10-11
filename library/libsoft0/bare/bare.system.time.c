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
void say(void*, ...);




static u64 date = 0;
static u64 time = 0;




void datewrite(u64 x)
{
	date = x;
	//eventwrite(x, __date__, 0, 0);
/*	char* p = (void*)&x;
	say("%02d:%02d:%02d:%02d:%02d:%02d:%02d:%02d\n",
		p[7],p[6],p[5],p[4],p[3],p[2],p[1],p[0]
	);*/
}
u64 dateread()
{
	return date;
}




void sleep_us(int t)
{
}
void sleep_ms(int t)
{
}
void timewrite(u64 t)
{
	time = t;
}
u64 timeread()
{
	return time;
}
