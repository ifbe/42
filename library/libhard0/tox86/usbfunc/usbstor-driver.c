#include "libhard.h"
void DEVICE_REQUEST_SET_CONFIGURATION(void* req, u16 conf);
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

//subclass
#define subclass_RBC        1
#define subclass_MMC5_ATAPI 2
#define subclass_UFI        4
#define subclass_SCSI       6
#define subclass_LSDFS      7
#define subclass_IEEE1667   8
//interface
#define interface_CBI_int   0		//cmd-bulk-int
#define interface_CBI_noint 1		//cmd-bulk
#define interface_BBB       0x50	//bulk only
#define interface_UAS       0x62	//usb attached scsi
//scsi
#define scsi_TestUnitReady      0x00
#define scsi_RequestSense       0x03
#define scsi_Inquiry            0x12
#define scsi_ModeSense6         0x1A
#define scsi_StartStop          0x1B
#define scsi_MediumRemoval      0x1E
#define scsi_ReadFormatCapacity 0x23
#define scsi_ReadCapacity       0x25
#define scsi_Read_10            0x28
#define scsi_Write_10           0x2A
#define scsi_Verify             0x2F
#define scsi_ModeSense10        0x5A
/*
struct CommandBlock{
	u8      Operation;	//0

	u8         Rsvd:5;	//1.[0,4]
	u8 LogicUnitNum:3;	//1.[5,7]

	u32 LBA_MsbNotLsb;	//[2,5]
	u8        unused6;	//6

	u16 LEN_MsbNotLsb;	//[7,8]
	u8        unused9;	//9
	u8        unuseda;	//10
	u8        unusedb;	//11
}__attribute__((packed));*/
struct INQUERY_Command{
	u8      Operation;	//0: 0x12

	u8         EVPD:1;	//1.0
	u8         Rsvd:4;	//1.[0,4]
	u8 LogicUnitNum:3;	//1.[5,7]

	u8       PageCode;	//2

	u8         Rsvd_3;	//3

	u8    AllocLength;	//4

	u8      unused[7];	//[5,11]
}__attribute__((packed));
struct INQUERY_Reply{
	u8 PeripheralDeviceType:5;	//0
		//00h SBC Direct-access device (e.g., UHD Floppy disk)
		//05h CD-ROM device
		//07h Optical memory device (e.g., Non-CD optical disks)
		//0Eh RBC Direct-access device (e.g., UHD Floppy disk)
	u8                Rsvd0:3;

	u8                Rsvd1:6;	//1
	u8                  RMB:2;	//removable=1, notremove=0

	u8          ANSIVersion:3;	//2
	u8          ECMAVersion:3;
	u8           ISOVersion:2;

	u8   ResponseDataFormat:4;	//3
	u8                Rsvd3:4;

	u8       AdditionalLength;	//4

	u8               Rsvd5[3];	//[5,7]

	u8   VendorInformation[8];	//[8,15]
	u8 ProductInformation[16];	//[16,31]
	u8     ProductRevision[4];	//[32,35]
}__attribute__((packed));
//
struct TESTUNITREADY{
	u8      Operation;	//0: 0x0
	u8        Rsvd1:5;	//1
	u8 LogicUnitNum:3;
	u8        Rsvd[4];	//[2,5]
	u8         PAD[6];	//[6,11]
}__attribute__((packed));
//
struct READ_CAPACITY{
	u8      Operation;	//0: 0x25
	u8        Rsvd1:5;	//1
	u8 LogicUnitNum:3;
	u8        Rsvd[8];	//[2,9]
	u16           PAD;	//[10,11]
}__attribute__((packed));
struct READ_CAPACITY_Reply{
	u32 LastLBA_MsbNotLsb;
	u32 BlockSz_MsbNotLsb;
}__attribute__((packed));
//
struct READ10{
	u8      Operation;	//0: 0x28
	u8        Rsvd1:5;	//1
	u8 LogicUnitNum:3;
	u32 LBA_MsbNotLsb;	//[2,5]
	u8        unused6;	//6

	u16 LEN_MsbNotLsb;	//[7,8]
	u8        unused9;	//9
	u16           PAD;	//[10,11]
}__attribute__((packed));
struct WRITE10{
	u8      Operation;	//0: 0x2a
	u8        Rsvd1:5;	//1
	u8 LogicUnitNum:3;
	u32 LBA_MsbNotLsb;	//[2,5]
	u8        unused6;	//6

	u16 LEN_MsbNotLsb;	//[7,8]
	u8        unused9;	//9
	u16           PAD;	//[10,11]
}__attribute__((packed));
struct FORMATUNIT10{
	u8            Operation;	//0: 0x4
	u8   DefectListFormat:3;	//1
	u8            CmpList:1;
	u8            FmtData:1;
	u8       LogicUnitNum:3;
	u8       VendorSpecific;	//2
	u8 Interleave_MsbnotLsb;	//[3,4]
	u8                 Rsvd;	//5
	u16              PAD[6];	//[6,11]
}__attribute__((packed));
struct VERIFY10{
	u8      Operation;	//0: 0x2f
	u8      Reserve:1;
	u8      ByteChk:1;
	u8        Rsvd1:3;
	u8 LogicUnitNum:3;
	u32 LBA_MsbNotLsb;	//[2,5]
	u8        unused6;	//6

	u16 LEN_MsbNotLsb;	//[7,8]
	u8        unused9;	//9
	u16           PAD;	//[10,11]
}__attribute__((packed));
//
struct CommandBlockWrapper{
	u32   Signature;	//[0,3]: 0x43425355	//'USBC'
	u32         Tag;	//[4,7]: Transaction Unique Identifier
	u32     DataLen;	//[8,b]
	u8      DataDir;	//c: 0x80=d2h, 00=h2d
	u8 LogicUnitNum;	//d
	u8       CmdLen;	//e
	u8  CmdData[16];	//f
}__attribute__((packed));
struct CommandStatusWrapper{
	u32 Signature;	//[0,3]: 0x43425355	//'USBS'
	u32       Tag;	//[4,7]: Copied From CBW
	u32   Residue;	//[8,b]: Difference Between CBW Length And Actual Length
	u8     Status;	//c
		//0 Command Passed(good state)
		//1 Command Failed
		//2 Phase Error
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
void BULK_ONLY_MASS_STORAGE_RESET(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0x21;
	req->bRequest = 0xff;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 0;
}
void BULK_ONLY_GET_LUN(struct UsbRequest* req, u16 intf)
{
	req->bmRequestType = 0xa1;
	req->bRequest = 0xfe;
	req->wValue = 0;
	req->wIndex = intf;
	req->wLength = 1;
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




u32 usbstor_endian(u32 in)
{
	return (in>>24) | ((in>>8)&0xff00) | ((in<<8)&0xff0000) | ((in<<24)&0xff000000);
}
int usbstor_ZeroMemory(void* buf, int len)
{
	int j;
	u8* tmp = buf;
	for(j=0;j<len;j++)tmp[j] = 0;
	return j;
}
int usbstor_driver(struct device* usb,int xxx, struct device* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
{
	say("[usbdisk]begin...\n");

	int j,ret;
	int inaddr=0,outaddr=0;
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
	if(subclass_SCSI != intfdesc->bInterfaceSubClass){
		say("[usbdisk]not SCSI, bye bye\n");
		return -1;
	}
	if(interface_BBB == intfdesc->bInterfaceProtocol){
		say("[usbdisk]bulk only\n");
		//lets go
	}
	else if(interface_UAS == intfdesc->bInterfaceProtocol){
		say("[usbdisk]UASP, bye bye\n");
		return -2;
	}
	else{
		say("[usbdisk]proto=%x, bye bye\n", intfdesc->bInterfaceProtocol);
		return -2;
	}


//------------------------host side + my parse------------------------
	j = intfnode->lchild;
	while(1){
		if(0 == j)break;
		endpnode = (void*)perusb + j;
		endpdesc = (void*)perusb + endpnode->real;

		switch(endpdesc->bDescriptorType){
		case 5:{
			say("[usbdisk]endpdesc: addr=%x, attr=%x, pktlen=%x, interval=%x\n",
				endpdesc->bEndpointAddress, endpdesc->bmAttributes,
				endpdesc->wMaxPacketSize, endpdesc->bInterval
			);

			ret = endpdesc->bEndpointAddress;
			ret = ((ret&0xf)<<1) + (ret>>7);
			if(ret&1)inaddr = ret;
			else outaddr = ret;

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
		default:{
			say("[usbdisk]desctype=%x\n", endpdesc->bDescriptorType);
		}//report desc?
		}//switch

		j = endpnode->rfellow;
	}
	say("[usbdisk]in@%x, out@%x\n", inaddr, outaddr);


//------------------------set configuration------------------------
	DEVICE_REQUEST_SET_CONFIGURATION(&req, confdesc->bConfigurationValue);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, 0,0);


//------------------------send INQUERY------------------------
	say("[usbdisk]Inquiry...\n");
	struct CommandBlockWrapper* inqcbw = (void*)(perusb->freebuf);
	usbstor_ZeroMemory(inqcbw, 0x1f);
	inqcbw->Signature    = 0x43425355;
	inqcbw->Tag          = hex32('i','n','q',0);
	inqcbw->DataLen      = 0x24;
	inqcbw->DataDir      = 0x80;
	inqcbw->LogicUnitNum = 0;
	inqcbw->CmdLen       = 6;
	inqcbw->CmdData[0] = scsi_Inquiry;	//SCSI operation code
	inqcbw->CmdData[1] =    0;	//SCSI reserved
	inqcbw->CmdData[2] =    0;	//SCSI page or operation code
	inqcbw->CmdData[3] =    0;	//SCSI reserved
	inqcbw->CmdData[4] = 0x24;	//SCSI allocation length
	inqcbw->CmdData[5] =    0;	//SCSI control
	for(j=6;j<16;j++)inqcbw->CmdData[j] = 0;
	ret = xhci_giveorderwaitevent(xhci,slot|(outaddr<<8),'d',0, inqcbw, 0x1f, 0,0);

	struct INQUERY_Reply* inqrep = (void*)(perusb->freebuf + 0x100);
	usbstor_ZeroMemory(inqrep, 0x24);
	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, inqrep, 0x24, 0,0);
	printmemory(inqrep, 0x24);

	struct CommandStatusWrapper* inqcsw = (void*)(perusb->freebuf + 0x200);
	usbstor_ZeroMemory(inqcsw, 0x0d);
	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, inqcsw, 0x0d, 0,0);

	printmemory(inqcsw, 0x0d);
	if(0 != inqcsw->Status)say("[usbdisk]csw.status=%x\n", inqcsw->Status);


//------------------------send TestUnitReady------------------------
	say("[usbdisk]Test Unit Ready...\n");
	struct CommandBlockWrapper* turcbw = (void*)(perusb->freebuf+0x300);
	usbstor_ZeroMemory(turcbw, 0x1f);
	turcbw->Signature    = 0x43425355;
	turcbw->Tag          = hex32('t','u','r',0);
	turcbw->DataLen      = 0;
	turcbw->DataDir      = 0x80;
	turcbw->LogicUnitNum = 0;
	turcbw->CmdLen       = 6;
	turcbw->CmdData[0] = scsi_TestUnitReady;
	for(j=1;j<16;j++)turcbw->CmdData[j] = 0;
	ret = xhci_giveorderwaitevent(xhci,slot|(outaddr<<8),'d',0, turcbw, 0x1f, 0,0);

	struct CommandStatusWrapper* turcsw = (void*)(perusb->freebuf + 0x400);
	usbstor_ZeroMemory(turcsw, 0x0d);
	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, turcsw, 0x0d, 0,0);

	printmemory(turcsw, 0x0d);
	if(0 != turcsw->Status)say("[usbdisk]csw.status=%x\n", turcsw->Status);


//------------------------send ReadCapacity------------------------
	say("[usbdisk]Read Capacity...\n");
	struct CommandBlockWrapper* capcbw = (void*)(perusb->freebuf + 0x600);
	usbstor_ZeroMemory(capcbw, 0x1f);
	capcbw->Signature    = 0x43425355;
	capcbw->Tag          = hex32('c','a','p',0);
	capcbw->DataLen      = 8;
	capcbw->DataDir      = 0x80;
	capcbw->LogicUnitNum = 0;
	capcbw->CmdLen       = 6;
	capcbw->CmdData[0]   = scsi_ReadCapacity;
	for(j=1;j<16;j++)capcbw->CmdData[j] = 0;
	ret = xhci_giveorderwaitevent(xhci,slot|(outaddr<<8),'d',0, capcbw, 0x1f, 0,0);

	struct READ_CAPACITY_Reply* caprep = (void*)(perusb->freebuf + 0x700);
	usbstor_ZeroMemory(caprep, 8);
	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, caprep, 8, 0,0);

	printmemory(caprep, 8);
	u32 aa = usbstor_endian(caprep->LastLBA_MsbNotLsb) + 1;
	u32 bb = usbstor_endian(caprep->BlockSz_MsbNotLsb);
	u64 cc = aa*bb;
	say("blockcnt=0x%x,blocksize=0x%x,totalsize=0x%llx\n", aa, bb, cc);

	struct CommandStatusWrapper* capcsw = (void*)(perusb->freebuf + 0x800);
	usbstor_ZeroMemory(capcsw, 0x0d);
	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, capcsw, 0x0d, 0,0);

	printmemory(capcsw, 0x0d);
	if(0 != capcsw->Status)say("[usbdisk]csw.status=%x\n", capcsw->Status);

	return 0;
}