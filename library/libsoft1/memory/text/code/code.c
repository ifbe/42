#define u64 unsigned long long




static void code_list()
{
}
static void code_change()
{
}
static void code_read()
{
}
static void code_write()
{
}
static void code_start()
{
}
static void code_stop()
{
}
void code_create(char* world,u64* p)
{
	//
	p[0]=0x6d656d;
	p[1]=0x65646f63;
	p[2]=(u64)code_start;
	p[3]=(u64)code_stop;
	p[4]=(u64)code_list;
	p[5]=(u64)code_change;
	p[6]=(u64)code_read;
	p[7]=(u64)code_write;
}
void code_delete()
{
}

