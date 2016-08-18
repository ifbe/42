#define QWORD unsigned long long




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
static void slip_start(QWORD type,char* p)
{
}
static void slip_stop()
{
}
void slip_create(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x70696c73;	//id
	p[2]=(QWORD)slip_start;
	p[3]=(QWORD)slip_stop;
	p[4]=(QWORD)slip_list;
	p[5]=(QWORD)slip_choose;
	p[6]=(QWORD)slip_read;
	p[7]=(QWORD)slip_write;
}
void slip_delete()
{
	slip_stop();
}
