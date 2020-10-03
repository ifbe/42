#include "libhard.h"
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
int xhci_giveorderwaitevent(void* hc,int id, u32,u32, void* sendbuf,int sendlen, void* recvbuf, int recvlen);
//
#define desctype_HID 0x21
#define desctype_report 0x22
#define desctype_descriptor 0x23
#define bRequest_GET_REPORT 1
#define bRequest_GET_IDLE 2
#define bRequest_GET_PROTOCOL 3
#define bRequest_SET_REPORT 9
#define bRequest_SET_IDLE 0xa
#define bRequest_SET_PROTOCOL 0xb
void DEVICE_REQUEST_SET_CONFIGURATION(void* req, u16 conf);
//error
#define NoKeyPress 0
#define InvalidScanCode 1
#define SelfTestFail 2
#define UndefinedError 3
//[A,B,C...Z]: [4, 29]
#define USBKEY_A 4
#define USBKEY_Z 29
//[1,2,3...0]: [30, 39]
#define USBKEY_0 30
#define USBKEY_9 39
//kbd
#define USBKEY_Enter 40
#define USBKEY_Esc   41
#define USBKEY_BSp   42
#define USBKEY_Tab   43
#define USBKEY_Space 44
//45: -_
//46: =+
//47: [{
//48: ]}
//49: \|
//50: ...
//51: ;:
//52: '"
//53: `~
//54: ,<
//55: .>
//56: /?
//57: CapsLock
//[f1,f12]: [58,69]
//70: PrtScr
//71: ScrollLock
//72: Pause
//73: Insert
//74: Home
//75: PgUp
//76: Delete
//77: End
//78: PgDn
//79: Right
//80: Left
//81: Down
//82: Up
//83: NumLock
//84: KP /
//85: KP *
//86: KP -
//87: KP +
//88: KP Enter
//89: KP 1End
//90: KP 2Down
//91: KP 3PgDn
//92: KP 4Left
//93: KP 5
//94: KP 6Right
//95: KP 7Home
//96: KP 8Up
//97: KP 9PgUp
//98: KP 0Ins
//99: KP .Del
//100: ...
//101: Applic
//102: Power
//103: KP =
//[F13,F24]: [104,115]
//116: Execute
//117: Help
//118: Menu
//119: Select
//120: Stop
//121: Again
//122: Undo
//123: Cut
//124: Copy
//125: Paste
//126: Find
//127: Mute
//128: VolumeUp
//129: VolumeDown
//130: Locking CapsLock
//131: LockingNumLock
//132: LockingScrollLock
//133: KP ,
//134: KP =
//[135,143]: Internat
//[144,152]: LANG
//153: AltErase
//154: SysRq
//155: Cancel
//156: Clear
//157: Prior
//158: Return
//159: Separ
//160: Out
//161: Oper
//162: Clear / Again
//163: CrSel / Props
//164: ExSel
//224: LCtrl
//225: LShift
//226: LAlt
//227: LSuper
//228: RCtrl
//229: RShift
//230: RAlt
//231: RSuper
//
#define LEDLAMP_NUMLOCK    0
#define LEDLAMP_CAPSLOCK   1
#define LEDLAMP_SCROLLLOCK 2
#define LEDLAMP_COMPOSE    3
#define LEDLAMP_KANA       4
//
#define MODIFIER_LEFTCTRL   0
#define MODIFIER_LEFTSHIFT  1
#define MODIFIER_LEFTALT    2
#define MODIFIER_LEFTSUPER  3
#define MODIFIER_RIGHTCTRL  4
#define MODIFIER_RIGHTSHIFT 5
#define MODIFIER_RIGHTALT   6
#define MODIFIER_RIGHTSUPER 7
struct report_keyboard{
	u8 modifier;
	u8 reserved;
	u8 keys[6];
}__attribute__((packed));
//
#define MOUSE_LEFTBUTTON
#define MOUSE_RIGHTBUTTON
#define MOUSE_MIDDLEBUTTON
struct report_mouse{
	u8 btn;
	char dx;
	char dy;
}__attribute__((packed));




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




void INTERFACE_REQUEST_GET_REPORT(struct UsbRequest* req, u16 intf, u16 len)
{
	req->bmRequestType = 0xa1;
	req->bRequest = bRequest_GET_REPORT;
	req->wValue = 0;	//report type and report id: 1=input, 2=output, 3=feature
	req->wIndex = intf;
	req->wLength = len;	//report length
}
void INTERFACE_REQUEST_SET_REPORT(struct UsbRequest* req, u16 intf, u16 len)
{
	req->bmRequestType = 0x21;
	req->bRequest = bRequest_SET_REPORT;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = len;
}
void INTERFACE_REQUEST_GET_IDLE(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0xa1;
	req->bRequest = bRequest_GET_IDLE;
	req->wValue = 0;	//0 and report id
	req->wIndex = intf;
	req->wLength = 1;
}
void INTERFACE_REQUEST_SET_IDLE(struct UsbRequest* req, u16 intf, u16 val)
{
	req->bmRequestType = 0x21;
	req->bRequest = bRequest_SET_IDLE;
	req->wValue = val;	//duration and report id
	req->wIndex = intf;
	req->wLength = 0;
}
void INTERFACE_REQUEST_GET_PROTOCOL(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0xa1;
	req->bRequest = bRequest_GET_PROTOCOL;
	req->wValue = 0;	//0 and report id
	req->wIndex = intf;
	req->wLength = 1;
}
void INTERFACE_REQUEST_SET_PROTOCOL(struct UsbRequest* req, u16 intf, u16 val)
{
	req->bmRequestType = 0x21;
	req->bRequest = bRequest_SET_PROTOCOL;
	req->wValue = val;	//duration and report id
	req->wIndex = intf;
	req->wLength = 0;
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
	struct descnode node[0];	//0xf00/0x20=0x78, enough for node
	u8 padding1[0xf00];

	//[0x1000, 0xffff]
	u8 desc[0];					//0xf000=61440, enough for desc
	u8 padding2[0xf000];

	//[0x10000, 0xfffff]
	u8 freebuf[0];
}__attribute__((packed));




int parsekeyboard(struct report_keyboard* report)
{
	int j;
	for(j=0;j<6;j++){
		if(report->keys[j] >= USBKEY_A){
			if(report->keys[j] <= USBKEY_Z)say("[usbkbd]key_%c\n", report->keys[j]-USBKEY_A+'A');
			else if(report->keys[j] <= USBKEY_9)say("[usbkbd]key_%c\n", report->keys[j]-USBKEY_0+'0');
			else say("[usbkbd]key=%x\n", report->keys[j]);
		}
	}
	return 0;
}
int parsemouse(struct report_mouse* report)
{
	say("[usbmouse]btn=%x,dx=%d,dy=%d\n", report->btn, report->dx, report->dy);
	return 0;
}/*
int parsehid()
{
	//parse keyboard
	if(1 == intfdesc->bInterfaceProtocol){
	}
    //parse mouse
	if(2 == intfdesc->bInterfaceProtocol){
		 = (void*)perusb->freebuf;
	}
}*/
int usbhid_driver(struct device* usb,int xxx, struct device* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
{
	int j,ret;
	struct UsbRequest req;
	struct perusb* perusb;
	//per device
	struct descnode* devnode;
	struct DeviceDescriptor* devdesc;
	struct descnode* confnode;
	struct ConfigurationDescriptor* confdesc;
	//per interface
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;
	struct descnode* hidnode;
	struct HIDDescriptor* hiddesc;


//------------------------basic information------------------------
	perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	if(0 == perusb->devnode)return -1;		//no devdesc?
	devnode = (void*)perusb + perusb->devnode;
	devdesc = (void*)perusb + devnode->real;

	if(0 == devnode->lchild)return -2;		//no confdesc?
	confnode = (void*)perusb + perusb->confnode;
	confdesc = (void*)perusb + confnode->real;


//------------------------check type------------------------
	if(3 == intfdesc->bInterfaceClass){
		say("[usbhid]this is hid device\n");
	}
	else{
		say("[usbhid]non-hid, byebye\n");
		return -4;
	}

	if(1 == intfdesc->bInterfaceSubClass){
		say("[usbhid]bootmode\n");
	}
	else{
		say("[usbhid]reportmode, byebye\n");
		return -5;
	}

	if(1 == intfdesc->bInterfaceProtocol){		//keyboard
		say("[usbhid]keyboard\n");
	}
	else if(2 == intfdesc->bInterfaceProtocol){	//mouse
		say("[usbhid]mouse\n");
	}
	else{
		say("[usbhid]proto=%x, unknown, byebye\n",intfdesc->bInterfaceProtocol);
		return -6;
	}


//------------------------host side + my parse------------------------
	int epaddr = 3;
	int pktlen = 0x81;

	j = intfnode->lchild;
	while(1){
		if(0 == j)break;
		endpnode = (void*)perusb + j;
		endpdesc = (void*)perusb + endpnode->real;

		switch(endpdesc->bDescriptorType){
		case 5:{
			epaddr = (endpdesc->bEndpointAddress & 0xf) << 1;
			if(endpdesc->bEndpointAddress & 0x80)epaddr += 1;
			pktlen = endpdesc->wMaxPacketSize;
			say("[usbhid]endpdesc: addr=%x, attr=%x, pktlen=%x, interval=%x\n",
				endpdesc->bEndpointAddress, endpdesc->bmAttributes,
				endpdesc->wMaxPacketSize, endpdesc->bInterval
			);
			if(1){
				//inform the xHC of the value of the Max Exit Latency parameter
				//ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EvaluateContext, buf,8, 0,0);
				//if(ret < 0)return -9;

				//inform the xHC of the endpoint
				ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_ConfigureEndpoint, endpdesc,sizeof(struct EndpointDescriptor), 0,0);
				if(ret < 0)return -9;
			}
			break;
		}//ep desc
		case 0x21:{
			hidnode = (void*)endpnode;
			hiddesc = (void*)endpdesc;
			say("[usbhid]hiddesc: country=%x, numdesc=%x, reporttype=%x, reportlen=%x\n",
				hiddesc->bCountryCode, hiddesc->bNumDescriptors,
				hiddesc->bReportDescType, hiddesc->wReportDescLength
			);
			break;
		}//hid desc
		default:{
			say("[usbhid]desctype=%x\n", endpdesc->bDescriptorType);
		}//report desc?
		}//switch

		j = endpnode->rfellow;
	}


//------------------------device side------------------------
	say("[usbhid]set_config\n");
	DEVICE_REQUEST_SET_CONFIGURATION(&req, confdesc->bConfigurationValue);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, 0,0);
	if(ret < 0)return -10;
/*
	INTERFACE_REQUEST_SET_INTERFACE(&req, my->intf, 0);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf,req.wLength);
	if(4 != ret)return -11;
*/
	say("[usbhid]making trb\n");
	ret = xhci_giveorderwaitevent(xhci,slot|(epaddr<<8), 'd',0, 0,0, perusb->freebuf,pktlen);

	return 0;
}