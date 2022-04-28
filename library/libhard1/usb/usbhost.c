#include "libhard.h"
#include "drv-usb.h"
#define usbhost_print(fmt, ...) say("%08lld usbhost@%p "fmt, timeread_us(), usb, ##__VA_ARGS__)
//device driver
int usbvmware_driver(struct item* usb, int xxx, struct item* xhci, int slot);
int usbdualshock_driver(struct item* usb, int xxx, struct item* xhci, int slot);
int usbxbox_driver(struct item* usb, int xxx, struct item* xhci, int slot);
//interface driver
int usbhub_driver(struct item* usb, int xxx, struct item* xhci, int slot, void*, void*);
int usbhid_driver(struct item* usb, int xxx, struct item* xhci, int slot, void*, void*);
int usbstor_driver(struct item* usb, int xxx, struct item* xhci, int slot, void*, void*);
int usbuvc_driver(struct item* usb, int xxx, struct item* xhci, int slot, void*, void*);




int usbdesc_addr2offs(struct perusb* perusb, void* desc)
{
	return desc - (void*)perusb;
}
void* usbdesc_offs2addr(struct perusb* perusb, int offs)
{
	return (void*)perusb + offs;
}




void H2D_STD_DEV_CLRFEATURE(struct UsbRequest* req, u16 feature)
{
	req->bmRequestType = 0;
	req->bRequest = CLEAR_FEATURE;
	req->wValue = feature;
	req->wIndex = 0;
	req->wLength = 0;
}
void H2D_STD_DEV_SETFEATURE(struct UsbRequest* req, u16 feature)
{
	req->bmRequestType = 0;
	req->bRequest = SET_FEATURE;
	req->wValue = feature;
	req->wIndex = 0;
	req->wLength = 0;
}
void H2D_STD_DEV_SETADDR(struct UsbRequest* req, u16 addr)
{
	req->bmRequestType = 0;
	req->bRequest = SET_ADDRESS;
	req->wValue = addr;
	req->wIndex = 0;
	req->wLength = 0;
}
void H2D_STD_DEV_SETDESC(struct UsbRequest* req, u16 type, u16 lang, u16 len)
{
	req->bmRequestType = 0;
	req->bRequest = SET_DESCRIPTOR;
	req->wValue = type;
	req->wIndex = lang;
	req->wLength = len;
}
void H2D_STD_DEV_SETCONF(struct UsbRequest* req, u16 conf)
{
	req->bmRequestType = 0;
	req->bRequest = SET_CONFIGURATION;
	req->wValue = conf;
	req->wIndex = 0;
	req->wLength = 0;
}
void D2H_STD_DEV_GETSTAT(struct UsbRequest* req)
{
	req->bmRequestType = 0x80;
	req->bRequest = GET_STATUS;
	req->wValue = 0;
	req->wIndex = 0;
	req->wLength = 2;
}
void D2H_STD_DEV_GETDESC(struct UsbRequest* req, u16 type, u16 lang, u16 len)
{
	req->bmRequestType = 0x80;
	req->bRequest = GET_DESCRIPTOR;
	req->wValue = type;
	req->wIndex = lang;
	req->wLength = len;
}
void D2H_STD_DEV_GETCONF(struct UsbRequest* req)
{
	req->bmRequestType = 0x80;
	req->bRequest = GET_CONFIGURATION;
	req->wValue = 0;
	req->wIndex = 0;
	req->wLength = 1;
}




void H2D_STD_INTF_CLRFEATURE(struct UsbRequest* req, u16 intf, u16 feature)
{
	req->bmRequestType = 0x01;
	req->bRequest = CLEAR_FEATURE;
	req->wValue = feature;
	req->wIndex = intf;
	req->wLength = 0;
}
void H2D_STD_INTF_SETFEATURE(struct UsbRequest* req, u16 intf, u16 feature)
{
	req->bmRequestType = 0x01;
	req->bRequest = SET_FEATURE;
	req->wValue = feature;
	req->wIndex = intf;
	req->wLength = 0;
}
void H2D_STD_INTF_SETINTF(struct UsbRequest* req, u16 intf, u16 alt)
{
	req->bmRequestType = 0x01;
	req->bRequest = SET_INTERFACE;
	req->wValue = alt;
	req->wIndex = intf;
	req->wLength = 0;
}
void D2H_STD_INTF_GETSTAT(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0x81;
	req->bRequest = GET_STATUS;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 2;
}
void D2H_STD_INTF_GETDESC(struct UsbRequest* req, u16 intf, u16 typeindex, u16 len)
{
	req->bmRequestType = 0x81;
	req->bRequest = 6;
	req->wValue = typeindex;
	req->wIndex = intf;
	req->wLength = len;
}
void D2H_STD_INTF_GETINTF(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0x81;
	req->bRequest = GET_INTERFACE;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 1;
}




void H2D_STD_ENDP_CLRFEATURE(struct UsbRequest* req, u16 endp, u16 feature)
{
	req->bmRequestType = 2;
	req->bRequest = CLEAR_FEATURE;
	req->wValue = feature;
	req->wIndex = endp;
	req->wLength = 0;
}
void H2D_STD_ENDP_SETFEATURE(struct UsbRequest* req, u16 endp, u16 feature)
{
	req->bmRequestType = 2;
	req->bRequest = SET_FEATURE;
	req->wValue = feature;
	req->wIndex = endp;
	req->wLength = 0;
}
void D2H_STD_ENDP_GETSTAT(struct UsbRequest* req, u16 endp)
{
	req->bmRequestType = 0x82;
	req->bRequest = GET_STATUS;
	req->wValue = 0;
	req->wIndex = endp;
	req->wLength = 2;
}
void D2H_STD_ENDP_SYNCHFRAME(struct UsbRequest* req, u16 endp)
{
	req->bmRequestType = 0x82;
	req->bRequest = SYNCH_FRAME;
	req->wValue = 0;
	req->wIndex = endp;
	req->wLength = 2;
}




void explaindevdesc(struct DeviceDescriptor* desc)
{
	say(".bLength=%x\n",            desc->bLength);
	say(".bDescriptorType=DeviceDescriptor\n");
	say(".bcdUSB=%x\n",             desc->bcdUSB);
	say(".bDeviceClass=%x\n",       desc->bDeviceClass);
	say(".bDeviceSubClass=%x\n",    desc->bDeviceSubClass);
	say(".bDeviceProtocol=%x\n",    desc->bDeviceProtocol);
	say(".bMaxPacketSize0=%x\n",    desc->bMaxPacketSize0);
	say(".idVendor=%04x\n",         desc->idVendor);
	say(".idProduct=%04x\n",        desc->idProduct);
	say(".bcdDevice=%x\n",          desc->bcdDevice);
	say(".iManufacturer=%x\n",      desc->iManufacturer);
	say(".iProduct=%x\n",           desc->iProduct);
	say(".iSerialNumber=%x\n",      desc->iSerialNumber);
	say(".bNumConfigurations=%x\n", desc->bNumConfigurations);
}
void explainconfdesc(struct ConfigurationDescriptor* desc)
{
	say(".bLength=%x\n",             desc->bLength);
	say(".bDescriptorType=ConfigurationDescriptor\n");
	say(".wTotalLength=%x\n",        desc->wTotalLength);
	say(".bNumInterfaces=%x\n",      desc->bNumInterfaces);
	say(".bConfigurationValue=%x\n", desc->bConfigurationValue);
	say(".iConfiguration=%x\n",      desc->iConfiguration);
	say(".bmAttributes=%x\n",        desc->bmAttributes);
	say(".bMaxPower=%dmA\n",         desc->bMaxPower*2);
}
void explainintfdesc(struct InterfaceDescriptor* desc)
{
	say(".bLength=%x\n",            desc->bLength);
	say(".bDescriptorType=InterfaceDescriptor\n");
	say(".bInterfaceNumber=%x\n",   desc->bInterfaceNumber);
	say(".bAlternateSetting=%x\n",  desc->bAlternateSetting);
	say(".bNumEndpoints=%x\n",      desc->bNumEndpoints);
	say(".bInterfaceClass=%x\n",    desc->bInterfaceClass);
	say(".bInterfaceSubClass=%x\n", desc->bInterfaceSubClass);
	say(".bInterfaceProtocol=%x\n", desc->bInterfaceProtocol);
	say(".iInterface=%x\n",         desc->iInterface);
}
void explainendpdesc(struct EndpointDescriptor* desc)
{
	int tmp = desc->bEndpointAddress;
	say(".bLength=%x\n",          desc->bLength);
	say(".bDescriptorType=EndpointDescriptor\n");
	say(".bEndpointAddress=%x,%s\n", tmp&0x1f, (tmp>=0x80)?"in":"out");
	say(".bmAttributes=%x\n",     desc->bmAttributes);
	say(".wMaxPacketSize=%x\n",   desc->wMaxPacketSize);
	say(".bInterval=%d\n",        desc->bInterval);
}
void explainInterfaceAssociation(struct InterfaceAssociation* desc)
{
	say(".bLength=%x\n",           desc->bLength);
	say(".bDescriptorType=InterfaceAssociation\n");
	say(".bFirstInterface=%x\n",   desc->bFirstInterface);
	say(".bInterfaceCount=%x\n",   desc->bInterfaceCount);
	say(".bFunctionClass=%x\n",    desc->bFunctionClass);
	say(".bFunctionSubclass=%x\n", desc->bFunctionSubclass);
	say(".bFunctionProtocol=%x\n", desc->bFunctionProtocol);
	say(".iFunction=%x\n",         desc->iFunction);
}
void explainHIDdesc(struct HIDDescriptor* desc)
{
	say(".bLength=%x\n",            desc->bLength);
	say(".bDescriptorType=HIDDescriptor\n");
	say(".bcdHID=%x\n",             desc->bcdHID);
	say(".bCountryCode=%x\n",       desc->bCountryCode);
	say(".bNumDescriptors=%x\n",    desc->bNumDescriptors);
	say(".bReportDescType=%x\n",    desc->bReportDescType);
	say(".wReportDescLength=%x\n",  desc->wReportDescLength);
}
void explainotherdesc(u8* buf)
{
	say(".bLength=%x\n",         buf[0]);
	say(".bDescriptorType=%x\n", buf[1]);
}




int usbany_handleaddress(struct item* xhci, int slot)
{
	if(1){	//if(xhci)
		//int slot = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EnableSlot, 0,0, 0,0);

		int ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			0,_tohc_addr_,
			0,0
		);
		if(ret < 0)return -1;
	}
	return 0;
}




void explaindevdesc_tomy(struct DeviceDescriptor* desc, struct perusb* perusb)
{
	perusb->parsed.iManufac = desc->iManufacturer;
	perusb->parsed.iProduct = desc->iProduct;
	perusb->parsed.iSerial  = desc->iSerialNumber;
	perusb->parsed.numconf = desc->bNumConfigurations;
}




int usbany_handledevdesc(struct item* usb, int xxx, struct item* xhci, int slot)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return -1;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;

	int ret;
	struct UsbRequest req;
	//GET_DESCRIPTOR devdesc 8B (FS device only)
	if(1){
		//GET_DESCRIPTOR devdesc[0,7]
		D2H_STD_DEV_GETDESC(&req, 0x100, 0, 8);
		ret = xhci->give_pxpxpxpx(
			xhci, slot,
			0, 0,
			&req, 8,
			devdesc, req.wLength
		);
		if(8 != ret)return -2;

		//tell xhci bMaxPacketSize0 changed
		if(1){
			xhci->give_pxpxpxpx(
				xhci, slot,
				0, 0,
				0, _tohc_eval_,
				devdesc, 8
			);
		}
	}

	//GET_DESCRIPTOR devdesc all
	D2H_STD_DEV_GETDESC(&req, 0x100, 0, 0x12);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		devdesc, req.wLength
	);
	if(0x12 != ret)return -3;

	//update curr
	perusb->origin.byteused = usbdesc_addr2offs(perusb, (void*)devdesc + devdesc->bLength);

	//parse Device Descriptor
	explaindevdesc(&perusb->origin.devdesc);
	explaindevdesc_tomy(&perusb->origin.devdesc, perusb);
	return 0;
}




int usbany_handlestrdesc(struct item* usb, int xxx, struct item* xhci, int slot)
{
	struct perusb* perusb = usb->priv_ptr;

	struct StringDescriptor* strdesc = &perusb->origin.strdesc;

	int ret;
	struct UsbRequest req;
	//GET_DESCRIPTOR stringdesc 4B
	D2H_STD_DEV_GETDESC(&req, 0x300, 0, 4);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		strdesc,req.wLength
	);
	if(4 != ret)return -6;

	//GET_DESCRIPTOR stringdesc all
	D2H_STD_DEV_GETDESC(&req, 0x300, 0, strdesc->bLength);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		strdesc,strdesc->bLength
	);
	if(req.wLength != ret)return -7;

	//update curr
	perusb->origin.byteused = usbdesc_addr2offs(perusb, (void*)strdesc + strdesc->bLength);

	//lang0
	perusb->parsed.lang0 = perusb->origin.strdesc.wLANGID[0];
	usbhost_print("wLANGID=%04x\n", perusb->parsed.lang0);
	return 0;
}
int usbany_ReadAndHandleString(struct item* usb, int xxx, struct item* xhci, int slot, u16 lang, u16 id)
{
	usbhost_print("readstr: lang=%x,id=%x\n", lang, id);
	struct perusb* perusb = usb->priv_ptr;

	struct StringDescriptor* strdesc = usbdesc_offs2addr(perusb, perusb->origin.byteused);

	int j,ret;
	struct UsbRequest req;
	D2H_STD_DEV_GETDESC(&req, 0x300 + id, lang, 4);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		strdesc,req.wLength
	);
	if(4 != ret)return -7;

	D2H_STD_DEV_GETDESC(&req, 0x300 + id, lang, strdesc->bLength);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		strdesc,req.wLength
	);
	if(req.wLength != ret)return -8;

	//update curr
	perusb->origin.byteused += strdesc->bLength;

	//printmemory(tmp, req.wLength);
	say("unicode2ascii{");
	for(j=0;j<(strdesc->bLength-2)/2;j++){
		say("%c",(strdesc->unicode[j])&0x7f);
	}
	say("}\n");

	return 0;
}




void explainconfdesc_tostr(struct item* usb, int xxx, struct item* xhci, int slot, u8* buf, int len)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return;

	struct ConfigurationDescriptor* confdesc;
	struct InterfaceAssociation* assodesc;
	struct InterfaceDescriptor* intfdesc;

	int j = 0;
	while(1){
		if(j >= len)break;

		say("@[%x,%x]:type=%x\n", j, j+buf[j]-1, buf[j+1]);
		switch(buf[j+1]){
		case 0x02:
			confdesc = (void*)(buf+j);
			explainconfdesc(confdesc);
			if(confdesc->iConfiguration)usbany_ReadAndHandleString(usb,xxx, xhci,slot, perusb->parsed.lang0,confdesc->iConfiguration);
			break;
		case 0x04:
			intfdesc = (void*)(buf+j);
			explainintfdesc(intfdesc);
			if(intfdesc->iInterface)usbany_ReadAndHandleString(usb,xxx, xhci,slot, perusb->parsed.lang0,intfdesc->iInterface);
			break;
		case 0x05:
			explainendpdesc((void*)(buf+j));
			break;
		case 0x0b:
			assodesc = (void*)(buf+j);
			explainInterfaceAssociation(assodesc);
			if(assodesc->iFunction)usbany_ReadAndHandleString(usb,xxx, xhci,slot, perusb->parsed.lang0,assodesc->iFunction);
			break;
		case 0x21:
			explainHIDdesc((void*)(buf+j));
			break;
		default:
			explainotherdesc(buf+j);
		}

		if(buf[j] <= 2)break;
		j += buf[j];
	}
}
void explainconfdesc_tomy(struct item* usb, int xxx, struct item* xhci, int slot, struct ConfigurationDescriptor* confdesc, int len)
{
	struct perusb* perusb = usb->priv_ptr;

	struct descnode* helpnode;
	struct descnode* confnode;

	struct descnode* assonode;
	struct InterfaceAssociation* assodesc;
	struct descnode* intfnode;
	struct InterfaceDescriptor* intfdesc;
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;


	//1
	confnode = &perusb->parsed.node[0];
	perusb->parsed.nodecount = 1;

	confnode->type = 2;
	confnode->real = usbdesc_addr2offs(perusb, confdesc);
	confnode->id = confdesc->bConfigurationValue;
	confnode->altid = 0;
	confnode->lfellow = 0;
	confnode->rfellow = 0;
	confnode->lchild = 0;
	confnode->rchild = 0;


	//2
	int j = 0;
	u8* buf = (u8*)confdesc + confdesc->bLength;
	while(1){
		if(j >= len)break;

		//usbhost_print("@[%x,%x]:%x\n", j, j+buf[j]-1, buf[j+1]);
		switch(buf[j+1]){
		case 0x0b:		//association
			assodesc = (void*)(buf+j);
			assonode = &perusb->parsed.node[perusb->parsed.nodecount];
			perusb->parsed.nodecount += 1;

			assonode->type = buf[j+1];
			assonode->real = usbdesc_addr2offs(perusb, assodesc);
			assonode->id = assodesc->bFirstInterface;
			assonode->altid = assodesc->bInterfaceCount;
			assonode->lfellow = 0;
			assonode->rfellow = 0;
			assonode->lchild = 0;
			assonode->rchild = 0;
			if(confnode->rchild){
				assonode->lfellow = confnode->rchild;

				helpnode = usbdesc_offs2addr(perusb, confnode->rchild);
				helpnode->rfellow = confnode->rchild = usbdesc_addr2offs(perusb, assonode);
			}
			else{
				confnode->lchild = confnode->rchild = usbdesc_addr2offs(perusb, assonode);
			}
			break;
		case 0x04:		//interface
			intfdesc = (void*)(buf+j);
			intfnode = &perusb->parsed.node[perusb->parsed.nodecount];
			perusb->parsed.nodecount += 1;

			intfnode->type = buf[j+1];
			intfnode->real = usbdesc_addr2offs(perusb, intfdesc);
			intfnode->id = intfdesc->bInterfaceNumber;
			intfnode->altid = intfdesc->bAlternateSetting;
			intfnode->lfellow = 0;
			intfnode->rfellow = 0;
			intfnode->lchild = 0;
			intfnode->rchild = 0;
			if(confnode->rchild){
				intfnode->lfellow = confnode->rchild;

				helpnode = usbdesc_offs2addr(perusb, confnode->rchild);
				helpnode->rfellow = confnode->rchild = usbdesc_addr2offs(perusb, intfnode);
			}
			else{
				confnode->lchild = confnode->rchild = usbdesc_addr2offs(perusb, intfnode);
			}
			break;
		case 0x05:		//ep desc
		case 0x21:		//hid desc
			endpdesc = (void*)(buf+j);
			endpnode = &perusb->parsed.node[perusb->parsed.nodecount];
			perusb->parsed.nodecount += 1;

			endpnode->type = buf[j+1];
			endpnode->real = usbdesc_addr2offs(perusb, endpdesc);
			endpnode->id = 0;
			endpnode->altid = 0;
			endpnode->lfellow = 0;
			endpnode->rfellow = 0;
			endpnode->lchild = 0;
			endpnode->rchild = 0;

			if(intfnode->rchild){
				endpnode->lfellow = intfnode->rchild;

				helpnode = usbdesc_offs2addr(perusb, intfnode->rchild);
				helpnode->rfellow = intfnode->rchild = usbdesc_addr2offs(perusb, endpnode);
			}
			else{
				intfnode->lchild = intfnode->rchild = usbdesc_addr2offs(perusb, endpnode);
			}
			//my->endp = endp->bEndpointAddress;
			break;
		default:
			break;
		}

		if(buf[j] <= 2)break;
		j += buf[j];
	}

	//printmemory(perusb->parsed.node, perusb->parsed.nodecount*sizeof(struct descnode));
	struct descnode* stack[4];
	int sp = 0;
	char* tabs = "        ";

	helpnode = confnode;
	while(1){
		if(0 == helpnode){		//0 represent error or last brother
			if(sp > 0){
				sp--;
				helpnode = stack[sp];
				continue;
			}
			break;
		}

		switch(helpnode->type){
		case 0x2:
			say("%.*sconfig:%d.%d\n", 2*sp,tabs, helpnode->id, helpnode->altid);
			break;
		case 0xb:
			say("%.*sassociate:%d.%d\n", 2*sp,tabs, helpnode->id, helpnode->altid);
			break;
		case 0x4:
			say("%.*sinterface:%d.%d\n", 2*sp,tabs, helpnode->id, helpnode->altid);
			break;
		case 0x5:
			say("%.*sendpoint:%d.%d\n", 2*sp,tabs, helpnode->id, helpnode->altid);
			break;
		case 0x21:
			say("%.*shid:%d.%d\n", 2*sp,tabs, helpnode->id, helpnode->altid);
			break;
		}

		//push brother, goto child
		if(helpnode->lchild){
			if(helpnode->rfellow){
				stack[sp] = usbdesc_offs2addr(perusb, helpnode->rfellow);
				sp++;
			}
			else{
				stack[sp] = 0;	//indicate go back
				sp++;
			}
			helpnode = usbdesc_offs2addr(perusb, helpnode->lchild);
			continue;
		}

		//goto brother
		if(helpnode->rfellow){
			helpnode = usbdesc_offs2addr(perusb, helpnode->rfellow);
			continue;
		}
		else{
			helpnode = 0;	//indicate go back
		}
	}
}
int usbany_ReadAndHandleConfigure(struct item* usb, int xxx, struct item* xhci, int slot, u16 value)
{
	struct perusb* perusb = usb->priv_ptr;

	struct ConfigurationDescriptor* confdesc = (void*)&perusb->origin + perusb->origin.byteused;

	int ret;
	struct UsbRequest req;
	//GET_DESCRIPTOR confdesc 8B
	D2H_STD_DEV_GETDESC(&req, 0x200+value, 0, 8);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		confdesc,req.wLength
	);
	if(8 != ret)return -4;

	//GET_DESCRIPTOR confdesc all
	D2H_STD_DEV_GETDESC(&req, 0x200+value, 0, confdesc->wTotalLength);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		confdesc,confdesc->wTotalLength
	);
	if(req.wLength != ret)return -5;

	//update curr
	perusb->origin.byteused += confdesc->wTotalLength;

	//parse Configure Descriptor
	explainconfdesc_tostr(usb,xxx, xhci,slot, (void*)confdesc,ret);
	explainconfdesc_tomy(usb,xxx, xhci,slot, (void*)confdesc,ret);
	return 0;
}




int usbany_FirstConfig(struct item* usb, int xxx, struct item* xhci, int slot)
{
	struct perusb* perusb = usb->priv_ptr;

	struct descnode* confnode;
	struct ConfigurationDescriptor* confdesc;
	struct descnode* intfnode;
	struct InterfaceDescriptor* intfdesc;

	confnode = &perusb->parsed.node[0];
	confdesc = usbdesc_offs2addr(perusb, confnode->real);

	intfnode = usbdesc_offs2addr(perusb, confnode->lchild);
	intfdesc = usbdesc_offs2addr(perusb, intfnode->real);

	//if not composite device
	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	if( (0xef != devdesc->bDeviceClass) | (2 != devdesc->bDeviceSubClass) | (1 != devdesc->bDeviceProtocol) ){
		usbhost_print("class=%x,subclass=%x,protocol=%x\n", intfdesc->bInterfaceClass, intfdesc->bInterfaceSubClass, intfdesc->bInterfaceProtocol);

		switch(intfdesc->bInterfaceClass){
		case class_hid:
			usbhid_driver(usb,xxx, xhci,slot, intfnode, intfdesc);
			break;
		case class_storage:
			usbstor_driver(usb,xxx, xhci,slot, intfnode, intfdesc);
			break;
		case class_hub:
			//usbhost_print("usb hub\n");
			usbhub_driver(usb,xxx, xhci,slot, intfnode, intfdesc);
			break;
		}
		return 0;
	}

	//composite device, all interface
	struct descnode* assonode;
	struct InterfaceAssociation* assodesc;
	while(1){
		if(0xb == intfnode->type){
			assonode = (void*)intfnode;
			assodesc = usbdesc_offs2addr(perusb, assonode->real);
			usbhost_print("association: node=%p,desc=%p, first=%x,count=%x, c=%x,s=%x,p=%x\n",
			intfnode,intfdesc, assodesc->bFirstInterface, assodesc->bInterfaceCount,
			assodesc->bFunctionClass, assodesc->bFunctionSubclass, assodesc->bFunctionProtocol);
		}
		else{
			usbhost_print("interface: node=%p,desc=%p, num=%x, alt=%x, c=%x,s=%x,p=%x\n",
				intfnode,intfdesc, intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting,
				intfdesc->bInterfaceClass, intfdesc->bInterfaceSubClass, intfdesc->bInterfaceProtocol);
		}

		if(0 == intfnode->rfellow)break;
		intfnode = usbdesc_offs2addr(perusb, intfnode->rfellow);
		intfdesc = usbdesc_offs2addr(perusb, intfnode->real);
	}
	return 0;
}




int usbany_discon()
{
	return 0;
}
int usbany_linkup(struct item* usb, int xxx, struct item* xhci, int slot)
{
	usbhost_print("@usblinkup: %p,%x,%p,%x\n",usb,xxx,xhci,slot);

	int j,num,ret;
	struct perusb* perusb = usb->priv_ptr = memorycreate(0x100000, 0);

	//clear memory
	u8* tmp = (void*)perusb;
	for(j=0;j<0x100000;j++)tmp[j] = 0;


//-------------let xhci prepare device-----------------
	usbhost_print("set_address\n");
	usbany_handleaddress(xhci,slot);


//-----------------device descroptor------------------
	usbhost_print("get_desc: device desc\n");
	usbany_handledevdesc(usb,xxx, xhci,slot);


//----------------------string descriptor-----------------
	usbhost_print("get_desc: string desc\n");
	usbany_handlestrdesc(usb,xxx, xhci,slot);
	if(perusb->parsed.iManufac)usbany_ReadAndHandleString(usb,xxx, xhci,slot, perusb->parsed.lang0,perusb->parsed.iManufac);
	if(perusb->parsed.iProduct)usbany_ReadAndHandleString(usb,xxx, xhci,slot, perusb->parsed.lang0,perusb->parsed.iProduct);
	if(perusb->parsed.iSerial) usbany_ReadAndHandleString(usb,xxx, xhci,slot, perusb->parsed.lang0,perusb->parsed.iSerial);
	//if(my->iConfigure)usbany_ReadAndHandleString(usb,xxx, xhci,slot, lang,my->iConfigure);
	//if(my->iInterface)usbany_ReadAndHandleString(usb,xxx, xhci,slot, lang,my->iInterface);


//---------------------configure descriptor--------------------
	usbhost_print("get_desc: config desc\n");
	num = perusb->origin.devdesc.bNumConfigurations;
	for(j=0;j<num;j++){
		usbany_ReadAndHandleConfigure(usb,xxx, xhci,slot, j);
	}


//-----------now that all read, choose one-------------
	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	if(0x0e0f == devdesc->idVendor){
		if(0x0003 == devdesc->idProduct){
			return usbvmware_driver(usb,xxx, xhci,slot);
		}
	}
	if(0x045e == devdesc->idVendor){
		switch(devdesc->idProduct){
		case 0x0202:
		case 0x0285:
		case 0x0289:
		case 0x028e:
		case 0x028f:
		case 0x02d1:
		case 0x02dd:
		case 0x02e3:
		case 0x02e6:
		case 0x02ea:return usbxbox_driver(usb,xxx, xhci,slot);
		}
	}
	if(0x054c == devdesc->idVendor){
		switch(devdesc->idProduct){
		case 0x05c4:
		case 0x09cc:
		case 0x0ba0:
		case 0x0ce6:return usbdualshock_driver(usb,xxx, xhci,slot);
		}
	}

	usbany_FirstConfig(usb,xxx, xhci,slot);
	return 0;
}
int usbany_delete()
{
	return 0;
}
int usbany_create()
{
	return 0;
}
