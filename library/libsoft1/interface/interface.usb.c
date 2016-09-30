#define u64 unsigned long long
void systemusb_list();




static void usb_list()
{
	systemusb_list();
}
static void usb_choose()
{
}
static void usb_read()
{
}
static void usb_write()
{
}
static void usb_start(u64 type,char* p)
{
}
static void usb_stop()
{
}
void usb_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x627375;		//id

	p[10]=(u64)usb_start;
	p[11]=(u64)usb_stop;
	p[12]=(u64)usb_list;
	p[13]=(u64)usb_choose;
	p[14]=(u64)usb_read;
	p[15]=(u64)usb_write;
}
void usb_delete()
{
	usb_stop();
}
