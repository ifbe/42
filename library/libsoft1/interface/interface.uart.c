#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
int systemuart_list();
int systemuart_choose(char* p, int);
int systemuart_read();
int systemuart_write(char*, int);
int systemuart_start(char*);
int systemuart_stop();
//
int decstr2data(u8*, u64*);
void say(char*, ...);




static int uart_list(char* p)
{
	systemuart_list();
}
static int uart_choose(char* p)
{
	int j;
	u64 speed = 115200;
	char name[64];

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
	systemuart_choose(name, speed);
}
static int uart_read(char* p)
{
	systemuart_read();
}
static int uart_write(char* buf)
{
	int len;
	for(len=0;len<256;len++)
	{
		if(buf[len] == 0)break;
	}
	if(len <= 0)return 0;
	if(len >= 256)return 0;

	systemuart_write(buf, len);
}
static int uart_start(char* p)
{
	systemuart_start(p);
}
static int uart_stop(char* p)
{
	systemuart_stop();
}
void uart_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x74726175;	//id

	p[10]=(u64)uart_start;
	p[11]=(u64)uart_stop;
	p[12]=(u64)uart_list;
	p[13]=(u64)uart_choose;
	p[14]=(u64)uart_read;
	p[15]=(u64)uart_write;
}
void uart_delete()
{
	uart_stop(0);
}
