#include "libhard.h"
#include "drv/usb.h"
#define usbhub_print(fmt, ...) say("%08lld usbhub@%p "fmt, timeread_us(), usb, ##__VA_ARGS__)
//
int usbdesc_addr2offs(struct perusb* perusb, void* desc);
void* usbdesc_offs2addr(struct perusb* perusb, int offs);
//
void D2H_STD_DEV_GETSTAT(struct UsbRequest* req);
void D2H_STD_INTF_GETSTAT(struct UsbRequest* req, u16 intf);
void H2D_STD_DEV_SETCONF(void* req, u16 conf);
void H2D_STD_INTF_SETINTF(void* req, u16 intf, u16 alt);




//hub feature
#define FEATURE_C_HUB_LOCAL_POWER  0
#define FEATURE_C_HUB_OVER_CURRENT 1
//port feature
#define FEATURE_PORT_CONNECTION     0
#define FEATURE_PORT_ENABLE         1
#define FEATURE_PORT_SUSPEND        2
#define FEATURE_PORT_OVER_CURRENT   3
#define FEATURE_PORT_RESET          4
#define FEATURE_PORT_POWER          8
#define FEATURE_PORT_LOW_SPEED      9
#define FEATURE_C_PORT_CONNECTION   16
#define FEATURE_C_PORT_ENABLE       17
#define FEATURE_C_PORT_SUSPEND      18
#define FEATURE_C_PORT_OVER_CURRENT	19
#define FEATURE_C_PORT_RESET		20
#define FEATURE_PORT_TEST           21
#define FEATURE_PORT_INDICATOR      22
//3.0 hub command
#define SSHUB_SET_DEPTH          12
#define SSHUB_GET_PORT_ERR_COUNT 13


void D2H_CLASS_DEV_GETSTAT(struct UsbRequest* req, int len)
{
	req->bmRequestType = 0xa0;
	req->bRequest = GET_STATUS;
	req->wValue = 0;
	req->wIndex = 0;
	req->wLength = len;
}
void D2H_CLASS_DEV_GETDESC(struct UsbRequest* req, u16 type, u16 len)
{
	req->bmRequestType = 0xa0;  //USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE
	req->bRequest = GET_DESCRIPTOR;
	req->wValue = type;
	req->wIndex = 0;
	req->wLength = len;
}
void H2D_CLASS_HUB_SETDEPTH(struct UsbRequest* req, u16 depth)
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
	req->wValue = FEATURE_PORT_POWER;
	req->wIndex = countfrom1;
	req->wLength = 0;
}
void H2D_CLASS_HUB_CLRFEATURE_PORTCHANGE_CONNECTION(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = CLEAR_FEATURE;
	req->wValue = FEATURE_C_PORT_CONNECTION;
	req->wIndex = countfrom1;
	req->wLength = 0;
}
void H2D_CLASS_HUB_CLRFEATURE_PORTCHANGE_ENABLE(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = CLEAR_FEATURE;
	req->wValue = FEATURE_C_PORT_ENABLE;
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
	req->wValue = FEATURE_PORT_RESET;
	req->wIndex = countfrom1;
	req->wLength = 0;
}
void HUB_PORT_CLEARRESET(struct UsbRequest* req, int countfrom1)
{
	req->bmRequestType = 0x23;
	req->bRequest = CLEAR_FEATURE;
	req->wValue = FEATURE_C_PORT_RESET;
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
		(p64)&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);


	usbhub_print("%d:power on\n", id);
	HUB_PORT_POWERON(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		0,0
	);
	if(ret < 0)return -10;

	usbhub_print("%d:get stat\n", id);
	HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);

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
		(p64)&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);


	usbhub_print("%d:clear change.conn\n", id);
	H2D_CLASS_HUB_CLRFEATURE_PORTCHANGE_CONNECTION(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		0,0
	);
	if(ret < 0)return -10;


	usbhub_print("%d:get stat\n", id);
	HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);

	if(0 == (perfunc->portstat[id]&HUB_STATUS_CONNECTION))return -11;


	usbhub_print("%d:reset\n", id);
	HUB_PORT_RESET(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		0,0
	);
	if(ret < 0)return -10;


	usbhub_print("%d:get stat\n", id);
	HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		&perfunc->portstat[id],4
	);
	if(ret < 0)return -10;
	say("stat=%x\n",perfunc->portstat[id]);

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
		(p64)&req,8,
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
		(p64)&req,8,
		0,0
	);
	if(ret < 0)return -10;


	usbhub_print("%d:get stat\n", id);
	HUB_PORT_GETSTATUS(&req, id+1);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
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
		else if(perfunc->portstat[id] & HSHUB_PORTSTATUS_HIGHSPEED){
			usbhub_print("speed=hs(480mbps)\n");
		}
		else{
			usbhub_print("speed=fs(12mbps)\n");
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

	int j,ret;
	for(j=0;j<perfunc->portcount;j++){
		ret = usbhub_powerone(usb, j);
		if(ret < 0)continue;

	}

	sleep_ms(200);

	for(j=0;j<perfunc->portcount;j++){

		ret = usbhub_resetone(usb, j);
		if(ret < 0)goto thisdone;

		sleep_ms(100);

		ret = usbhub_checkone(usb, j);
thisdone:
		say("----------------\n");
	}
	return 0;
}
int usbhub_stopall(struct item* usb)
{
	usbhub_print("usbhub_checkall\n");
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	return 0;
}





static int usbhub_ongive(struct item* usb,int xxx, struct item* xhci,int endp, p64 arg,int slen, void* rbuf,int rlen)
{
	//usbhub_print("usbhub_ongive\n");
	void** sbuf = (void**)arg;
	void* data = *(void**)sbuf;
	//printmemory(data, 16);

	u8 bitmap = *(u8*)data;
	if(bitmap&1)usbhub_print("hub status change!\n");

	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;
	int slot = perfunc->hostslot;

	int j;
	int ret;
	struct UsbRequest req;
	for(j=1;j<8;j++){
		if(0 == (bitmap & (1<<j)))continue;
		usbhub_print("port %d(count from 1) status change!\n", j);

		HUB_PORT_GETSTATUS(&req, j);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			(p64)&req,8,
			&perfunc->portstat[j],4
		);
		if(ret < 0)return -10;
		usbhub_print("stat=%x\n",perfunc->portstat[j]);

		if(perfunc->portstat[j]&0x10000){
			if(perfunc->portstat[j] & HUB_STATUS_CONNECTION){
				usbhub_print("plug in\n");
			}
			else{
				usbhub_print("plug out\n");
			}

			usbhub_print("%d:clear change.con\n", j);
			H2D_CLASS_HUB_CLRFEATURE_PORTCHANGE_CONNECTION(&req, j);
			ret = xhci->give_pxpxpxpx(
				xhci,slot,
				0,0,
				(p64)&req,8,
				0,0
			);
		}
		if(perfunc->portstat[j]&0x20000){
			usbhub_print("%d:clear change.en\n", j);
			H2D_CLASS_HUB_CLRFEATURE_PORTCHANGE_ENABLE(&req, j);
			ret = xhci->give_pxpxpxpx(
				xhci,slot,
				0,0,
				(p64)&req,8,
				0,0
			);
		}
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
	D2H_CLASS_DEV_GETDESC(&req, (0x29<<8) | 0, 8);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
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
		D2H_STD_DEV_GETSTAT(&req);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			(p64)&req,8,
			&status, 4
		);
		usbhub_print("devicestatus=%x\n",status);

		usbhub_print("get interfacestatus\n");
		D2H_STD_INTF_GETSTAT(&req, intfdesc->bInterfaceNumber);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			(p64)&req,8,
			&status, 4
		);
		usbhub_print("interfacestatus=%x\n",status);

		usbhub_print("get hubstatus\n");
		D2H_CLASS_DEV_GETSTAT(&req, 4);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			(p64)&req,8,
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

	//set config
	int set_config_error = 0;
	usbhub_print("set config %d\n", confdesc->bConfigurationValue);
	H2D_STD_DEV_SETCONF(&req, confdesc->bConfigurationValue);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		0,0
	);
	if(ret < 0){
		set_config_error = 1;
		//return -10;
	}

	//2.0-multiTT hub: set interface
	if(2 == perfunc->hubtype){
		usbhub_print("get devicestatus\n");
		D2H_STD_DEV_GETSTAT(&req);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			(p64)&req,8,
			&status, 4
		);
		usbhub_print("devicestatus=%x\n",status);

		usbhub_print("get interfacestatus\n");
		D2H_STD_INTF_GETSTAT(&req, intfdesc->bInterfaceNumber);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			(p64)&req,8,
			&status, 4
		);
		usbhub_print("interfacestatus=%x\n",status);

		usbhub_print("get hubstatus\n");
		D2H_CLASS_DEV_GETSTAT(&req, 4);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			(p64)&req,8,
			&status, 4
		);
		usbhub_print("hubstatus=%x\n",status);

		usbhub_print("set interface\n");
		H2D_STD_INTF_SETINTF(&req, intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			(p64)&req,8,
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
		H2D_CLASS_HUB_SETDEPTH(&req, 0);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			(p64)&req,8,
			0,0
		);
		if(ret < 0)return -10;
	}

	perfunc->hostnode = xhci;
	perfunc->hostslot = slot;
	usbhub_checkall(usb);


//------------------------ep-----------------------------
	usbhub_print("ep check...\n");
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;
	endpnode = usbdesc_offs2addr(perusb, intfnode->lchild);
	endpdesc = usbdesc_offs2addr(perusb, endpnode->real);
	if(5 != endpdesc->bDescriptorType){
		usbhub_print("wrong endpdesc\n");
		return 0;
	}

	if(0 == (endpdesc->bEndpointAddress & 0x80)){
		usbhub_print("error:epdir=host to device\n");
		return -20;
	}
	int inaddr = (endpdesc->bEndpointAddress & 0xf) << 1;
	inaddr += 1;

	int pktlen = endpdesc->wMaxPacketSize;
	if(pktlen > 0x40)pktlen = 0x40;

	//configure endpoint
	usbhub_print("configure endpoint\n");
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		0,_tohc_conf_,
		endpdesc,sizeof(struct EndpointDescriptor)
	);
	if(ret < 0)return -9;

//------------------------callback------------------------
	usb->ongiving = (void*)usbhub_ongive;

//------------------------transfer ring------------------------
	usbhub_print("making trb@%p\n", perfunc->buf);
	ret = xhci->give_pxpxpxpx(
		xhci, slot|(inaddr<<8),
		0, 0,
		(p64)perfunc->buf, pktlen,
		usb, 0
	);
	return 0;
}