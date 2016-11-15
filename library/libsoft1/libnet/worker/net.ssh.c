#define u64 unsigned long long




static void ssh_list()
{
}
static void ssh_choose()
{
}
static void ssh_read()
{
}
static void ssh_write()
{
}
static void ssh_start(u64 type,char* p)
{
}
static void ssh_stop()
{
}
void ssh_create(char* world,u64* p)
{
/*
	p[0]=0x74656e;		//type
	p[1]=0x687373;		//id

	p[10]=(u64)ssh_start;
	p[11]=(u64)ssh_stop;
	p[12]=(u64)ssh_list;
	p[13]=(u64)ssh_choose;
	p[14]=(u64)ssh_read;
	p[15]=(u64)ssh_write;
*/
}
void ssh_delete()
{
	ssh_stop();
}
