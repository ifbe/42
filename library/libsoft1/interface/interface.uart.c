#define u64 unsigned long long
int systemuart_list();
int systemuart_choose();
int systemuart_read();
int systemuart_write(char*,int);
int systemuart_start(char*);
int systemuart_stop();




static int uart_list()
{
	systemuart_list();
}
static int uart_choose()
{
}
static int uart_read()
{
}
static int uart_write(char* p)
{
	//size?
	systemuart_write(p,1);
}
static int uart_start(u64 type,char* p)
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
	p[2]=(u64)uart_start;
	p[3]=(u64)uart_stop;
	p[4]=(u64)uart_list;
	p[5]=(u64)uart_choose;
	p[6]=(u64)uart_read;
	p[7]=(u64)uart_write;
}
void uart_delete()
{
	uart_stop();
}
