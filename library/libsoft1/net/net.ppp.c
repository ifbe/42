#define QWORD unsigned long long




static void ppp_list()
{
}
static void ppp_switch()
{
}
static void ppp_read()
{
}
static void ppp_write()
{
}
static void ppp_start(QWORD type,char* p)
{
}
static void ppp_stop()
{
}
void ppp_init(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x707070;		//id
	p[2]=(QWORD)ppp_start;
	p[3]=(QWORD)ppp_stop;
	p[4]=(QWORD)ppp_list;
	p[5]=(QWORD)ppp_switch;
	p[6]=(QWORD)ppp_read;
	p[7]=(QWORD)ppp_write;
}
void ppp_kill()
{
	ppp_stop();
}
