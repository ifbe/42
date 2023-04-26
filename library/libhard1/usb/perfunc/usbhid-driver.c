#include "libhard.h"
#include "drv/usb.h"
void H2D_STD_DEV_SETCONF(void* req, u16 conf);
void D2H_STD_INTF_GETDESC(struct UsbRequest* req, u16 intf, u16 typeindex, u16 len);

int usbdesc_addr2offs(struct perusb* perusb, void* desc);
void* usbdesc_offs2addr(struct perusb* perusb, int offs);
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
//#define MOUSE_LEFTBUTTON
//#define MOUSE_RIGHTBUTTON
//#define MOUSE_MIDDLEBUTTON
struct report_mouse{
	u8 btn;
	char dx;
	char dy;
}__attribute__((packed));




struct perkbd{
	int modifer;
}__attribute__((packed));

struct permouse{
	int button;
}__attribute__((packed));

struct perfunc{		//0x10000 byte per func
	union{
		struct perkbd perkbd;
		struct permouse permouse;
		u8 padding[0x1000];
	};
	u8 trb[0xf000];
}__attribute__((packed));




void H2D_CLASS_INTF_SETREPORT(struct UsbRequest* req, u16 intf, u16 len)
{
	req->bmRequestType = 0x21;
	req->bRequest = bRequest_SET_REPORT;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = len;
}
void H2D_CLASS_INTF_SETIDLE(struct UsbRequest* req, u16 intf, u16 val)
{
	req->bmRequestType = 0x21;
	req->bRequest = bRequest_SET_IDLE;
	req->wValue = val;	//duration and report id
	req->wIndex = intf;
	req->wLength = 0;
}
void H2D_CLASS_INTF_SETPROTOCOL(struct UsbRequest* req, u16 intf, u16 val)
{
	req->bmRequestType = 0x21;
	req->bRequest = bRequest_SET_PROTOCOL;
	req->wValue = val;	//duration and report id
	req->wIndex = intf;
	req->wLength = 0;
}
void D2H_CLASS_INTF_GETREPORT(struct UsbRequest* req, u16 intf, u16 len)
{
	req->bmRequestType = 0xa1;
	req->bRequest = bRequest_GET_REPORT;
	req->wValue = 0;	//report type and report id: 1=input, 2=output, 3=feature
	req->wIndex = intf;
	req->wLength = len;	//report length
}
void D2H_CLASS_INTF_GETIDLE(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0xa1;
	req->bRequest = bRequest_GET_IDLE;
	req->wValue = 0;	//0 and report id
	req->wIndex = intf;
	req->wLength = 1;
}
void D2H_CLASS_INTF_GETPROTOCOL(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0xa1;
	req->bRequest = bRequest_GET_PROTOCOL;
	req->wValue = 0;	//0 and report id
	req->wIndex = intf;
	req->wLength = 1;
}




int printreport_getvalue(u8* buf, int len)
{
	int j;
	int val = 0;
	for(j=0;j<len;j++){
		val |= buf[j]<<(j*8);
	}
	return val;
}
void printreport(u8* buf, int len)
{
	int j = 0;
	int type,size;
	while(j<len){
		type = buf[j] & 0xfc;
		size = buf[j] & 0x3;
		switch(type){
		case 0x04:
			say("USAGE_PAGE%d: %x\n", size, buf[j+1]);
			break;
		case 0x08:
			say("USAGE%d: %x\n", size, buf[j+1]);
			break;
		case 0x14:
			say("LOGICAL_MINIMUM%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0x18:
			say("USAGE_MINIMUM%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0x24:
			say("LOGICAL_MAXIMUM%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0x28:
			say("USAGE_MAXIMUM%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0x34:
			say("PHYSICAL_MINIMUM%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0x38:
			say("DESINATOR_INDEX%d: %x\n", size, buf[j+1]);
			break;
		case 0x44:
			say("PHYSICAL_MAXIMUM%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0x48:
			say("DESINATOR_MINIMUM%d: %x\n", size, buf[j+1]);
			break;
		case 0x54:
			say("UNIT_EXPONENT%d: %x\n", size, buf[j+1]);
			break;
		case 0x58:
			say("DESINATOR_MAXIMUM%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0x64:
			say("UNIT%d: %x\n", size, buf[j+1]);
			break;
		case 0x74:
			say("REPORT_SIZE%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0x78:
			say("STRING_INDEX%d: %x\n", size, buf[j+1]);
			break;
		case 0x80:
			say("INPUT%d: %x\n", size, buf[j+1]);
			break;
		case 0x84:
			say("REPORT_ID%d: %x\n", size, buf[j+1]);
			break;
		case 0x88:
			say("STRING_MINIMUM%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0x90:
			say("OUTPUT%d: %x\n", size, buf[j+1]);
			break;
		case 0x94:
			say("REPORT_COUNT%d: %x\n", size, buf[j+1]);
			break;
		case 0x98:
			say("STRING_MAXIMUM%d: %x\n", size, printreport_getvalue(buf+j+1, size));
			break;
		case 0xa0:
			say("COLLECTION%d: %x\n", size, buf[j+1]);
			break;
		case 0xa4:
			say("PUSH%d: %x\n", size, buf[j+1]);
			break;
		case 0xa8:
			say("DELIMITER%d: %x\n", size, buf[j+1]);
			break;
		case 0xb0:
			say("FEATURE%d: %x\n", size, buf[j+1]);
			break;
		case 0xb4:
			say("POP%d: %x\n", size, buf[j+1]);
			break;
		case 0xc0:
			say("END_COLLECTION%d: %x\n", size, buf[j+1]);
			break;
		default:
			say("??: %02x\n", buf[j]);
		}

		j += 1+size;
	}
}




static int parsekeyboard(struct item* usb,int xxx, struct item* xhci,int endp,
	void* sbuf,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	struct report_keyboard* report = *(void**)sbuf;
	perfunc->perkbd.modifer = report->modifier;

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
static int parsemouse(struct item* usb,int xxx, struct item* xhci,int endp,
	void* sbuf,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	struct report_mouse* report = *(void**)sbuf;
	//say("[usbmouse]btn=%x,dx=%d,dy=%d\n", report->btn, report->dx, report->dy);
	//printmemory(in, 

	u64 type = point_dlt;
	if(perfunc->permouse.button != report->btn){
		say("mouse key: old=%x,new=%x\n",perfunc->permouse.button, report->btn);
		if(report->btn)type = point_onto;
		else type = point_away;

		perfunc->permouse.button = report->btn;
	}

	short xx[4];
	xx[0] = report->dx;
	xx[1] = report->dy;
	eventwrite(*(u64*)xx, type, 0, 0);
	return 0;
}
static int parsemouse_g502(struct item* usb,int xxx, struct item* xhci,int endp,
	void* sbuf,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	char* in = *(void**)sbuf;
	//say("[usbmouse]btn=%x,dx=%d,dy=%d\n", in[0], in[1], in[2]);

	u64 type = point_dlt;
	if(perfunc->permouse.button != in[0]){
		say("mouse key: old=%x,new=%x\n",perfunc->permouse.button, in[0]);
		if(in[0])type = point_onto;
		else type = point_away;

		perfunc->permouse.button = in[0];
	}

	short xx[4];
	xx[0] = in[2];
	xx[1] = in[4];
	eventwrite(*(u64*)xx, type, 0, 0);
	return 0;
}
static int parsemouse_qemu(struct item* usb,int xxx, struct item* xhci,int endp,
	void* sbuf,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	char* in = *(void**)sbuf;
	//say("[usbmouse]btn=%x,dx=%d,dy=%d\n", in[0], in[1], in[2]);

	u64 type = point_per;
	if(perfunc->permouse.button != in[0]){
		say("mouse key: old=%x,new=%x\n",perfunc->permouse.button, in[0]);
		if(in[0])type = point_onto;
		else type = point_away;

		perfunc->permouse.button = in[0];
	}

	short* pos = (void*)(in+1);
	//say("%x,%x\n", pos[0], pos[1]);

	short xx[4];
	xx[0] = pos[0];
	xx[1] = pos[1];
	eventwrite(*(u64*)xx, type, 0, 0);
	return 0;
}
static int parsemouse_vbox(struct item* usb,int xxx, struct item* xhci,int endp,
	void* sbuf,int slen, void* rbuf,int rlen)
{
	//must do:
	//1.install virtualbox extension pack
	//2.settings -> usbdevice -> xhci controller
	//3.settings -> pointer device -> usb tablet
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	char* in = *(void**)sbuf;
	//say("[usbmouse]btn=%x,dx=%d,dy=%d\n", in[0], in[1], in[2]);

	u64 type = point_per;
	if(perfunc->permouse.button != in[0]){
		say("mouse key: old=%x,new=%x\n",perfunc->permouse.button, in[0]);
		if(in[0])type = point_onto;
		else type = point_away;

		perfunc->permouse.button = in[0];
	}

	short* pos = (void*)(in+4);
	//say("%x,%x\n", pos[0], pos[1]);

	short xx[4];
	xx[0] = pos[0];
	xx[1] = pos[1];
	eventwrite(*(u64*)xx, type, 0, 0);
	return 0;
}
static int usbhid_ongive(struct item* usb,int xxx, struct item* xhci,int endp, void* sbuf,int slen, void* rbuf,int rlen)
{
	void* data = *(void**)sbuf;
	printmemory(data, 16);
	return 0;
}
int usbhid_driver(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
{
	//per device
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc;
	//if(0 == perfunc)return 0;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	struct descnode* confnode = &perusb->parsed.node[0];
	struct ConfigurationDescriptor* confdesc = usbdesc_offs2addr(perusb, confnode->real);

//------------------------check type------------------------
	if(3 == intfdesc->bInterfaceClass){
		say("[usbhid]this is hid device\n");
	}
	else{
		say("[usbhid]non-hid, byebye\n");
		return -4;
	}

	if(1 == intfdesc->bInterfaceProtocol){		//keyboard
		say("[usbhid]keyboard\n");
	}
	else if(2 == intfdesc->bInterfaceProtocol){	//mouse
		say("[usbhid]mouse\n");
	}
	else{
		if( (0x0627 == devdesc->idVendor) && (0x0001 == devdesc->idProduct) ){
			say("qemu: mouse\n");
		}
		else if( (0x80ee == devdesc->idVendor) && (0x0021 == devdesc->idProduct) ){
			say("vbox: tablet\n");
		}
		else{
			say("[usbhid]proto=%x, unknown, byebye\n",intfdesc->bInterfaceProtocol);
			return -6;
		}
	}

	if(1 == intfdesc->bInterfaceSubClass){
		say("[usbhid]bootmode\n");
	}
	else{
		if( (0x0627 == devdesc->idVendor) && (0x0001 == devdesc->idProduct) ){
			say("qemu: unknown mode\n");
		}
		else if( (0x80ee == devdesc->idVendor) && (0x0021 == devdesc->idProduct) ){
			say("vbox: unknown mode\n");
		}
		else{
			say("[usbhid]reportmode, byebye\n");
			return -5;
		}
	}


//------------------------host side + my parse------------------------
	//per interface
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;
	struct descnode* hidnode;
	struct HIDDescriptor* hiddesc;

	int j,ret;
	struct UsbRequest req;
	int outaddr = 2;
	int inaddr = 3;
	int pktlen = 8;

	j = intfnode->lchild;
	while(1){
		if(0 == j)break;
		endpnode = usbdesc_offs2addr(perusb, j);
		endpdesc = usbdesc_offs2addr(perusb, endpnode->real);

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
				//ret = xhci->ongiving(xhci,slot, 'h',TRB_command_EvaluateContext, buf,8, 0,0);
				//if(ret < 0)return -9;

				//inform the xHC of the endpoint
				ret = xhci->give_pxpxpxpx(
					xhci,slot,
					0,0,
					0,_tohc_conf_,
					endpdesc,sizeof(struct EndpointDescriptor)
				);
				if(ret < 0)return -9;
			}
			break;
		}//ep desc
		case 0x21:{
			hidnode = (void*)endpnode;
			hiddesc = (void*)endpdesc;
			say("[usbhid]hiddesc: country=%x, numdesc=%x, reportdesctype=%x, reportdesclen=%x\n",
				hiddesc->bCountryCode, hiddesc->bNumDescriptors,
				hiddesc->bReportDescType, hiddesc->wReportDescLength
			);

			say("get reportdesc\n");
			void* temp = usbdesc_offs2addr(perusb, perusb->origin.byteused);
			D2H_STD_INTF_GETDESC(&req,
				intfdesc->bInterfaceNumber,
				0 | (hiddesc->bReportDescType<<8),
				hiddesc->wReportDescLength
			);
			ret = xhci->give_pxpxpxpx(
				xhci,slot,
				0,0,
				&req,8,
				temp,hiddesc->wReportDescLength
			);
			if(ret >= 0){
				//printmemory(temp,hiddesc->wReportDescLength);
				printreport(temp,hiddesc->wReportDescLength);
			}
			break;
		}//hid desc
		default:{
			say("[usbhid]desctype=%x\n", endpdesc->bDescriptorType);
		}//report desc?
		}//switch

		j = endpnode->rfellow;
	}


//------------------------device side------------------------
/*	if(	(2 == intfdesc->bInterfaceProtocol)&&
		(0x0461 == devdesc->idVendor)&&
		(0x4d81 == devdesc->idProduct)){		//special mouse must set protocol
	if(	(1 == intfdesc->bInterfaceProtocol)&&
		(0x04d9 == devdesc->idVendor)&&
		(0x1702 == devdesc->idProduct)){		//special keyboard must set protocol
*/
	int need_to_set_protocol = 1;
	if( (0x0627 == devdesc->idVendor) && (0x0001 == devdesc->idProduct) ){
		need_to_set_protocol = 0;
	}
	if( (0x80ee == devdesc->idVendor) && (0x0021 == devdesc->idProduct) ){
		need_to_set_protocol = 0;
	}
	if(2 == intfdesc->bInterfaceProtocol){
		if( (0x046d == devdesc->idVendor) && (0xc092 == devdesc->idProduct) ){	//g502
			need_to_set_protocol = 0;
		}
	}
	if(need_to_set_protocol){
		say("[usbhid]set_protocol\n");
		H2D_CLASS_INTF_SETPROTOCOL(&req, intfdesc->bInterfaceNumber, 0);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			0,0
		);
		if(ret < 0)return -11;
	}

	int need_to_set_configuration = 1;
	if(need_to_set_configuration){
		say("[usbhid]set_config\n");
		H2D_STD_DEV_SETCONF(&req, confdesc->bConfigurationValue);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			0,0
		);
		//ignore if not stall error
		//if(ret < 0)return -10;
	}

//------------------------callback------------------------
	usb->ongiving = (void*)usbhid_ongive;
	if(0 == intfdesc->bInterfaceProtocol){
		if( (0x0627 == devdesc->idVendor) && (0x0001 == devdesc->idProduct) ){
			usb->ongiving = (void*)parsemouse_qemu;
		}
		if( (0x80ee == devdesc->idVendor) && (0x0021 == devdesc->idProduct) ){
			usb->ongiving = (void*)parsemouse_vbox;
		}
	}
	else if(1 == intfdesc->bInterfaceProtocol){
		usb->ongiving = (void*)parsekeyboard;
	}
	else if(2 == intfdesc->bInterfaceProtocol){
		if( (0x046d == devdesc->idVendor) && (0xc092 == devdesc->idProduct) ){
			usb->ongiving = (void*)parsemouse_g502;
		}
		else{
			usb->ongiving = (void*)parsemouse;
		}
	}

//------------------------transfer ring------------------------
	say("[usbhid]making trb@%p\n", perfunc->trb);
	if(pktlen > 0x40)pktlen = 0x40;
	ret = xhci->give_pxpxpxpx(
		xhci, slot|(inaddr<<8),
		0, 0,
		perfunc->trb, pktlen,
		usb, 0
	);
	return 0;
}
