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
	p[2]=(u64)usb_start;
	p[3]=(u64)usb_stop;
	p[4]=(u64)usb_list;
	p[5]=(u64)usb_choose;
	p[6]=(u64)usb_read;
	p[7]=(u64)usb_write;
}
void usb_delete()
{
	usb_stop();
}
