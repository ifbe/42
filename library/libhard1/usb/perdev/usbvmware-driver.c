#include "libhard.h"
#include "drv-usb.h"
int usbdesc_addr2offs(struct perusb* perusb, void* desc);
void* usbdesc_offs2addr(struct perusb* perusb, int offs);
int printreport(void*,int);
//
void DEVICE_REQUEST_SET_CONFIGURATION(void* req, u16 conf);
void INTERFACE_REQUEST_SET_INTERFACE(struct UsbRequest* req, u16 intf, u16 alt);
void INTERFACE_REQUEST_CLEAR_FEATURE(struct UsbRequest* req, u16 intf, u16 feature);
void INTERFACE_REQUEST_GET_REPORT_DESC(struct UsbRequest* req, u16 intf, u16 typeindex, u16 len);
void INTERFACE_REQUEST_GET_REPORT(struct UsbRequest* req, u16 intf, u16 len);
void INTERFACE_REQUEST_SET_REPORT(struct UsbRequest* req, u16 intf, u16 len);
void INTERFACE_REQUEST_GET_IDLE(struct UsbRequest* req, u16 intf);
void INTERFACE_REQUEST_SET_IDLE(struct UsbRequest* req, u16 intf, u16 val);
void INTERFACE_REQUEST_GET_PROTOCOL(struct UsbRequest* req, u16 intf);
void INTERFACE_REQUEST_SET_PROTOCOL(struct UsbRequest* req, u16 intf, u16 val);



struct perfunc{		//0x10000 byte per func
	union{
		u8 padding[0x1000];
	};
	u8 trb[0xf000];
}__attribute__((packed));




/*
//https://wiki.osdev.org/VMware_tools
typedef struct {
	union {
		uint32_t ax;
		uint32_t magic;
	};
	union {
		uint32_t bx;
		size_t size;
	};
	union {
		uint32_t cx;
		uint16_t command;
	};
	union {
		uint32_t dx;
		uint16_t port;
	};
	uint32_t si;
	uint32_t di;
} vmware_cmd;
#define VMWARE_MAGIC  0x564D5868
#define VMWARE_PORT   0x5658
#define VMWARE_PORTHB 0x5659
void vmware_send(vmware_cmd * cmd) {
	cmd->magic = VMWARE_MAGIC;
	cmd->port = VMWARE_PORT;
	asm volatile("in %%dx, %0" : "+a"(cmd->ax), "+b"(cmd->bx), "+c"(cmd->cx), "+d"(cmd->dx), "+S"(cmd->si), "+D"(cmd->di));
}
static void vmware_send_hb(vmware_cmd * cmd) {
	cmd->magic = VMWARE_MAGIC;
	cmd->port = VMWARE_PORTHB;
	asm volatile("cld; rep; outsb" : "+a"(cmd->ax), "+b"(cmd->bx), "+c"(cmd->cx), "+d"(cmd->dx), "+S"(cmd->si), "+D"(cmd->di));
}
static void vmware_get_hb(vmware_cmd * cmd) {
	cmd->magic = VMWARE_MAGIC;
	cmd->port = VMWARE_PORTHB;
	asm volatile("cld; rep; insb" : "+a"(cmd->ax), "+b"(cmd->bx), "+c"(cmd->cx), "+d"(cmd->dx), "+S"(cmd->si), "+D"(cmd->di));
}
int is_vmware_backdoor(void) {
	vmware_cmd cmd;
	cmd.bx = ~VMWARE_MAGIC;
	cmd.command = CMD_GETVERSION;
	vmware_send(&cmd);
 
	if (cmd.bx != VMWARE_MAGIC || cmd.ax == 0xFFFFFFFF) {
		return 0;
	}
 
	return 1;
}
#define CMD_ABSPOINTER_DATA    39
#define CMD_ABSPOINTER_STATUS  40
#define CMD_ABSPOINTER_COMMAND 41
#define ABSPOINTER_ENABLE   0x45414552
#define ABSPOINTER_RELATIVE 0xF5
#define ABSPOINTER_ABSOLUTE 0x53424152
void vmware_backdoor_mouse_enable(void) {
	vmware_cmd cmd;
 
	//Enable
	cmd.bx = ABSPOINTER_ENABLE;
	cmd.command = CMD_ABSPOINTER_COMMAND;
	vmware_send(&cmd);
 
	//Status
	cmd.bx = 0;
	cmd.command = CMD_ABSPOINTER_STATUS;
	vmware_send(&cmd);
 
	///Read data
	cmd.bx = 1;
	cmd.command = CMD_ABSPOINTER_DATA;
	vmware_send(&cmd);
 
	//Enable absolute
	cmd.bx = ABSPOINTER_ABSOLUTE;
	cmd.command = CMD_ABSPOINTER_COMMAND;
	vmware_send(&cmd);
}
*/




static int vmmouse_ongive(struct item* usb,int xxx, struct item* xhci,int endp, void* sbuf,int slen, void* rbuf,int rlen)
{
	void* data = *(void**)sbuf;
	printmemory(data, 16);
	return 0;
}


int usbvmware_perintf(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
{
	//per device
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;
	struct perfunc* perfunc = (void*)perusb->perfunc + intfdesc->bInterfaceNumber*0x10000;
	//if(0 == perfunc)return 0;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	struct descnode* confnode = &perusb->parsed.node[0];
	struct ConfigurationDescriptor* confdesc = usbdesc_offs2addr(perusb, confnode->real);

	int j,ret;
	struct UsbRequest req;

	int need_to_set_configuration = 1;
	if(need_to_set_configuration){
		say("[vmmouse]set_config\n");
		DEVICE_REQUEST_SET_CONFIGURATION(&req, confdesc->bConfigurationValue);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			0,0
		);
		if(ret < 0)return -10;
	}

	int need_to_set_idle = 1;
	if(need_to_set_idle){
		say("set idle\n");
		INTERFACE_REQUEST_SET_IDLE(&req,
			intfdesc->bInterfaceNumber,
			0
		);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			0,0
		);
	}

	//per interface
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;
	struct descnode* hidnode;
	struct HIDDescriptor* hiddesc;

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
			say("[vmmouse]endpdesc: addr=%x, attr=%x, pktlen=%x, interval=%x\n",
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
			say("[vmmouse]hiddesc: country=%x, numdesc=%x, reportdesctype=%x, reportdesclen=%x\n",
				hiddesc->bCountryCode, hiddesc->bNumDescriptors,
				hiddesc->bReportDescType, hiddesc->wReportDescLength
			);

			if( (0x0e0f == devdesc->idVendor) && (0x0003 == devdesc->idProduct) ){
				say("get reportdesc\n");
				void* temp = usbdesc_offs2addr(perusb, perusb->origin.byteused);
				INTERFACE_REQUEST_GET_REPORT_DESC(&req,
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
			}
			break;
		}//hid desc
		default:{
			say("[vmmouse]desctype=%x\n", endpdesc->bDescriptorType);
		}//report desc?
		}//switch

		j = endpnode->rfellow;
	}


//------------------------device side------------------------
	int need_to_get_report = 1;
	if(need_to_get_report){
		void* rrrr = usbdesc_offs2addr(perusb, perusb->origin.byteused);

		say("get report1\n");
		INTERFACE_REQUEST_GET_REPORT(&req, intfdesc->bInterfaceNumber, 0x40);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			rrrr,0x40
		);
		printmemory(rrrr, 0x40);

		say("get report2\n");
		INTERFACE_REQUEST_GET_REPORT(&req, intfdesc->bInterfaceNumber, 0x40);
		ret = xhci->give_pxpxpxpx(
			xhci,slot,
			0,0,
			&req,8,
			rrrr,0x40
		);
		printmemory(rrrr, 0x40);
	}

//------------------------callback------------------------
	usb->ongiving = (void*)vmmouse_ongive;

//------------------------transfer ring------------------------
	say("[vmmouse]making trb@%p\n", perfunc->trb);
	if(pktlen > 0x40)pktlen = 0x40;
	ret = xhci->give_pxpxpxpx(
		xhci, slot|(inaddr<<8),
		0, 0,
		perfunc->trb, pktlen,
		usb, 0
	);

	return 0;
}
int usbvmware_driver(struct item* usb,int xxx, struct item* xhci,int slot)
{
	//per device
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	if( (0x0e0f == devdesc->idVendor) && (0x0003 == devdesc->idProduct) ){
		say("vmware virtual mouse\n");
	}

	struct descnode* confnode = &perusb->parsed.node[0];
	struct ConfigurationDescriptor* confdesc = usbdesc_offs2addr(perusb, confnode->real);

	//two interface
	struct descnode* intfnode = usbdesc_offs2addr(perusb, confnode->lchild);
	struct InterfaceDescriptor* intfdesc = usbdesc_offs2addr(perusb, intfnode->real);
	usbvmware_perintf(usb,xxx, xhci,slot, intfnode,intfdesc);

	intfnode = usbdesc_offs2addr(perusb, confnode->rchild);
	intfdesc = usbdesc_offs2addr(perusb, intfnode->real);
	usbvmware_perintf(usb,xxx, xhci,slot, intfnode,intfdesc);

	return 0;
}