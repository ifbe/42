#define u64 unsigned long long
void systemwifi_list(int);




static int stage=0;
static void wifi_list(char* p)
{
	systemwifi_list(stage);
}
static void wifi_choose(char* p)
{
	if(p == 0)return;
	else if( (p[0]=='.') && (p[1]=='.') )stage--;
	else stage = 1;
}
static void wifi_read()
{
}
static void wifi_write()
{
}
static void wifi_start(u64 type,char* p)
{
}
static void wifi_stop()
{
}
void wifi_create(void* w)
{
}
void wifi_delete()
{
	wifi_stop();
}
