#include "libhard.h"
#include "drv/usb.h"
void H2D_STD_DEV_SETCONF(void* req, u16 conf);
void D2H_STD_INTF_GETDESC(void* req, u16 intf, u16 typeindex, u16 len);
void H2D_CLASS_INTF_SETIDLE(struct UsbRequest* req, u16 intf, u16 val);

int usbdesc_addr2offs(struct perusb* perusb, void* desc);
void* usbdesc_offs2addr(struct perusb* perusb, int offs);




struct xbox360report{
	u8 msgtype;	//0: 0x00
	u8 msglen;	//1: 0x14

	u8 dpad_u:1;	//2
	u8 dpad_d:1;
	u8 dpad_l:1;
	u8 dpad_r:1;
	u8  start:1;
	u8   back:1;
	u8     l3:1;
	u8     r3:1;

	u8     lb:1;	//3
	u8     rb:1;
	u8   xbox:1;
	u8   rsvd:1;
	u8 kpad_a:1;
	u8 kpad_b:1;
	u8 kpad_x:1;
	u8 kpad_y:1;

	u8 lt;	//4
	u8 rt;	//5

	short lx;	//[6,7]
	short ly;	//[8,9]
	short rx;	//[10,11]
	short ry;	//[12,13]
}__attribute__((packed));
struct xbox360command_rumble{
	u8 cmdtype;	//0: 0x00
	u8 cmdlen;	//1: 0x08
	u8 rsvd2;
	u8 rumble_left;
	u8 rumble_right;
	u8 rsvd5;
	u8 rsvd6;
	u8 rsvd7;
}__attribute__((packed));
struct xbox360command_led{
	u8 cmdtype;	//0: 0x00
	u8 cmdlen;	//1: 0x03
	u8 led;
}__attribute__((packed));




struct xboxonereport{
	u8 type;	//0
	u8 unknown;		//1

	u16 id;			//[2,3]

	u8   sync:1;	//4
	u8   what:1;
	u8  start:1;
	u8   back:1;
	u8 kpad_a:1;
	u8 kpad_b:1;
	u8 kpad_x:1;
	u8 kpad_y:1;

	u8 dpad_u:1;	//5
	u8 dpad_d:1;
	u8 dpad_l:1;
	u8 dpad_r:1;
	u8     lb:1;
	u8     rb:1;
	u8     ls:1;
	u8     rs:1;

	u16 lt;		//[6,7]
	u16 rt;		//[8,9]

	short lx;		//[a.b]
	short ly;		//[c,d]
	short rx;		//[e,f]
	short ry;		//[10,11]
}__attribute__((packed));
struct xboxonecommand{
	u8 what;
}__attribute__((packed));




struct perhid{
	u8 stat;
};
struct perxbox{
	union{
		struct perhid hid;
		u8 hid_padding[0x1000];
	};
	u8 trb[0];
}__attribute__((packed));




static int xboxhid_ongive(struct item* usb,int xxx, struct item* xhci,int endp, p64 arg,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perxbox* perxbox = (void*)perusb->perfunc;
	struct perhid* perhid = (void*)&perxbox->hid;

	void** sbuf = (void**)arg;
	struct xboxonereport* data = *sbuf;
	//printmemory(data, 0x40);

	int notold, now;
	switch(data->type){
	case 0x01:	//invalid op data
	case 0x02:	//waiting for connection
	case 0x03:	//heart beat
	case 0x07:	//guide button status
		break;
	case 0x20:	//button data
		//logtoall("l=%x,r=%x,d=%x,u=%x,a=%x,b=%x,x=%x,y=%x\n",data->dpad_l,data->dpad_r,data->dpad_d,data->dpad_u,data->kpad_a,data->kpad_b,data->kpad_x,data->kpad_y);
		//logtoall("lb=%d,rb=%d,ls=%d,rs=%d,back=%d,start=%d\n",data->lb,data->rb,data->ls,data->rs,data->back,data->start);
		//logtoall("lx=%d,ly=%d,rx=%d,ry=%d,lt=%d,rt=%d\n",data->lx,data->ly,data->rx,data->ry,data->lt,data->rt);

		//key
		notold = ~perhid->stat;
		now = data->dpad_l|(data->dpad_r<<1)|(data->dpad_d<<2)|(data->dpad_u<<3);
		if( (now&1) && (notold&1) )eventwrite(0x4b, _kbd_, 0, 0);
		if( (now&2) && (notold&2) )eventwrite(0x4d, _kbd_, 0, 0);
		if( (now&4) && (notold&4) )eventwrite(0x50, _kbd_, 0, 0);
		if( (now&8) && (notold&8) )eventwrite(0x48, _kbd_, 0, 0);
		perhid->stat = now;

		//stick
		short xx[4];
		xx[0] = data->lx;
		if((xx[0] < -0x800)|(xx[0] > 0x800))xx[0] /= 0x800;
		else xx[0] = 0;
		xx[1] = 0 - data->ly;
		if((xx[1] < -0x800)|(xx[1] > 0x800))xx[1] /= 0x800;
		else xx[1] = 0;
		if(xx[0] | xx[1])eventwrite(*(u64*)xx, point_dlt, 0, 0);
		break;
	}

	return 0;
}
int xboxhid_driver(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
{
	//per device
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perxbox* perxbox = (void*)perusb->perfunc;
	//if(0 == perxbox)return 0;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	struct descnode* confnode = &perusb->parsed.node[0];
	struct ConfigurationDescriptor* confdesc = usbdesc_offs2addr(perusb, confnode->real);

	logtoall("[xbox]intf=%x,alt=%x,epcnt=%x\n",intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting, intfdesc->bNumEndpoints);
	if(0 == intfdesc->bNumEndpoints){
		logtoall("[xbox]bNumEndpoints=0, trying next\n");
		return 0;
	}


//------------------------host side + my parse------------------------
	//per interface
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;
	int j,ret;
	struct UsbRequest req;
	int outaddr = 2;
	int inaddr = 3;
	int pktlen = 0x40;

	j = intfnode->lchild;
	while(1){
		if(0 == j)break;
		endpnode = (void*)perusb + j;
		endpdesc = (void*)perusb + endpnode->real;

		switch(endpdesc->bDescriptorType){
		case 5:{
			if(3 != endpdesc->bmAttributes){
				logtoall("[xbox]bmAttributes=%x, trying next\n", endpdesc->bmAttributes);
				return 0;
			}

			ret = (endpdesc->bEndpointAddress & 0xf) << 1;
			if(0 == (endpdesc->bEndpointAddress & 0x80)){
				outaddr = ret;
			}
			else{
				inaddr = ret + 1;
			}

			pktlen = endpdesc->wMaxPacketSize;
			logtoall("[xboxhid]endpdesc: addr=%x, attr=%x, pktlen=%x, interval=%x\n",
				endpdesc->bEndpointAddress, endpdesc->bmAttributes,
				endpdesc->wMaxPacketSize, endpdesc->bInterval
			);

			if(1){
				//inform the xHC of the value of the Max Exit Latency parameter
				//ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EvaluateContext, buf,8, 0,0);
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
		default:{
			logtoall("[xboxhid]desctype=%x\n", endpdesc->bDescriptorType);
		}//report desc?
		}//switch

		j = endpnode->rfellow;
	}


//------------------------device side------------------------
	logtoall("[xboxhid]set_config\n");
	H2D_STD_DEV_SETCONF(&req, confdesc->bConfigurationValue);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		0,0
	);
	if(ret < 0)return -10;


//------------------------start work------------------------
	logtoall("[xboxhid]start_work\n");
	u8 tmp[8];
	tmp[0] = 0x05;
	tmp[1] = 0x20;
	tmp[2] = 0x00;
	tmp[3] = 0x01;
	tmp[4] = 0x00;
	ret = xhci->give_pxpxpxpx(
		xhci,slot|(outaddr<<8),
		0,0,
		(p64)tmp,5,
		0,0
	);


//------------------------transfer ring------------------------
	logtoall("[xboxhid]making trb\n");
	usb->ongiving = (void*)xboxhid_ongive;

	if(pktlen > 0x40)pktlen = 0x40;
	ret = xhci->give_pxpxpxpx(
		xhci, slot|(inaddr<<8),
		0, 0,
		(p64)perxbox->trb, pktlen,
		usb,0
	);
	return 0;
}
int usbxbox_driver(struct item* usb, int xxx, struct item* xhci, int slot)
{
	struct perusb* perusb = usb->priv_ptr;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	struct descnode* confnode = &perusb->parsed.node[0];
	struct ConfigurationDescriptor* confdesc = usbdesc_offs2addr(perusb, confnode->real);

	struct descnode* intfnode;
	struct InterfaceDescriptor* intfdesc;
	if(0 == confnode->lchild)return -3;		//no intfdesc?
	intfnode = (void*)perusb + confnode->lchild;
	intfdesc = (void*)perusb + intfnode->real;

	//check version
	switch(devdesc->idProduct){
	case 0x0202:logtoall("xbox\n");break;
	case 0x0285:
	case 0x0289:logtoall("xbox-s\n");break;
	case 0x028e:logtoall("xbox360\n");break;
	case 0x028f:logtoall("xbox360-wireless\n");break;
	case 0x02d1:logtoall("xboxone\n");break;
	case 0x02dd:logtoall("xboxone-2015\n");break;
	case 0x02e3:logtoall("xboxone-elite\n");break;
	case 0x02e6:logtoall("xbox-dongle\n");break;
	case 0x02ea:logtoall("xboxones\n");break;
	}

	//composite device, all interface
	while(1){
		logtoall("[xbox]interface: node=%p,desc=%p, num=%x, alt=%x, c=%x,s=%x,p=%x\n",
			intfnode,intfdesc, intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting,
			intfdesc->bInterfaceClass, intfdesc->bInterfaceSubClass, intfdesc->bInterfaceProtocol);

		xboxhid_driver(usb,xxx, xhci,slot, intfnode,intfdesc);

		if(0 == intfnode->rfellow)break;
		intfnode = (void*)perusb + intfnode->rfellow;
		intfdesc = (void*)perusb + intfnode->real;
	}

	usb->kind = _usb_;
	usb->type = _gpad_;
	usb->vfmt = _xbox_;
	return 0;
}
