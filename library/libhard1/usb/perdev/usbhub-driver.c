#include "libhard.h"
#include "drv-usb.h"
#define usbhub_print(fmt, ...) say("<%08lld,usbhub>" fmt, timeread_us(), ##__VA_ARGS__)
//
#define HUB_FEATURE_PORTPOWER	8
#define HUB_FEATURE_PORTCNCTN	0x10
#define HUB_FEATURE_RESET		4
#define HUB_FEATURE_CRESET		0x14
//
#define HUB_STATUS_CONNECTION	1
#define HUB_STATUS_ENABLE		(1<<1)
#define HUB_STATUS_LS			(1<<8)
#define HUB_STATUS_CNCTSTSCHG	(1<<24)
#define HUB_STATUS_PECHANGE		(1<<25)
#define HUB_STATUS_RESET		(1<<28)
#define bmHUB_PORT_STATUS_PORT_CONNECTION       0x0001
#define bmHUB_PORT_STATUS_PORT_ENABLE           0x0002
#define bmHUB_PORT_STATUS_PORT_SUSPEND          0x0004
#define bmHUB_PORT_STATUS_PORT_OVER_CURRENT     0x0008
#define bmHUB_PORT_STATUS_PORT_RESET            0x0010
#define bmHUB_PORT_STATUS_PORT_POWER            0x0100
#define bmHUB_PORT_STATUS_PORT_LOW_SPEED        0x0200
#define bmHUB_PORT_STATUS_PORT_HIGH_SPEED       0x0400
#define bmHUB_PORT_STATUS_PORT_TEST             0x0800
#define bmHUB_PORT_STATUS_PORT_INDICATOR        0x1000
//
int usbdesc_addr2offs(struct perusb* perusb, void* desc);
void* usbdesc_offs2addr(struct perusb* perusb, int offs);
//
void DEVICE_REQUEST_SET_CONFIGURATION(void* req, u16 conf);
//
void GET_DESC_HUB(struct UsbRequest* req, u16 type, u16 len)
{
	req->bmRequestType = 0xa0;  //USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE
	req->bRequest = GET_DESCRIPTOR;
	req->wValue = type;
	req->wIndex = 0;
	req->wLength = len;
}
void USB_HUB_PORT_POWERON(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = SET_FEATURE;
	req->wValue = HUB_FEATURE_PORTPOWER;
	req->wIndex = countfrom1;
	req->wLength = 0;
}
void USB_HUB_PORT_CLEARCNCTN(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = CLEAR_FEATURE;
	req->wValue = HUB_FEATURE_PORTCNCTN;
	req->wIndex = countfrom1;
	req->wLength = 0;
}
void USB_HUB_PORT_GETSTATUS(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0xa3;
	req->bRequest = GET_STATUS;
	req->wValue = 0;
	req->wIndex = countfrom1;
	req->wLength = 4;
}
void USB_HUB_PORT_RESET(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = SET_FEATURE;
	req->wValue = HUB_FEATURE_RESET;
	req->wIndex = countfrom1;
	req->wLength = 0;
}
void USB_HUB_PORT_CLEARRESET(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = CLEAR_FEATURE;
	req->wValue = HUB_FEATURE_CRESET;
	req->wIndex = countfrom1;
	req->wLength = 0;
}





struct perfunc{		//0x10000 byte per func
	void* hostnode;
	int hostslot;

	int portcount;
	u32 portstat[16];

	u8 buf[0];
};




int usbhub_enumone(struct item* usb, int id)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	struct item* xhci = perfunc->hostnode;
	int slot = perfunc->hostslot;

	int ret;
	struct UsbRequest req;

	usbhub_print("%d:get stat\n", id);
	USB_HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);


	usbhub_print("%d:power on\n", id);
	USB_HUB_PORT_POWERON(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;

	//power stable
	sleep_ms(100);


	usbhub_print("%d:get stat\n", id);
	USB_HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);


	usbhub_print("%d:clear cnctn\n", id);
	USB_HUB_PORT_CLEARCNCTN(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;


	usbhub_print("%d:get stat\n", id);
	USB_HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);

	if(0 == (perfunc->portstat[id]&HUB_STATUS_CONNECTION))return 0;


	usbhub_print("%d:reset\n", id);
	USB_HUB_PORT_RESET(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;

	//reset wait
	sleep_ms(50);


	usbhub_print("%d:get stat\n", id);
	USB_HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);
	if(0 == (perfunc->portstat[id]&HUB_STATUS_CONNECTION))return 0;


	usbhub_print("%d:clear reset\n", id);
	USB_HUB_PORT_CLEARRESET(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;


	usbhub_print("%d:get stat\n", id);
	USB_HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);


	usbhub_print("%d:notify xhci enable device\n", id);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		0,_tohc_new_,
		0,id+1
	);
	if(ret < 0)return -9;
/*
	struct item* ccc = device_create(_usb_, 0, 0, 0);
	if(ccc)usbany_linkup(ccc, 0, xhci, slot);
*/
	return 0;
}
int usbhub_enumall(struct item* usb)
{
	usbhub_print("usbhub_enumall\n");
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	int j;
	for(j=0;j<perfunc->portcount;j++){
		usbhub_enumone(usb, j);
		say("......\n");
	}
	return 0;
}
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

	//set config
	int ret;
	struct UsbRequest req;
	usbhub_print("set_config\n");
	DEVICE_REQUEST_SET_CONFIGURATION(&req, confdesc->bConfigurationValue);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;

	//hub desc
	usbhub_print("get_hubdesc\n");
	struct HubDescriptor* hubdesc = (void*)perfunc->buf;
	GET_DESC_HUB(&req, (0x29<<8) | 0, 8);
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

	//notify xhci its hub, and send hub desc
	usbhub_print("notify ishub\n");
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		0,_tohc_hub_,
		hubdesc,0
	);
	if(ret < 0)return -9;

	perfunc->hostnode = xhci;
	perfunc->hostslot = slot;
	perfunc->portcount = hubdesc->bNbrPorts;
	usbhub_enumall(usb);
	return 0;
}