#include "libhard.h"
#include "usb.h"
int xhci_giveorderwaitevent(void* hc,int id, u32,u32, void* sendbuf,int sendlen, void* recvbuf, int recvlen);
//device driver
int usbds4_driver(struct item* usb, int xxx, struct item* xhci, int slot);
int usbxbox_driver(struct item* usb, int xxx, struct item* xhci, int slot);
//interface driver
int usbhid_driver(struct item* usb, int xxx, struct item* xhci, int slot, void*, void*);
int usbstor_driver(struct item* usb, int xxx, struct item* xhci, int slot, void*, void*);
int usbuvc_driver(struct item* usb, int xxx, struct item* xhci, int slot, void*, void*);




void DEVICE_REQUEST_GET_STATUS(struct UsbRequest* req)
{
	req->bmRequestType = 0x80;
	req->bRequest = 0;
	req->wValue = 0;
	req->wIndex = 0;
	req->wLength = 2;
}
void DEVICE_REQUEST_CLEAR_FEATURE(struct UsbRequest* req, u16 feature)
{
	req->bmRequestType = 0;
	req->bRequest = 1;
	req->wValue = feature;
	req->wIndex = 0;
	req->wLength = 0;
}
void DEVICE_REQUEST_SET_FEATURE(struct UsbRequest* req, u16 feature)
{
	req->bmRequestType = 0;
	req->bRequest = 3;
	req->wValue = feature;
	req->wIndex = 0;
	req->wLength = 0;
}
void DEVICE_REQUEST_SET_ADDRESS(struct UsbRequest* req, u16 addr)
{
	req->bmRequestType = 0;
	req->bRequest = 5;
	req->wValue = addr;
	req->wIndex = 0;
	req->wLength = 0;
}
void DEVICE_REQUEST_GET_DESCRIPTOR(struct UsbRequest* req, u16 type, u16 lang, u16 len)
{
	req->bmRequestType = 0x80;
	req->bRequest = 6;
	req->wValue = type;
	req->wIndex = lang;
	req->wLength = len;
}
void DEVICE_REQUEST_SET_DESCRIPTOR(struct UsbRequest* req, u16 type, u16 lang, u16 len)
{
	req->bmRequestType = 0;
	req->bRequest = 7;
	req->wValue = type;
	req->wIndex = lang;
	req->wLength = len;
}
void DEVICE_REQUEST_GET_CONFIGURATION(struct UsbRequest* req)
{
	req->bmRequestType = 0x80;
	req->bRequest = 8;
	req->wValue = 0;
	req->wIndex = 0;
	req->wLength = 1;
}
void DEVICE_REQUEST_SET_CONFIGURATION(struct UsbRequest* req, u16 conf)
{
	req->bmRequestType = 0;
	req->bRequest = 9;
	req->wValue = conf;
	req->wIndex = 0;
	req->wLength = 0;
}




void INTERFACE_REQUEST_GET_STATUS(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0x81;
	req->bRequest = 0;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 2;
}
void INTERFACE_REQUEST_CLEAR_FEATURE(struct UsbRequest* req, u16 intf, u16 feature)
{
	req->bmRequestType = 0x01;
	req->bRequest = 1;
	req->wValue = feature;
	req->wIndex = intf;
	req->wLength = 0;
}
void INTERFACE_REQUEST_SET_FEATURE(struct UsbRequest* req, u16 intf, u16 feature)
{
	req->bmRequestType = 0x01;
	req->bRequest = 3;
	req->wValue = feature;
	req->wIndex = intf;
	req->wLength = 0;
}
void INTERFACE_REQUEST_GET_INTERFACE(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0x81;
	req->bRequest = 0x0a;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 1;
}
void INTERFACE_REQUEST_SET_INTERFACE(struct UsbRequest* req, u16 intf, u16 alt)
{
	req->bmRequestType = 0x01;
	req->bRequest = 0x11;
	req->wValue = alt;
	req->wIndex = intf;
	req->wLength = 0;
}




void ENDPOINT_REQUEST_GET_STATUS(struct UsbRequest* req, u16 endp)
{
	req->bmRequestType = 0x82;
	req->bRequest = 0;
	req->wValue = 0;
	req->wIndex = endp;
	req->wLength = 2;
}
void ENDPOINT_REQUEST_CLEAR_FEATURE(struct UsbRequest* req, u16 endp, u16 feature)
{
	req->bmRequestType = 2;
	req->bRequest = 1;
	req->wValue = feature;
	req->wIndex = endp;
	req->wLength = 0;
}
void ENDPOINT_REQUEST_SET_FEATURE(struct UsbRequest* req, u16 endp, u16 feature)
{
	req->bmRequestType = 2;
	req->bRequest = 3;
	req->wValue = feature;
	req->wIndex = endp;
	req->wLength = 0;
}
void ENDPOINT_REQUEST_SYNCH_FRAME(struct UsbRequest* req, u16 endp)
{
	req->bmRequestType = 0x82;
	req->bRequest = 0x12;
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
void explaineverydesc(u8* buf, int len)
{
	int j = 0;
	while(1){
		if(j >= len)break;

		say("@[%x,%x]:type=%x\n", j, j+buf[j]-1, buf[j+1]);
		switch(buf[j+1]){
		case 0x02:explainconfdesc((void*)(buf+j));break;
		case 0x04:explainintfdesc((void*)(buf+j));break;
		case 0x05:explainendpdesc((void*)(buf+j));break;
		case 0x0b:explainInterfaceAssociation((void*)(buf+j));break;
		case 0x21:explainHIDdesc((void*)(buf+j));break;
		default:explainotherdesc(buf+j);
		}

		if(buf[j] <= 2)break;
		j += buf[j];
	}
}




void usb_handledevdesc(struct item* usb, int xxx, struct item* xhci, int slot, struct DeviceDescriptor* desc, int len)
{
	explaindevdesc(desc);
	if( (0xef == desc->bDeviceClass) && (2 == desc->bDeviceSubClass) && (1 == desc->bDeviceProtocol) ){
		say("[usbcore]composite device ?!\n");
	}

	struct perusb* perusb = usb->priv_ptr;
	struct descnode* node = &perusb->node[perusb->my.nodelen];
	node->type = 1;
	node->index = 0;
	node->real = (u8*)desc - (u8*)perusb;
	node->chosen = 0;
	node->lfellow = 0;
	node->rfellow = 0;
	node->lchild = 0;
	node->rchild = 0;
	perusb->my.nodelen += 1;
	perusb->my.devnode = (u8*)node - (u8*)perusb;

	perusb->my.iManufac = desc->iManufacturer;
	perusb->my.iProduct = desc->iProduct;
	perusb->my.iSerial  = desc->iSerialNumber;
}




void usb_handleconfdesc(struct item* usb, int xxx, struct item* xhci, int slot, u8* buf, int len)
{
	explaineverydesc(buf, len);

	struct perusb* perusb;
	struct descnode* devnode;
	struct descnode* helpnode;

	struct descnode* confnode;
	struct descnode* intfnode;
	struct descnode* endpnode;
	struct ConfigurationDescriptor* conf;
	struct InterfaceDescriptor* intf;
	struct EndpointDescriptor* endp;

	perusb = usb->priv_ptr;
	devnode = (void*)perusb + perusb->my.devnode;

	int j = 0;
	while(1){
		if(j >= len)break;

		//say("[usbcore]@[%x,%x]:%x\n", j, j+buf[j]-1, buf[j+1]);
		switch(buf[j+1]){
		case 0x02:
			conf = (void*)(buf+j);
			confnode = &perusb->node[perusb->my.nodelen];
			confnode->type = 2;
			confnode->index = conf->bConfigurationValue;
			confnode->real = (u8*)conf - (u8*)perusb;
			confnode->chosen = 0;
			confnode->lfellow = 0;
			confnode->rfellow = 0;
			confnode->lchild = 0;
			confnode->rchild = 0;
			perusb->my.nodelen += 1;
			if(devnode->rchild){
				confnode->lfellow = devnode->rchild;

				helpnode = (void*)perusb + devnode->rchild;
				helpnode->rfellow = devnode->rchild = (u8*)confnode - (u8*)perusb;
			}
			else{
				devnode->lchild = devnode->rchild = (u8*)confnode - (u8*)perusb;
			}
			//perusb->iConfigure = conf->iConfiguration;
			break;
		case 0x04:		//interface
		case 0x0b:		//association
			intf = (void*)(buf+j);
			intfnode = &perusb->node[perusb->my.nodelen];
			intfnode->type = buf[j+1];
			intfnode->index = 0;
			intfnode->real = (u8*)intf - (u8*)perusb;
			intfnode->chosen = 0;
			intfnode->lfellow = 0;
			intfnode->rfellow = 0;
			intfnode->lchild = 0;
			intfnode->rchild = 0;
			perusb->my.nodelen += 1;
			if(confnode->rchild){
				intfnode->lfellow = confnode->rchild;

				helpnode = (void*)perusb + confnode->rchild;
				helpnode->rfellow = confnode->rchild = (u8*)intfnode - (u8*)perusb;
			}
			else{
				confnode->lchild = confnode->rchild = (u8*)intfnode - (u8*)perusb;
			}
			//my->class = intf->bInterfaceClass;
			//my->subcl = intf->bInterfaceSubClass;
			//my->proto = intf->bInterfaceProtocol;

			//my->intf = intf->bInterfaceNumber;
			//my->iInterface = intf->iInterface;
			break;
		case 0x05:		//ep desc
		case 0x21:		//hid desc
			endp = (void*)(buf+j);
			endpnode = &perusb->node[perusb->my.nodelen];
			endpnode->type = buf[j+1];
			endpnode->index = 0;
			endpnode->real = (u8*)endp - (u8*)perusb;
			endpnode->chosen = 0;
			endpnode->lfellow = 0;
			endpnode->rfellow = 0;
			endpnode->lchild = 0;
			endpnode->rchild = 0;
			perusb->my.nodelen += 1;

			if(intfnode->rchild){
				endpnode->lfellow = intfnode->rchild;

				helpnode = (void*)perusb + intfnode->rchild;
				helpnode->rfellow = intfnode->rchild = (u8*)endpnode - (u8*)perusb;
			}
			else{
				intfnode->lchild = intfnode->rchild = (u8*)endpnode - (u8*)perusb;
			}
			//my->endp = endp->bEndpointAddress;
			break;
		default:
			break;
		}

		if(buf[j] <= 2)break;
		j += buf[j];
	}
}
int usb_ReadAndHandleConfigure(struct item* usb, int xxx, struct item* xhci, int slot, u16 value)
{
	int ret;
	struct UsbRequest req;
	struct perusb* perusb = usb->priv_ptr;
	u8* tmp = perusb->desc + perusb->my.desclen;

	//GET_DESCRIPTOR confdesc 8B
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x200+value, 0, 8);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(8 != ret)return -4;

	//GET_DESCRIPTOR confdesc all
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x200+value, 0, *(u16*)(tmp+2));
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(req.wLength != ret)return -5;

	//parse Configure Descriptor
	usb_handleconfdesc(usb,xxx, xhci,slot, tmp,ret);

	perusb->my.desclen += req.wLength;
	tmp = perusb->desc + perusb->my.desclen;
	return 0;
}
int usb_FirstConfig(struct item* usb, int xxx, struct item* xhci, int slot)
{
	struct perusb* perusb = usb->priv_ptr;
	printmemory(perusb->node, perusb->my.nodelen*sizeof(struct descnode));

	struct descnode* devnode;
	struct DeviceDescriptor* devdesc;
	struct descnode* confnode;
	struct ConfigurationDescriptor* confdesc;
	struct descnode* intfnode;
	struct InterfaceDescriptor* intfdesc;

	if(0 == perusb->my.devnode)return -1;		//no devdesc?
	devnode = (void*)perusb + perusb->my.devnode;
	devdesc = (void*)perusb + devnode->real;

	if(0 == devnode->lchild)return -2;		//no confdesc?
	confnode = (void*)perusb + devnode->lchild;
	confdesc = (void*)perusb + confnode->real;
	perusb->my.confnode = (u8*)confnode - (u8*)perusb;

	if(0 == confnode->lchild)return -3;		//no intfdesc?
	intfnode = (void*)perusb + confnode->lchild;
	intfdesc = (void*)perusb + intfnode->real;

	//if not composite device
	if( (0xef != devdesc->bDeviceClass) | (2 != devdesc->bDeviceSubClass) | (1 != devdesc->bDeviceProtocol) ){
		say("[usbcore]class=%x,subclass=%x,protocol=%x\n", intfdesc->bInterfaceClass, intfdesc->bInterfaceSubClass, intfdesc->bInterfaceProtocol);

		perusb->my.intfnode = (u8*)intfnode - (u8*)perusb;
		switch(intfdesc->bInterfaceClass){
		case class_hid:
			usbhid_driver(usb,xxx, xhci,slot, intfnode, intfdesc);
			break;
		case class_storage:
			usbstor_driver(usb,xxx, xhci,slot, intfnode, intfdesc);
			break;
		case class_hub:
			say("[usbcore]usb hub\n");
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
			assodesc = (void*)perusb + assonode->real;
			say("[usbcore]association: node=%p,desc=%p, first=%x,count=%x, c=%x,s=%x,p=%x\n",
			intfnode,intfdesc, assodesc->bFirstInterface, assodesc->bInterfaceCount,
			assodesc->bFunctionClass, assodesc->bFunctionSubclass, assodesc->bFunctionProtocol);
		}
		else{
			say("[usbcore]interface: node=%p,desc=%p, num=%x, alt=%x, c=%x,s=%x,p=%x\n",
				intfnode,intfdesc, intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting,
				intfdesc->bInterfaceClass, intfdesc->bInterfaceSubClass, intfdesc->bInterfaceProtocol);
		}

		if(0 == intfnode->rfellow)break;
		intfnode = (void*)perusb + intfnode->rfellow;
		intfdesc = (void*)perusb + intfnode->real;
	}
	return 0;
}




int usb_ReadAndHandleString(struct item* usb, int xxx, struct item* xhci, int slot, u16 lang, u16 id)
{
	int j,ret;
	struct UsbRequest req;
	struct perusb* perusb = usb->priv_ptr;
	u8* tmp = perusb->desc + perusb->my.desclen;

	say("[usbcore]readstr: lang=%x,id=%x\n", lang, id);

	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + id, lang, 4);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(4 != ret)return -7;

	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + id, lang, tmp[0]);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(req.wLength != ret)return -8;

	perusb->my.desclen += req.wLength;

	//printmemory(tmp, req.wLength);
	say("unicode2ascii{");
	for(j=2;j<req.wLength;j+=2){
		say("%c",tmp[j]);
	}
	say("}\n");

	return 0;
}
int usb_ReadAndHandleLang(struct item* usb, int xxx, struct item* xhci, int slot, u16 lang)
{
	say("[usbcore]wLANGID=%04x\n", lang);
	int ret;
	struct UsbRequest req;
	struct perusb* perusb = usb->priv_ptr;
	u8* tmp = perusb->desc + perusb->my.desclen;

	if(perusb->my.iManufac)  usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,perusb->my.iManufac);
	if(perusb->my.iProduct)  usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,perusb->my.iProduct);
	if(perusb->my.iSerial)   usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,perusb->my.iSerial);
	//if(my->iConfigure)usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,my->iConfigure);
	//if(my->iInterface)usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,my->iInterface);
	return 0;
}




int usb_discon()
{
	return 0;
}
int usb_linkup(struct item* usb, int xxx, struct item* xhci, int slot)
{
	say("[usbcore]@usblinkup: %p,%x,%p,%x\n",usb,xxx,xhci,slot);

	int j,num,ret;
	struct UsbRequest req;
	struct perusb* perusb = usb->priv_ptr = memorycreate(0x100000, 0);

	//clear memory
	u8* tmp = (void*)perusb;
	for(j=0;j<0x100000;j++)tmp[j] = 0;


//-------------let xhci prepare device-----------------
	say("[usbcore]set_address\n");
	if(1){	//if(xhci)
		//int slot = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EnableSlot, 0,0, 0,0);

		int ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_AddressDevice, 0,0, 0,0);
		if(ret < 0)return -1;
	}


//-----------------device descroptor------------------
	say("[usbcore]get_desc: device desc\n");
	//begin reading
	tmp = perusb->desc + perusb->my.desclen;

	//GET_DESCRIPTOR devdesc 8B (FS device only)
	if(1){
		//GET_DESCRIPTOR devdesc[0,7]
		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x100, 0, 8);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
		if(8 != ret)return -2;

		//tell xhci bMaxPacketSize0 changed
		if(1){
			xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EvaluateContext, tmp,8, 0,0);
		}
	}

	//GET_DESCRIPTOR devdesc all
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x100, 0, 0x12);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(0x12 != ret)return -3;
	perusb->my.desclen += 0x12;

	//parse Device Descriptor
	usb_handledevdesc(usb,xxx, xhci,slot, (void*)tmp,ret);


//---------------------configure descriptor--------------------
	say("[usbcore]get_desc: config desc\n");
	//foreach value, read configure descriptor
	num = tmp[0x11];		//bNumConfigurations
	for(j=0;j<num;j++){
		usb_ReadAndHandleConfigure(usb,xxx, xhci,slot, j);
	}


//----------------------string descriptor-----------------
	say("[usbcore]get_desc: string desc\n");
	tmp = perusb->desc + perusb->my.desclen;

	//GET_DESCRIPTOR stringdesc 4B
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300, 0, 4);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(4 != ret)return -6;

	//GET_DESCRIPTOR stringdesc all
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300, 0, tmp[0]);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(req.wLength != ret)return -7;
	perusb->my.desclen += req.wLength;

	//foreach lang, read string descriptor
	num = req.wLength;
	for(j=2;j<num;j+=2){
		usb_ReadAndHandleLang(usb,xxx, xhci,slot, *(u16*)(tmp+j));
	}


//-----------now that all read, choose one-------------
	struct descnode* devnode = (void*)perusb + perusb->my.devnode;
	struct DeviceDescriptor* devdesc = (void*)perusb + devnode->real;

	if(0x045e == devdesc->idVendor){
		switch(devdesc->idProduct){
		case 0x0202:say("xbox\n");break;
		case 0x0285:
		case 0x0289:say("xbox-s\n");break;
		case 0x028e:say("xbox360\n");break;
		case 0x028f:say("xbox360-wireless\n");break;
		case 0x02d1:say("xboxone\n");break;
		case 0x02dd:say("xboxone-2015\n");break;
		case 0x02e3:say("xboxone-elite\n");break;
		case 0x02e6:say("xbox-dongle\n");break;
		case 0x02ea:say("xboxones\n");break;
		}
	}
	if(0x054c == devdesc->idVendor){
		switch(devdesc->idProduct){
		case 0x05c4:
		case 0x09cc:
		case 0x0ba0:return usbds4_driver(usb,xxx, xhci,slot);
		}
	}

	usb_FirstConfig(usb,xxx, xhci,slot);
	return 0;
}
int usb_delete()
{
	return 0;
}
int usb_create()
{
	return 0;
}
