#define u64 unsigned long long




static void slip_list()
{
}
static void slip_choose()
{
}
static void slip_read()
{
}
static void slip_write()
{
}
static void slip_start(u64 type,char* p)
{
}
static void slip_stop()
{
}
void slip_create(char* world,u64* p)
{
/*
	p[0]=0x74656e;		//type
	p[1]=0x70696c73;	//id

	p[10]=(u64)slip_start;
	p[11]=(u64)slip_stop;
	p[12]=(u64)slip_list;
	p[13]=(u64)slip_choose;
	p[14]=(u64)slip_read;
	p[15]=(u64)slip_write;
*/
}
void slip_delete()
{
	slip_stop();
}
