#define QWORD unsigned long long




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
static void http_start(QWORD type,char* p)
{
}
static void http_stop()
{
}
void http_init(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x70747468;	//id
	p[2]=(QWORD)http_start;
	p[3]=(QWORD)http_stop;
	p[4]=(QWORD)http_list;
	p[5]=(QWORD)http_choose;
	p[6]=(QWORD)http_read;
	p[7]=(QWORD)http_write;
}
void http_kill()
{
	http_stop();
}
