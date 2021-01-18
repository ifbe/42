#include "libhard.h"
#include "usb.h"
void DEVICE_REQUEST_SET_CONFIGURATION(void* req, u16 conf);
int xhci_giveorderwaitevent(void* hc,int id, u32,u32, void* sendbuf,int sendlen, void* recvbuf, int recvlen);
void filemanager_registersupplier(void*);

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
#define scsi_Read10             0x28
#define scsi_Write10            0x2A
#define scsi_Verify10           0x2F
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




static u8 KMGTPE[8] = {' ','K','M','G','T','P','E', 0};
u32 usbstor_endian(u32 in)
{
	return (in>>24) | ((in>>8)&0xff00) | ((in<<8)&0xff0000) | ((in<<24)&0xff000000);
}
int usbstor_shift(u64 in)
{
	int sh = 0;
	while(in > 1024){
		sh += 10;
		in = in>>10;
	}
	return sh;
}
int usbstor_ZeroMemory(void* buf, int len)
{
	int j;
	u8* tmp = buf;
	for(j=0;j<len;j++)tmp[j] = 0;
	return j;
}




struct perstor{
	void* host;
	int slot;
	int bulkin;
	int bulkout;
	u64 blocksize;
	u64 totalsize;
};
static int usbstor_ontake(struct item* usb,void* foot,struct halfrel* stack,int sp, void* arg,int off, void* buf,int len)
{
	//arg: 0=readfile, "blocksize", "totalsize", ...
	say("usbstor_ontake:%p,%x,%p,%x\n",arg,off,buf,len);

	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct perstor* info = (void*)(perusb->freebuf);
	if(0 == info->host)return 0;
	if(0 == info->blocksize)return 0;

	int j,ret;
	int bytecur = off;	//byte cur
	int bytecnt =   0;	//byte cnt
	int blocklba;
	int blockcnt;
	struct CommandBlockWrapper cbw;
	struct CommandStatusWrapper rsw;
	while(1){
		if(bytecur >= off+len)break;

		bytecnt = off+len-bytecur;
		if(bytecnt > 0x10000)bytecnt = 0x10000;

		blocklba = bytecur / (info->blocksize);
		blockcnt = bytecnt / (info->blocksize);
		say("lba=%x,cnt=%x\n",blocklba,blockcnt);

		usbstor_ZeroMemory(&cbw, 0x1f);
		usbstor_ZeroMemory(&rsw, 0x0d);
		cbw.Signature    = 0x43425355;
		cbw.Tag          = blocklba;
		cbw.DataLen      = bytecnt;
		cbw.DataDir      = 0x80;
		cbw.LogicUnitNum = 0;
		cbw.CmdLen       = 6;
		cbw.CmdData[0]   = scsi_Read10;
		cbw.CmdData[1]   = 0;
		cbw.CmdData[2]   = (blocklba>>24) & 0xff;
		cbw.CmdData[3]   = (blocklba>>16) & 0xff;
		cbw.CmdData[4]   = (blocklba>> 8) & 0xff;
		cbw.CmdData[5]   = (blocklba    ) & 0xff;
		cbw.CmdData[6]   = 0;
		cbw.CmdData[7]   = blockcnt >> 8;
		cbw.CmdData[8]   = blockcnt & 0xff;
		for(j=9;j<16;j++)cbw.CmdData[j] = 0;

		ret = xhci_giveorderwaitevent(info->host,info->slot|(info->bulkout<<8),'d',0, &cbw, 0x1f, 0,0);
		if(ret < 0){
			say("[usbdisk]error@send cbw\n");
			break;
		}

		ret = xhci_giveorderwaitevent(info->host,info->slot|(info->bulkin<<8), 'd',0, buf+bytecur-off, bytecnt, 0,0);
		if(ret < 0){
			say("[usbdisk]error@recv dat\n");
			break;
		}

		ret = xhci_giveorderwaitevent(info->host,info->slot|(info->bulkin<<8), 'd',0, &rsw, 0x0d, 0,0);
		if(ret < 0){
			say("[usbdisk]error@recv csw\n");
			break;
		}
		if(0 != rsw.Status){
			say("[usbdisk]error@csw.status=%x\n", rsw.Status);
			break;
		}

		bytecur += bytecnt;
	}
	return bytecur-off;
}
static int usbstor_ongive(struct item* usb,void* foot,struct halfrel* stack,int sp, void* sbuf,int slen, void* rbuf,int rlen)
{
	return 0;
}
int usbstor_driver(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
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

	if(0 == perusb->my.devnode)return -1;		//no devdesc?
	devnode = (void*)perusb + perusb->my.devnode;
	devdesc = (void*)perusb + devnode->real;

	if(0 == devnode->lchild)return -2;		//no confdesc?
	confnode = (void*)perusb + perusb->my.confnode;
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
	struct CommandStatusWrapper* inqcsw = (void*)(perusb->freebuf + 0x100);
	struct INQUERY_Reply* inqrep = (void*)(perusb->freebuf + 0x200);
	usbstor_ZeroMemory(inqcbw, 0x1f);	//31byte
	usbstor_ZeroMemory(inqcsw, 0x0d);	//14byte
	usbstor_ZeroMemory(inqrep, 0x24);	//36byte

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

	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, inqrep, 0x24, 0,0);
	printmemory(inqrep, 0x24);

	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, inqcsw, 0x0d, 0,0);

	printmemory(inqcsw, 0x0d);
	if(0 != inqcsw->Status)say("[usbdisk]csw.status=%x\n", inqcsw->Status);


//------------------------send TestUnitReady------------------------
	say("[usbdisk]Test Unit Ready...\n");
	struct CommandBlockWrapper* turcbw = (void*)(perusb->freebuf);
	struct CommandStatusWrapper* turcsw = (void*)(perusb->freebuf + 0x100);
	usbstor_ZeroMemory(turcbw, 0x1f);
	usbstor_ZeroMemory(turcsw, 0x0d);

	turcbw->Signature    = 0x43425355;
	turcbw->Tag          = hex32('t','u','r',0);
	turcbw->DataLen      = 0;
	turcbw->DataDir      = 0x80;
	turcbw->LogicUnitNum = 0;
	turcbw->CmdLen       = 6;
	turcbw->CmdData[0] = scsi_TestUnitReady;
	for(j=1;j<16;j++)turcbw->CmdData[j] = 0;
	ret = xhci_giveorderwaitevent(xhci,slot|(outaddr<<8),'d',0, turcbw, 0x1f, 0,0);

	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, turcsw, 0x0d, 0,0);

	printmemory(turcsw, 0x0d);
	if(0 != turcsw->Status)say("[usbdisk]csw.status=%x\n", turcsw->Status);


//------------------------send ReadCapacity------------------------
	say("[usbdisk]Read Capacity...\n");
	struct CommandBlockWrapper* capcbw = (void*)(perusb->freebuf);
	struct CommandStatusWrapper* capcsw = (void*)(perusb->freebuf + 0x100);
	struct READ_CAPACITY_Reply* caprep = (void*)(perusb->freebuf + 0x200);
	usbstor_ZeroMemory(capcbw, 0x1f);
	usbstor_ZeroMemory(capcsw, 0x0d);
	usbstor_ZeroMemory(caprep, 8);
	capcbw->Signature    = 0x43425355;
	capcbw->Tag          = hex32('c','a','p',0);
	capcbw->DataLen      = 8;
	capcbw->DataDir      = 0x80;
	capcbw->LogicUnitNum = 0;
	capcbw->CmdLen       = 6;
	capcbw->CmdData[0]   = scsi_ReadCapacity;
	for(j=1;j<16;j++)capcbw->CmdData[j] = 0;
	ret = xhci_giveorderwaitevent(xhci,slot|(outaddr<<8),'d',0, capcbw, 0x1f, 0,0);

	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, caprep, 8, 0,0);

	printmemory(caprep, 8);
	u32 blocklast = usbstor_endian(caprep->LastLBA_MsbNotLsb);
	u32 blocksize = usbstor_endian(caprep->BlockSz_MsbNotLsb);
	u64 totalsize = (1 + (u64)blocklast) * blocksize;
	int sh = usbstor_shift(totalsize);
	say("blocklast=0x%x,blocksize=0x%x,totalsize=0x%llx(%d%cB)\n", blocklast, blocksize, totalsize, totalsize>>sh, KMGTPE[sh/10]);

	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, capcsw, 0x0d, 0,0);

	printmemory(capcsw, 0x0d);
	if(0 != capcsw->Status)say("[usbdisk]csw.status=%x\n", capcsw->Status);


//------------------------serve for the people------------------------
	struct perstor* info = (void*)(perusb->freebuf);
	usbstor_ZeroMemory(info, 0x100);
	info->host = xhci;
	info->slot = slot;
	info->bulkin = inaddr;
	info->bulkout = outaddr;
	info->blocksize = blocksize;
	info->totalsize = totalsize;
	usb->ongiving = usbstor_ongive;
	usb->ontaking = usbstor_ontake;


//------------------------file prober------------------------
	filemanager_registersupplier(usb);
/*	struct artery* probe = arterycreate(_fileauto_,0,0,0);
	if(0 == probe)return 0;
	struct relation* rel = relationcreate(probe,0,_art_,_src_, usb,0,_dev_,0);
	if(0 == rel)return 0;
	arterylinkup((void*)&rel->dst, (void*)&rel->src);*/
	return 0;
}
