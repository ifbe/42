#define QWORD unsigned long long




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
static void nfc_start(QWORD type,char* p)
{
}
static void nfc_stop()
{
}
void nfc_create(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x63666e;		//id
	p[2]=(QWORD)nfc_start;
	p[3]=(QWORD)nfc_stop;
	p[4]=(QWORD)nfc_list;
	p[5]=(QWORD)nfc_choose;
	p[6]=(QWORD)nfc_read;
	p[7]=(QWORD)nfc_write;
}
void nfc_delete()
{
	nfc_stop();
}
