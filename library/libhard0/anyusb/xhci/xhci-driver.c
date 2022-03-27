#include "libhard.h"
#define debug_bulktransfer 0
#define debug_transfer 0
#define debug_waitevent 0
#define xhci_print(fmt, ...) say("<%08lld,xhci>"fmt, timeread_us(), ##__VA_ARGS__)
int usbany_linkup(void*,int,void*,int);




#define _addr_ hex32('a','d','d','r')	//prepare slotctx+ep0ctx
#define _eval_ hex32('e','v','a','l')	//modify epctx
#define _conf_ hex32('c','o','n','f')	//prepare ep*ctx
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
struct EndpointDescriptor{
	u8          bLength;		//0: 0x09
	u8  bDescriptorType;		//1: 0x05
	u8 bEndpointAddress;		//2: endpoint number and direction
	u8     bmAttributes;		//3: endpoint attribute
	u16  wMaxPacketSize;		//[4,5]
	u8        bInterval;		//6: interval between two access
}__attribute__((packed));




//speed
#define SPEED_FS 1
#define SPEED_LS 2
#define SPEED_HS 3
#define SPEED_SS_1x1 4
#define SPEED_SSP_2x1 5
#define SPEED_SSP_1x2 6
#define SPEED_SSP_2x2 7
//eptype
#define EPType_NotValid     0
#define EPType_IsochOut     1
#define EPType_BulkOut      2
#define EPType_InterruptOut 3
#define EPType_ControlBidir 4
#define EPType_IsochIn      5
#define EPType_BulkIn       6
#define EPType_InterruptIn  7
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
//completion
#define TRB_completion_Success                     1
#define TRB_completion_DataBufferError             2
#define TRB_completion_BabbleDetectedError         3
#define TRB_completion_USBTransactionError         4
#define TRB_completion_TRBError                    5
#define TRB_completion_StallError                  6
#define TRB_completion_ResourceError               7
#define TRB_completion_BandwidthError              8
#define TRB_completion_NoSlotsAvailableError       9
#define TRB_completion_InvalidStreamTypeError      10
#define TRB_completion_SlotNotEnabledError         11
#define TRB_completion_EndpointNotEnabledError     12
#define TRB_completion_ShortPacket                 13
#define TRB_completion_RingUnderrun                14
#define TRB_completion_RingOverrun                 15
#define TRB_completion_VFEventRingFullError        16
#define TRB_completion_ParameterError              17
#define TRB_completion_BandwidthOverrunError       18
#define TRB_completion_ContextStateError           19
#define TRB_completion_NoPingResponseError         20
#define TRB_completion_EventRingFullError          21
#define TRB_completion_IncompatibleDeviceError     22
#define TRB_completion_MissedServiceError          23
#define TRB_completion_CommandRingStopped          24
#define TRB_completion_CommandAborted              25
#define TRB_completion_Stopped                     26
#define TRB_completion_Stopped_LengthInvalid       27
#define TRB_completion_Stopped_ShoftPacket         28
#define TRB_completion_MaxExitLatencyTooLargeError 29
#define TRB_completion_IsochBufferOverrun          31
#define TRB_completion_EventLostError              32
#define TRB_completion_UndefinedError              33
#define TRB_completion_InvalidStreamIDError        34
#define TRB_completion_SecondaryBandwidthError     35
#define TRB_completion_SplitTransationError        36
//#define TRB_completion_VendorDefinedError 192-223
//#define TRB_completion_VendorDefinedInfo 224-255
//runtime registers
struct InterruptRegisters{
	volatile u32      IMAN;	//[00,03]: bit0=pending, bit1=enable
	volatile u32      IMOD;	//[04,07]: Interrupter Moderation, lo16=Interval, hi16=Counter
	volatile u32    ERSTSZ;	//[08,0b]: Event Ring Segment Table Size
	volatile u32     RsvdP;	//[0c,0f]
	volatile u32 ERSTBA_lo;	//[10,13]: Event Ring Segment Table Base Address
	volatile u32 ERSTBA_hi;	//[14,17]
	volatile u32   ERDP_lo;	//[18,1b]: Event Ring Dequeue Pointer
	volatile u32   ERDP_hi;	//[1c,1f]
}__attribute__((packed));	//must volatile, must packed
struct RuntimeRegisters{
	volatile u32 MFINDEX;	//[00,03]
	volatile u32 rsvd[7];	//[04,1f]
	struct InterruptRegisters ir[0];	//[20,x]
}__attribute__((packed));	//must volatile, must packed
//operational registers
struct PortRegisters{
	volatile u32 PORTSC;
	volatile u32 PORTPMSC;
	volatile u32 PORTLI;
	volatile u32 PORTHLPMC;
}__attribute__((packed));	//must volatile, must packed
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
}__attribute__((packed));	//must volatile, must packed
struct DoorbellRegisters{
	volatile u32 bell[0];
}__attribute__((packed));	//must volatile, must packed
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
}__attribute__((packed));	//must volatile, must packed




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
	u8 RootHubPortNumber;
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
	u64 DequeueCycleAndPointer;
	//[10,13]
	u16 AverageTRBLength;
	u16 MaxESITPayloadLo;
}__attribute__((packed));




struct NoOpTRB{
	//[0,7]
	u64 zero;
	//[8,b]
	u32          Rsvd_8:22;
	u32 InterruptTarget:10;
	//[c,d]
	u16              Cyclebit:1;	//0
	u16       EvaluateNextTRB:1;	//1
	u16              Rsvd_2_3:2;	//[2,3]
	u16              Chainbit:1;	//4
	u16 InterruptOnCompletion:1;	//5
	u16              Rsvd_6_9:4;	//[6,9]
	u16               TRBType:6;	//[10,15]
	//[e,f]
	u16 unused;
}__attribute__((packed));
struct LinkTRB{
	u64 addr;
	//[8,b]
	u32 Rsvd:22;
	u32 InterruptTarget:10;
	//[c,d]
	u16              Cyclebit:1;	//0
	u16           ToggleCycle:1;	//1
	u16              Rsvd_c_2:2;	//[2,3]
	u16              Chainbit:1;	//4
	u16 InterruptOnCompletion:1;	//5
	u16              Rsvd_c_6:4;	//[6,9]
	u16               TRBType:6;	//[10,15]
	//[e,f]
	u16 unused;
}__attribute__((packed));
struct NormalTRB{
	//[0,7]
	u64 DataBufferPointer;
	//[8,b]
	u32 TRBTransferLength:17;
	u32            TDSize: 5;
	u32   InterruptTarget:10;
	//[c,d]
	u16               Cyclebit:1;	//0
	u16        EvaluateNextTRB:1;	//1
	u16 InterruptonShortPacket:1;	//2
	u16                NoSnoop:1;	//3
	u16               Chainbit:1;	//4
	u16  InterruptOnCompletion:1;	//5
	u16          ImmediateData:1;	//6
	u16                   Rsvd:2;	//[7,8]
	u16    BlockEventInterrupt:1;	//9
	u16                TRBType:6;	//[10,15]
	//[e,f]
	u16 unused;
}__attribute__((packed));
struct IsochTRB{
	//[0,7]
	u64 DataBufferPointer;
	//[8,b]
	u32 TRBTransferLength:17;
	u32            TDSize: 5;
	u32   InterruptTarget:10;
	//[c,f]
	u32                     Cyclebit: 1;	//0
	u32              EvaluateNextTRB: 1;	//1
	u32       InterruptonShortPacket: 1;	//2
	u32                      NoSnoop: 1;	//3
	u32                     Chainbit: 1;	//4
	u32        InterruptOnCompletion: 1;	//5
	u32                ImmediateData: 1;	//6
	u32           TransferBurstCount: 2;	//[7,8]
	u32          BlockEventInterrupt: 1;	//9
	u32                      TRBType: 6;	//[10,15]
	u32 TransferLastBurstPacketCount: 4;	//[16,19]
	u32                      FrameID:11;	//[20,30]
	u32               StartIsochASAP: 1;	//31
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
	u32 eptype;
	u32 packetsize;
	u32 cycle;
	u32 rsvd;
	u32 myenq;
	u32 hcdeq;
	u64 contractor;
}__attribute__((packed));
struct myctx{
	//[000,7ff]: hc can write, me read only
	u8 unusable[0x800];

	//[800,81f]: device thing, [820,fff]: endpoint thing
	union{
		struct perdev devctx;
		struct perendp epnctx[32];
		u8 padding[0x800];
	};
}__attribute__((packed));
struct perslot{
	union{
		//[0,7ff]: hc can write, me read only
		u8 hcctx[0x800];

		//[0,fff]: 0x20B per DCI, first 0x800B unused
		struct myctx myctx;

		//[0,ffff]: 0x1000B per DCI, first 0x1000B reserved
		u8 cmdring[32][0x1000];
	};
}__attribute__((packed));
struct perport{
	u32 protocol;	//u2 or u3
	u32 slot;
}__attribute__((packed));
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
	struct perport* perport;
	u8* scratch;
	struct perslot* perslot;

	//me -> hc
	u32 order_len;
	u32 order_cycle;
	u32 order_myenq;	//me enq
	u32 order_hcdeq;	//hc deq

	//hc -> me
	u32 event_len;
	u32 event_cycle;
	u32 event_hcenq;	//hc enq
	u32 event_mydeq;	//me deq
}__attribute__((packed));




int xhci_UsbInterval2XhcInterval(int usbspeed, int eptype, int bInterval)
{
	//realtime in us
	int time;
	switch(usbspeed){
	case 2:
		//2=LS,interrupt: i*1000us
		time = bInterval*1000;
		break;
	case 1:
		if((EPType_InterruptOut == eptype) | (EPType_InterruptIn == eptype)){
			//1=FS,interrupt: i*1000us
			time = bInterval*1000;
		}
		else{
			//1=FS,isoch: 2^(i-1)*1000us
			time = (1<<(bInterval-1))*1000;
		}
		break;
	default:
		//HS,SS,SS+: 2^(i-1)*125us
		time = (1<<(bInterval-1))*125;
	}
//xhci_print("usbspeed=%x,eptype=%x,binterval=%x,time=%x\n", usbspeed, eptype, bInterval, time);
	//
	time /= 125;
	bInterval = 0;
	while(1){
		if(1 >= time)break;
		time = time >> 1;
		bInterval++;
	}
	return bInterval;
}
int xhci_ZeroMemory(void* buf, int len)
{
	int j;
	u8* tmp = buf;
	for(j=0;j<len;j++)tmp[j] = 0;
	return j;
}




int maketrb_controltransfer(u8* trb, struct UsbRequest* req, u8* buf, int len)
{
	struct SetupStage* setup   = (void*)(trb+0x00);
	xhci_ZeroMemory(setup, 0x10);
	setup->TRBType = TRB_common_SetupStage;
	setup->TransferType = 3;
	setup->TRBTransferLength = 8;
	setup->InterruptOnCompletion = 0;
	setup->ImmediateData = 1;
	setup->bmRequestType = req->bmRequestType;
	setup->bRequest = req->bRequest;
	setup->wValue = req->wValue;
	setup->wIndex = req->wIndex;
	setup->wLength = req->wLength;

	if(0 == req->wLength){
		struct StatusStage* status = (void*)(trb+0x10);
		xhci_ZeroMemory(status, 0x10);

		status->TRBType = TRB_common_StatusStage;
		status->Direction = 0;
		status->Chainbit = 0;
		status->InterruptOnCompletion = 1;

		setup->Cyclebit = 1;
		status->Cyclebit = 1;
		return 0x20;
	}
	else{
		struct DataStage* data     = (void*)(trb+0x10);
		struct StatusStage* status = (void*)(trb+0x20);
		xhci_ZeroMemory(data, 0x20);
		//
		data->TRBType = TRB_common_DataStage;
		data->Direction = 1;
		data->TRBTransferLength = len;
		data->Chainbit = 0;
		data->InterruptOnCompletion = 0;
		data->ImmediateData = 0;
		data->buffer = (u64)buf;
		//
		status->TRBType = TRB_common_StatusStage;
		status->Direction = 0;
		status->Chainbit = 0;
		status->InterruptOnCompletion = 1;

		setup->Cyclebit = 1;
		data->Cyclebit = 1;
		status->Cyclebit = 1;
		return 0x30;
	}
	return 0;
}
int maketrb_bulktransfer(u8* ring, int xxx, u8* buf, int len)
{
	struct NormalTRB* trb = (void*)ring;
	xhci_ZeroMemory(trb, 0x10);

	trb->DataBufferPointer = (u64)(buf);

	trb->TRBTransferLength = len;
	trb->TDSize = 0;
	trb->InterruptTarget = 0;

	trb->EvaluateNextTRB = 0;
	trb->InterruptonShortPacket = 0;
	trb->NoSnoop = 0;
	trb->Chainbit = 0;
	trb->InterruptOnCompletion = 1;
	trb->ImmediateData = 0;
	trb->Rsvd = 0;
	trb->BlockEventInterrupt = 0;
	trb->TRBType = TRB_common_Normal;
	trb->unused = 0;

	trb->Cyclebit = 1;
	return 0x10;
}
int maketrb_interruptout(u8* ring, int xxx, u8* buf, int len)
{
	struct NormalTRB* trb;
	xhci_ZeroMemory(ring, 0x10);

	trb = (void*)ring;
	trb->DataBufferPointer = (u64)buf;

	trb->TRBTransferLength = len;
	trb->TDSize = 0;
	trb->InterruptTarget = 0;

	trb->EvaluateNextTRB = 0;
	trb->InterruptonShortPacket = 0;
	trb->NoSnoop = 0;
	trb->Chainbit = 0;
	trb->InterruptOnCompletion = 1;
	trb->ImmediateData = 0;
	trb->Rsvd = 0;
	trb->BlockEventInterrupt = 0;
	trb->TRBType = TRB_common_Normal;
	trb->unused = 0;

	trb->Cyclebit = 1;
	return 0x10;
}
int maketrb_interruptin(u8* ring, int xxx, u8* buf, int len)
{
	int j;
	struct NormalTRB* trb;
	xhci_ZeroMemory(ring, 0x1000);

	for(j=0;j<0xff;j++){
		trb = (void*)(ring + j*0x10);
		trb->DataBufferPointer = (u64)(buf + j*len);

		trb->TRBTransferLength = len;
		trb->TDSize = 0;
		trb->InterruptTarget = 0;

		trb->EvaluateNextTRB = 0;
		trb->InterruptonShortPacket = 0;
		trb->NoSnoop = 0;
		trb->Chainbit = 0;
		trb->InterruptOnCompletion = 1;
		trb->ImmediateData = 0;
		trb->Rsvd = 0;
		trb->BlockEventInterrupt = 0;
		trb->TRBType = TRB_common_Normal;
		trb->unused = 0;

		trb->Cyclebit = 1;
	}

	struct LinkTRB* link = (void*)(ring+0xff0);
	link->addr = (u64)ring;

	link->Rsvd = 0;
	link->InterruptTarget = 0;

	link->ToggleCycle = 0;		//sametrb forever
	link->Chainbit = 0;
	link->InterruptOnCompletion = 0;
	link->TRBType = TRB_common_Link;

	link->Cyclebit = 1;
	return 0x1000;
}




void* xhci_takeevent(struct item* xhci)
{
	struct perxhci* my = (void*)(xhci->priv_256b);
	u32* ev = (u32*)(my->eventring + my->event_mydeq);
	if(my->event_cycle != (ev[3]&1))return 0;

	//my->runreg->ir[0].ERDP_lo = ((u64)ev) & 0xffffffff;
	//my->runreg->ir[0].ERDP_hi = ((u64)ev) >> 32;
	my->event_mydeq += 0x10;
	if(my->event_mydeq >= my->event_len){
		my->event_mydeq = 0;
		my->event_cycle ^= 1;
	}

	struct RuntimeRegisters* runreg = my->runreg;
	u64 erdp = ((u64)my->eventring + my->event_mydeq) | 0x8;
	runreg->ir[0].ERDP_lo = erdp & 0xffffffff;
	runreg->ir[0].ERDP_hi = erdp >> 32;
	return ev;
}
int xhci_parseevent(struct item* xhci, u32* ev)
{
	u32 stat = ev[2]>>24;
	u32 type = (ev[3]>>10)&0x3f;
	u32 slot = ev[3]>>24;
	u32 endp;
	u32 port;
	struct perxhci* xhcidata;
	struct perslot* slotdata;
	struct perendp* endpdata;
	struct item* usb;
	switch(type){
	case TRB_event_Transfer: 	 			//32
		endp = (ev[3]>>16)&0x1f;
		//update perslot.epctx.hcdeq

		xhcidata = (void*)(xhci->priv_256b);
		slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
		endpdata = &slotdata->myctx.epnctx[endp];
		usb = (void*)endpdata->contractor;
		if(usb){
			if(usb->ongiving){
				usb->ongiving(usb,0, xhci,endp, *(u8**)ev,0, 0,0);
				break;
			}
		}

		if(debug_transfer)xhci_print("%d@Transfer: cmd=%p, len=%x, slot=%x, ep=%x\n", stat, *(u8**)ev, ev[2]&0xffffff, slot, endp);
		break;

	case TRB_event_CommandCompletion:		//33
		xhci_print("%d@CommandCompletion: cmd=%p, param=%x, slot=%x\n", stat, *(u8**)ev, ev[2]&0xffffff, slot);
		break;

	case TRB_event_PortStatusChange:		//34
		port = ev[0]>>24;
		xhci_print("%d@PortStatusChange: port=%x(Count from 1)\n", stat, port);
		break;

	case TRB_event_HostController:		//37
		xhci_print("%d@HostController\n", stat);
		break;

	default:
		printmemory(ev, 16);
		xhci_print("code=%d@evtype=%d\n", stat, type);
	}

	return 0;
}
int xhci_waitevent(struct item* xhci, u32 wanttype, u32 wantarg)
{
	u32 stat,port;
	u32 slot,endp;
	u32 type,arg;
	u32* ev;

	u64 endtime = timeread_us() + 5000*1000;
	u32 endcycle = 0xffffff;
	while(1){
		ev = xhci_takeevent(xhci);
		if(ev){
			//print
			if(debug_waitevent)printmemory(ev, 16);

			//parse
			xhci_parseevent(xhci, ev);

			//return
			stat = ev[2]>>24;
			slot = ev[3]>>24;
			type = (ev[3]>>10)&0x3f;
			arg = wantarg;
			switch(type){
			case TRB_event_Transfer: 	 			//32
				endp = (ev[3]>>16)&0x1f;
				arg = slot | (endp<<8);
				//update perslot.epctx.hcdeq
				if((wanttype == type)&&(wantarg == arg)){
					if(1 == stat)return arg;
					else return -1;
				}
				break;

			case TRB_event_CommandCompletion:		//33
				if(wanttype == type){
					if(1 == stat)return slot;
					else return -1;
				}
				break;

			case TRB_event_PortStatusChange:		//34
				port = ev[0]>>24;
				arg = port;
				if((wanttype == type)&&(wantarg == arg)){
					if(1 == stat)return arg;
					else return -1;
				}
				break;
			}

			if(debug_waitevent)xhci_print("event unwanted: wanttype=%x,thistype=%x,wantarg=%x,thisarg=%x\n", wanttype, type, wantarg, arg);
		}//if(ev)

		if(timeread_us() > endtime)break;

		endcycle -= 1;
		if(0 == endcycle)break;
	}

	xhci_print("timeout!!!!!!\n");
	return -1;
}




void xhci_giveorder(struct item* xhci, u32 SlotEndpointStream)
{
	struct perxhci* my = (void*)(xhci->priv_256b);
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
void xhci_hostorder(struct item* xhci, int slot, u32 d0,u32 d1,u32 d2,u32 d3)
{
	struct perxhci* my = (void*)(xhci->priv_256b);

	//写ring
	u32* ptr = (void*)(my->cmdring + my->order_myenq);
	ptr[0] = d0;
	ptr[1] = d1;
	ptr[2] = d2;
	ptr[3] = d3 | (my->order_cycle);

	//enqueue指针怎么变
	my->order_myenq += 0x10;
	if(my->order_myenq + 0x10 >= my->order_len){
		my->order_myenq = 0;
		my->order_cycle ^= 1;
	}

	//slot: 0=to xhci, n=to device
	my->dblreg->bell[slot] = 0;
}




int xhci_EnableSlot(struct item* xhci)
{
	xhci_print("--------------------------------\n");
	xhci_print("EnableSlot\n");

	xhci_hostorder(xhci,0, 0,0,0,(TRB_command_EnableSlot<<10));
	int slot = xhci_waitevent(xhci, TRB_event_CommandCompletion, 0);
	if(slot <= 0){
		xhci_print("error=%d\n", slot);
		return -1;
	}
	xhci_print("slot:allocated=%d\n", slot);
	if(slot >= 16)return -1;

	struct perxhci* xhcidata = (void*)(xhci->priv_256b);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
	u64* dcbatable = (u64*)(xhcidata->dcba);
	dcbatable[slot] = (u64)(slotdata->hcctx);
	return slot;
}
int xhci_DisableSlot(struct item* xhci, int slot)
{
	xhci_print("--------------------------------\n");
	xhci_print("DisableSlot\n");

	return 0;
}
int xhci_AddressDevice(struct item* xhci, int slot)
{
	xhci_print("--------------------------------\n");
	xhci_print("AddressDevice\n");
	struct perxhci* xhcidata = (void*)(xhci->priv_256b);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;

//-------------from myown get something------------
	u32 usbspeed = slotdata->myctx.devctx.usbspeed;
	u32 rootport = slotdata->myctx.devctx.rootport;
	u32 routestring = slotdata->myctx.devctx.routestring;
	xhci_print("speed=%x,port=%x,route=%x\n",usbspeed,rootport,routestring);

//-------------from speed know maxpacksz-------------
	u32 packetsize;
	switch(usbspeed){
	case 1:
	case 2:packetsize = 8;break;
	case 3:packetsize = 0x40;break;
	default:packetsize = 0x200;
	}
	xhci_print("packetsize=0x%x\n", packetsize);

//-------------from xhci known slotctxsz------------
	u32 contextsize = 0x20;
	if(0x4 == (xhcidata->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;
	xhci_print("contextsize=0x%x\n", contextsize);

//---------------------------------------------------
	//some buffer
	int j;
	u8* buf = xhcidata->freebuf;
	for(j=0;j<0x800;j++)buf[j] = 0;

	u32* incon  = (void*)(buf + 0);
	u32* devctx = (void*)(buf + contextsize);
	u32* ep0ctx = (void*)(buf + contextsize*2);

	u32 EP0DCI = 1;		//ep0 bidir
	u64 EP0CMD = (u64)(slotdata->cmdring[EP0DCI]);
	u32 lo = EP0CMD & 0xffffffff;
	u32 hi = EP0CMD >> 32;
	xhci_print("ep0cmd@%llx\n", EP0CMD);

	//fill buffer
	incon[0] = 0;
	incon[1] = 3;
	for(j=2;j<8;j++)incon[j] = 0;

	devctx[0] = (EP0DCI<<27) + (usbspeed<<20) + routestring;
	devctx[1] = rootport << 16;
	devctx[2] = 0;
	devctx[3] = 0;

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
	if(xhci_waitevent(xhci, TRB_event_CommandCompletion, 0) != slot){
		xhci_print("error@address device\n");
		return -1;
	}
	xhci_print("slot:addressed device\n");

	u32 slotstate = (*(u32*)(slotdata->hcctx+0xc))>>27;
	u32 ep0state = (*(u32*)(slotdata->hcctx+contextsize))&0x3;
	xhci_print("slotstate=%x, ep0state=%x\n", slotstate, ep0state);
	if(2 != slotstate){
		xhci_print("slot state:%x\n",slotstate);
		return -2;
	}
	if(0 == ep0state){
		xhci_print("ep0 wrong\n");
		return -3;
	}

	slotdata->myctx.epnctx[EP0DCI].eptype = EPType_ControlBidir;
	slotdata->myctx.epnctx[EP0DCI].packetsize = packetsize;
	slotdata->myctx.epnctx[EP0DCI].cycle = 1;
	slotdata->myctx.epnctx[EP0DCI].myenq = 0;
	slotdata->myctx.epnctx[EP0DCI].hcdeq = 0;
	return 1;
}
int xhci_ResetDevice(struct item* xhci, int slot)
{
	xhci_print("--------------------------------\n");
	xhci_print("ResetDevice\n");
	return 0;
}
int xhci_EvaluateContext(struct item* xhci, int slot, u8* devdesc, int len)
{
	//Evaluate Context Command
	xhci_print("--------------------------------\n");
	xhci_print("EvaluateContext\n");

	struct perxhci* xhcidata = (void*)(xhci->priv_256b);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
	if(1 != slotdata->myctx.devctx.usbspeed){
		xhci_print("not fullspeed, dont change\n");
		return 0;
	}

	u32 EP0DCI = 1;
	u32 usbspeed = slotdata->myctx.devctx.usbspeed;
	u32 oldsize = slotdata->myctx.epnctx[EP0DCI].packetsize;
	u32 newsize = (usbspeed>=4) ? (1<<devdesc[7]) : devdesc[7];
	if(newsize == oldsize){
		xhci_print("same bMaxPacketSize0, nothing change\n");
		return 0;
	}

//-------------from packet known maxpacksz------------
	slotdata->myctx.epnctx[EP0DCI].packetsize = newsize;		//bMaxPacketSize0
	xhci_print("packetsize=%x now\n", newsize);

//-------------from xhci known slotctxsz------------
	u32 contextsize = 0x20;
	if(0x4 == (xhcidata->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;
	xhci_print("contextsize=0x%x\n", contextsize);

//---------------------------------------------------
	//some buffer
	int j;
	u8* buf = xhcidata->freebuf;
	for(j=0;j<0x800;j++)buf[j] = 0;

	u32* incon  = (void*)(buf + 0);
	u32* devctx = (void*)(buf + contextsize);
	u32* ep0ctx = (void*)(buf + contextsize*2);

	u32 lo = ((u64)slotdata->cmdring[EP0DCI]) & 0xffffffff;
	u32 hi = ((u64)slotdata->cmdring[EP0DCI]) >> 32;

	//change bMaxPacketSize0
	incon[0] = 0;
	incon[1] = 2;
	for(j=2;j<8;j++)incon[j] = 0;

	devctx[0] = 0;
	devctx[1] = 0;
	devctx[2] = 0;
	devctx[3] = 0;

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
	if(xhci_waitevent(xhci, TRB_event_CommandCompletion, 0) != slot){
		xhci_print("error@evaluate context\n");
		return -1;
	}
	xhci_print("evaluated\n");

	u32 slotstate = (*(u32*)(slotdata->hcctx + 0xc))>>27;
	u32 ep0state = (*(u32*)(slotdata->hcctx + contextsize))&0x3;
	xhci_print("slotstate=%x, ep0state=%x\n", slotstate, ep0state);
	if(2 != slotstate){
		xhci_print("slot state:%x\n",slotstate);
		return -3;
	}
	if(0 == ep0state){
		xhci_print("ep0 wrong\n");
		return -2;
	}
	return 0;
}
int xhci_ConfigureEndpoint(struct item* xhci, int slot, struct EndpointDescriptor* epdesc, int len)
{
	xhci_print("--------------------------------\n");
	xhci_print("ConfigureEndpoint\n");

	struct perxhci* xhcidata = (void*)(xhci->priv_256b);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;

//----------------from xhci known slotctxsz------------
	u32 contextsize = 0x20;
	if(0x4 == (xhcidata->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;
	xhci_print("contextsize=0x%x\n", contextsize);

//----------------from packet known things------------
	if(5 != epdesc->bDescriptorType){
		xhci_print("not epdesc, bye bye\n");
		return -1;
	}

	u8 DCI = (epdesc->bEndpointAddress & 0xf) << 1;
	if(epdesc->bEndpointAddress & 0x80)DCI += 1;

	u32 eptype = epdesc->bmAttributes & 3;
	if(epdesc->bEndpointAddress & 0x80)eptype |= 4;
	if(0 == eptype)eptype = 4;		//control out -> control bidir

	u32 CErr;
	u32 interval;
	u32 avgtrblen;
	u32 mult;
	u32 burstsize;
	u32 packetsize;
	u32 esitpayload;
	if( (EPType_BulkOut == eptype) | (EPType_BulkIn == eptype) ){
		CErr = 3;
		avgtrblen = 3072;	//Average TRB Length: Control=8B, Interrupt=1024, Bulk and Isoch=3072
		interval = xhci_UsbInterval2XhcInterval(slotdata->myctx.devctx.usbspeed, eptype, epdesc->bInterval);

		mult = 0;
		burstsize = 0;	//USB2 ? 0 : (SuperSpeed Endpoint Companion Descriptor:bMaxBurst)
		packetsize = epdesc->wMaxPacketSize;
		esitpayload = packetsize * (burstsize + 1);
	}
	else if( (EPType_IsochOut == eptype) | (EPType_IsochIn == eptype) ){
		CErr = 0;
		avgtrblen = 3072;	//Average TRB Length: Control=8B, Interrupt=1024, Bulk and Isoch=3072
		interval = xhci_UsbInterval2XhcInterval(slotdata->myctx.devctx.usbspeed, eptype, epdesc->bInterval);

		mult = 0;
		packetsize = epdesc->wMaxPacketSize;
		burstsize = 0;	//USB2 ? 0 : (SuperSpeed Endpoint Companion Descriptor:bMaxBurst)
		esitpayload = packetsize * (burstsize + 1);
	}
	else{		//EPType_InterruptOut or EPType_InterruptIn
		CErr = 3;
		avgtrblen = 1024;
		interval = xhci_UsbInterval2XhcInterval(slotdata->myctx.devctx.usbspeed, eptype, epdesc->bInterval);

		mult = 0;
		packetsize = epdesc->wMaxPacketSize & 0x7ff;
		burstsize = (epdesc->wMaxPacketSize & 0x1800) >> 11;
		esitpayload = packetsize * (burstsize + 1);
	}
	xhci_print("DCI=%x, eptype=%x, packetsize=%x, interval=%x\n", DCI, eptype, packetsize, interval);

//----------------store in my ctx----------------
	struct SlotContext* hcslotctx = (void*)(slotdata->hcctx);
	u32 maxdci = hcslotctx->ContextEntries;
	if(maxdci < DCI)maxdci = DCI;
	xhci_print("maxdci=%x now\n", maxdci);


	//some buffer
	int j;
	u8* buf = xhcidata->freebuf;
	for(j=0;j<0x800;j++)buf[j] = 0;

	u32* incon  = (void*)(buf + 0);
	incon[0] = 0;
	incon[1] = 1 | (1<<DCI);
	for(j=2;j<8;j++)incon[j] = 0;

	struct SlotContext* devctx = (void*)(buf + contextsize);
	//[0,3]
	devctx->RouteString = slotdata->myctx.devctx.routestring;
	devctx->Speed = slotdata->myctx.devctx.usbspeed;
	devctx->ContextEntries = maxdci;
	//[4,7]
	devctx->MaxExitLatency = 0;
	devctx->RootHubPortNumber = slotdata->myctx.devctx.rootport;
	devctx->NumberofPorts = 0;
	//[8,b]
	devctx->ParentHubSlotID = 0;
	devctx->ParentPortNumber = 0;
	devctx->TTThinkTime = 0;
	devctx->InterrupterTarget = 0;
	//[c,f]
	devctx->USBDeviceAddress = 0;
	devctx->SlotState = 0;

	struct EndpointContext* epctx = (void*)(buf + contextsize * (DCI+1));
	//[0,3]
	epctx->EndpointState = 0;
	epctx->Mult = mult;
	epctx->MaxPrimaryStreams = 0;
	epctx->LinerStreamArray = 0;
	epctx->Interval = interval;
	epctx->MaxESITPayloadHi = (esitpayload >> 16) & 0xff;
	//[4,7]
	epctx->ErrorCount = CErr;
	epctx->EndpointType = eptype;
	epctx->HostInitiateDisable = 0;
	epctx->MaxBurstSize = burstsize;
	epctx->MaxPacketSize = packetsize;
	//[8,b]
	epctx->DequeueCycleAndPointer = ((u64)slotdata + 0x1000*DCI) | 1;
	//[c,f]
	epctx->AverageTRBLength = avgtrblen;
	epctx->MaxESITPayloadLo = esitpayload & 0xffff;

	u32 lo = ((u64)incon) & 0xffffffff;
	u32 hi = ((u64)incon) >> 32;
	xhci_hostorder(xhci,0, lo,hi,0,(slot<<24)+(TRB_command_ConfigureEndpoint<<10) );
	if(xhci_waitevent(xhci, TRB_event_CommandCompletion, 0) != slot){
		xhci_print("error@configure endpoint\n");
		return -1;
	}
	xhci_print("configured\n");

	u32 slotstate = (*(u32*)(slotdata->hcctx + 0xc))>>27;
	u32 epstate = (*(u32*)(slotdata->hcctx + contextsize*DCI))&0x3;
	xhci_print("slotstate=%x, epstate=%x\n", slotstate, epstate);

	slotdata->myctx.epnctx[DCI].eptype = eptype;
	slotdata->myctx.epnctx[DCI].packetsize = packetsize;
	slotdata->myctx.epnctx[DCI].cycle = 1;
	slotdata->myctx.epnctx[DCI].myenq = 0;
	slotdata->myctx.epnctx[DCI].hcdeq = 0;
/*	if(2 != slotstate){
		xhci_print("slot state:%x\n",slotstate);
		return -3;
	}
	if(0 == epstate){
		xhci_print("ep wrong\n");
		return -2;
	}
*/
	return 0;
}
int xhci_ControlTransfer(struct item* xhci, int slot, struct UsbRequest* req, int slen, void* recvbuf, int recvlen)
{
	int DCI = 1;
	slot &= 0xff;

	xhci_print("--------------------------------\n");
	xhci_print("ControlTransfer slot=%x,dci=%x: (bm=%x,br=%x,val=%x,idx=%x,len=%x)\n",
		slot, DCI,
		req->bmRequestType,req->bRequest,req->wValue,req->wIndex,req->wLength
	);

	struct perxhci* xhcidata = (void*)(xhci->priv_256b);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
	u8* cmdenq = slotdata->cmdring[DCI] + slotdata->myctx.epnctx[DCI].myenq;
	slotdata->myctx.epnctx[DCI].myenq += maketrb_controltransfer(cmdenq, req, recvbuf, recvlen);

	//
	xhci_giveorder(xhci, slot | (DCI<<8));
	if(xhci_waitevent(xhci, TRB_event_Transfer, slot|0x100) != (slot|0x100)){	//return slot|(endp<<8)
		u32 contextsize = 0x20;
		if(0x4 == (xhcidata->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;

		u32 slotstate = (*(u32*)(slotdata->hcctx+0xc))>>27;
		u32 ep0state = (*(u32*)(slotdata->hcctx+contextsize))&0x3;
		xhci_print("slotstate=%x, ep0state=%x\n", slotstate, ep0state);
		return 0;
	}
	//printmemory(recvbuf, recvlen);
	return recvlen;
}
int xhci_BulkTransfer(struct item* xhci, int slotendp, void* sendbuf, int sendlen, void* recvbuf, int recvlen)
{
	int slot = slotendp & 0xff;
	int DCI = (slotendp>>8)&0xff;
	if(debug_bulktransfer)xhci_print("--------------------------------\n");
	if(debug_bulktransfer)xhci_print("BulkTransfer slot=%x,dci=%x\n", slot, DCI);

	struct perxhci* xhcidata = (void*)(xhci->priv_256b);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
	void* cmdenq = slotdata->cmdring[DCI] + slotdata->myctx.epnctx[DCI].myenq;
	slotdata->myctx.epnctx[DCI].myenq += maketrb_bulktransfer(cmdenq, 0, sendbuf, sendlen);
	if(slotdata->myctx.epnctx[DCI].myenq >= 0x1000)slotdata->myctx.epnctx[DCI].myenq = 0;

	xhci_giveorder(xhci, slot | (DCI<<8));
	if(xhci_waitevent(xhci, TRB_event_Transfer, slotendp) != slotendp){
		u32 contextsize = 0x20;
		if(0x4 == (xhcidata->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;

		u32 slotstate = (*(u32*)(slotdata->hcctx+0xc))>>27;
		u32 epstate = (*(u32*)(slotdata->hcctx+contextsize*DCI))&0x3;
		if(debug_bulktransfer)xhci_print("slotstate=%x, epstate=%x\n", slotstate, epstate);
		return 0;
	}
	return 0;
}
int xhci_InterruptTransferOut(struct item* xhci, int slotendp, void* sendbuf, int sendlen, void* recvbuf, int recvlen)
{
	int slot = slotendp & 0xff;
	int DCI = (slotendp>>8)&0xff;
	xhci_print("--------------------------------\n");
	xhci_print("InterruptTransferOut slot=%x,dci=%x\n", slot, DCI);

	struct perxhci* xhcidata = (void*)(xhci->priv_256b);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
	struct perendp* endpdata = &slotdata->myctx.epnctx[DCI];
	void* cmdenq = slotdata->cmdring[DCI] + endpdata->myenq;
	endpdata->myenq += maketrb_interruptout(cmdenq, 0, sendbuf, sendlen);
	if(endpdata->myenq >= 0x1000)endpdata->myenq = 0;

	//
	xhci_giveorder(xhci, slot | (DCI<<8));
	if(xhci_waitevent(xhci, TRB_event_Transfer, slotendp) != slotendp){
		u32 contextsize = 0x20;
		if(0x4 == (xhcidata->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;

		u32 slotstate = (*(u32*)(slotdata->hcctx+0xc))>>27;
		u32 epstate = (*(u32*)(slotdata->hcctx+contextsize*DCI))&0x3;
		xhci_print("slotstate=%x, epstate=%x\n", slotstate, epstate);
		return 0;
	}
	//printmemory(recvbuf, recvlen);
	return recvlen;
}
int xhci_InterruptTransferIn(struct item* xhci, int slotendp, void* sendbuf, int sendlen, void* recvbuf, int recvlen)
{
	int slot = slotendp & 0xff;
	int DCI = (slotendp>>8)&0xff;
	xhci_print("--------------------------------\n");
	xhci_print("InterruptTransferIn slot=%x,dci=%x\n", slot, DCI);

	struct perxhci* xhcidata = (void*)(xhci->priv_256b);
	struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
	struct perendp* endpdata = &slotdata->myctx.epnctx[DCI];
	void* cmdenq = slotdata->cmdring[DCI] + endpdata->myenq;
	endpdata->myenq += maketrb_interruptin(cmdenq, 0, sendbuf, sendlen);
	if(endpdata->myenq >= 0x1000)endpdata->myenq = 0;
	endpdata->contractor = (u64)recvbuf;

	//
	xhci_giveorder(xhci, slot | (DCI<<8));
/*	if(xhci_waitevent(xhci, TRB_event_Transfer, slotendp) != slotendp){
		u32 contextsize = 0x20;
		if(0x4 == (xhcidata->capreg->CAPPARAMS1 & 0x4))contextsize = 0x40;

		u32 slotstate = (*(u32*)(slotdata->hcctx+0xc))>>27;
		u32 epstate = (*(u32*)(slotdata->hcctx+contextsize*DCI))&0x3;
		xhci_print("slotstate=%x, epstate=%x\n", slotstate, epstate);
		return 0;
	}
	//
	printmemory(recvbuf, recvlen);*/
	return recvlen;
}




static int xhci_take(struct item* xhci,void* foot, void* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	if(0 == xhci)return 0;

	void* ev;
	while(1){
		ev = xhci_takeevent(xhci);
		if(0 == ev)break;

		xhci_parseevent(xhci, ev);
	}
	return 0;
}
static int xhci_give(struct item* xhci,u32 SlotEndp, void* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	if(cmd & 0x80000000){	//to xhci
		switch(cmd&0x7fffffff){
		case _addr_:     //prepare slotctx+ep0ctx
			return xhci_AddressDevice(xhci, SlotEndp);
		case _eval_:     //modify ep0ctx
			return xhci_EvaluateContext(xhci, SlotEndp, buf, len);
		case _conf_:     //prepare ep*ctx
			return xhci_ConfigureEndpoint(xhci, SlotEndp, buf, len);
		}
	}//to host

	else{	//to device
		int slot = SlotEndp & 0xff;
		int DCI =(SlotEndp>>8) & 0xff;
		if(DCI <= 1){		//to ep0
			return xhci_ControlTransfer(xhci, SlotEndp, arg, cmd, buf, len);
		}

		struct perxhci* xhcidata = (void*)(xhci->priv_256b);
		struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
		switch(slotdata->myctx.epnctx[DCI].eptype){
		case EPType_InterruptOut:
			return xhci_InterruptTransferOut(xhci, SlotEndp, arg, cmd, buf, len);
		case EPType_InterruptIn:
			return xhci_InterruptTransferIn(xhci, SlotEndp, arg, cmd, buf, len);
		case EPType_BulkOut:
		case EPType_BulkIn:
			return xhci_BulkTransfer(xhci, SlotEndp, arg, cmd, buf, len);
		}
	}//to device

	return 0;
}




int resetport(struct item* xhci, int countfrom0)
{
	struct perxhci* my = (void*)(xhci->priv_256b);
	struct OperationalRegisters* optreg = my->optreg;
	struct PortRegisters* port = &optreg->port[countfrom0];

	//bit4 = PR = reset
	port->PORTSC |= 0x10;
	xhci_print("write reset, wait reseted...\n");

	//wait for reset done
	int endcycle = 0xffffff;
	u64 endtime = timeread_us() + 3000*1000;
	while(1){
		if(0 == (port->PORTSC & 0x10))break;

		if(timeread_us() > endtime)return -1;

		endcycle--;
		if(0 == endcycle)return -1;
	}
	xhci_print("port reseted, wait enabled...\n");

	//wait for enable=1
	endcycle = 0xffffff;
	endtime = timeread_us() + 3000*1000;
	while(1){
		if(2 == (port->PORTSC & 2))break;

		if(timeread_us() > endtime)return -2;

		endcycle--;
		if(0 == endcycle)return -2;
	}
	xhci_print("port enabled, wait changed...\n");

	//wait for portchange event
	if(xhci_waitevent(xhci, TRB_event_PortStatusChange, countfrom0+1) < 0){
		xhci_print("portstatus unchanged: USBSTS=%x\n", optreg->USBSTS);
		return -3;
	}
	xhci_print("port changed\n");

	return 1;
}
void xhci_listall(struct item* xhci, int count)
{
	int j;
	u32 tmp,speed;
	struct perxhci* my = (void*)(xhci->priv_256b);
	struct perport* pp = my->perport;
	struct OperationalRegisters* optreg = my->optreg;
	struct PortRegisters* port = optreg->port;
	for(j=0;j<count;j++){
		tmp = port[j].PORTSC;
		xhci_print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
		xhci_print("%02x@%p:psi=%x,portsc=%x\n", j, &port[j], pp[j].protocol, tmp);
		if(0 == (tmp & 0x1))continue;

		//if(0 == PORTSC.bit1)ver <= 2.0, have to reset
		if(0 == (tmp & 0x2)){
			if(resetport(xhci, j) <= 0){
				xhci_print("reset failed: portsc=%x\n", port[j].PORTSC);
				continue;
			}
		}
		//todo:检查错误

		//link state
		tmp = port[j].PORTSC;
		xhci_print("portsc=%08x,linkstate=%x\n", tmp, (tmp >> 5) & 0xf);

		//usb speed
		speed = (port[j].PORTSC >> 10) & 0xf;
		switch(speed){
		case 7:xhci_print("10Gb,x2\n");break;
		case 6:xhci_print("5Gb,x2\n");break;
		case 5:xhci_print("10Gb,x1\n");break;
		case 4:xhci_print("5Gb,x1\n");break;
		case 3:xhci_print("480Mb\n");break;
		case 2:xhci_print("1.5Mb\n");break;	//yes, 2=low speed
		case 1:xhci_print("12Mb\n");break;	//yes, 1=full speed
		default:xhci_print("speed=%x\n", speed);
		}

		//probedevice(xhci, speed, j+1, 0);

		//alloc slot
		int slot = xhci_EnableSlot(xhci);
		if(slot <= 0 | slot >= 16)continue;

		struct perxhci* xhcidata = (void*)(xhci->priv_256b);
		struct perslot* slotdata = (void*)(xhcidata->perslot) + slot*0x10000;
		slotdata->myctx.devctx.usbspeed = speed;
		slotdata->myctx.devctx.rootport = j+1;
		slotdata->myctx.devctx.routestring = 0;

		//let usb do rest
		struct item* usb = device_create(_usb_, 0, 0, 0);
		if(usb)usbany_linkup(usb, 0, xhci, slot);
	}
}




int ownership(volatile u32* p)
{
	//set hc os owned semaphore
	xhci_print("before handoff:%08x,%08x\n", p[0],p[1]);
	p[0] |= 0x1000000;

	//wait until bit16==0 && bit24==1
	volatile int endcycle = 0xffffff;
	volatile u64 endtime = timeread_us() + 3000*1000;
	while(1){
		if(0x01000000 == (p[0] & 0x01010000)){
			xhci_print("after handoff:%08x,%08x\n", p[0],p[1]);
			return 1;
		}

		if(timeread_us() > endtime){
			xhci_print("handoff: timeout@endtime\n");
			return -1;
		}

		endcycle--;
		if(0 == endcycle){
			xhci_print("handoff: timeout@endcycle\n");
			return -1;
		}
	}
}
void supportedprotocol(struct item* dev, u32* p)
{
	u32 major = p[0] >> 24;
	u32 minor =(p[0] >> 16) & 0xff;
	u32 port0 =(p[2] & 0xff) - 1;
	u32 portn =(port0-1) + ((p[2] >> 8) & 0xff);
	xhci_print("[%x,%x]: usb%x.%x\n", port0, portn, major, minor);

	int j;
	struct perxhci* my = (void*)(dev->priv_256b);
	struct perport* pp = my->perport;
	for(j=port0;j<=portn;j++){
		pp[j].protocol = (major<<8) | minor;
		pp[j].slot = 0;
	}
}
void explainxecp(struct item* dev, u32* at)
{
	xhci_print("xecp parsing\n");
	while(1){
		xhci_print("@%p: %08x,%08x,%08x,%08x\n", at, at[0], at[1], at[2], at[3]);

		u32 type = at[0] & 0xff;
		switch(type){
			case 1:{
				ownership(at);
				break;
			}
			case 2:{
				supportedprotocol(dev, at);
				break;
			}
			defualt:{
				xhci_print("???\n");
			}
		}

		u32 next = ((at[0] >> 8) & 0xff) << 2;
		if(0 == next)break;

		at = (void*)at + next;
	}
}




int xhci_mmioinit(struct item* dev, u8* xhciaddr)
{
	xhci_print("mmio@%p{\n", xhciaddr);
	//printmmio(xhciaddr, 0x1000);


//---------------clear home---------------------
	int j,k;
	u8* ptr = memorycreate(0x200000, 0);
	for(j=0;j<0x200000;j++)ptr[j] = 0;

	u64 dcbahome      = (u64)(ptr + 0x000000);	//64k*[ 0, 1)
	u64 cmdringhome   = (u64)(ptr + 0x010000);	//64k*[ 1, 2)
	u64 ersthome      = (u64)(ptr + 0x020000);	//64k*[ 2, 3)
	u64 eventringhome = (u64)(ptr + 0x030000);	//64k*[ 3, 4)
	u64 freebufhome   = (u64)(ptr + 0x040000);	//64k*[ 4, 6)
	u64 perport       = (u64)(ptr + 0x060000);	//64k*[ 6, 8)
	u64 scratchpad    = (u64)(ptr + 0x080000);	//64k*[ 8, f)
	u64 perslot       = (u64)(ptr + 0x100000);	//64k*[16,32)

//--------------basic infomation--------------------
	struct CapabilityRegisters* capreg = (void*)xhciaddr;
	u32 version = capreg->lenandver >> 16;
	u32 caplen = capreg->lenandver & 0xff;

	struct OperationalRegisters* optreg = (void*)(xhciaddr + caplen);
	struct RuntimeRegisters*     runreg = (void*)(xhciaddr + capreg->RTSOFF);
	struct DoorbellRegisters*    dblreg = (void*)(xhciaddr + capreg->DBOFF);

	xhci_print("base information:version=%x,caplen=%x\n", version, caplen);

	xhci_print("capability@%p\n", capreg);
	xhci_print("operational@%p\n", optreg);
	xhci_print("runtime@%p\n", runreg);
	xhci_print("doorbell@%p\n", dblreg);

	u32 tmp;
	u32 maxscratch;
	u32 maxerst;
	tmp = capreg->HCSPARAMS1;
	xhci_print("hcsparams1=%08x\n", tmp);
	xhci_print(".maxslots=%x\n", tmp&0xff);
	xhci_print(".maxintrs=%x\n", (tmp>>8)&0x7ff);
	xhci_print(".maxports=%x\n", (tmp>>24)&0xff);
	tmp = capreg->HCSPARAMS2;
	xhci_print("hcsparams2=%08x\n", tmp);
	xhci_print(".Isochronous Scheduling Threshold=%x\n", tmp&0xf);
	maxerst = (tmp>>4)&0xf;
	xhci_print(".Event Ring Segment Table Max=2^%x\n", maxerst);
	maxscratch = (((tmp>>21)&0x1f)<<5) | ((tmp>>27)&0x1f);
	xhci_print(".Max Scratchpad Buffers=%xpages\n", maxscratch);
	tmp = capreg->HCSPARAMS3;
	xhci_print("hcsparams3=%08x\n", tmp);
	xhci_print(".U1 Device Exit Latency=%x\n", tmp&0xff);
	xhci_print(".U2 Device Exit Latency=%x\n", (tmp>>16)&0xffff);
	tmp = capreg->CAPPARAMS1;
	xhci_print("capparams1=%08x\n", tmp);
	xhci_print(".AC64=%x\n", tmp&1);
	xhci_print(".CSZ=%x\n", (tmp>>2)&1);
	tmp = capreg->CAPPARAMS2;
	xhci_print("capparams2=%08x\n", tmp);
	xhci_print(".VTIO support=%x\n", (tmp>>9)&1);


//--------------print operational---------------
	u32 psz      = optreg->PAGESIZE;
	u32 pagesize = 0x1000;
	while(1){
		if(0 == psz){
			xhci_print("psz:\n",psz);
			return -1;
		}
		if(1 == psz)break;

		psz >>= 1;
		pagesize <<= 1;
	}
	xhci_print("before anything:\n");
	xhci_print("usbcommand:%x\n", optreg->USBCMD);
	xhci_print("usbstatus:%x\n", optreg->USBSTS);
	xhci_print("psz=%x,pagesize=%x\n", psz, pagesize);
	xhci_print("crcr:%x\n", ((u64)optreg->CRCR_hi<<32) | optreg->CRCR_lo);
	xhci_print("dcbaa:%x\n", ((u64)optreg->DCBAAP_hi<<32) | optreg->DCBAAP_lo);
	xhci_print("config:%x\n", optreg->CONFIG);


//--------------grab ownership-----------------
	struct perxhci* my = (void*)(dev->priv_256b);
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
	my->perport   = (void*)perport;
	my->scratch   = (void*)scratchpad;
	my->perslot   = (void*)perslot;
	//cmd
	my->order_len = 0x10000;
	my->order_cycle = 1;
	my->order_myenq = 0;
	my->order_hcdeq = 0;
	//ev
	my->event_len = 0x10000;
	my->event_cycle = 1;
	my->event_hcenq = 0;
	my->event_mydeq = 0;

	u32* xecp = (void*)xhciaddr + ((capreg->CAPPARAMS1 >> 16) << 2);
	explainxecp(dev, xecp);


//------------operational registers----------------
	u32 endcycle;
	u64 endtime;
	xhci_print("setup optreg@%p\n", optreg);

	//xhci正在运行吗
	if(0 == (optreg->USBSTS&0x1)){		//=0: runing
		xhci_print("hc stopping\n");

		//按下停止按钮
		optreg->USBCMD = optreg->USBCMD & 0xfffffffe;

		//等一会
		endcycle = 0xffffff;
		endtime = timeread_us() + 3000*1000;
		while(1){
			if(1 == (optreg->USBSTS&0x1))break;

			if(timeread_us() > endtime){
				xhci_print("not stop: endtime\n");
				return -2;
			}

			endcycle--;
			if(0 == endcycle){
				xhci_print("not stop: endcycle\n");
				return -2;
			}
		}
	}
	xhci_print("hc stopped\n");

	//按下复位按钮
	optreg->USBCMD = optreg->USBCMD | 2;

	//等一会
	endcycle = 0xffffff;
	endtime = timeread_us() + 3000*1000;
	while(1){
		if(0 == (optreg->USBCMD&0x2))break;

		if(timeread_us() > endtime){
			xhci_print("reset: err=endtime, USBCMD=%x\n", optreg->USBCMD);
			return -3;
		}

		endcycle--;
		if(0 == endcycle){
			xhci_print("reset: err=endcycle, USBCMD=%x\n", optreg->USBCMD);
			return -3;
		}
	}
	xhci_print("hc reseted,wait=%x\n",endcycle);

	//controller not ready=1就是没准备好
	if(0x800 == (optreg->USBSTS&0x800)){
		xhci_print("not ready:%x\n", optreg->USBSTS);
		return -4;
	}
	xhci_print("hc ready\n");

	//maxslot=deviceslots
	optreg->CONFIG = (capreg->HCSPARAMS1) &0xff;
	xhci_print("CONFIG=%x\n", optreg->CONFIG);

	//scratcharray[0 to max]: each is a page
	xhci_print("devctx: scratcharray[0-max) = scratchbuffer@%llx\n", scratchpad + pagesize);
	for(j=0;j<maxscratch;j++){
		*(u64*)(scratchpad+j*8) = scratchpad + pagesize*(j+1);
	}

	//[dcbahome] = scratcharray
	xhci_print("devctx: dcbahome[0] = scratcharray@%llx\n", scratchpad);
	*(u64*)(dcbahome + 0) = scratchpad;

	//[dcbaap] = dcbahome
	xhci_print("devctx: dcbaap[0] = dcbahome@%llx\n", dcbahome);
	optreg->DCBAAP_lo = dcbahome & 0xffffffff;
	optreg->DCBAAP_hi = dcbahome >> 32;

	//command linktrb:lastone point to firstone
	xhci_print("crcr: lasttrb point to firsttrb\n");
	*(u64*)(cmdringhome + 0xfff*0x10 + 0x0) = cmdringhome;
	*(u32*)(cmdringhome + 0xfff*0x10 + 0x8) = 0;
	*(u32*)(cmdringhome + 0xfff*0x10 + 0xc) = (TRB_common_Link<<10) + 2;

	//[crcr] = cmdringhome
	xhci_print("crcr: set cmdring@%llx\n", cmdringhome+1);
	optreg->CRCR_lo = cmdringhome+1;
	optreg->CRCR_hi = 0;


//-------------------runtime registers-------------------
//msix table,message addr,message data,enable vector
//allocate&init msix pending bit array
//point table&pba offset to message control table and pending bit array
//init message control register of msix capability structure
//-------------------------------------------------
	xhci_print("setup runreg@%p\n", runreg);

	//setupisr(xhciaddr);
	//----------------------if(msix)--------------------------
	//----------------------if(msi)--------------------------
	//----------------------if(intx)--------------------------

	xhci_print("building eventring@%llx\n", eventringhome);

	//build the "event ring segment table"
	xhci_print("building erst@%llx\n", ersthome);
	*(u64*)(ersthome + 0x0) = eventringhome;
	*(u64*)(ersthome + 0x8) = 0x1000;            //0x1000*0x10=0x10000
/*
	*(u64*)(ersthome + 0x10) = secondeventringhome;
	*(u64*)(ersthome + 0x18) = 0x1000;            //0x1000*0x10=0x10000
	*(u64*)(ersthome + 0x20) = thirdeventringhome;
	*(u64*)(ersthome + 0x28) = 0x1000;            //0x1000*0x10=0x10000
*/

	xhci_print("setting ir[0]\n");
	runreg->ir[0].ERSTSZ = 1;
	runreg->ir[0].ERDP_lo = eventringhome & 0xffffffff;
	runreg->ir[0].ERDP_hi = eventringhome >> 32;
	runreg->ir[0].ERSTBA_lo = ersthome & 0xffffffff;
	runreg->ir[0].ERSTBA_hi = ersthome >> 32;
	runreg->ir[0].IMOD = 4000;
	runreg->ir[0].IMAN |= 0x2;


//---------------------xhci启动---------------------
//write usbcmd,turn host controller on
//-------------------------------------------------
	xhci_print("turnon hc:\n");

	//enable EWE|HSEIE|INTE(0x400|0x8|0x4) in USBCMD
//	*(u32*)operational |= 0x40c;
//	*(u32*)operational = (*(u32*)operational) | 0xc;      //no interrupt

	optreg->USBCMD = optreg->USBCMD | 0x1;
	xhci_print("USBCMD=%08x, USBSTS=%08x\n", optreg->USBCMD, optreg->USBSTS);

	xhci_print("}\n");


	//wait until device detected
	xhci_print("waiting for port change...\n");
	endcycle = 0xffffff;
	endtime = timeread_us() + 3000*1000;
	while(1){
		//bit4 = Port Change Detect
		if(0x10 == (optreg->USBSTS & 0x10))break;

		if(timeread_us() > endtime){
			xhci_print("USBCMD=%08x, USBSTS=%08x, no port change, endtime\n", optreg->USBCMD, optreg->USBSTS);
			break;
		}

		endcycle--;
		if(0 == endcycle){
			xhci_print("USBCMD=%08x, USBSTS=%08x, no port change, endcycle\n", optreg->USBCMD, optreg->USBSTS);
			break;
		}
	}

	//callback functions
	dev->ontaking = (void*)xhci_take;
	dev->ongiving = (void*)xhci_give;
	xhci_listall(dev, capreg->HCSPARAMS1>>24);
	return 0;
}
