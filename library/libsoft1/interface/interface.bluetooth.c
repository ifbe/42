#define u64 unsigned long long




static void bt_list()
{
}
static void bt_choose()
{
}
static void bt_read()
{
}
static void bt_write()
{
}
static void bt_start(u64 type,char* p)
{
}
static void bt_stop()
{
}
void bt_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x7462;		//id

	p[10]=(u64)bt_start;
	p[11]=(u64)bt_stop;
	p[12]=(u64)bt_list;
	p[13]=(u64)bt_choose;
	p[14]=(u64)bt_read;
	p[15]=(u64)bt_write;
}
void bt_delete()
{
	bt_stop();
}
