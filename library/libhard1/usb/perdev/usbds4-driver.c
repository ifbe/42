#include "libhard.h"
//#include "libuser.h"
#include "drv/usb.h"
void H2D_STD_DEV_SETCONF(void* req, u16 conf);
void D2H_STD_INTF_GETDESC(void* req, u16 intf, u16 typeindex, u16 len);
void H2D_CLASS_INTF_SETIDLE(struct UsbRequest* req, u16 intf, u16 val);

int usbdesc_addr2offs(struct perusb* perusb, void* desc);
void* usbdesc_offs2addr(struct perusb* perusb, int offs);
void printreport(void*, int);



struct dsfourreport{
	u8 ReportID;	//0

	u8   lx;	//1
	u8   ly;	//2
	u8   rx;	//3
	u8   ry;	//4

	u8 dpad:4;	//5: 0=N,1=NE,2=E,3=SE,4=S,5=SW,6=W,7=NW,8=reseased
	u8    x:1;
	u8    a:1;
	u8    b:1;
	u8    y:1;

	u8    l1:1;	//6
	u8    r1:1;
	u8    l2:1;
	u8    r2:1;
	u8 share:1;
	u8   opt:1;
	u8    l3:1;
	u8    r3:1;

	u8    ps:1;	//7
	u8  tpad:1;
	u8 count:6;

	u8 lt;	//8
	u8 rt;	//9

	u8 unknown_10;	//a
	u8 unknown_11;	//b
	u8 unknown_12;	//c

	short gx;	//[ d, e]
	short gy;	//[ f,10]
	short gz;	//[11,12]
	short ax;	//[13,14]
	short ay;	//[15,16]
	short az;	//[17,18]
}__attribute__((packed));

struct dsfivereport{
	u8 ReportID;	//0: 0x1

	u8   lx;	//1: 0=left,ff=right
	u8   ly;	//2: 0=up,ff=down
	u8   rx;	//3: 0=left,ff=right
	u8   ry;	//4: 0=up,ff=down

	u8 lt;		//5
	u8 rt;		//6
	u8 byte7;	//7

	u8 dpad:4;	//8: 0=N,1=NE,2=E,3=SE,4=S,5=SW,6=W,7=NW,8=reseased
	u8    x:1;
	u8    a:1;
	u8    b:1;
	u8    y:1;

	u8    l1:1;	//9
	u8    r1:1;
	u8    l2:1;
	u8    r2:1;
	u8 share:1;
	u8   opt:1;
	u8    l3:1;
	u8    r3:1;

	u8 mouse;	//a
	u8 byteb;	//b
	u32 time;	//c-f

	short gx;	//10
	short gy;	//12
	short gz;	//14
	short ax;	//16
	short ay;	//18
	short az;	//1a
	u32 what;	//1c-1f

}__attribute__((packed));




struct perhid{
	u8 stat;
};
struct peraudio{
};
struct perds{
	union{
		struct perhid hid;
		u8 hid_padding[0x1000];
	};
	union{
		struct peraudio audio;
		u8 audio_padding[0x1000];
	};
	u8 trb[0xe000];
}__attribute__((packed));




//1=l,2=r,4=d,8=u
//0=N,1=NE,2=E,3=SE,4=S,5=SW,6=W,7=NW,8=reseased
u8 dpadtable[8] = {0x8,0xa,0x2,0x6,0x4,0x5,0x1,0x9};
static u8 dpad2lrdu(u8 dpad)
{
	u8 val = 0;
	if(dpad >= 8)return 0;
	return dpadtable[dpad];
/*	if((dpad >= 5)&&(dpad <= 7))val |= 1;
	if((dpad >= 1)&&(dpad <= 3))val |= 2;
	if((dpad >= 3)&&(dpad <= 5))val |= 4;
	if((0==dpad)|(1==dpad)|(7==dpad))val |= 8;*/
}
static int dsfourhid_ongive(struct item* usb,int xxx, struct item* xhci,int endp, p64 arg,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perds* perds = (void*)perusb->perfunc;
	struct perhid* perhid = (void*)&perds->hid;
	u8 notold = ~perhid->stat;

	void** sbuf = (void**)arg;
	struct dsfourreport* data = *sbuf;
	//printmemory(data, 0x10);

	//key
	u8 now = dpad2lrdu(data->dpad);
	if( (now&1) && (notold&1) )eventwrite(0x4b, _kbd_, 0, 0);
	if( (now&2) && (notold&2) )eventwrite(0x4d, _kbd_, 0, 0);
	if( (now&4) && (notold&4) )eventwrite(0x50, _kbd_, 0, 0);
	if( (now&8) && (notold&8) )eventwrite(0x48, _kbd_, 0, 0);
	perhid->stat = now;

	//stick
	short xx[4];
	xx[0] = data->lx - 0x80;
	if((xx[0] < -8)|(xx[0] > 8))xx[0] /= 16;
	else xx[0] = 0;
	xx[1] = data->ly - 0x80;
	if((xx[1] < -8)|(xx[1] > 8))xx[1] /= 16;
	else xx[1] = 0;
	if(xx[0] | xx[1])eventwrite(*(u64*)xx, point_dlt, 0, 0);
/*
	logtoall("l=%d,r=%d,d=%d,u=%d,a=%d,b=%d,x=%d,y=%d\n",lrdu[0],lrdu[1],lrdu[2],lrdu[3],data->a,data->b,data->x,data->y);
	logtoall("l1=%d,r1=%d,l2=%d,r2=%d,l3=%d,r3=%d, share=%d,opt=%d,ps=%d\n",data->l1,data->r1,data->l2,data->r2,data->l3,data->r3,  data->share,data->opt,data->ps);
	logtoall("lx=%d,ly=%d,rx=%d,ry=%d,lt=%d,rt=%d\n",data->lx,255-data->ly,data->rx,255-data->ry,data->lt,data->rt);
	logtoall("gx=%d,gy=%d,gz=%d, ax=%d,ay=%d,az=%d\n",data->gx,-data->gz,data->gy,  -data->ax,data->az,-data->ay);*/
	return 0;
}
static int dsfivehid_ongive(struct item* usb,int xxx, struct item* xhci,int endp, p64 arg,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perds* perds = (void*)perusb->perfunc;
	struct perhid* perhid = (void*)&perds->hid;
	u8 notold = ~perhid->stat;

	void** sbuf = (void**)arg;
	struct dsfivereport* data = *sbuf;
	//printmemory(data, 0x10);

	//key
	u8 now = dpad2lrdu(data->dpad);
	if( (now&1) && (notold&1) )eventwrite(0x4b, _kbd_, 0, 0);
	if( (now&2) && (notold&2) )eventwrite(0x4d, _kbd_, 0, 0);
	if( (now&4) && (notold&4) )eventwrite(0x50, _kbd_, 0, 0);
	if( (now&8) && (notold&8) )eventwrite(0x48, _kbd_, 0, 0);
	perhid->stat = now;

	//stick
	short xx[4];
	xx[0] = data->lx - 0x80;
	if((xx[0] < -8)|(xx[0] > 8))xx[0] /= 16;
	else xx[0] = 0;
	xx[1] = data->ly - 0x80;
	if((xx[1] < -8)|(xx[1] > 8))xx[1] /= 16;
	else xx[1] = 0;
	if(xx[0] | xx[1])eventwrite(*(u64*)xx, point_dlt, 0, 0);

	return 0;
}
static int dswhat_ongive(struct item* usb,int xxx, struct item* xhci,int endp, p64 arg,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	int j;
	void** sbuf = (void**)arg;
	u8* tmp = *sbuf;
	u8* debug = (void*)0;
	for(j=0;j<0x40;j++)debug[j] = tmp[j];

	return 0;
}
int dsfourhid_driver(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
{
	//per device
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perds* perds = (void*)perusb->perfunc;
	//if(0 == perds)return 0;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	struct descnode* confnode = &perusb->parsed.node[0];
	struct ConfigurationDescriptor* confdesc = usbdesc_offs2addr(perusb, confnode->real);


	int j,ret;
	struct UsbRequest req;
	void* temp;
	//per interface
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;
	struct descnode* hidnode;
	struct HIDDescriptor* hiddesc;


//------------------------host side + my parse------------------------
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
			ret = (endpdesc->bEndpointAddress & 0xf) << 1;
			if(0 == (endpdesc->bEndpointAddress & 0x80)){
				outaddr = ret;
			}
			else{
				inaddr = ret + 1;
			}

			pktlen = endpdesc->wMaxPacketSize;
			logtoall("[dsfourhid]endpdesc: addr=%x, attr=%x, pktlen=%x, interval=%x\n",
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
		case 0x21:{
			hidnode = (void*)endpnode;
			hiddesc = (void*)endpdesc;
			logtoall("[dsfourhid]hiddesc: country=%x, numdesc=%x, reporttype=%x, reportlen=%x\n",
				hiddesc->bCountryCode, hiddesc->bNumDescriptors,
				hiddesc->bReportDescType, hiddesc->wReportDescLength
			);

			temp = usbdesc_offs2addr(perusb, perusb->origin.byteused);
			D2H_STD_INTF_GETDESC(&req,
				intfdesc->bInterfaceNumber,
				0 | (hiddesc->bReportDescType<<8),
				hiddesc->wReportDescLength
			);
			ret = xhci->give_pxpxpxpx(
				xhci,slot,
				0,0,
				(p64)&req,8,
				temp,hiddesc->wReportDescLength
			);
			if(ret >= 0){
				//printmemory(temp,hiddesc->wReportDescLength);
				printreport(temp,hiddesc->wReportDescLength);
			}

			break;
		}//hid desc
		default:{
			logtoall("[dsfourhid]desctype=%x\n", endpdesc->bDescriptorType);
		}//report desc?
		}//switch

		j = endpnode->rfellow;
	}


//------------------------device side------------------------
	logtoall("[dsfourhid]set_config\n");
	H2D_STD_DEV_SETCONF(&req, confdesc->bConfigurationValue);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		0,0
	);
	if(ret < 0)return -10;
/*
	INTERFACE_REQUEST_SET_INTERFACE(&req, my->intf, 0);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf,req.wLength);
	if(4 != ret)return -11;
*/
	logtoall("[dsfourhid]set_idle\n");
	H2D_CLASS_INTF_SETIDLE(&req, intfdesc->bInterfaceNumber, 0);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		0,0
	);


//------------------------transfer ring------------------------
	logtoall("[dsfourhid]making trb\n");
	if((0x054c == devdesc->idVendor) && (0x09cc == devdesc->idProduct)){
		usb->ongiving = (void*)dsfourhid_ongive;
	}
	else if((0x054c == devdesc->idVendor) && (0x0ce6 == devdesc->idProduct)){
		usb->ongiving = (void*)dsfivehid_ongive;
	}
	else{
		usb->ongiving = (void*)dswhat_ongive;
	}

	if(pktlen > 0x40)pktlen = 0x40;
	ret = xhci->give_pxpxpxpx(
		xhci, slot|(inaddr<<8),
		0, 0,
		(p64)perds->trb, pktlen,
		usb,0
	);
	return 0;
}
int usbdualshock_driver(struct item* usb, int xxx, struct item* xhci, int slot)
{
	struct perusb* perusb = usb->priv_ptr;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;

	struct descnode* confnode;
	struct ConfigurationDescriptor* confdesc;
	struct descnode* intfnode;
	struct InterfaceDescriptor* intfdesc;

	confnode = &perusb->parsed.node[0];
	confdesc = usbdesc_offs2addr(perusb, confnode->real);

	if(0 == confnode->lchild)return -3;		//no intfdesc?
	intfnode = (void*)perusb + confnode->lchild;
	intfdesc = (void*)perusb + intfnode->real;

    //check version
	switch(devdesc->idProduct){
	case 0x05c4:logtoall("[usbdsfour]ver=origin\n");break;
	case 0x09cc:logtoall("[usbdsfour]ver=ps4pro\n");break;
	case 0x0ba0:logtoall("[usbdsfour]ver=adapter\n");break;
	case 0x0ce6:logtoall("[usbdsfive]ver=ps5\n");break;
	}

	//composite device, all interface
	while(1){
        logtoall("[usbdsfour]interface: node=%p,desc=%p, num=%x, alt=%x, c=%x,s=%x,p=%x\n",
            intfnode,intfdesc, intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting,
            intfdesc->bInterfaceClass, intfdesc->bInterfaceSubClass, intfdesc->bInterfaceProtocol);

		if((class_hid == intfdesc->bInterfaceClass) &&(0 < intfdesc->bNumEndpoints) ){
			dsfourhid_driver(usb,xxx, xhci,slot, intfnode,intfdesc);
		}

		if(0 == intfnode->rfellow)break;
		intfnode = (void*)perusb + intfnode->rfellow;
		intfdesc = (void*)perusb + intfnode->real;
	}

	usb->kind = _usb_;
	usb->type = _gpad_;
	usb->vfmt = _sony_;
	return 0;
}
