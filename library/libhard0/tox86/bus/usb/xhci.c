#include "libhard.h"
//commmon
#define TRB_common_Normal      1
#define TRB_common_SetupStage  2
#define TRB_common_DataStage   3
#define TRB_common_StatusStage 4
#define TRB_common_Isoch       5
#define TRB_common_Link        6
#define TRB_common_EventData   7
#define TRB_common_NoOp        8
//command
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
//event
#define TRB_event_Transfer           32
#define TRB_event_CommandCompletion  33
#define TRB_event_PortStatusChange   34
#define TRB_event_Bandwidth Request  35
#define TRB_event_Doorbell           36		 //Optional, used with virtualization only
#define TRB_event_HostController     37
#define TRB_event_DeviceNotification 38
#define TRB_event_MFINDEXWrap        39
u32 in32(u16 port);
void out32(u16 port, u32 data);
void explaindevdesc(void*);
void explaineverydesc(void*, int);
int usb_linkup(void*,int,void*,int);




//runtime registers
struct InterruptRegisters{
	u32      IMAN;	//[00,03]: bit0=pending, bit1=enable
	u32      IMOD;	//[04,07]: Interrupter Moderation, lo16=Interval, hi16=Counter
	u32    ERSTSZ;	//[08,0b]: Event Ring Segment Table Size
	u32     RsvdP;	//[0c,0f]
	u32 ERSTBA_lo;	//[10,13]: Event Ring Segment Table Base Address
	u32 ERSTBA_hi;	//[14,17]
	u32   ERDP_lo;	//[18,1b]: Event Ring Dequeue Pointer
	u32   ERDP_hi;	//[1c,1f]
}__attribute__((packed));
struct RuntimeRegisters{
	u32 MFINDEX;	//[00,03]
	u32 rsvd[7];	//[04,1f]
	struct InterruptRegisters ir[0];	//[20,x]
}__attribute__((packed));

//operational registers
struct PortRegisters{
	volatile u32 PORTSC;
	volatile u32 PORTPMSC;
	volatile u32 PORTLI;
	volatile u32 PORTHLPMC;
}__attribute__((packed));
struct OperationalRegisters{
	volatile u32     USBCMD;		//[00,03]
	volatile u32     USBSTS;		//[04,07]
	volatile u32   PAGESIZE;		//[08,0b]
	volatile u32 Rsvd0cto0f;		//[0c,0f]
	volatile u32 Rsvd10to13;		//[10,13]
	volatile u32     DNCTRL;		//[14,17]
	volatile u32    CRCR_lo;		//[18,1b]
	volatile u32    CRCR_hi;		//[1c,1f]
	volatile u32 Rsvd16b[4];		//[20,2f]
	volatile u32  DCBAAP_lo;		//[30,33]
	volatile u32  DCBAAP_hi;		//[34,37]
	volatile u32     CONFIG;		//[38,3b]
	u8 Rsvd[0x400-0x3c];	//[3c,3ff]
	struct PortRegisters port[0];	//[400,1400]
}__attribute__((packed));

//doorbell registers
struct DoorbellRegisters{
	u32 bell[0];
}__attribute__((packed));

//first 0x20 byte
struct CapabilityRegisters{
	volatile u32  lenandver;		//[0,3]: byte0=len, byte23=ver
	volatile u32 HCSPARAMS1;		//[4,7]
	volatile u32 HCSPARAMS2;		//[8,b]
	volatile u32 HCSPARAMS3;		//[c,f]
	volatile u32 CAPPARAMS1;		//[10,13]
	volatile u32      DBOFF;		//[14,17]
	volatile u32     RTSOFF;		//[18,1b]
	volatile u32 CAPPARAMS2;		//[1c,1f]
}__attribute__((packed));

//
struct SlotContext{
	//[0,3]
	u32    RouteString:20;	//[ 0,19]
	u32          Speed: 4;	//[20,23]
	u32           Rsvd: 1;	//24
	u32        MultiTT: 1;	//25
	u32            Hub: 1;	//26
	u32 ContextEntries: 5;	//[27,31]
	//[4,7]
	u16 MaxExitLatency;
	u8 RootroutestringNumber;
	u8 NumberofPorts;
	//[8,b]
	u8 ParentHubSlotID;
	u8 ParentPortNumber;
	u16       TTThinkTime: 2;
	u16             RsvdZ: 4;
	u16 InterrupterTarget:10;
	//[c,f]
	u32 USBDeviceAddress: 8;
	u32         Rsvd_c_8:19;
	u32        SlotState: 5;
}__attribute__((packed));

struct EndpointContext{
	//[0,3]
	u32     EndpointState: 3;	//[0,2]
	u32          Rsvd_0_3: 5;	//[3,7]
	u32              Mult: 2;	//[8,9]
	u32 MaxPrimaryStreams: 5;	//[10,14]
	u32  LinerStreamArray: 1;	//15
	u32          Interval: 8;	//[16,23]
	u32  MaxESITPayloadHi: 8;	//[24,31]
	//[4,7]
	u8            Rsvd_4_0: 1;
	u8          ErrorCount: 2;
	u8        EndpointType: 3;
	u8            Rsvd_4_6: 1;
	u8 HostInitiateDisable: 1;
	u8 MaxBurstSize;
	u16 MaxPacketSize;
	//[8,f]
	u64 DequeueCycleState: 1;
	u64              Rsvd: 3;
	u64  TRDequeuePointer:60;
	//[10,13]
	u16 AverageTRBLength;
	u16 MaxESITPayloadLo;
}__attribute__((packed));
//
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

struct SetupStage{
	//[0,3]
	u8 bmRequestType;
	u8 bRequest;
	u16 wValue;
	//[4,7]
	u16 wIndex;
	u16 wLength;
	//[8,b]
	u32 TRBTransferLength:17;
	u32         Rsvd_8_17: 5;
	u32   InterruptTarget:10;
	//[c,f]
	u32              Cyclebit: 1;	//0
	u32              Rsvd_c_1: 4;	//[1,4]
	u32 InterruptOnCompletion: 1;	//5
	u32         ImmediateData: 1;	//6
	u32              Rsvd_c_7: 3;	//[7,9]
	u32               TRBType: 6;	//[10,15]
	u32          TransferType: 2;	//[16,17]
	u32             Rsvd_c_18:14;	//[18,31]
}__attribute__((packed));

struct DataStage{
	//[0,7]
	u64 buffer;
	//[8,b]
	u32 TRBTransferLength:17;
	u32            TDSize: 5;
	u32   InterruptTarget:10;
	//[c,f]
	u32               Cyclebit: 1;	//0
	u32        EvaluateNextTRB: 1;	//1
	u32 InterruptOnShortPacket: 1;	//2
	u32                NoSnoop: 1;	//3
	u32               Chainbit: 1;	//4
	u32  InterruptOnCompletion: 1;	//5
	u32          ImmediateData: 1;	//6
	u32               Rsvd_c_7: 3;	//[7,9]
	u32                TRBType: 6;	//[10,15]
	u32              Direction: 1;	//16
	u32              Rsvd_c_17:15;	//[17,31]
}__attribute__((packed));

struct StatusStage{
	//[0,7]
	u64 zero;
	//[8,b]
	u32 rsvd:22;
	u32 InterruptTarget:10;
	//[c,f]
	u32              Cyclebit: 1;	//0
	u32       EvaluateNextTRB: 1;	//1
	u32              Rsvd_c_2: 2;	//[2,3]
	u32              Chainbit: 1;	//4
	u32 InterruptOnCompletion: 1;	//5
	u32              Rsvd_c_6: 4;	//[6,9]
	u32               TRBType: 6;	//[10,15]
	u32             Direction: 1;	//16
	u32             Rsvd_c_17:15;	//[17,31]
}__attribute__((packed));

struct perdev{
	u32 rootport;
	u32 routestring;
	u32 usbspeed;
	u32 rsvd;
	u32 aa;
	u32 bb;
	u32 cc;
	u32 dd;
}__attribute__((packed));

struct perendp{
	u32 queue;
	u32 cycle;
	u32 packetsize;
	u32 errorstatus;
	u32 aa;
	u32 bb;
	u32 cc;
	u32 dd;
}__attribute__((packed));

struct perslot{
	//[000,7ff]: hc can write, me read only
	u8 slotctx[0x800];

	//[800,81f]: device thing, [820,fff]: endpoint thing
	struct perdev dev;
	struct perendp ep[31];
	u8 padding[0x800 - sizeof(struct perdev) - sizeof(struct perendp)];

	//[1000,ffff]: cmdrings
	u8 ep0cmd[0x1000];	//[1000,1fff]: control endpoint
	u8 ep1out[0x1000];	//[2000,2fff]
	u8 ep1in [0x1000];	//[3000,3fff]
	u8 ep2out[0x1000];	//[4000,4fff]
	u8 ep2in [0x1000];	//[5000,5fff]
	u8 ep3out[0x1000];	//[6000,6fff]
	u8 ep3in [0x1000];	//[7000,7fff]
	u8 ep4out[0x1000];	//[8000,8fff]
	u8 ep4in [0x1000];	//[9000,9fff]
	u8 ep5out[0x1000];	//[a000,afff]
	u8 ep5in [0x1000];	//[b000,bfff]
	u8 ep6out[0x1000];	//[c000,cfff]
	u8 ep6in [0x1000];	//[d000,dfff]
	u8 ep7out[0x1000];	//[e000,efff]
	u8 ep7in [0x1000];	//[f000,ffff]
/*
	u8 ep8out[0x1000];	//[10000,10fff]
	u8 ep8in [0x1000];	//[11000,11fff]
	u8 ep9out[0x1000];	//[12000,12fff]
	u8 ep9in [0x1000];	//[13000,13fff]
	u8 ep10out[0x1000];	//[14000,14fff]
	u8 ep10in [0x1000];	//[15000,15fff]
	u8 ep11out[0x1000];	//[16000,16fff]
	u8 ep11in [0x1000];	//[17000,17fff]
	u8 ep12out[0x1000];	//[18000,18fff]
	u8 ep12in [0x1000];	//[19000,19fff]
	u8 ep13out[0x1000];	//[1a000,1afff]
	u8 ep13in [0x1000];	//[1b000,1bfff]
	u8 ep14out[0x1000];	//[1c000,1cfff]
	u8 ep14in [0x1000];	//[1d000,1dfff]
	u8 ep15out[0x1000];	//[1e000,1efff]
	u8 ep15in [0x1000];	//[1f000,1ffff]
*/
}__attribute__((packed));

//
struct perxhci{
	//mmio
	struct CapabilityRegisters* capreg;
	struct OperationalRegisters* optreg;
	struct RuntimeRegisters* runreg;
	struct DoorbellRegisters* dblreg;

	//memory
	u8* dcba;
	u8* cmdring;
	u8* erst;
	u8* eventring;

	u8* freebuf;
	u8* scratch;
	struct perslot* perslot;

	//me -> hc	//me enq, hc deq
	u32 order_len;
	u32 order_enq;
	u32 order_cycle;

	//hc -> me	//hc enq, me deq
	u32 event_len;
	u32 event_deq;
	u32 event_cycle;
}__attribute__((packed));





int maketrb_usbrequest(u8* trb, struct UsbRequest* req, u8* buf, int len)
{
	struct SetupStage* setup   = (void*)(trb+0x00);
	struct DataStage* data     = (void*)(trb+0x10);
	struct StatusStage* status = (void*)(trb+0x20);

	setup->TRBType = TRB_common_SetupStage;
	setup->TransferType = 3;
	setup->TRBTransferLength = 8;
	setup->InterruptOnCompletion = 0;
	setup->ImmediateData = 1;
	setup->bmRequestType = 0x80;	//Dir = Device-to-Host, Type = Standard, Recipient = Device
	setup->bRequest = 6;
	setup->wValue = req->wValue;	//lo = Descriptor Index, hi = Descriptor type
	setup->wIndex = req->wIndex;
	setup->wLength = len;
	setup->Cyclebit = 1;
	//
	data->TRBType = TRB_common_DataStage;
	data->Direction = 1;
	data->TRBTransferLength = len;
	data->Chainbit = 0;
	data->InterruptOnCompletion = 0;
	data->ImmediateData = 0;
	data->buffer = (u64)buf;
	data->Cyclebit = 1;
	//
	status->TRBType = TRB_common_StatusStage;	//TRB Type = Status Stage TRB.
	status->Direction = 0;
	status->Chainbit = 0;
	status->InterruptOnCompletion = 1;
	status->Cyclebit = 1;

	return 0x30;
}
int maketrb_getdesc(u8* trb, int type, u8* buf, int len)
{
	struct SetupStage* setup   = (void*)(trb+0x00);
	struct DataStage* data     = (void*)(trb+0x10);
	struct StatusStage* status = (void*)(trb+0x20);

	setup->TRBType = TRB_common_SetupStage;
	setup->TransferType = 3;
	setup->TRBTransferLength = 8;
	setup->InterruptOnCompletion = 0;
	setup->ImmediateData = 1;
	setup->bmRequestType = 0x80;	//Dir = Device-to-Host, Type = Standard, Recipient = Device
	setup->bRequest = 6;
	setup->wValue = type;	//lo = Descriptor Index, hi = Descriptor type
	setup->wIndex = 0;
	setup->wLength = len;
	setup->Cyclebit = 1;
	//
	data->TRBType = TRB_common_DataStage;
	data->Direction = 1;
	data->TRBTransferLength = len;
	data->Chainbit = 0;
	data->InterruptOnCompletion = 0;
	data->ImmediateData = 0;
	data->buffer = (u64)buf;
	data->Cyclebit = 1;
	//
	status->TRBType = TRB_common_StatusStage;	//TRB Type = Status Stage TRB.
	status->Direction = 0;
	status->Chainbit = 0;
	status->InterruptOnCompletion = 1;
	status->Cyclebit = 1;

	return 0x30;
}
int maketrb_setconf(u8* trb, int type, u8* buf, int len)
{
	struct SetupStage* setup   = (void*)(trb+0x00);
	struct StatusStage* status = (void*)(trb+0x10);

	setup->TRBType = TRB_common_SetupStage;
	setup->TransferType = 3;
	setup->TRBTransferLength = 8;
	setup->InterruptOnCompletion = 0;
	setup->ImmediateData = 1;
	setup->bmRequestType = 0x80;	//Dir = Device-to-Host, Type = Standard, Recipient = Device
	setup->bRequest = 6;
	setup->wValue = type;	//lo = Descriptor Index, hi = Descriptor type
	setup->wIndex = 0;
	setup->wLength = 0;
	setup->Cyclebit = 1;

	status->TRBType = TRB_common_StatusStage;	//TRB Type = Status Stage TRB.
	status->Direction = 0;
	status->Chainbit = 0;
	status->InterruptOnCompletion = 1;
	status->Cyclebit = 1;

	return 0x20;
}




void* xhci_takeevent(struct device* dev)
{
	struct perxhci* my = (void*)(dev->data);
	u32* ev = (u32*)(my->eventring + my->event_deq);
	if(my->event_cycle != (ev[3]&1))return 0;

	//my->runreg->ir[0].ERDP_lo = ((u64)ev) & 0xffffffff;
	//my->runreg->ir[0].ERDP_hi = ((u64)ev) >> 32;
	my->event_deq += 0x10;
	if(my->event_deq >= my->event_len){
		my->event_deq = 0;
		my->event_cycle ^= 1;
	}
	printmemory(ev, 16);

	u32 type = (ev[3]>>10)&0x3f;
	u32 stat = ev[2]>>24;
	switch(type){
	case TRB_event_Transfer: 	 			//32
		say("%d@Transfer: cmd=%p, len=%x, slot=%x, ep=%x\n", stat, *(u8**)ev, ev[2]&0xffffff, ev[3]>>24, (ev[3]>>16)&0x1f);
		break;
	case TRB_event_CommandCompletion:		//33
		say("%d@CommandCompletion: cmd=%p, param=%x, slot=%x\n", stat, *(u8**)ev, ev[2]&0xffffff, ev[3]>>24);
		break;
	case TRB_event_PortStatusChange:		//34
		say("%d@PortStatusChange: port=%x(Count from 1)\n", stat, ev[0]>>24);
		break;
	default:
		say("%d@unknown\n");
	}
	return ev;
}
int xhci_waitevent(struct device* dev, u32 wanttype)
{
	u32* ev;
	while(1){
		u32 timeout = 0xffffff;
		while(1){
			ev = xhci_takeevent(dev);
			if(0 != ev)break;

			timeout--;
			if(0 == timeout)break;
		}
		if(0 == timeout){
			say("	timeout!!!!!!\n");
			break;
			//say("	timeout@%p: %08x,%08x,%08x,%08x\n", ev, ev[0], ev[1], ev[2], ev[3]);
			//printmemory(my->event_cycle, 0x10);
		}

		u32 stat = ev[2]>>24;
		if(1 != stat)continue;

		u32 type = (ev[3]>>10)&0x3f;
		if(type != wanttype)continue;

		if(0x21 == type)return ev[3] >> 24;
		return 1;
	}
	return -1;
}




void xhci_giveorder(struct device* dev, u32 SlotEndpointStream)
{
	struct perxhci* my = (void*)(dev->data);
	if(0 == SlotEndpointStream){
		my->dblreg->bell[0] = 0;
	}
	else{
		u32 slot = SlotEndpointStream & 0xff;
		u32 endp =(SlotEndpointStream >> 8) & 0x1f;
		u32 stre =(SlotEndpointStream >>16) & 0xffff;
		my->dblreg->bell[slot] = endp | (stre<<16);
	}
}
void xhci_hostorder(struct device* dev, int slot, u32 d0,u32 d1,u32 d2,u32 d3)
{
	struct perxhci* my = (void*)(dev->data);

	//写ring
	u32* ptr = (void*)(my->cmdring + my->order_enq);
	ptr[0] = d0;
	ptr[1] = d1;
	ptr[2] = d2;
	ptr[3] = d3 | (my->order_cycle);

	//enqueue指针怎么变
	my->order_enq += 0x10;
	if(my->order_enq + 0x10 >= my->order_len){
		my->order_enq = 0;
		my->order_cycle ^= 1;
	}

	//slot: 0=to xhci, n=to device
	my->dblreg->bell[slot] = 0;
}




int xhci_EnableSlot(struct device* xhci)
{
	say("	--------------------------------\n");
	say("	EnableSlot\n");

	xhci_hostorder(xhci,0, 0,0,0,(TRB_command_EnableSlot<<10));
	int slot = xhci_waitevent(xhci, TRB_event_CommandCompletion);
	if(slot <= 0){
		say("	error=%d\n", slot);
		return -1;
	}
	say("	slot:allocated=%d\n", slot);
	if(slot >= 16)return -1;

	struct perxhci* xhcidata = (void*)(xhci->data);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
	u64* dcbatable = (u64*)(xhcidata->dcba);
	dcbatable[slot] = (u64)(slotdata->slotctx);
	return slot;
}
int xhci_DisableSlot(struct device* xhci, int slot)
{
	return 0;
}
int xhci_AddressDevice(struct device* xhci, int slot)
{
	say("	--------------------------------\n");
	say("	AddressDevice\n");
	struct perxhci* xhcidata = (void*)(xhci->data);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;

//-------------from myown get something------------
	u32 usbspeed = slotdata->dev.usbspeed;
	u32 rootport = slotdata->dev.rootport;
	u32 routestring = slotdata->dev.routestring;
	say("	speed=%x,port=%x,route=%x\n",usbspeed,rootport,routestring);

//-------------from speed know maxpacksz-------------
	u32 packetsize;
	switch(usbspeed){
	case 1:
	case 2:packetsize = 8;break;
	case 3:packetsize = 0x40;break;
	default:packetsize = 0x200;
	}
	say("	packetsize=0x%x\n", packetsize);

//-------------from xhci known slotctxsz------------
	u32 contextsize = 0x20;
	if(0x4 == (xhcidata->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;
	say("	contextsize=0x%x\n", contextsize);

//---------------------------------------------------
	//some buffer
	int j;
	u8* buf = xhcidata->freebuf;
	for(j=0;j<0x800;j++)buf[j] = 0;

	u32* incon  = (void*)(buf + 0);
	u32* devctx = (void*)(buf + contextsize);
	u32* ep0ctx = (void*)(buf + contextsize*2);

	//fill buffer
	incon[0] = 0;
	incon[1] = 3;
	for(j=2;j<8;j++)incon[j] = 0;

	devctx[0] = (1<<27) + (usbspeed<<20) + routestring;
	devctx[1] = rootport << 16;
	devctx[2] = 0;
	devctx[3] = 0;

	u32 lo = ((u64)slotdata->ep0cmd) & 0xffffffff;
	u32 hi = ((u64)slotdata->ep0cmd) >> 32;
	ep0ctx[0] = 0;
	ep0ctx[1] = (packetsize<<16) + (4<<3) + 6;
	ep0ctx[2] = lo | 1;
	ep0ctx[3] = hi;
	ep0ctx[4] = 0x8;
	//printmemory(devctx, contextsize*2);

	//send command
	lo = ((u64)incon) & 0xffffffff;
	hi = ((u64)incon) >> 32;
	xhci_hostorder(xhci,0, lo,hi,0,(slot<<24)+(TRB_command_AddressDevice<<10) );
	if(xhci_waitevent(xhci, TRB_event_CommandCompletion) < 0){
		say("	error@address device\n");
		return -1;
	}
	say("	slot:addressed device\n");

	u32 slotstate = (*(u32*)(slotdata->slotctx+0xc))>>27;
	u32 ep0state = (*(u32*)(slotdata->slotctx+contextsize))&0x3;
	say("	slotstate=%x, ep0state=%x\n", slotstate, ep0state);
	if(2 != slotstate){
		say("	slot state:%x\n",slotstate);
		return -2;
	}
	if(0 == ep0state){
		say("	ep0 wrong\n");
		return -3;
	}

	slotdata->ep[0].queue = 0;
	slotdata->ep[0].cycle = 1;
	slotdata->ep[0].packetsize = packetsize;
	slotdata->ep[0].errorstatus = 0;
	return 1;
}
int xhci_EvaluateContext(struct device* xhci, int slot, u8* devdesc, int len)
{
	//Evaluate Context Command
	say("	--------------------------------\n");
	say("	EvaluateContext\n");

	struct perxhci* xhcidata = (void*)(xhci->data);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
	if(1 != slotdata->dev.usbspeed){
		say("	not fullspeed, dont change\n");
		return 0;
	}

	u32 oldsize = slotdata->ep[0].packetsize;
	u32 newsize = devdesc[7];
	if(newsize == oldsize){
		say("	same bMaxPacketSize0, nothing change\n");
		return 0;
	}

//-------------from packet known maxpacksz------------
	slotdata->ep[0].packetsize = newsize;		//bMaxPacketSize0
	say("	packetsize=%x now\n", newsize);

//-------------from xhci known slotctxsz------------
	u32 contextsize = 0x20;
	if(0x4 == (xhcidata->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;
	say("	contextsize=0x%x\n", contextsize);

//---------------------------------------------------
	//some buffer
	int j;
	u8* buf = xhcidata->freebuf;
	for(j=0;j<0x800;j++)buf[j] = 0;

	u32* incon  = (void*)(buf + 0);
	u32* devctx = (void*)(buf + contextsize);
	u32* ep0ctx = (void*)(buf + contextsize*2);

	//change bMaxPacketSize0
	incon[0] = 0;
	incon[1] = 2;
	for(j=2;j<8;j++)incon[j] = 0;

	devctx[0] = 0;
	devctx[1] = 0;
	devctx[2] = 0;
	devctx[3] = 0;

	u32 lo = ((u64)slotdata->ep0cmd) & 0xffffffff;
	u32 hi = ((u64)slotdata->ep0cmd) >> 32;
	ep0ctx[0] = 0;
	ep0ctx[1] = (newsize<<16) + (4<<3) + 6;
	ep0ctx[2] = lo | 1;
	ep0ctx[3] = hi;
	ep0ctx[4] = 0x8;
	//printmemory(devctx, contextsize*2);

	//send command
	lo = ((u64)incon) & 0xffffffff;
	hi = ((u64)incon) >> 32;
	xhci_hostorder(xhci,0, lo,hi,0,(slot<<24)+(TRB_command_EvaluateContext<<10) );
	if(xhci_waitevent(xhci, TRB_event_CommandCompletion) < 0){
		say("	error@evaluate context\n");
		return -1;
	}
	say("	evaluated\n");

	u32 slotstate = (*(u32*)(slotdata->slotctx + 0xc))>>27;
	u32 ep0state = (*(u32*)(slotdata->slotctx + contextsize))&0x3;
	say("	slotstate=%x, ep0state=%x\n", slotstate, ep0state);
	if(2 != slotstate){
		say("	slot state:%x\n",slotstate);
		return -3;
	}
	if(0 == ep0state){
		say("	ep0 wrong\n");
		return -2;
	}
	return 0;
}
int xhci_ConfigureEndpoint(struct device* xhci, int slot, u8* epdesc, int len)
{
	say("	--------------------------------\n");
	say("	ConfigureEndpoint\n");

	struct perxhci* xhcidata = (void*)(xhci->data);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
/*
//-------------from packet known maxpacksz------------
	perslot->epinf[0].packetsize = newsize;		//bMaxPacketSize0
	say("	packetsize=%x now\n", newsize);

//-------------from xhci known slotctxsz------------
	u32 contextsize = 0x20;
	if(0x4 == (my->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;
	say("	contextsize=0x%x\n", contextsize);

//---------------------------------------------------
	//some buffer
	int j;
	u8* buf = perslot->tmpbuf;
	u32* incon  = (void*)(buf + 0);
	u32* devctx = (void*)(buf + contextsize);
	u32* ep0ctx = (void*)(buf + contextsize*2);
	incon[0] = 0;
	incon[1] = 2;
	for(j=2;j<8;j++)incon[j] = 0;

	devctx[0] = 0;
	devctx[1] = 0;
	devctx[2] = 0;
	devctx[3] = 0;

	lo = ((u64)ep0cmd) & 0xffffffff;
	hi = ((u64)ep0cmd) >> 32;
	ep0ctx[0] = 0;
	ep0ctx[1] = (packetsize<<16) + (4<<3) + 6;
	ep0ctx[2] = lo | 1;
	ep0ctx[3] = hi;
	ep0ctx[4] = 0x8;

	lo = ((u64)incon) & 0xffffffff;
	hi = ((u64)incon) >> 32;
	xhci_hostorder(xhci,0, lo,hi,0,(slot<<24)+(TRB_command_ConfigureEndpoint<<10) );
	if(xhci_waitevent(xhci, TRB_event_CommandCompletion) < 0){
		say("	error@configure endpoint\n");
		return -1;
	}
	say("	configured\n");
*/
	return 0;
}
int xhci_giveorderwaitevent(
	struct device* xhci, int slot,
	u32 whom, u32 what,
	void* sendbuf, int sendlen,
	void* recvbuf, int recvlen)
{
	if('h' == whom){	//to xhci
		switch(what){
		case TRB_command_EnableSlot:	//alloc slot from xhci
			return xhci_EnableSlot(xhci);
		case TRB_command_AddressDevice:     //prepare slotctx+ep0ctx
			return xhci_AddressDevice(xhci, slot);
		case TRB_command_EvaluateContext:     //modify ep0ctx
			return xhci_EvaluateContext(xhci, slot, sendbuf, sendlen);
		case TRB_command_ConfigureEndpoint:     //prepare ep*ctx
			return xhci_ConfigureEndpoint(xhci, slot, sendbuf, sendlen);
		}
	}//to host

	else{	//to device
		say("	--------------------------------\n");
		say("	GET_DESCRIPTOR (0x%xB)\n", recvlen);
		struct perxhci* xhcidata = (void*)(xhci->data);
		struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;

		maketrb_usbrequest(slotdata->ep0cmd + slotdata->ep[0].queue, sendbuf, recvbuf, recvlen);
		slotdata->ep[0].queue += 0x30;
		//
		xhci_giveorder(xhci, slot | (1<<8));
		if(xhci_waitevent(xhci, TRB_event_Transfer) < 0)return 0;
		//
		printmemory(recvbuf, recvlen);
		return recvlen;
	}//to device

	return 0;
}




int resetport(struct device* xhci, struct PortRegisters* port)
{
	//bit4 = PR = reset
	port->PORTSC |= 0x10;

	//wait for reset done
	int j = 0xfffffff;
	while(1){
		if(0 == (port->PORTSC & 0x10))break;

		j--;
		if(0 == j)return -1;
	}
	say("	port reseted\n");

	//wait for enable=1
	j = 0xfffffff;
	while(1){
		if(2 == (port->PORTSC & 2))break;

		j--;
		if(0 == j)return -3;
	}
	say("	port enabled\n");

	//wait for portchange event
	if(xhci_waitevent(xhci, TRB_event_PortStatusChange) < 0)return -2;
	say("	port changed\n");

	return 1;
}
void xhci_listall(struct device* xhci, int xxx, struct PortRegisters* port, int count)
{
	int j;
	u32 tmp,speed;
	for(j=0;j<count;j++){
		tmp = port[j].PORTSC;
		say("%02x:portsc=%x\n", j, tmp);
		if(0 == (tmp & 0x1))continue;

		//if(0 == PORTSC.bit1)ver <= 2.0, have to reset
		if(0 == (tmp & 0x2)){
			if(resetport(xhci, &port[j]) <= 0){
				say("	reset failed: portsc=%x\n", port[j].PORTSC);
				continue;
			}
		}
		//todo:检查错误

		//link state
		tmp = port[j].PORTSC;
		say("	portsc=%08x,linkstate=%x\n", tmp, (tmp >> 5) & 0xf);

		//usb speed
		speed = (port[j].PORTSC >> 10) & 0xf;
		switch(speed){
		case 7:say("	10Gb,x2\n");break;
		case 6:say("	5Gb,x2\n");break;
		case 5:say("	10Gb,x1\n");break;
		case 4:say("	5Gb,x1\n");break;
		case 3:say("	480Mb\n");break;
		case 2:say("	1.5Mb\n");break;	//yes, 2=low speed
		case 1:say("	12Mb\n");break;	//yes, 1=full speed
		default:say("	speed=%x\n", speed);
		}

		//probedevice(xhci, speed, j+1, 0);

		//alloc slot
		int slot = xhci_giveorderwaitevent(xhci,0, 'h',TRB_command_EnableSlot, 0,0, 0,0);
		if(slot <= 0 | slot >= 16)continue;

		struct perxhci* xhcidata = (void*)(xhci->data);
		struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
		slotdata->dev.usbspeed = speed;
		slotdata->dev.rootport = j+1;
		slotdata->dev.routestring = 0;

		//let usb do rest
		struct device* usb = devicecreate(_usb_, 0, 0, 0);
		if(usb)usb_linkup(usb, 0, xhci, slot);
	}
}




int ownership(volatile u32* p)
{
	//set hc os owned semaphore
	say("	before handoff:%08x,%08x\n", p[0],p[1]);
	p[0] |= 0x1000000;

	//wait until bit16==0 && bit24==1
	volatile int timeout = 0xfffffff;
	while(1){
		if(0x01000000 == (p[0] & 0x01010000)){
			say("	after handoff:%08x,%08x\n", p[0],p[1]);
			return 1;
		}

		timeout--;
		if(0 == timeout){
			say("	timeout@handoff\n");
			return -1;
		}
	}
}
void supportedprotocol(u32* p)
{
	u32 major = p[0] >> 24;
	u32 minor =(p[0] >> 16) & 0xff;
	u32 port0 =(p[2] & 0xff) - 1;
	u32 portn =(port0-1) + ((p[2] >> 8) & 0xff);
	say("	[%x,%x]: usb%x.%x\n", port0, portn, major, minor);
}
void explainxecp(u32* at)
{
	say("xecp parsing\n");
	while(1){
		say("	@%p: %08x,%08x,%08x,%08x\n", at, at[0], at[1], at[2], at[3]);

		u32 type = at[0] & 0xff;
		switch(type){
			case 1:{
				ownership(at);
				break;
			}
			case 2:{
				supportedprotocol(at);
				break;
			}
			defualt:{
				say("???\n");
			}
		}

		u32 next = ((at[0] >> 8) & 0xff) << 2;
		if(0 == next)break;

		at = (void*)at + next;
	}
}




int xhci_mmioinit(struct device* dev, u8* xhciaddr)
{
	say("xhci@mmio:%p{\n", xhciaddr);
	//printmmio(addr, 0x20);


//---------------clear home---------------------
	int j,k;
	u8* ptr = memorycreate(0x200000, 0);
	for(j=0;j<0x200000;j++)ptr[j] = 0;

	u64 dcbahome      = (u64)(ptr + 0x000000);	//64k*[ 0, 1)
	u64 cmdringhome   = (u64)(ptr + 0x010000);	//64k*[ 1, 2)
	u64 ersthome      = (u64)(ptr + 0x020000);	//64k*[ 2, 3)
	u64 eventringhome = (u64)(ptr + 0x030000);	//64k*[ 3, 4)
	u64 freebufhome   = (u64)(ptr + 0x040000);	//64k*[ 4, 8)
	u64 scratchpad    = (u64)(ptr + 0x080000);	//64k*[ 8, f)
	u64 perslot       = (u64)(ptr + 0x100000);	//64k*[16,32)


//--------------basic infomation--------------------
	struct CapabilityRegisters* capreg = (void*)xhciaddr;
	u32 version = capreg->lenandver >> 16;
	u32 caplen = capreg->lenandver & 0xff;

	struct OperationalRegisters* optreg = (void*)(xhciaddr + caplen);
	struct RuntimeRegisters*     runreg = (void*)(xhciaddr + capreg->RTSOFF);
	struct DoorbellRegisters*    dblreg = (void*)(xhciaddr + capreg->DBOFF);

	say("base information:version=%x,caplen=%x\n", version, caplen);

	say("	capability@%p\n", capreg);
	say("	operational@%p\n", optreg);
	say("	runtime@%p\n", runreg);
	say("	doorbell@%p\n", dblreg);

	u32 tmp;
	u32 maxscratch;
	u32 maxerst;
	tmp = capreg->HCSPARAMS1;
	say("	hcsparams1=%08x\n", tmp);
	say("	.maxslots=%x\n", tmp&0xff);
	say("	.maxintrs=%x\n", (tmp>>8)&0x7ff);
	say("	.maxports=%x\n", (tmp>>24)&0xff);
	tmp = capreg->HCSPARAMS2;
	say("	hcsparams2=%08x\n", tmp);
	say("	.Isochronous Scheduling Threshold=%x\n", tmp&0xf);
	maxerst = (tmp>>4)&0xf;
	say("	.Event Ring Segment Table Max=2^%x\n", maxerst);
	maxscratch = (((tmp>>21)&0x1f)<<5) | ((tmp>>27)&0x1f);
	say("	.Max Scratchpad Buffers=%xpages\n", maxscratch);
	tmp = capreg->HCSPARAMS3;
	say("	hcsparams3=%08x\n", tmp);
	say("	.U1 Device Exit Latency=%x\n", tmp&0xff);
	say("	.U2 Device Exit Latency=%x\n", (tmp>>16)&0xffff);
	tmp = capreg->CAPPARAMS1;
	say("	capparams1=%08x\n", tmp);
	say("	.AC64=%x\n", tmp&1);
	say("	.CSZ=%x\n", (tmp>>2)&1);
	tmp = capreg->CAPPARAMS2;
	say("	capparams2=%08x\n", tmp);
	say("	.VTIO support=%x\n", (tmp>>9)&1);


//--------------print operational---------------
	u32 psz      = optreg->PAGESIZE;
	u32 pagesize = 0x1000;
	while(1){
		if(0 == psz){
			say("psz:\n",psz);
			return -1;
		}
		if(1 == psz)break;

		psz >>= 1;
		pagesize <<= 1;
	}
	say("before anything:\n");
	say("	usbcommand:%x\n", optreg->USBCMD);
	say("	usbstatus:%x\n", optreg->USBSTS);
	say("	psz=%x,pagesize=%x\n", psz, pagesize);
	say("	crcr:%x\n", ((u64)optreg->CRCR_hi<<32) | optreg->CRCR_lo);
	say("	dcbaa:%x\n", ((u64)optreg->DCBAAP_hi<<32) | optreg->DCBAAP_lo);
	say("	config:%x\n", optreg->CONFIG);


//--------------grab ownership-----------------
	u32* xecp = (void*)xhciaddr + ((capreg->CAPPARAMS1 >> 16) << 2);
	explainxecp(xecp);


//------------operational registers----------------
	say("setup optreg@%p\n", optreg);

	//xhci正在运行吗
	if(0 == (optreg->USBSTS&0x1)){		//=0: runing
		say("	hc stopping\n");

		//按下停止按钮
		optreg->USBCMD = optreg->USBCMD & 0xfffffffe;

		//等一会
		u32 wait1 = 0xfffffff;
		while(1){
			if(1 == (optreg->USBSTS&0x1))break;
			wait1--;
			if(0 == wait1){
				say("	not stop\n");
				return -2;
			}
		}
	}
	say("	hc stopped\n");

	//按下复位按钮
	optreg->USBCMD = optreg->USBCMD | 2;

	//等一会
	u32 wait2 = 0xfffffff;
	while(1){
		if(0 == (optreg->USBCMD&0x2))break;
		wait2--;
		if(0 == wait2){
			say("	reset failed:%x\n", optreg->USBCMD);
			return -3;
		}
	}
	say("	hc reseted\n");

	//controller not ready=1就是没准备好
	if(0x800 == (optreg->USBSTS&0x800)){
		say("	not ready:%x\n", optreg->USBSTS);
		return -4;
	}
	say("	hc ready\n");

	//maxslot=deviceslots
	optreg->CONFIG = (capreg->HCSPARAMS1) &0xff;
	say("	CONFIG=%x\n", optreg->CONFIG);

	//scratcharray[0 to max]: each is a page
	say("	devctx: scratcharray[0-max) = scratchbuffer@%llx\n", scratchpad + pagesize);
	for(j=0;j<maxscratch;j++){
		*(u64*)(scratchpad+j*8) = scratchpad + pagesize*(j+1);
	}

	//[dcbahome] = scratcharray
	say("	devctx: dcbahome[0] = scratcharray@%llx\n", scratchpad);
	*(u64*)(dcbahome + 0) = scratchpad;

	//[dcbaap] = dcbahome
	say("	devctx: dcbaap[0] = dcbahome@%llx\n", dcbahome);
	optreg->DCBAAP_lo = dcbahome & 0xffffffff;
	optreg->DCBAAP_hi = dcbahome >> 32;

	//command linktrb:lastone point to firstone
	say("	crcr: lasttrb point to firsttrb\n");
	*(u64*)(cmdringhome + 0xfff*0x10 + 0x0) = cmdringhome;
	*(u32*)(cmdringhome + 0xfff*0x10 + 0x8) = 0;
	*(u32*)(cmdringhome + 0xfff*0x10 + 0xc) = (TRB_common_Link<<10) + 2;

	//[crcr] = cmdringhome
	say("	crcr: set cmdring@%llx\n", cmdringhome+1);
	optreg->CRCR_lo = cmdringhome+1;
	optreg->CRCR_hi = 0;


//-------------------runtime registers-------------------
//msix table,message addr,message data,enable vector
//allocate&init msix pending bit array
//point table&pba offset to message control table and pending bit array
//init message control register of msix capability structure
//-------------------------------------------------
	say("setup runreg@%p\n", runreg);

	//setupisr(xhciaddr);
	//----------------------if(msix)--------------------------
	//----------------------if(msi)--------------------------
	//----------------------if(intx)--------------------------

	say("	building eventring@%llx\n", eventringhome);

	//build the "event ring segment table"
	say("	building erst@%llx\n", ersthome);
	*(u64*)(ersthome + 0x0) = eventringhome;
	*(u64*)(ersthome + 0x8) = 0x100;            //0x1000*0x10=0x10000
/*
	*(u64*)(ersthome + 0x10) = secondeventringhome;
	*(u64*)(ersthome + 0x18) = 0x100;            //0x1000*0x10=0x10000
	*(u64*)(ersthome + 0x20) = thirdeventringhome;
	*(u64*)(ersthome + 0x28) = 0x100;            //0x1000*0x10=0x10000
*/

	say("	setting ir[0]\n");
	runreg->ir[0].ERSTSZ = 1;
	runreg->ir[0].ERSTBA_lo = ersthome & 0xffffffff;
	runreg->ir[0].ERSTBA_hi = ersthome >> 32;
	runreg->ir[0].ERDP_lo = eventringhome & 0xffffffff;
	runreg->ir[0].ERDP_hi = eventringhome >> 32;
	runreg->ir[0].IMOD = 4000;
	runreg->ir[0].IMAN |= 0x2;


//---------------------xhci启动---------------------
//write usbcmd,turn host controller on
//-------------------------------------------------
	say("turnon hc:\n");

	//enable EWE|HSEIE|INTE(0x400|0x8|0x4) in USBCMD
//	*(u32*)operational |= 0x40c;
//	*(u32*)operational = (*(u32*)operational) | 0xc;      //no interrupt

	optreg->USBCMD = optreg->USBCMD | 0x1;
	say("	USBCMD=%08x\n", optreg->USBCMD);

	say("}\n");


//-------------------list device------------------
	say("each port:\n");
	struct perxhci* my = (void*)(dev->data);
	//mmio
	my->capreg = capreg;
	my->optreg = optreg;
	my->runreg = runreg;
	my->dblreg = dblreg;
	//memory
	my->dcba      = (void*)dcbahome;
	my->cmdring   = (void*)cmdringhome;
	my->erst      = (void*)ersthome;
	my->eventring = (void*)eventringhome;
	my->freebuf   = (void*)freebufhome;
	my->scratch   = (void*)scratchpad;
	my->perslot   = (void*)perslot;
	//ev
	my->event_len = 0x10000;
	my->event_deq = 0;
	my->event_cycle = 1;
	//cmd
	my->order_len = 0x10000;
	my->order_enq = 0;
	my->order_cycle = 1;
	xhci_listall(dev, 0, optreg->port, capreg->HCSPARAMS1>>24);
	return 0;
}
int xhci_portinit(struct device* dev, u32 addr)
{
	u64 temp,high;
	say("xhci@port:%x{\n", addr);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc);
	say("sts,cmd=%x\n", temp);

	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp | (1<<10) | (1<<2));	//bus master=1

	out32(0xcf8, addr+0x10);
	temp = in32(0xcfc);
	say("bar0=%x\n", temp);			//lo
	out32(0xcf8, addr+0x14);
	high = in32(0xcfc);
	say("bar1=%x\n", high);			//hi
	out32(0xcf8, addr+0x18);
	say("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	say("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	say("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	say("bar5=%x\n", in32(0xcfc));

	say("}\n");


	//xhci mmio
	u8* mmio = (u8*)((temp&0xfffffff0) | (high<<32));
	return xhci_mmioinit(dev, mmio);
}
