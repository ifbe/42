#include "libhard.h"
#include "drv/usb.h"
void H2D_STD_DEV_SETCONF(void* req, u16 conf);
void filemanager_registerdisk(void*, void*);

int usbdesc_addr2offs(struct perusb* perusb, void* desc);
void* usbdesc_offs2addr(struct perusb* perusb, int offs);

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




int scsi_cmd_inquery(u8* ptr)
{
	int j;
	ptr[0] = scsi_Inquiry;	//SCSI operation code
	ptr[1] =            0;	//SCSI reserved
	ptr[2] =            0;	//SCSI page or operation code
	ptr[3] =            0;	//SCSI reserved
	ptr[4] =         0x24;	//SCSI allocation length
	ptr[5] =            0;	//SCSI control
	for(j=6;j<16;j++)ptr[j] = 0;
	return 6;
}
int scsi_cmd_testunitready(u8* ptr)
{
	int j;
	ptr[0] = scsi_TestUnitReady;
	for(j=1;j<16;j++)ptr[j] = 0;
	return 6;
}
int scsi_cmd_readcapacity10(u8* ptr)
{
	int j;
	ptr[0]   = scsi_ReadCapacity;
	for(j=1;j<16;j++)ptr[j] = 0;
	return 10;
}
int scsi_cmd_readcapacity16(u8* ptr)
{
	return 16;
}
int scsi_cmd_readdata10(u8* ptr, u64 blocklba, int blockcnt)
{
	int j;
	ptr[0]   = scsi_Read10;
	ptr[1]   = 0;
	ptr[2]   = (blocklba>>24) & 0xff;
	ptr[3]   = (blocklba>>16) & 0xff;
	ptr[4]   = (blocklba>> 8) & 0xff;
	ptr[5]   = (blocklba    ) & 0xff;
	ptr[6]   = 0;
	ptr[7]   = blockcnt >> 8;
	ptr[8]   = blockcnt & 0xff;
	for(j=9;j<16;j++)ptr[j] = 0;
	return 10;
}
int scsi_cmd_readdata16(u8* ptr, u64 blocklba, int blockcnt)
{
	return 16;
}




struct perstor{
	struct item* host;
	int slot;
	int bulkin;
	int bulkout;
	u64 blocksize;
	u64 totalsize;
};
static int usbstor_inquery(struct item* usb,int xxx, struct item* xhci,int slot, int inaddr,int outaddr)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct perstor* perstor = (void*)perusb->perfunc;

	struct CommandBlockWrapper* inqcbw = (void*)perstor + 0x100;
	struct INQUERY_Reply* inqrep = (void*)perstor + 0x300;
	struct CommandStatusWrapper* inqcsw = (void*)perstor + 0x200;
	usbstor_ZeroMemory(inqcbw, 0x1f);	//31byte
	usbstor_ZeroMemory(inqrep, 0x24);	//36byte
	usbstor_ZeroMemory(inqcsw, 0x0d);	//14byte

	inqcbw->Signature    = 0x43425355;
	inqcbw->Tag          = hex32('i','n','q',0);
	inqcbw->DataLen      = 0x24;
	inqcbw->DataDir      = 0x80;
	inqcbw->LogicUnitNum = 0;
	inqcbw->CmdLen       = scsi_cmd_inquery(inqcbw->CmdData);

	int ret = xhci->give_pxpxpxpx(
		xhci,slot|(outaddr<<8),
		0,0,
		(p64)inqcbw, 0x1f,
		0,0
	);

	ret = xhci->give_pxpxpxpx(
		xhci,slot|(inaddr<<8),
		0,0,
		(p64)inqrep, 0x24,
		0,0
	);
	printmemory(inqrep, 0x24);

	ret = xhci->give_pxpxpxpx(
		xhci,slot|(inaddr<<8),
		0,0,
		(p64)inqcsw, 0x0d,
		0,0
	);

	printmemory(inqcsw, 0x0d);
	if(0 != inqcsw->Status){
		logtoall("[usbdisk]csw.status=%x\n", inqcsw->Status);
		return -1;
	}
	return 0;
}
static int usbstor_testunitready(struct item* usb,int xxx, struct item* xhci,int slot, int inaddr,int outaddr)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct perstor* perstor = (void*)perusb->perfunc;

	struct CommandBlockWrapper* turcbw = (void*)perstor + 0x100;
	struct CommandStatusWrapper* turcsw = (void*)perstor + 0x200;
	usbstor_ZeroMemory(turcbw, 0x1f);
	usbstor_ZeroMemory(turcsw, 0x0d);

	turcbw->Signature    = 0x43425355;
	turcbw->Tag          = hex32('t','u','r',0);
	turcbw->DataLen      = 0;
	turcbw->DataDir      = 0x80;
	turcbw->LogicUnitNum = 0;
	turcbw->CmdLen       = scsi_cmd_testunitready(turcbw->CmdData);

	int ret = xhci->give_pxpxpxpx(
		xhci,slot|(outaddr<<8),
		0,0,
		(p64)turcbw, 0x1f,
		0,0
	);

	ret = xhci->give_pxpxpxpx(
		xhci,slot|(inaddr<<8),
		0,0,
		(p64)turcsw, 0x0d,
		0,0
	);

	printmemory(turcsw, 0x0d);
	if(0 != turcsw->Status){
		logtoall("[usbdisk]csw.status=%x\n", turcsw->Status);
		return -1;
	}
	return 0;
}
static int usbstor_readcapacity(struct item* usb,int xxx, struct item* xhci,int slot, int inaddr,int outaddr)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct perstor* perstor = (void*)perusb->perfunc;

	struct CommandBlockWrapper* capcbw = (void*)perstor + 0x100;
	struct READ_CAPACITY_Reply* caprep = (void*)perstor + 0x300;
	struct CommandStatusWrapper* capcsw = (void*)perstor + 0x200;
	usbstor_ZeroMemory(capcbw, 0x1f);
	usbstor_ZeroMemory(caprep, 8);
	usbstor_ZeroMemory(capcsw, 0x0d);
	capcbw->Signature    = 0x43425355;
	capcbw->Tag          = hex32('c','a','p',0);
	capcbw->DataLen      = 8;
	capcbw->DataDir      = 0x80;
	capcbw->LogicUnitNum = 0;
	capcbw->CmdLen       = scsi_cmd_readcapacity10(capcbw->CmdData);

	int ret = xhci->give_pxpxpxpx(
		xhci,slot|(outaddr<<8),
		0,0,
		(p64)capcbw, 0x1f,
		0,0
	);

	ret = xhci->give_pxpxpxpx(
		xhci,slot|(inaddr<<8),
		0,0,
		(p64)caprep, 8,
		0,0
	);

	ret = xhci->give_pxpxpxpx(
		xhci,slot|(inaddr<<8),
		0,0,
		(p64)capcsw, 0x0d,
		0,0
	);

	printmemory(caprep, 8);
	printmemory(capcsw, 0x0d);
	if(0 != capcsw->Status){
		logtoall("[usbdisk]csw.status=%x\n", capcsw->Status);
		return -1;
	}
	return 0;
}
//in: bytecur and bytecnt, must be integer multiple of blocksize
//out:
static int usbstor_readdata_piece(struct item* usb, u8* buf, u64 bytecur, int bytecnt)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct perstor* info = (void*)(perusb->perfunc);
	if(0 == info)return 0;

	struct item* xhci = info->host;
	int slot = info->slot;
	int bulkin = info->bulkin;
	int bulkout = info->bulkout;

	u64 blocklba = bytecur / (info->blocksize);
	int blockcnt = bytecnt / (info->blocksize);
	logtoall("lba=%x,cnt=%x\n",blocklba,blockcnt);

	struct CommandBlockWrapper cbw;
	struct CommandStatusWrapper rsw;
	usbstor_ZeroMemory(&cbw, 0x1f);
	usbstor_ZeroMemory(&rsw, 0x0d);
	cbw.Signature    = 0x43425355;
	cbw.Tag          = blocklba;
	cbw.DataLen      = bytecnt;
	cbw.DataDir      = 0x80;
	cbw.LogicUnitNum = 0;
	cbw.CmdLen       = scsi_cmd_readdata10(cbw.CmdData, blocklba, blockcnt);

	int ret = xhci->give_pxpxpxpx(
		xhci,slot|(info->bulkout<<8),
		0,0,
		(p64)&cbw, 0x1f,
		0,0
	);
	if(ret < 0){
		logtoall("[usbdisk]error@send cbw\n");
		return -1;
	}

	ret = xhci->give_pxpxpxpx(
		xhci,slot|(info->bulkin<<8),
		0,0,
		(p64)buf, bytecnt,
		0,0
	);
	if(ret < 0){
		logtoall("[usbdisk]error@recv dat\n");
		return -2;
	}

	ret = xhci->give_pxpxpxpx(
		xhci,slot|(info->bulkin<<8),
		0,0,
		(p64)&rsw, 0x0d,
		0,0
	);
	if(ret < 0){
		logtoall("[usbdisk]error@recv csw\n");
		return -3;
	}
	if(0 != rsw.Status){
		logtoall("[usbdisk]error@csw.status=%x\n", rsw.Status);
		return -4;
	}
	return bytecnt;
}




static int usbstor_readinfo(struct item* usb,void* foot,struct halfrel* stack,int sp, void* buf,int len)
{
	//logtoall("@usbstor_readinfo: %p,%p\n",usb,foot);

	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct perstor* info = (void*)(perusb->perfunc);
	if(0 == info->host){
		logtoall("host=0?\n");
		return 0;
	}
	if(0 == info->blocksize){
		logtoall("blocksize=0?\n");
		return 0;
	}

	u32 blocksize = info->blocksize;
	u64 totalsize = info->totalsize;
	int sh = usbstor_shift(totalsize);
	logtoall("type=usbdisk\n"
		"blocksize=0x%x\n"
		"totalsize=0x%llx(%d%cB)\n", blocksize, totalsize, totalsize>>sh, KMGTPE[sh/10]);
	return 0;
}
static int usbstor_readdata(struct item* usb,void* foot,struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	logtoall("@usbstor_readdata: %p,%p,%p,%x,%llx,%x,%p,%x\n",usb,foot,stack,sp,arg,cmd,buf,len);
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct perstor* info = (void*)(perusb->perfunc);
	if(0 == info->host)return 0;
	if(0 == info->blocksize)return 0;

	int j,ret;
	u64 off = arg;
	u64 bytecur = off;	//byte cur
	int bytecnt =   0;	//byte cnt
	while(1){
		if(bytecur >= off+len)break;

		bytecnt = off+len-bytecur;
		if(bytecnt > 0x10000)bytecnt = 0x10000;

		usbstor_readdata_piece(usb, buf+bytecur-off, bytecur, bytecnt);

		bytecur += bytecnt;
	}
	return bytecur-off;
}




static int usbstor_ontake(struct item* usb,void* foot,struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("usbstor_readfile:%llx,%x,%p,%x\n",arg,cmd,buf,len);
	if(_info_ == cmd)return usbstor_readinfo(usb,foot, stack,sp, buf,len);

	if(0 == buf){
		logtoall("error: buf=0\n");
		return 0;
	}
	if(0 == len){
		logtoall("error: len=0\n");
		return 0;
	}
	return usbstor_readdata(usb,foot, stack,sp, arg,cmd, buf,len);
}
static int usbstor_ongive(struct item* usb,void* foot,struct halfrel* stack,int sp, p64 arg,int off, void* buf,int len)
{
	logtoall("@ahci_ongive: %p,%p\n", usb,foot);
	return 0;
}




int usbstor_driver(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
{
	logtoall("[usbdisk]begin...\n");

	//per device
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct perstor* perstor = (void*)perusb->perfunc;

	struct DeviceDescriptor* devdesc = &perusb->origin.devdesc;
	struct descnode* confnode = &perusb->parsed.node[0];
	struct ConfigurationDescriptor* confdesc = usbdesc_offs2addr(perusb, confnode->real);


//------------------------check type------------------------
	if(subclass_SCSI != intfdesc->bInterfaceSubClass){
		logtoall("[usbdisk]not SCSI, bye bye\n");
		return -1;
	}
	if(interface_BBB == intfdesc->bInterfaceProtocol){
		logtoall("[usbdisk]bulk only\n");
		//lets go
	}
	else if(interface_UAS == intfdesc->bInterfaceProtocol){
		logtoall("[usbdisk]UASP, bye bye\n");
		return -2;
	}
	else{
		logtoall("[usbdisk]proto=%x, bye bye\n", intfdesc->bInterfaceProtocol);
		return -2;
	}


//------------------------host side + my parse------------------------
	//per interface
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;

	int j,ret;
	int inaddr=0,outaddr=0;
	struct UsbRequest req;

	j = intfnode->lchild;
	while(1){
		if(0 == j)break;
		endpnode = usbdesc_offs2addr(perusb,  j);
		endpdesc = usbdesc_offs2addr(perusb, endpnode->real);

		switch(endpdesc->bDescriptorType){
		case 5:{
			logtoall("[usbdisk]endpdesc: addr=%x, attr=%x, pktlen=%x, interval=%x\n",
				endpdesc->bEndpointAddress, endpdesc->bmAttributes,
				endpdesc->wMaxPacketSize, endpdesc->bInterval
			);

			ret = endpdesc->bEndpointAddress;
			ret = ((ret&0xf)<<1) + (ret>>7);
			if(ret&1)inaddr = ret;
			else outaddr = ret;

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
		default:{
			logtoall("[usbdisk]desctype=%x\n", endpdesc->bDescriptorType);
		}//report desc?
		}//switch

		j = endpnode->rfellow;
	}
	logtoall("[usbdisk]in@%x, out@%x\n", inaddr, outaddr);


//------------------------set configuration------------------------
	logtoall("[usbdisk]set_config\n");
	H2D_STD_DEV_SETCONF(&req, confdesc->bConfigurationValue);
	ret = xhci->give_pxpxpxpx(
		xhci,slot,
		0,0,
		(p64)&req,8,
		0,0
	);


//------------------------send INQUERY------------------------
	logtoall("[usbdisk]Inquiry...\n");
	usbstor_inquery(usb,xxx, xhci,slot, inaddr,outaddr);

//------------------------send TestUnitReady------------------------
	logtoall("[usbdisk]Test Unit Ready...\n");
	usbstor_testunitready(usb,xxx, xhci,slot, inaddr,outaddr);

//------------------------send ReadCapacity------------------------
	logtoall("[usbdisk]Read Capacity...\n");
	usbstor_readcapacity(usb,xxx, xhci,slot, inaddr,outaddr);

	struct READ_CAPACITY_Reply* caprep = (void*)perstor + 0x300;
	u32 blocklast = usbstor_endian(caprep->LastLBA_MsbNotLsb);
	u32 blocksize = usbstor_endian(caprep->BlockSz_MsbNotLsb);
	u64 totalsize = (1 + (u64)blocklast) * blocksize;
	int sh = usbstor_shift(totalsize);
	logtoall("blocklast=0x%x,blocksize=0x%x,totalsize=0x%llx(%d%cB)\n", blocklast, blocksize, totalsize, totalsize>>sh, KMGTPE[sh/10]);

//------------------------serve for the people------------------------
	usbstor_ZeroMemory(perstor, 0x100);
	perstor->host = xhci;
	perstor->slot = slot;
	perstor->bulkin = inaddr;
	perstor->bulkout = outaddr;
	perstor->blocksize = blocksize;
	perstor->totalsize = totalsize;

//------------------------important infomation------------------
	usb->kind = _usb_;
	usb->type = _stor_;
	usb->ongiving = (void*)usbstor_ongive;
	usb->ontaking = (void*)usbstor_ontake;


//------------------------file prober------------------------
	filemanager_registerdisk(usb, 0);
/*	struct artery* probe = arterycreate(_fileauto_,0,0,0);
	if(0 == probe)return 0;
	struct relation* rel = relationcreate(probe,0,_art_,_src_, usb,0,_dev_,0);
	if(0 == rel)return 0;
	arterylinkup((void*)&rel->dst, (void*)&rel->src);*/
	return 0;
}
