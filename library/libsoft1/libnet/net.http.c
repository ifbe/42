#define u64 unsigned long long




static void http_list()
{
}
static void http_choose()
{
}
static void http_read()
{
}
static void http_write()
{
}
static void http_start(u64 type,char* p)
{
}
static void http_stop()
{
}
void http_create(char* world,u64* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x70747468;	//id

	p[10]=(u64)http_start;
	p[11]=(u64)http_stop;
	p[12]=(u64)http_list;
	p[13]=(u64)http_choose;
	p[14]=(u64)http_read;
	p[15]=(u64)http_write;
}
void http_delete()
{
	http_stop();
}
