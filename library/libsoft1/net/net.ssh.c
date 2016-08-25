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
	//
	p[0]=0x74656e;		//type
	p[1]=0x687373;		//id
	p[2]=(u64)ssh_start;
	p[3]=(u64)ssh_stop;
	p[4]=(u64)ssh_list;
	p[5]=(u64)ssh_choose;
	p[6]=(u64)ssh_read;
	p[7]=(u64)ssh_write;
}
void ssh_delete()
{
	ssh_stop();
}
