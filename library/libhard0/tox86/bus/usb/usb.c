#include "libhard.h"
int xhci_giveorderwaitevent(void* hc,int id, u32,u32, void* sendbuf,int sendlen, void* recvbuf, int recvlen);




//xhci command
#define TRB_command_EnableSlot           9
#define TRB_command_DisableSlot         10
#define TRB_command_AddressDevice       11
#define TRB_command_ConfigureEndpoint   12
#define TRB_command_EvaluateContext     13
#define TRB_command_ResetEndpoint       14
#define TRB_command_StopEndpoint        15
#define TRB_command_SetTRDequeuePointer 16
#define TRB_command_ResetDevice         17
#define TRB_command_ForceEvent          18		//Optional, used with virtualization only
#define TRB_command_NegotiateBandwidth  19
#define TRB_command_SetLatencyTolerance 20
#define TRB_command_GetPortBandwidth    21
#define TRB_command_ForceHeader         22
#define TRB_command_NoOp                23
#define TRB_command_GetExtendedProperty 24
#define TRB_command_SetExtendedProperty 25
//usb bRequest
#define GET_STATUS        0
#define CLEAR_FEATURE     1
#define SET_FEATURE       3
#define SET_ADDRESS       5
#define GET_DESCRIPTOR    6
#define SET_DESCRIPTOR    7
#define GET_CONFIGURATION 8
#define SET_CONFIGURATION 9
#define GET_INTERFACE     10
#define SET_INTERFACE     11
#define SYNCH_FRAME       12
//usb class
#define class_reserve       0x00	//去看interface descriptor
#define class_audio         0x01	//音频类
#define class_cdccom        0x02	//CDC控制类
#define class_hid           0x03	//人机接口类（HID）
#define class_physical      0x05	//物理类
#define class_image         0x06	//图像类
#define class_printer       0x07	//打印机类
#define class_storage       0x08	//大数据存储类
#define class_hub           0x09	//集线器类
#define class_cdcdata       0x0A	//CDC数据类
#define class_smartcard     0x0B	//智能卡类
#define class_security      0x0D	//安全类
#define class_video         0x0E	//视频设备
#define class_healthcare    0x0F	//个人健康
#define class_audiovideo    0x10	//声音/视频
#define class_billboard     0x11	//广告牌
#define class_typecbridge   0x12	//typec bridge
#define class_diagnostic    0xDC	//诊断设备类
#define class_wireless      0xE0	//无线控制器类
#define class_miscellaneous 0xEF	//混杂
#define class_specific      0xFE	//特定应用类（包括红外的桥接器等）
#define class_vendor        0xFF	//厂商定义的设备




struct DeviceDescriptor{
	u8            bLength;		//0: 0x12
	u8    bDescriptorType;		//1: 0x01
	u16            bcdUSB;		//[2,3]: usb version
	u8       bDeviceClass;		//4: 3=hid, 9=hub
	u8    bDeviceSubClass;		//5
	u8    bDeviceProtocol;		//6
	u8    bMaxPacketSize0;		//7: max packet size on ep0
	u16          idVendor;		//[8,9]
	u16         idProduct;		//[a,b]
	u16         bcdDevice;		//[c,d]: device version
	u8      iManufacturer;		//0e: index of manufacturer string
	u8           iProduct;		//0f: index of product string
	u8      iSerialNumber;		//10: index of serialnumber string
	u8 bNumConfigurations;		//11: how many configuration in this device
}__attribute__((packed));

struct ConfigurationDescriptor{
	u8             bLength;		//0: 0x09
	u8     bDescriptorType;		//1: 0x02
	u16       wTotalLength;		//[2,3]: 
	u8      bNumInterfaces;		//4: how many interface in this configuration
	u8 bConfigurationValue;		//5: this configuration
	u8      iConfiguration;		//6: index of configuration string
	u8        bmAttributes;		//7: 7=bus power, 6=self power, 5=remote wakeup
	u8           bMaxPower;		//8: 50=100mA, 250=500mA
}__attribute__((packed));

struct StringDescriptor{
	u8            bLength;		//0
	u8    bDescriptorType;		//1: 0x03
	u16        wLANGID[0];		//[2,3]: at least one, 0409=en-US
}__attribute__((packed));

struct InterfaceDescriptor{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x04
	u8   bInterfaceNumber;		//2: this interface
	u8  bAlternateSetting;		//3
	u8      bNumEndpoints;		//4: how many endpoint in this interface
	u8    bInterfaceClass;		//5: 3=HID
	u8 bInterfaceSubClass;		//6: if(HID)1=BootInterface
	u8 bInterfaceProtocol;		//7: if(HID)1=kbd,2=mouse
	u8         iInterface;		//8: index of interface string
}__attribute__((packed));

struct EndpointDescriptor{
	u8          bLength;		//0: 0x09
	u8  bDescriptorType;		//1: 0x05
	u8 bEndpointAddress;		//2: endpoint number and direction
	u8     bmAttributes;		//3: endpoint attribute
	u16  wMaxPacketSize;		//[4,5]
	u8        bInterval;		//6: interval between two access
}__attribute__((packed));

struct DeviceQualifier{
	u8            bLength;		//0
	u8    bDescriptorType;		//1: 0x03
}__attribute__((packed));

struct HIDDescriptor{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x21
	u16            bcdHID;		//2: HID version
	u8       bCountryCode;
	u8    bNumDescriptors;		//how many HID class descriptors
	u8    bReportDescType;		//report descriptor type
	u16 wReportDescLength;		//report descriptor length
}__attribute__((packed));

struct CS_INTERFACE{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x24, for audio?
}__attribute__((packed));

struct CS_ENDPOINT{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x25, for audio?
}__attribute__((packed));

struct UsbRequest{
	//[0,3]
	u8 bmRequestType;
		//bit[0,4]: 0=device, 1=interface, 2=endpoint
		//bit[5,6]: 0=normal, 1=class, 2=vendor
		//bit7: 0=host to device, 1=device to host
	u8 bRequest;
		//0: GET_STATUS
		//1: CLEAR_FEATURE
		//3: SET_FEATURE
		//5: SET_ADDRESS
		//6: GET_DESCRIPTOR
		//7: SET_DESCRIPTOR
		//8: GET_CONFIGURATION
		//9: SET_CONFIGURATION
		//a: GET_INTERFACE
		//b: SET_INTERFACE
		//c: SYNCH_FRAME
	u16 wValue;
		//if(GET_DESCRIPTOR)hi = type, lo = index
	//[4,7]
	u16 wIndex;
		//if(GET_DESCRIPTOR_string)wIndex = LANGID
	u16 wLength;
}__attribute__((packed));




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
	say(".bInterval=%dms\n",      desc->bInterval);
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

		say("@[%x,%x]:%x\n", j, j+buf[j]-1, buf[j+1]);
		switch(buf[j+1]){
		case 0x02:explainconfdesc((void*)(buf+j));break;
		case 0x04:explainintfdesc((void*)(buf+j));break;
		case 0x05:explainendpdesc((void*)(buf+j));break;
		case 0x21:explainHIDdesc((void*)(buf+j));break;
		default:explainotherdesc(buf+j);
		}

		if(buf[j] <= 2)break;
		j += buf[j];
	}
}




struct mydata{
	u8 iManufac;
	u8 iProduct;
	u8 iSerial;
	u8 iConfigure;
	u8 iInterface;

	u8 class;
	u8 subcl;
	u8 proto;

	u8 conf;
	u8 intf;
	u8 endp;
}__attribute__((packed));
void usb_handledevdesc(struct device* usb, int xxx, struct device* xhci, int slot, u8* buf, int len)
{
	struct mydata* my = (void*)(usb->data);

	my->iManufac = buf[0x0e];
	my->iProduct = buf[0x0f];
	my->iSerial  = buf[0x10];
}
void usb_handleconfdesc(struct device* usb, int xxx, struct device* xhci, int slot, u8* buf, int len)
{
	struct mydata* my = (void*)(usb->data);
	struct ConfigurationDescriptor* conf;
	struct InterfaceDescriptor* intf;
	struct EndpointDescriptor* endp;
	struct HIDDescriptor* hid;

	int j = 0;
	while(1){
		if(j >= len)break;

		//say("@[%x,%x]:%x\n", j, j+buf[j]-1, buf[j+1]);
		switch(buf[j+1]){
		case 0x02:
			conf = (void*)(buf+j);
			my->conf = conf->bConfigurationValue;
			my->iConfigure = conf->iConfiguration;
			break;
		case 0x04:
			intf = (void*)(buf+j);
			my->class = intf->bInterfaceClass;
			my->subcl = intf->bInterfaceSubClass;
			my->proto = intf->bInterfaceProtocol;

			my->intf = intf->bInterfaceNumber;
			my->iInterface = intf->iInterface;
			break;
		case 0x05:
			endp = (void*)(buf+j);
			my->endp = endp->bEndpointAddress;
			break;
		case 0x21:
			hid = (void*)(buf+j);
			break;
		}

		if(buf[j] <= 2)break;
		j += buf[j];
	}
}




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




int usb_discon()
{
	return 0;
}
int usb_linkup(struct device* usb, int xxx, struct device* xhci, int slot)
{
	say("@usblinkup: %p,%x,%p,%x\n",usb,xxx,xhci,slot);

	int ret;
	struct UsbRequest req;
	struct mydata* my = (void*)(usb->data);

	int cur = 0;
	u8* buf = usb->buf0 = memorycreate(0x100000, 0);

	//tell xhci address device
	if(1){	//if(xhci)
		//int slot = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EnableSlot, 0,0, 0,0);

		int ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_AddressDevice, 0,0, 0,0);
		if(ret < 0)return -1;
	}

	//FS device only
	if(1){
		//GET_DESCRIPTOR devdesc[0,7]
		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x100, 0, 8);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(8 != ret)return -2;

		//tell xhci bMaxPacketSize0 changed
		if(1){
			xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EvaluateContext, buf+cur,8, 0,0);
		}
	}

	//GET_DESCRIPTOR devdesc
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x100, 0, 0x12);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
	if(0x12 != ret)return -3;

	explaindevdesc((void*)(buf+cur));
	usb_handledevdesc(usb,xxx, xhci,slot, buf+cur,ret);
	cur += 0x12;

	//GET_DESCRIPTOR confdesc[0,7]
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x200, 0, 8);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
	if(8 != ret)return -4;

	//GET_DESCRIPTOR confdesc[0,7]
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x200, 0, *(u16*)(buf+cur+2));
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
	if(req.wLength != ret)return -5;

	explaineverydesc(buf+cur, ret);
	usb_handleconfdesc(usb,xxx, xhci,slot, buf+cur,ret);
	cur += req.wLength;

	//GET_DESCRIPTOR stringdesc.wLANGID
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300, 0, 4);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
	if(4 != ret)return -6;

	int lang = *(u16*)(buf+cur+2);
	say("	wLANGID[0]=%04x\n", lang);
	cur += 4;

	if(my->iManufac){
		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iManufac, lang, 4);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(4 != ret)return -7;

		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iManufac, lang, buf[cur+0]);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(req.wLength != ret)return -8;

		say("	iManufac:\n");
		printmemory(buf+cur, req.wLength);
		cur += req.wLength;
	}
	if(my->iProduct){
		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iProduct, lang, 4);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(4 != ret)return -7;

		req.bmRequestType = 0x80;
		req.bRequest = GET_DESCRIPTOR;
		req.wValue = 0x300 + my->iProduct;
		req.wIndex = lang;
		req.wLength = buf[cur+0];
		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iProduct, lang, buf[cur+0]);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(req.wLength != ret)return -8;

		say("	iProduct:\n");
		printmemory(buf+cur, req.wLength);
		cur += req.wLength;
	}
	if(my->iSerial){
		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iSerial, lang, 4);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(4 != ret)return -7;

		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iSerial, lang, buf[cur+0]);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(req.wLength != ret)return -8;

		say("	iSerial:\n");
		printmemory(buf+cur, req.wLength);
		cur += req.wLength;
	}
	if(my->iConfigure){
		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iConfigure, lang, 4);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(4 != ret)return -7;

		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iConfigure, lang, buf[cur+0]);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(req.wLength != ret)return -8;

		say("	iConfigure:\n");
		printmemory(buf+cur, req.wLength);
		cur += req.wLength;
	}
	if(my->iInterface){
		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iInterface, lang, 4);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(4 != ret)return -7;

		DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + my->iInterface, lang, buf[cur+0]);
		ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf+cur,req.wLength);
		if(req.wLength != ret)return -8;

		say("	iInterface:\n");
		printmemory(buf+cur, req.wLength);
		cur += req.wLength;
	}

	printmemory(buf,cur);
	say("	class=%x,subclass=%x,protocol=%x\n", my->class, my->subcl, my->proto);
	say("	conf=%x,intf=%x,endp=%x\n", my->conf, my->intf, my->endp);
	switch(my->class){
	case class_hid:
		if(1== my->subcl){			//boot
			if(1 == my->proto){		//keyboard
				say("	hid keyboard\n");
			}
			if(2 == my->proto){	//mouse
				say("	hid mouse\n");
			}
		}
		break;
	case class_storage:
		say("	usb disk\n");
		break;
	case class_hub:
		say("	usb hub\n");
		break;
	}

	if(1){
		//inform the xHC of the value of the Max Exit Latency parameter
		//ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EvaluateContext, buf,8, 0,0);
		//if(ret < 0)return -9;

		//inform the xHC of the endpoint
		ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_ConfigureEndpoint, buf,8, 0,0);
		if(ret < 0)return -9;
	}
/*
	req.bmRequestType = 0x80;
	req.bRequest = SET_CONFIGURATION;
	req.wValue = my->conf;
	req.wIndex = 0;
	req.wLength = 0;
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf,req.wLength);
	if(4 != ret)return -10;

	req.bmRequestType = 0x80;
	req.bRequest = SET_INTERFACE;
	req.wValue = my->intf;
	req.wIndex = 0;
	req.wLength = 0;
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf,req.wLength);
	if(4 != ret)return -11;
*/
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