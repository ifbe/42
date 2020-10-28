#include "usb.h"
#define DFU_DETACH 0
#define DFU_DNLOAD 1
#define DFU_UPLOAD 2
#define DFU_GETSTATUS 3
#define DFU_CLRSTATUS 4
#define DFU_GETSTATE 5
#define DFU_ABORT 6




void INTERFACE_REQUEST_DFU_DETACH(struct UsbRequest* req, u16 intf, u16 timeout)
{
	req->bmRequestType = 0x21;
	req->bRequest = DFU_DETACH;
	req->wValue = timeout;
	req->wIndex = intf;
	req->wLength = 0;
}
void INTERFACE_REQUEST_DFU_DNLOAD(struct UsbRequest* req, u16 intf, u16 blocknum, u16 len)
{
	req->bmRequestType = 0x21;
	req->bRequest = DFU_DNLOAD;
	req->wValue = blocknum;
	req->wIndex = intf;
	req->wLength = len;
}
void INTERFACE_REQUEST_DFU_UPLOAD(struct UsbRequest* req, u16 intf, u16 blocknum, u16 len)
{
	req->bmRequestType = 0x81;
	req->bRequest = DFU_UPLOAD;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = len;
}
void INTERFACE_REQUEST_DFU_GETSTATUS(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0x21;
	req->bRequest = DFU_GETSTATUS;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 6;
}
void INTERFACE_REQUEST_DFU_CLRSTATUS(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0x21;
	req->bRequest = DFU_CLRSTATUS;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 0;
}
void INTERFACE_REQUEST_DFU_GETSTATE(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0x21;
	req->bRequest = DFU_GETSTATE;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 1;
}
void INTERFACE_REQUEST_DFU_ABORT(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0x21;
	req->bRequest = DFU_ABORT;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 0;
}