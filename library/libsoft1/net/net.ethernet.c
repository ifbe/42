#define QWORD unsigned long long




static void eth_list()
{
}
static void eth_choose()
{
}
static void eth_read()
{
}
static void eth_write()
{
}
static void eth_start(QWORD type,char* p)
{
}
static void eth_stop()
{
}
void eth_init(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x687465;		//id
	p[2]=(QWORD)eth_start;
	p[3]=(QWORD)eth_stop;
	p[4]=(QWORD)eth_list;
	p[5]=(QWORD)eth_choose;
	p[6]=(QWORD)eth_read;
	p[7]=(QWORD)eth_write;
}
void eth_kill()
{
	eth_stop();
}
