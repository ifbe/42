#include "libhard.h"
int xhci_giveorderwaitevent(void* hc,int id, u32,u32, void* sendbuf,int sendlen, void* recvbuf, int recvlen);
int usbhid_driver(struct device* usb, int xxx, struct device* xhci, int slot);
int usbstor_driver(struct device* usb, int xxx, struct device* xhci, int slot);




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
			//bit[0,1]: 0=control, 1=isochronous, 2=bulk, 3=interrupt
			//(onlyif isoch)bit[2,3]: 0=nosync..., 1=async, 2=adaptive, 3=sync
			//(onlyif isoch)bit[4,5]: 0=data, 1=feedback, 2=explicit feedback data
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
	say(".bInterval=%d\n",      desc->bInterval);
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
		case 0x21:explainHIDdesc((void*)(buf+j));break;
		default:explainotherdesc(buf+j);
		}

		if(buf[j] <= 2)break;
		j += buf[j];
	}
}




struct descnode{
	u32    type;	//dev,conf,intf,endp,str
	u32   index;	//value of this desc
	u32    real;	//offset to real desc
	u32  chosen;	//setted as current
	u32 lfellow;	//prev brother node, if(0){first}
	u32 rfellow;	//next brother node, if(0){last}
	u32  lchild;	//first child node
	u32  rchild;	//last child node
}__attribute__((packed));
struct perusb{
	//[0x00, 0xff]
	u32  devnode;	//chosen node for device descriptor
	u32 confnode;	//chosen node for configure descriptor
	u32 intfnode;	//chosen node for interface descriptor
	u32  strnode;	//chosen node for string descriptor(chosen language)
	u32 nodelen;
	u32 desclen;

	u8 iManufac;	//won't change
	u8 iProduct;	//won't change
	u8 iSerial; 	//won't change
	u8 iConfigure;	//change when new conf
	u8 iInterface;	//change when new intf
	u8 padding0[0x100 - sizeof(u32)*6 - 5];

	//[0x100, 0xfff]
	struct descnode node[0];	//0xf00/0x20=0x78, basically enough
	u8 padding1[0xf00];

	//[0x1000, 0xffff]
	u8 desc[0];
	u8 padding2[0xf000];

	//[0x10000, 0xfffff]
	u8 freebuf[0];
}__attribute__((packed));
void usb_handledevdesc(struct device* usb, int xxx, struct device* xhci, int slot, struct DeviceDescriptor* desc, int len)
{
	explaindevdesc(desc);
	if( (0xef == desc->bDeviceClass) && (2 == desc->bDeviceSubClass) && (1 == desc->bDeviceProtocol) ){
		say("	composite device ?!\n");
	}

	struct perusb* perusb = usb->priv_ptr;
	struct descnode* node = &perusb->node[perusb->nodelen];
	node->type = 1;
	node->index = 0;
	node->real = (u8*)desc - (u8*)perusb;
	node->chosen = 0;
	node->lfellow = 0;
	node->rfellow = 0;
	node->lchild = 0;
	node->rchild = 0;
	perusb->nodelen += 1;
	perusb->devnode = (u8*)node - (u8*)perusb;

	perusb->iManufac = desc->iManufacturer;
	perusb->iProduct = desc->iProduct;
	perusb->iSerial  = desc->iSerialNumber;
}
void usb_handleconfdesc(struct device* usb, int xxx, struct device* xhci, int slot, u8* buf, int len)
{
	explaineverydesc(buf, len);

	struct perusb* perusb;
	struct descnode* devnode;
	struct descnode* helpnode;
	struct descnode* confnode;
	struct descnode* intfnode;
	struct descnode* endpnode;
	struct descnode* hidnode;
	struct ConfigurationDescriptor* conf;
	struct InterfaceDescriptor* intf;
	struct EndpointDescriptor* endp;
	struct HIDDescriptor* hid;

	perusb = usb->priv_ptr;
	devnode = (void*)perusb + perusb->devnode;

	int j = 0;
	while(1){
		if(j >= len)break;

		//say("@[%x,%x]:%x\n", j, j+buf[j]-1, buf[j+1]);
		switch(buf[j+1]){
		case 0x02:
			conf = (void*)(buf+j);
			confnode = &perusb->node[perusb->nodelen];
			confnode->type = 2;
			confnode->index = conf->bConfigurationValue;
			confnode->real = (u8*)conf - (u8*)perusb;
			confnode->chosen = 0;
			confnode->lfellow = 0;
			confnode->rfellow = 0;
			confnode->lchild = 0;
			confnode->rchild = 0;
			perusb->nodelen += 1;
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
		case 0x04:
			intf = (void*)(buf+j);
			intfnode = &perusb->node[perusb->nodelen];
			intfnode->type = 4;
			intfnode->index = 0;
			intfnode->real = (u8*)intf - (u8*)perusb;
			intfnode->chosen = 0;
			intfnode->lfellow = 0;
			intfnode->rfellow = 0;
			intfnode->lchild = 0;
			intfnode->rchild = 0;
			perusb->nodelen += 1;
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
		case 0x05:
			endp = (void*)(buf+j);
			endpnode = &perusb->node[perusb->nodelen];
			endpnode->type = 5;
			endpnode->index = 0;
			endpnode->real = (u8*)endp - (u8*)perusb;
			endpnode->chosen = 0;
			endpnode->lfellow = 0;
			endpnode->rfellow = 0;
			endpnode->lchild = 0;
			endpnode->rchild = 0;
			perusb->nodelen += 1;

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
		case 0x21:
			hid = (void*)(buf+j);
			hidnode = &perusb->node[perusb->nodelen];
			hidnode->type = 0x21;
			hidnode->index = 0;
			hidnode->real = (u8*)hid - (u8*)perusb;
			hidnode->chosen = 0;
			hidnode->lfellow = 0;
			hidnode->rfellow = 0;
			hidnode->lchild = 0;
			hidnode->rchild = 0;
			perusb->nodelen += 1;

			if(intfnode->rchild){
				hidnode->lfellow = intfnode->rchild;

				helpnode = (void*)perusb + intfnode->rchild;
				helpnode->rfellow = intfnode->rchild = (u8*)hidnode - (u8*)perusb;
			}
			else{
				intfnode->lchild = intfnode->rchild = (u8*)hidnode - (u8*)perusb;
			}
			break;
		default:
			break;
		}

		if(buf[j] <= 2)break;
		j += buf[j];
	}
}




int usb_ReadAndHandleConfigure(struct device* usb, int xxx, struct device* xhci, int slot, u16 value)
{
	int ret;
	struct UsbRequest req;
	struct perusb* perusb = usb->priv_ptr;
	u8* tmp = perusb->desc + perusb->desclen;

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

	perusb->desclen += req.wLength;
	tmp = perusb->desc + perusb->desclen;
	return 0;
}




int usb_ReadAndHandleString(struct device* usb, int xxx, struct device* xhci, int slot, u16 lang, u16 id)
{
	int ret;
	struct UsbRequest req;
	struct perusb* perusb = usb->priv_ptr;
	u8* tmp = perusb->desc + perusb->desclen;

	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + id, lang, 4);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(4 != ret)return -7;

	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300 + id, lang, tmp[0]);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(req.wLength != ret)return -8;

	say("	iManufac:\n");
	printmemory(tmp, req.wLength);

	perusb->desclen += req.wLength;
	tmp = perusb->desc + perusb->desclen;
	return 0;
}
int usb_ReadAndHandleLang(struct device* usb, int xxx, struct device* xhci, int slot, u16 lang)
{
	say("	wLANGID=%04x\n", lang);
	int ret;
	struct UsbRequest req;
	struct perusb* perusb = usb->priv_ptr;
	u8* tmp = perusb->desc + perusb->desclen;

	//if(my->iManufac)  usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,my->iManufac);
	//if(my->iProduct)  usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,my->iProduct);
	//if(my->iSerial)   usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,my->iSerial);
	//if(my->iConfigure)usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,my->iConfigure);
	//if(my->iInterface)usb_ReadAndHandleString(usb,xxx, xhci,slot, lang,my->iInterface);
	return 0;
}




int usb_ChooseFirst(struct device* usb, int xxx, struct device* xhci, int slot)
{
	struct perusb* perusb = usb->priv_ptr;
	printmemory(perusb->node, perusb->nodelen*sizeof(struct descnode));

	struct descnode* devnode;
	struct DeviceDescriptor* devdesc;
	struct descnode* confnode;
	struct ConfigurationDescriptor* confdesc;
	struct descnode* intfnode;
	struct InterfaceDescriptor* intfdesc;

	if(0 == perusb->devnode)return -1;		//no devdesc?
	devnode = (void*)perusb + perusb->devnode;
	devdesc = (void*)perusb + devnode->real;

	if(0 == devnode->lchild)return -2;		//no confdesc?
	confnode = (void*)perusb + devnode->lchild;
	confdesc = (void*)perusb + confnode->real;
	perusb->confnode = (u8*)confnode - (u8*)perusb;

	if(0 == confnode->lchild)return -3;		//no intfdesc?
	intfnode = (void*)perusb + confnode->lchild;
	intfdesc = (void*)perusb + intfnode->real;
	perusb->intfnode = (u8*)intfnode - (u8*)perusb;

	say("	class=%x,subclass=%x,protocol=%x\n", intfdesc->bInterfaceClass, intfdesc->bInterfaceSubClass, intfdesc->bInterfaceProtocol);
	switch(intfdesc->bInterfaceClass){
	case class_hid:
		usbhid_driver(usb,xxx, xhci,slot);
		break;
	case class_storage:
		usbstor_driver(usb,xxx, xhci,slot);
		break;
	case class_hub:
		say("	usb hub\n");
		break;
	}

	return 0;
}




int usb_discon()
{
	return 0;
}
int usb_linkup(struct device* usb, int xxx, struct device* xhci, int slot)
{
	//say("@usblinkup: %p,%x,%p,%x\n",usb,xxx,xhci,slot);

	int j,num,ret;
	struct UsbRequest req;
	struct perusb* perusb = usb->priv_ptr = memorycreate(0x100000, 0);

	//clear memory
	u8* tmp = (void*)perusb;
	for(j=0;j<0x100000;j++)tmp[j] = 0;


//-------------let xhci prepare device-----------------
	if(1){	//if(xhci)
		//int slot = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EnableSlot, 0,0, 0,0);

		int ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_AddressDevice, 0,0, 0,0);
		if(ret < 0)return -1;
	}


//-----------------device descroptor------------------
	//begin reading
	tmp = perusb->desc + perusb->desclen;

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
	perusb->desclen += 0x12;

	//parse Device Descriptor
	usb_handledevdesc(usb,xxx, xhci,slot, (void*)tmp,ret);


//---------------------configure descriptor--------------------
	//foreach value, read configure descriptor
	num = tmp[0x11];		//bNumConfigurations
	for(j=0;j<num;j++){
		usb_ReadAndHandleConfigure(usb,xxx, xhci,slot, j);
	}


//----------------------string descriptor-----------------
	tmp = perusb->desc + perusb->desclen;

	//GET_DESCRIPTOR stringdesc 4B
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300, 0, 4);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(4 != ret)return -6;

	//GET_DESCRIPTOR stringdesc all
	DEVICE_REQUEST_GET_DESCRIPTOR(&req, 0x300, 0, tmp[0]);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, tmp,req.wLength);
	if(req.wLength != ret)return -7;
	perusb->desclen += req.wLength;

	//foreach lang, read string descriptor
	num = req.wLength;
	for(j=2;j<num;j+=2){
		usb_ReadAndHandleLang(usb,xxx, xhci,slot, *(u16*)(tmp+j));
	}


//-----------now that all read, choose one-------------
	usb_ChooseFirst(usb,xxx, xhci,slot);

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