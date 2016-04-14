#define QWORD unsigned long long




static void ir_list()
{
}
static void ir_switch()
{
}
static void ir_read()
{
}
static void ir_write()
{
}
static void ir_start(QWORD type,char* p)
{
}
static void ir_stop()
{
}
void ir_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x7269;		//id
	p[2]=(QWORD)ir_start;
	p[3]=(QWORD)ir_stop;
	p[4]=(QWORD)ir_list;
	p[5]=(QWORD)ir_switch;
	p[6]=(QWORD)ir_read;
	p[7]=(QWORD)ir_write;
}
void ir_kill()
{
	ir_stop();
}
