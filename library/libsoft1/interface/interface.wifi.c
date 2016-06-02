#define QWORD unsigned long long
void systemwifi_list();




static void wifi_list()
{
	systemwifi_list();
}
static void wifi_choose()
{
}
static void wifi_read()
{
}
static void wifi_write()
{
}
static void wifi_start(QWORD type,char* p)
{
}
static void wifi_stop()
{
}
void wifi_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x69666977;	//id
	p[2]=(QWORD)wifi_start;
	p[3]=(QWORD)wifi_stop;
	p[4]=(QWORD)wifi_list;
	p[5]=(QWORD)wifi_choose;
	p[6]=(QWORD)wifi_read;
	p[7]=(QWORD)wifi_write;
}
void wifi_kill()
{
	wifi_stop();
}
