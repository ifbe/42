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
void wifi_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x69666977;	//id

	p[10]=(u64)wifi_start;
	p[11]=(u64)wifi_stop;
	p[12]=(u64)wifi_list;
	p[13]=(u64)wifi_choose;
	p[14]=(u64)wifi_read;
	p[15]=(u64)wifi_write;
}
void wifi_delete()
{
	wifi_stop();
}
