#include "libhard.h"
#define usbperi_print(fmt, ...) say("%08lld usbperi@%p "fmt, timeread_us(), usb, ##__VA_ARGS__)




int usbperi_reply_getdesc()
{
	return 0;
}
int usbperi_reply_setfeature()
{
	return 0;
}




int usbperi_reply_control()
{
	return 0;
}
int usbperi_reply_interrupt()
{
	return 0;
}
int usbperi_reply_bulk()
{
	return 0;
}
int usbperi_reply_isochronous()
{
	return 0;
}




int usbperi_create(struct item* usb)
{
	return 0;
}
int usbperi_delete(struct item* usb)
{
	return 0;
}
int usbperi_reader(struct item* usb,int xxx, p64 arg,int cmd, void* rbuf,int rlen)
{
    return 0;
}
int usbperi_writer(struct item* usb,int xxx, p64 arg,int cmd, void* rbuf,int rlen)
{
    return 0;
}




int usbperi_linkup(struct item* usb, int xxx, struct item* xhci, int slot)
{
	usbperi_print("@usblinkup: %p,%x,%p,%x\n",usb,xxx,xhci,slot);

	return 0;
}
int usbperi_discon()
{
	return 0;
}
int usbperi_takeby(struct item* usb,int xxx, struct item* xhci,int endp, p64 arg,int cmd, void* rbuf,int rlen)
{
    return 0;
}
int usbperi_giveby(struct item* usb,int xxx, struct item* xhci,int endp, p64 arg,int cmd, void* rbuf,int rlen)
{
    return 0;
}