#define QWORD unsigned long long




static void quic_list()
{
}
static void quic_choose()
{
}
static void quic_read()
{
}
static void quic_write()
{
}
static void quic_start(QWORD type,char* p)
{
}
static void quic_stop()
{
}
void quic_init(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x63697571;	//id
	p[2]=(QWORD)quic_start;
	p[3]=(QWORD)quic_stop;
	p[4]=(QWORD)quic_list;
	p[5]=(QWORD)quic_choose;
	p[6]=(QWORD)quic_read;
	p[7]=(QWORD)quic_write;
}
void quic_kill()
{
	quic_stop();
}
