#define QWORD unsigned long long




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
void code_create(char* world,QWORD* p)
{
	//
	p[0]=0x6d656d;
	p[1]=0x65646f63;
	p[2]=(QWORD)code_start;
	p[3]=(QWORD)code_stop;
	p[4]=(QWORD)code_list;
	p[5]=(QWORD)code_change;
	p[6]=(QWORD)code_read;
	p[7]=(QWORD)code_write;
}
void code_delete()
{
}

