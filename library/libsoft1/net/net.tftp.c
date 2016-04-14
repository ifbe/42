#define QWORD unsigned long long




static void tftp_list()
{
}
static void tftp_switch()
{
}
static void tftp_read()
{
}
static void tftp_write()
{
}
static void tftp_start(QWORD type,char* p)
{
}
static void tftp_stop()
{
}
void tftp_init(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x70746674;	//id
	p[2]=(QWORD)tftp_start;
	p[3]=(QWORD)tftp_stop;
	p[4]=(QWORD)tftp_list;
	p[5]=(QWORD)tftp_switch;
	p[6]=(QWORD)tftp_read;
	p[7]=(QWORD)tftp_write;
}
void tftp_kill()
{
	tftp_stop();
}
