#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
int systemuart_list();
int systemuart_choose(u8* p, int);
int systemuart_read();
int systemuart_write(u8*, int);
int systemuart_start(u8*);
int systemuart_stop();
//
int decstr2data(u8*, u64*);
void say(char*, ...);




//
static u64* worker = 0;




static int uart_list(u8* p)
{
	return systemuart_list();
}
static int uart_choose(u8* p)
{
	int j;
	u64 speed = 115200;
	u8 name[64];

	if(p==0)
	{
		systemuart_choose(0,0);
		return 0;
	}

	for(j=0;j<64;j++)
	{
		if(p[j] == 0)break;
		if(p[j] == ',')
		{
			decstr2data(p+j+1, &speed);
			break;
		}
		else name[j] = p[j];
	}
	name[j] = 0;

	j = systemuart_choose(name, speed);
	worker[0] = 1;
	return 0;
}
static int uart_read(u8* p)
{
	systemuart_read();
	return 0;
}
static int uart_write(u8* buf)
{
	int len;
	for(len=0;len<256;len++)
	{
		if(buf[len] == 0)break;
	}
	if(len <= 0)return 0;
	if(len >= 256)return 0;

	return systemuart_write(buf, len);
}
static int uart_start(u8* p)
{
	return systemuart_start(p);
}
static int uart_stop(u8* p)
{
	return systemuart_stop();
}
void uart_create(void* world,u64* p)
{
	worker = world+0x100000;

	p[0]=0x6563616669;	//type
	p[1]=0x74726175;	//id
	p[2]=(u64)uart_start;
	p[3]=(u64)uart_stop;
	p[4]=(u64)uart_list;
	p[5]=(u64)uart_choose;
	p[6]=(u64)uart_read;
	p[7]=(u64)uart_write;
}
void uart_delete()
{
	uart_stop(0);
}
