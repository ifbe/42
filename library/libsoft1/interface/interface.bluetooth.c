#define QWORD unsigned long long




static void bt_list()
{
}
static void bt_switch()
{
}
static void bt_read()
{
}
static void bt_write()
{
}
static void bt_start(QWORD type,char* p)
{
}
static void bt_stop()
{
}
void bt_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x7462;		//id
	p[2]=(QWORD)bt_start;
	p[3]=(QWORD)bt_stop;
	p[4]=(QWORD)bt_list;
	p[5]=(QWORD)bt_switch;
	p[6]=(QWORD)bt_read;
	p[7]=(QWORD)bt_write;
}
void bt_kill()
{
	bt_stop();
}
