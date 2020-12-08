#include "libhard.h"
#include "usb.h"
void eventwrite(u64,u64,u64,u64);
void DEVICE_REQUEST_SET_CONFIGURATION(void* req, u16 conf);
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
//error
#define NoKeyPress 0
#define InvalidScanCode 1
#define SelfTestFail 2
#define UndefinedError 3
static u8 usb2kbd[0x100] = {
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,0x4d,
0x4b,0x50,0x48
};
static u8 usb2char[0x100] = {
   0,   0,   0,   0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',
 '3', '4', '5', '6', '7', '8', '9', '0', 0xd,0x1b, 0x8,   0, ' ', '-', '=', '[',
 ']','\\',   0, ';','\'', '`', ',', '.', '/'
};
static u8 usb2shift[0x100] = {
   0,   0,   0,   0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@',
 '#', '$', '%', '^', '&', '*', '(', ')', 0xd,0x1b, 0x8,   0, ' ', '_', '+', '{',
 '}', '|',   0, ':','\"', '~', '<', '>', '?'
};
//[A,B,C...Z]: [4, 29]
#define USBKEY_A 4
#define USBKEY_Z 29
//[1,2,3...0]: [30, 39]
#define USBKEY_1 30
#define USBKEY_0 39
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




void INTERFACE_REQUEST_GET_REPORT_DESC(struct UsbRequest* req, u16 intf, u16 typeindex, u16 len)
{
	req->bmRequestType = 0x81;
	req->bRequest = 6;
	req->wValue = typeindex;
	req->wIndex = intf;
	req->wLength = len;
}
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




static int parsekeyboard(struct report_keyboard* report)
{
	int j,k,v;
	for(j=0;j<6;j++){
		k = report->keys[j];
		if(k < 4)continue;

		v = usb2kbd[k];
		if(0 != v){
			//say("[usbkbd]%x->kbd %x\n", k, v);
			eventwrite(v, _kbd_, 0, 0);
			continue;
		}

		v = usb2char[k];
		if(0 != v){
			//say("[usbkbd]%x->char %x\n", k, v);
			eventwrite(v, _char_, 0, 0);
			continue;
		}
	}
	return 0;
}
static int parsemouse(struct report_mouse* report)
{
	//say("[usbmouse]btn=%x,dx=%d,dy=%d\n", report->btn, report->dx, report->dy);

	short xx[4];
	xx[0] = report->dx;
	xx[1] = report->dy;
	eventwrite(*(u64*)xx, 0x4070, 0, 0);
	return 0;
}
static int usbhid_ongive(struct item* usb,int xxx, struct item* xhci,int endp, void* sbuf,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct descnode* intfnode = (void*)perusb + perusb->my.intfnode;
	struct InterfaceDescriptor* intfdesc = (void*)perusb + intfnode->real;

	void* data = *(void**)sbuf;
	if(1 == intfdesc->bInterfaceProtocol){
		parsekeyboard(data);
	}
	else if(2 == intfdesc->bInterfaceProtocol){
		parsemouse(data);
	}
	else{
		printmemory(data, 8);
	}
	return 0;
}
int usbhid_driver(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
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

	if(0 == perusb->my.devnode)return -1;		//no devdesc?
	devnode = (void*)perusb + perusb->my.devnode;
	devdesc = (void*)perusb + devnode->real;

	if(0 == devnode->lchild)return -2;		//no confdesc?
	confnode = (void*)perusb + perusb->my.confnode;
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
	int outaddr = 2;
	int inaddr = 3;
	int pktlen = 8;

	j = intfnode->lchild;
	while(1){
		if(0 == j)break;
		endpnode = (void*)perusb + j;
		endpdesc = (void*)perusb + endpnode->real;

		switch(endpdesc->bDescriptorType){
		case 5:{
			ret = (endpdesc->bEndpointAddress & 0xf) << 1;
			if(0 == (endpdesc->bEndpointAddress & 0x80)){
				outaddr = ret;
			}
			else{
				inaddr = ret + 1;
			}

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
	INTERFACE_REQUEST_SET_PROTOCOL(&req, 0, 0);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, 0,0);
	if(ret < 0)return -11;

	say("[usbhid]set_config\n");
	DEVICE_REQUEST_SET_CONFIGURATION(&req, confdesc->bConfigurationValue);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, 0,0);
	if(ret < 0)return -10;


//------------------------transfer ring------------------------
	say("[usbhid]making trb\n");
	usb->ongiving = (void*)usbhid_ongive;

	if(pktlen > 0x40)pktlen = 0x40;
	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, perusb->freebuf,pktlen, usb,0);
	return 0;
}
