#define QWORD unsigned long long




static void usb_list()
{
}
static void usb_switch()
{
}
static void usb_read()
{
}
static void usb_write()
{
}
static void usb_start(QWORD type,char* p)
{
}
static void usb_stop()
{
}
void usb_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x627375;		//id
	p[2]=(QWORD)usb_start;
	p[3]=(QWORD)usb_stop;
	p[4]=(QWORD)usb_list;
	p[5]=(QWORD)usb_switch;
	p[6]=(QWORD)usb_read;
	p[7]=(QWORD)usb_write;
}
void usb_kill()
{
	usb_stop();
}
