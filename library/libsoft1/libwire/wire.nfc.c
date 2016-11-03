#define u64 unsigned long long




static void nfc_list()
{
}
static void nfc_choose()
{
}
static void nfc_read()
{
}
static void nfc_write()
{
}
static void nfc_start(u64 type,char* p)
{
}
static void nfc_stop()
{
}
void nfc_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x63666e;		//id

	p[10]=(u64)nfc_start;
	p[11]=(u64)nfc_stop;
	p[12]=(u64)nfc_list;
	p[13]=(u64)nfc_choose;
	p[14]=(u64)nfc_read;
	p[15]=(u64)nfc_write;
}
void nfc_delete()
{
	nfc_stop();
}
