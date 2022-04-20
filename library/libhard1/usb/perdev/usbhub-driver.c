#include "libhard.h"
#include "drv-usb.h"
#define usbhub_print(fmt, ...) say("<%08lld,usbhub>" fmt, timeread_us(), ##__VA_ARGS__)
//
int usbdesc_addr2offs(struct perusb* perusb, void* desc);
void* usbdesc_offs2addr(struct perusb* perusb, int offs);
//
void DEVICE_REQUEST_GET_STATUS(struct UsbRequest* req);
void DEVICE_REQUEST_SET_CONFIGURATION(void* req, u16 conf);
void INTERFACE_REQUEST_GET_STATUS(struct UsbRequest* req, u16 intf);
void INTERFACE_REQUEST_SET_INTERFACE(void* req, u16 intf, u16 alt);




//any hub feature
#define HUB_FEATURE_PORTPOWER	8
#define HUB_FEATURE_PORTCNCTN	0x10
#define HUB_FEATURE_RESET		4
#define HUB_FEATURE_CRESET		0x14
//3.0 hub command
#define SSHUB_SET_DEPTH		12
#define SSHUB_GET_PORT_ERR_COUNT	13


void HUB_GETSTATUS(struct UsbRequest* req, int len)
{
	req->bmRequestType = 0xa0;
	req->bRequest = GET_STATUS;
	req->wValue = 0;
	req->wIndex = 0;
	req->wLength = len;
}
void GET_DESC_HUB(struct UsbRequest* req, u16 type, u16 len)
{
	req->bmRequestType = 0xa0;  //USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE
	req->bRequest = GET_DESCRIPTOR;
	req->wValue = type;
	req->wIndex = 0;
	req->wLength = len;
}
void HUB_SET_DEPTH(struct UsbRequest* req, u16 depth)
{
	req->bmRequestType = 0x23;
	req->bRequest = SSHUB_SET_DEPTH;
	req->wValue = depth;
	req->wIndex = 0;
	req->wLength = 0;
}




//any hub port status(byte0,1)
#define HUB_STATUS_CONNECTION	1
#define HUB_STATUS_ENABLE		(1<<1)


//2.0 hub port status(byte0,1)
#define HSHUB_PORTSTATUS_CONNECTION	 1
#define HSHUB_PORTSTATUS_ENABLED     (1<<1)
#define HSHUB_PORTSTATUS_SUSPEND     (1<<2)
#define HSHUB_PORTSTATUS_OVERCURRENT (1<<3)
#define HSHUB_PORTSTATUS_RESETSIGNAL (1<<4)
#define HSHUB_PORTSTATUS_POWERSTATE	 (1<<8)
#define HSHUB_PORTSTATUS_LOWSPEED    (1<<9)
#define HSHUB_PORTSTATUS_HIGHSPEED   (1<<10)
#define HSHUB_PORTSTATUS_TESTMODE    (1<<11)
#define HSHUB_PORTSTATUS_INDICATOR    (1<<12)
//#define HSHUB_STATUS_CNCTSTSCHG	(1<<24)
//#define HSHUB_STATUS_PECHANGE		(1<<25)
//#define HSHUB_STATUS_RESET		(1<<28)
//2.0 hub port change(byte2,3)
#define HSHUB_PORTCHANGE_CONNECTION  1
#define HSHUB_PORTCHANGE_ERRORCOND   (1<<1)
#define HSHUB_PORTCHANGE_RESUMED     (1<<2)
#define HSHUB_PORTCHANGE_OVERCURRENT (1<<3)
#define HSHUB_PORTCHANGE_RESETDONE   (1<<4)


//3.0 hub port status(byte0,1)
#define SSHUB_PORTSTATUS_CONNECTION  1
#define SSHUB_PORTSTATUS_ENABLED     (1<<1)
#define SSHUB_PORTSTATUS_OVERCURRENT (1<<3)
#define SSHUB_PORTSTATUS_RESETSIGNAL (1<<4)
#define SSHUB_PORTSTATUS_LINKSTATE    0x1e0
#define SSHUB_PORTSTATUS_POWERSTATE  (1<<9)
#define SSHUB_PORTSTATUS_SPEED        0x1c00
//3.0 hub port change(byte2,3)
#define SSHUB_PORTCHANGE_CONNECTION 1
#define SSHUB_PORTCHANGE_OVERCURRENT (1<<3)
#define SSHUB_PORTCHANGE_ANYRESETDONE (1<<4)
#define SSHUB_PORTCHANGE_WARMRESETDONE (1<<5)
#define SSHUB_PORTCHANGE_LINKSTATE (1<<6)
#define SSHUB_PORTCHANGE_CONFIGERROR (1<<7)


void HUB_PORT_POWERON(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = SET_FEATURE;
	req->wValue = HUB_FEATURE_PORTPOWER;
	req->wIndex = countfrom1;
	req->wLength = 0;
}
void HUB_PORT_CLEARCNCTN(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = CLEAR_FEATURE;
	req->wValue = HUB_FEATURE_PORTCNCTN;
	req->wIndex = countfrom1;
	req->wLength = 0;
}
void HUB_PORT_GETSTATUS(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0xa3;
	req->bRequest = GET_STATUS;
	req->wValue = 0;
	req->wIndex = countfrom1;
	req->wLength = 4;
}
void HUB_PORT_RESET(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = SET_FEATURE;
	req->wValue = HUB_FEATURE_RESET;
	req->wIndex = countfrom1;
	req->wLength = 0;
}
void HUB_PORT_CLEARRESET(struct UsbRequest* req, int countfrom1)
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

	int hubtype;	//0=FS, 1=HS.singleTT, 2=HS.multiTT, 3=SS
	int portcount;
	u32 portstat[16];

	u8 buf[0];
};




int usbhub_powerone(struct item* usb, int id)
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
	HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);


	usbhub_print("%d:power on\n", id);
	HUB_PORT_POWERON(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;
}
int usbhub_powerall(struct item* usb)
{
	usbhub_print("usbhub_powerall\n");
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	int j;
	for(j=0;j<perfunc->portcount;j++){
		usbhub_powerone(usb, j);
		say("......\n");
	}
	return 0;
}




int usbhub_resetone(struct item* usb, int id)
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
	HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);


	usbhub_print("%d:clear cnctn\n", id);
	HUB_PORT_CLEARCNCTN(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;


	usbhub_print("%d:get stat\n", id);
	HUB_PORT_GETSTATUS(&req, id+1);
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
	HUB_PORT_RESET(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;
}
int usbhub_resetall(struct item* usb)
{
	usbhub_print("usbhub_resetall\n");
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	int j;
	for(j=0;j<perfunc->portcount;j++){
		usbhub_resetone(usb, j);
		say("......\n");
	}
	return 0;
}




int usbhub_checkone(struct item* usb, int id)
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
	HUB_PORT_GETSTATUS(&req, id+1);
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
	HUB_PORT_CLEARRESET(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;


	usbhub_print("%d:get stat\n", id);
	HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);


	if(perfunc->hubtype >= 3){
		if(0){		//(bcdUSB >= 0x0310 && bos->ssp_cap);
		}
		else{
			usbhub_print("speed=ss(5gbps)\n");
		}
	}
	else{
		if(perfunc->portstat[id] & HSHUB_PORTSTATUS_LOWSPEED){
			usbhub_print("speed=ls(1.5mbps)\n");
		}
		else if(perfunc->portstat[id] & HSHUB_PORTSTATUS_LOWSPEED){
			usbhub_print("speed=hs(480mbps)\n");
		}
		else{
			usbhub_print("speed=fs(12mbsp)\n");
		}
	}

	usbhub_print("%d:notify xhci enable device\n", id);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		0,_tohc_new_,
		&perfunc->portstat[id],id+1
	);
	if(ret < 0)return -9;
/*
	struct item* ccc = device_create(_usb_, 0, 0, 0);
	if(ccc)usbany_linkup(ccc, 0, xhci, slot);
*/
	return 0;
}
int usbhub_checkall(struct item* usb)
{
	usbhub_print("usbhub_checkall\n");
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	int j;
	for(j=0;j<perfunc->portcount;j++){
		usbhub_checkone(usb, j);
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




	int ret;
	struct UsbRequest req;

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

	//information
	perfunc->hubtype = devdesc->bDeviceProtocol;
	perfunc->portcount = hubdesc->bNbrPorts;

	//choose interface
	u32 status;
	if(2 == perfunc->hubtype){
		usbhub_print("get devicestatus\n");
		DEVICE_REQUEST_GET_STATUS(&req);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			&status, 4
		);
		usbhub_print("devicestatus=%x\n",status);

		usbhub_print("get interfacestatus\n");
		INTERFACE_REQUEST_GET_STATUS(&req, intfdesc->bInterfaceNumber);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			&status, 4
		);
		usbhub_print("interfacestatus=%x\n",status);

		usbhub_print("get hubstatus\n");
		HUB_GETSTATUS(&req, 4);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			&status, 4
		);
		usbhub_print("hubstatus=%x\n",status);

		//find interface
		struct descnode* p0node = 0;
		struct InterfaceDescriptor* p0desc = 0;
		struct descnode* p1node = 0;
		struct InterfaceDescriptor* p1desc = 0;
		struct descnode* p2node = 0;
		struct InterfaceDescriptor* p2desc = 0;
		while(1){
			if(4 == intfnode->type){
				usbhub_print("interface%d.%d: Protocol=%x\n", intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting, intfdesc->bInterfaceProtocol);
				if(0 == intfdesc->bInterfaceProtocol){
					p0node = intfnode;
					p0desc = intfdesc;
				}
				if(1 == intfdesc->bInterfaceProtocol){
					p1node = intfnode;
					p1desc = intfdesc;
				}
				if(2 == intfdesc->bInterfaceProtocol){
					p2node = intfnode;
					p2desc = intfdesc;
				}
			}

			if(0 == intfnode->rfellow)break;
			intfnode = usbdesc_offs2addr(perusb, intfnode->rfellow);
			intfdesc = usbdesc_offs2addr(perusb, intfnode->real);
		}
		intfnode = p1node;
		intfdesc = p1desc;
	}
	else{
		intfnode = usbdesc_offs2addr(perusb, confnode->lchild);
		intfdesc = usbdesc_offs2addr(perusb, intfnode->real);
	}
	if(4 != intfdesc->bDescriptorType){
		usbhub_print("wrong intfdesc\n");
		return 0;
	}

	//configure endpoint
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;
	endpnode = usbdesc_offs2addr(perusb, intfnode->lchild);
	endpdesc = usbdesc_offs2addr(perusb, endpnode->real);
	if(5 != endpdesc->bDescriptorType){
		usbhub_print("wrong endpdesc\n");
		return 0;
	}
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		0,_tohc_conf_,
		endpdesc,sizeof(struct EndpointDescriptor)
	);
	if(ret < 0)return -9;

	//set config
	int set_config_error = 0;
	usbhub_print("set config %d\n", confdesc->bConfigurationValue);
	DEVICE_REQUEST_SET_CONFIGURATION(&req, confdesc->bConfigurationValue);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0){
		set_config_error = 1;
		//return -10;
	}

	//2.0-multiTT hub: set interface
	if(2 == perfunc->hubtype){
		usbhub_print("get devicestatus\n");
		DEVICE_REQUEST_GET_STATUS(&req);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			&status, 4
		);
		usbhub_print("devicestatus=%x\n",status);

		usbhub_print("get interfacestatus\n");
		INTERFACE_REQUEST_GET_STATUS(&req, intfdesc->bInterfaceNumber);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			&status, 4
		);
		usbhub_print("interfacestatus=%x\n",status);

		usbhub_print("get hubstatus\n");
		HUB_GETSTATUS(&req, 4);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			&status, 4
		);
		usbhub_print("hubstatus=%x\n",status);

		usbhub_print("set interface\n");
		INTERFACE_REQUEST_SET_INTERFACE(&req, intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			0,0
		);
		if(ret < 0){
			if(set_config_error){
				usbhub_print("setconfig and setinterface error\n");
				return -1;
			}
		}
	}

	if(set_config_error){
		usbhub_print("set config error\n");
		return -9;
	}

	//3.0 hub: set depth
	if(perfunc->hubtype >= 3){		//superspeed hub
		usbhub_print("set depth\n");
		HUB_SET_DEPTH(&req, 0);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			0,0
		);
		if(ret < 0)return -10;
	}

	perfunc->hostnode = xhci;
	perfunc->hostslot = slot;
	sleep_ms(1000);
	usbhub_powerall(usb);
	sleep_ms(1000);
	usbhub_resetall(usb);
	sleep_ms(1000);
	usbhub_checkall(usb);
	return 0;
}