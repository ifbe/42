#define QWORD unsigned long long




static void zigbee_list()
{
}
static void zigbee_choose()
{
}
static void zigbee_read()
{
}
static void zigbee_write()
{
}
static void zigbee_start(QWORD type,char* p)
{
}
static void zigbee_stop()
{
}
void zigbee_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x65656267697a;	//id
	p[2]=(QWORD)zigbee_start;
	p[3]=(QWORD)zigbee_stop;
	p[4]=(QWORD)zigbee_list;
	p[5]=(QWORD)zigbee_choose;
	p[6]=(QWORD)zigbee_read;
	p[7]=(QWORD)zigbee_write;
}
void zigbee_kill()
{
	zigbee_stop();
}
