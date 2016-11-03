#define u64 unsigned long long




static void ppp_list()
{
}
static void ppp_choose()
{
}
static void ppp_read()
{
}
static void ppp_write()
{
}
static void ppp_start(u64 type,char* p)
{
}
static void ppp_stop()
{
}
void ppp_create(char* world,u64* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x707070;		//id

	p[10]=(u64)ppp_start;
	p[11]=(u64)ppp_stop;
	p[12]=(u64)ppp_list;
	p[13]=(u64)ppp_choose;
	p[14]=(u64)ppp_read;
	p[15]=(u64)ppp_write;
}
void ppp_delete()
{
	ppp_stop();
}
