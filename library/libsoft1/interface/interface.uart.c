#define QWORD unsigned long long
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
static int uart_start(QWORD type,char* p)
{
	systemuart_start(p);
}
static int uart_stop()
{
	systemuart_stop();
}
void uart_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x74726175;	//id
	p[2]=(QWORD)uart_start;
	p[3]=(QWORD)uart_stop;
	p[4]=(QWORD)uart_list;
	p[5]=(QWORD)uart_choose;
	p[6]=(QWORD)uart_read;
	p[7]=(QWORD)uart_write;
}
void uart_kill()
{
	uart_stop();
}
