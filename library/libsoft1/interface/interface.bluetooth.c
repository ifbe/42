#define u64 unsigned long long
void systembt_list();
void systembt_choose();
void systembt_read();
void systembt_write();




static void bt_list()
{
	systembt_list();
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
	p[2]=(u64)bt_start;
	p[3]=(u64)bt_stop;
	p[4]=(u64)bt_list;
	p[5]=(u64)bt_choose;
	p[6]=(u64)bt_read;
	p[7]=(u64)bt_write;
}
void bt_delete()
{
	bt_stop();
}
