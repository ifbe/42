#include "libhard.h"
#include "drv-usb.h"
#define usbhub_print(fmt, ...) say("<%08lld,usbhub>" fmt, timeread_us(), ##__VA_ARGS__)
int usbdesc_addr2offs(struct perusb* perusb, void* desc);
void* usbdesc_offs2addr(struct perusb* perusb, int offs);
//
void GET_DESC_HUB(struct UsbRequest* req, u16 type, u16 len)
{
	req->bmRequestType = 0xa0;  //USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE
	req->bRequest = 6;
	req->wValue = type;
	req->wIndex = 0;
	req->wLength = len;
}





struct perfunc{		//0x10000 byte per func
	u8 buf[0];
};




int usbhub_driver(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
{
	usbhub_print("@usbhub_driver\n");
	//per device
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	usbhub_print("pid=%x,vid=%x\n", devdesc->idVendor, devdesc->idProduct);

	struct descnode* confnode = &perusb->parsed.node[0];
	struct ConfigurationDescriptor* confdesc = usbdesc_offs2addr(perusb, confnode->real);
	usbhub_print("bmAttributes=%x,bMaxPower=%x\n", confdesc->bmAttributes, confdesc->bMaxPower);

	//interface
	usbhub_print("bInterfaceClass=%x\n", intfdesc->bInterfaceClass);

	struct UsbRequest req;
	GET_DESC_HUB(&req, (0x29<<8) | 0, 8);

	int ret;
	struct HubDescriptor* hubdesc = (void*)perfunc->buf;
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		hubdesc,8
	);
	say("bDescLength=%x\n",         hubdesc->bDescLength);
	say("bDescriptorType=%x\n",     hubdesc->bDescriptorType);
	say("bNbrPorts=%x\n",           hubdesc->bNbrPorts);
	say("wHubCharacteristics=%x\n", hubdesc->wHubCharacteristics);
	say("  LogPwrSwitchMode=%x\n",        hubdesc->character.LogPwrSwitchMode);
	say("  CompoundDevice=%x\n",          hubdesc->character.CompoundDevice);
	say("  OverCurrentProtectMode=%x\n",  hubdesc->character.OverCurrentProtectMode);
	say("  TTThinkTime=%x\n",             hubdesc->character.TTThinkTime);
	say("  PortIndicatorsSupported=%x\n", hubdesc->character.PortIndicatorsSupported);
	say("bPwrOn2PwrGood=%x\n",      hubdesc->bPwrOn2PwrGood);
	say("bHubContrCurrent=%x\n",    hubdesc->bHubContrCurrent);
	return 0;
}