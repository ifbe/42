#define u64 unsigned long long




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
static void eth_start(u64 type,char* p)
{
}
static void eth_stop()
{
}
void eth_create(char* world,u64* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x687465;		//id
	p[2]=(u64)eth_start;
	p[3]=(u64)eth_stop;
	p[4]=(u64)eth_list;
	p[5]=(u64)eth_choose;
	p[6]=(u64)eth_read;
	p[7]=(u64)eth_write;
}
void eth_delete()
{
	eth_stop();
}
