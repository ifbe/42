#define u64 unsigned long long
int systembt_list(char*);




static int bt_list(char* p)
{
	return systembt_list(p);
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
void bt_create(char* w)
{
}
void bt_delete()
{
	bt_stop();
}
