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
void usb_create(void* w)
{
}
void usb_delete()
{
	usb_stop();
}
