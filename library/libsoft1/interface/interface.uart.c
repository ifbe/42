#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
int systemuart_list();
int systemuart_choose();
int systemuart_read();
int systemuart_write(char*);
int systemuart_start(char*);
int systemuart_stop();
void say(char*,...);




static int uart_list()
{
	systemuart_list();
}
static int uart_choose()
{
}
static int uart_read()
{
	systemuart_read();
}
static int uart_write(char* p)
{
	systemuart_write(p);
}
static int uart_start(char* p)
{
	systemuart_start(p);
}
static int uart_stop()
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
	uart_stop();
}
