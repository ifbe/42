#define QWORD unsigned long long




static void uart_list()
{
}
static void uart_switch()
{
}
static void uart_read()
{
}
static void uart_write()
{
}
static void uart_start(QWORD type,char* p)
{
}
static void uart_stop()
{
}
void uart_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x74726175;	//id
	p[2]=(QWORD)uart_start;
	p[3]=(QWORD)uart_stop;
	p[4]=(QWORD)uart_list;
	p[5]=(QWORD)uart_switch;
	p[6]=(QWORD)uart_read;
	p[7]=(QWORD)uart_write;
}
void uart_kill()
{
	uart_stop();
}
