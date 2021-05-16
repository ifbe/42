#include "libhard.h"
#include "usb.h"
void eventwrite(u64,u64,u64,u64);
void DEVICE_REQUEST_SET_CONFIGURATION(void* req, u16 conf);
void INTERFACE_REQUEST_GET_REPORT_DESC(void* req, u16 intf, u16 typeindex, u16 len);
void INTERFACE_REQUEST_SET_IDLE(struct UsbRequest* req, u16 intf, u16 val);
struct ds4report{
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




static void dpad2lrdu(u8 dpad, u8* p)
{
	p[0] = p[1] = p[2] = p[3] = 0;
	if(dpad >= 8)return;

	//0=N,1=NE,2=E,3=SE,4=S,5=SW,6=W,7=NW,8=reseased
	if((dpad >= 5)&&(dpad <= 7))p[0] = 1;
	if((dpad >= 1)&&(dpad <= 3))p[1] = 1;
	if((dpad >= 3)&&(dpad <= 5))p[2] = 1;
	if((0==dpad)|(1==dpad)|(7==dpad))p[3] = 1;
}
static int ds4hid_ongive(struct item* usb,int xxx, struct item* xhci,int endp, void* sbuf,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct ds4report* data = *(void**)sbuf;
	//printmemory(data, 0x40);

	u8 lrdu[4];
	dpad2lrdu(data->dpad, lrdu);
	if(lrdu[0])eventwrite(0x4b, _kbd_, 0, 0);
	if(lrdu[1])eventwrite(0x4d, _kbd_, 0, 0);
	if(lrdu[2])eventwrite(0x50, _kbd_, 0, 0);
	if(lrdu[3])eventwrite(0x48, _kbd_, 0, 0);
/*
	say("l=%d,r=%d,d=%d,u=%d,a=%d,b=%d,x=%d,y=%d\n",lrdu[0],lrdu[1],lrdu[2],lrdu[3],data->a,data->b,data->x,data->y);
	say("l1=%d,r1=%d,l2=%d,r2=%d,l3=%d,r3=%d, share=%d,opt=%d,ps=%d\n",data->l1,data->r1,data->l2,data->r2,data->l3,data->r3,  data->share,data->opt,data->ps);
	say("lx=%d,ly=%d,rx=%d,ry=%d,lt=%d,rt=%d\n",data->lx,255-data->ly,data->rx,255-data->ry,data->lt,data->rt);
	say("gx=%d,gy=%d,gz=%d, ax=%d,ay=%d,az=%d\n",data->gx,-data->gz,data->gy,  -data->ax,data->az,-data->ay);*/
	return 0;
}
int ds4hid_driver(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
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
			say("[ds4hid]endpdesc: addr=%x, attr=%x, pktlen=%x, interval=%x\n",
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
			say("[ds4hid]hiddesc: country=%x, numdesc=%x, reporttype=%x, reportlen=%x\n",
				hiddesc->bCountryCode, hiddesc->bNumDescriptors,
				hiddesc->bReportDescType, hiddesc->wReportDescLength
			);

			INTERFACE_REQUEST_GET_REPORT_DESC(&req,
				intfdesc->bInterfaceNumber,
				0 | (hiddesc->bReportDescType<<8),
				hiddesc->wReportDescLength);
			ret = xhci->give_pxpxpxpx(
				xhci,slot,
				0,0,
				&req,8,
				perusb->desc + perusb->my.desclen,hiddesc->wReportDescLength
			);
			if(ret >= 0)printmemory(perusb->desc + perusb->my.desclen,hiddesc->wReportDescLength);

			break;
		}//hid desc
		default:{
			say("[ds4hid]desctype=%x\n", endpdesc->bDescriptorType);
		}//report desc?
		}//switch

		j = endpnode->rfellow;
	}


//------------------------device side------------------------
	say("[ds4hid]set_config\n");
	DEVICE_REQUEST_SET_CONFIGURATION(&req, confdesc->bConfigurationValue);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);
	if(ret < 0)return -10;
/*
	INTERFACE_REQUEST_SET_INTERFACE(&req, my->intf, 0);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, buf,req.wLength);
	if(4 != ret)return -11;
*/
	say("[ds4hid]set_idle\n");
	INTERFACE_REQUEST_SET_IDLE(&req, intfdesc->bInterfaceNumber, 0);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		&req,8,
		0,0
	);


//------------------------transfer ring------------------------
	say("[ds4hid]making trb\n");
	usb->ongiving = (void*)ds4hid_ongive;

	if(pktlen > 0x40)pktlen = 0x40;
	ret = xhci->give_pxpxpxpx(
		xhci,slot|(inaddr<<8),
		0,0,
		perusb->freebuf,pktlen,
		usb,0
	);
	return 0;
}
int usbds4_driver(struct item* usb, int xxx, struct item* xhci, int slot)
{
	struct perusb* perusb = usb->priv_ptr;

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

    //check version
	switch(devdesc->idProduct){
	case 0x05c4:say("[usbds4]ver=origin\n");break;
	case 0x09cc:say("[usbds4]ver=ps4pro\n");break;
	case 0x0ba0:say("[usbds4]ver=adapter\n");break;
	case 0x0ce6:say("[usbds5]ver=ps5\n");break;
	}

	//composite device, all interface
	while(1){
        say("[usbds4]interface: node=%p,desc=%p, num=%x, alt=%x, c=%x,s=%x,p=%x\n",
            intfnode,intfdesc, intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting,
            intfdesc->bInterfaceClass, intfdesc->bInterfaceSubClass, intfdesc->bInterfaceProtocol);

		if((class_hid == intfdesc->bInterfaceClass) &&(0 < intfdesc->bNumEndpoints) ){
			ds4hid_driver(usb,xxx, xhci,slot, intfnode,intfdesc);
		}

		if(0 == intfnode->rfellow)break;
		intfnode = (void*)perusb + intfnode->rfellow;
		intfdesc = (void*)perusb + intfnode->real;
	}
	return 0;
}
