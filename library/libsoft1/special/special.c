#define QWORD unsigned long long




static void special_list()
{
}
static void special_switch()
{
}
static void special_read()
{
}
static void special_write()
{
}
static void special_start(QWORD type,char* p)
{
}
static void special_stop()
{
}
void special_init(char* world,QWORD* p)
{
	//
	p[0]=0;			//type
	p[1]=0x6c616963657073;	//id
	p[2]=(QWORD)special_start;
	p[3]=(QWORD)special_stop;
	p[4]=(QWORD)special_list;
	p[5]=(QWORD)special_switch;
	p[6]=(QWORD)special_read;
	p[7]=(QWORD)special_write;
}
void special_kill()
{
	special_stop();
}
