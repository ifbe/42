#define u64 unsigned long long




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
static void zigbee_start(u64 type,char* p)
{
}
static void zigbee_stop()
{
}
void zigbee_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x65656267697a;	//id

	p[10]=(u64)zigbee_start;
	p[11]=(u64)zigbee_stop;
	p[12]=(u64)zigbee_list;
	p[13]=(u64)zigbee_choose;
	p[14]=(u64)zigbee_read;
	p[15]=(u64)zigbee_write;
}
void zigbee_delete()
{
	zigbee_stop();
}
