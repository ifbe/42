#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _S5_ hex32('_','S','5','_')
#define _APIC_ hex32('A','P','I','C')
#define _DSDT_ hex32('D','S','D','T')
#define _FACP_ hex32('F','A','C','P')
#define _HPET_ hex32('H','P','E','T')
#define _MCFG_ hex32('M','C','F','G')
#define RSD_PTR_ hex64('R','S','D',' ','P','T','R',' ')
void printmemory(void*, int);
void logtoall(void*, ...);



struct RSDPDescriptor {
	char Signature[8];
	u8 Checksum;
	char OEMID[6];
	u8 Revision;
	u32 RsdtAddress;
} __attribute__ ((packed));
struct RSDPDescriptor20 {
	struct RSDPDescriptor firstPart;
	u32 Length;
	u64 XsdtAddress;
	u8 ExtendedChecksum;
	u8 reserved[3];
} __attribute__ ((packed));




struct ACPIHEAD{
	u32    signature;	//[0,3]
	u32          len;	//[4,7]
	u8      revision;	//8
	u8      checksum;	//9
	u8      OEMID[6];	//[a,f]
	u8 OEMtableID[8];	//[10,17]
	u32  OEMrevision;	//[18,1b]
	u32    creatorID;	//[1c,1f]
	u32   creatorrev;	//[20,23]
}__attribute__((packed));
struct FADT{
	struct ACPIHEAD h;
	u32 FirmwareCtrl;
	u32 Dsdt;

	// field used in ACPI 1.0; no longer in use, for compatibility only
	u8  Reserved;
	u8  PreferredPowerManagementProfile;
	u16 SCI_Interrupt;
	u32 SMI_CommandPort;
	u8  AcpiEnable;
	u8  AcpiDisable;
	u8  S4BIOS_REQ;
	u8  PSTATE_Control;
	u32 PM1aEventBlock;
	u32 PM1bEventBlock;
	u32 PM1aControlBlock;
	u32 PM1bControlBlock;
	u32 PM2ControlBlock;
	u32 PMTimerBlock;
	u32 GPE0Block;
	u32 GPE1Block;
	u8  PM1EventLength;
	u8  PM1ControlLength;
	u8  PM2ControlLength;
	u8  PMTimerLength;
	u8  GPE0Length;
	u8  GPE1Length;
	u8  GPE1Base;
	u8  CStateControl;
	u16 WorstC2Latency;
	u16 WorstC3Latency;
	u16 FlushSize;
	u16 FlushStride;
	u8  DutyOffset;
	u8  DutyWidth;
	u8  DayAlarm;
	u8  MonthAlarm;
	u8  Century;

	// reserved in ACPI 1.0; used since ACPI 2.0+
	u16 BootArchitectureFlags;

	u8  Reserved2;
	u32 Flags;

	// 12 byte structure; see below for details
	u8 ResetReg[0x12];

	u8  ResetValue;
	u8  Reserved3[3];

	// 64bit pointers - Available on ACPI 2.0+
	u64 X_FirmwareControl;
	u64 X_Dsdt;
}__attribute__((packed));
struct HPET{
	struct ACPIHEAD         head;	//[00,23]

	u8                 hw_rev_id;	//[24,24]
	u8        comparator_count:5;	//[25,25]
	u8            counter_size:1;
	u8                reserved:1;
	u8          legacy_replace:1;
	u16                 vendorid;	//[26,27]

	//struct HPET_BASEADDR    base;	//[28,33]
	u8             addr_space_id;	//0
	u8             reg_bit_width;	//1
	u8              reg_bit_offs;	//2
	u8                      rsvd;	//3
	u64                     addr;	//[4,b]

	u8                HPETNumber;	//34
	u16 MinimumClockPeriodicMode;	//[35,36]
	u8   PageProtectOEMAttribute;	//37
}__attribute__((packed));


struct MADT_TYPE0{	//local apic
	u8 type;
	u8 len;
	u8 cpuID;
	u8 apicID;
	u32 flag;
}__attribute__((packed));
struct MADT_TYPE1{	//io apic
	u8 type;
	u8 len;
	u8 ioapicID;
	u8 rsvd;
	u32 ioapicaddr;
	u32 GlobalSystemInterruptBase;
}__attribute__((packed));
struct MADT_TYPE2{	//interrupt source override
	u8 type;
	u8 len;
	u8 bus;
	u8 irq;
	u32 GlobalSystemInterrupt;
	u16 flag;
}__attribute__((packed));
struct MADT_TYPE4{	//non maskable interrupt
	u8 type;
	u8 len;
	u8 cpuID;
	u16 flag;
	u8 LINT;
}__attribute__((packed));
struct MADT_TYPE5{	//local apic address override
	u8 type;
	u8 len;
	u16 rsvd;
	u64 apicaddr;
}__attribute__((packed));
struct MADT{
	struct ACPIHEAD head;	//[00,23]
	u32    LocalApicAddr;	//[24,27]
	u32     Dual8259Flag;	//[28,2b]
	u8          entry[0];	//[2c,??]
}__attribute__((packed));


struct MCFG_CONFSPACE{
	u64    BaseAddr;	//[0.7]
	u16    GroupNum;	//[8,9]
	u8 BusNum_start;	//a
	u8   BusNum_end;	//b
	u32        Rsvd;	//[c,f]
}__attribute__((packed));
struct MCFG{
	struct ACPIHEAD          head;	//[00,23]
	u64                      what;	//[24,2b]
	struct MCFG_CONFSPACE conf[0];	//[2c,??]
}__attribute__((packed));




static u8 oemid[8];
u8* getoemid()
{
	return oemid;
}




static u16 power_port;
static u16 power_data;
void acpi_getportanddata(u16* p, u16* d)
{
	*p = power_port;
	*d = power_data;
}


static u8 have8042 = 1;
static u8 cmos_rtc_not_present = 0;
int acpi_have8042()
{
	return have8042;
}
int acpi_cmosrtc()
{
	return !cmos_rtc_not_present;
}


static u64 knowncores[4] = {0};
u64 acpi_getknowncores()
{
	return knowncores[0];
}


static void* pcietable_addr = 0;
static int pcietable_size = 0;
int acpi_getpcie(void** addr, int* size)
{
	if(0 == pcietable_addr)return 0;
	if(0 == pcietable_size)return 0;
	if(addr){
		*addr = pcietable_addr;
	}
	if(size){
		*size = pcietable_size;
	}
	return 1;
}


static int have8259 = 0;
int acpi_getdual8259()
{
	return have8259;
}


static void* addr_localapic = 0;
void* acpi_getlocalapic()
{
	return addr_localapic;
}


static void* addr_irqioaddr = 0;
static u8 isa2gsi[32] = {
	0x0,0x1,0x2,0x3, 0x4,0x5,0x6,0x7,
	0x8,0x9,0xa,0xb, 0xc,0xd,0xe,0xf
};
void* acpi_getirqioapic()
{
	return addr_irqioaddr;
}
void* acpi_getredirtbl()
{
	return isa2gsi;
}




static void* hpet_addr = 0;
static int hpet_replacepit = 0;
void* acpi_hpet_addr()
{
	return hpet_addr;
}
int acpi_hpet_replacepit()
{
	return hpet_replacepit;
}




void parse_S5_(void* p)
{
	u16 data;
	logtoall("%.4s@%p\n", p, p);

	data = (*(u8*)(p+8))<<10;
	logtoall("data=%p\n", data);

	power_data = data;
}
void acpi_DSDT(void* p)
{
	int j,cnt;
	logtoall("%.4s@%p\n", p, p);

	cnt = *(u32*)(p+4);
	for(j=4;j<cnt;j++){
		if(_S5_ == *(u32*)(p+j)){
			parse_S5_(p+j);
			break;
		}
	}
}
void acpi_FACP(void* p)
{
	struct FADT* fadt = p;
	logtoall("FACP:ver=%d\n", fadt->h.revision);

	u16 BootArchitectureFlags = fadt->BootArchitectureFlags;
	logtoall("BootArchFlag:offs=%d,data=%x\n", (void*)&fadt->BootArchitectureFlags-p, BootArchitectureFlags);
	if((fadt->h.revision > 1) && (0 == (BootArchitectureFlags&2)) ){
		have8042 = 0;
	}
	if(BootArchitectureFlags&0x20)cmos_rtc_not_present = 1;

	u16 port;
	port = *(u16*)(p+0x40);
	logtoall("port=%p\n", port);
	power_port = port;

	u64 addr;
	addr = *(u32*)(p+0x28);
	acpi_DSDT((void*)addr);
}
void acpi_HPET(void* p)
{
	struct HPET* a = p;
	hpet_addr = (void*)a->addr;
	hpet_replacepit = a->legacy_replace;
	logtoall("legacy_relpace=%x,addr=%p\n", hpet_replacepit, hpet_addr);

}
void acpi_MADT(void* p)
{
	struct MADT* madt = p;
	logtoall(".localapic=%x, have8259=%d\n", madt->LocalApicAddr, madt->Dual8259Flag);
	addr_localapic = (void*)(u64)(madt->LocalApicAddr);
	have8259 = madt->Dual8259Flag;

	int j = 0;
	int len = madt->head.len - 0x2c;
	struct MADT_TYPE0* t0;
	struct MADT_TYPE1* t1;
	struct MADT_TYPE2* t2;
	struct MADT_TYPE4* t4;
	struct MADT_TYPE5* t5;
	while(1){
		switch(madt->entry[j]){
		case 0:
			t0 = (void*)(madt->entry+j);
			logtoall("%x-type0: cpu=%x,apic=%x,flag=%x\n", j, t0->cpuID,t0->apicID,t0->flag);
			if(0 != (t0->flag&3))knowncores[t0->apicID>>6] |= 1<<(t0->apicID&0x3f);
			break;
		case 1:
			t1 = (void*)(madt->entry+j);
			logtoall("%x-type1: ioapicid=%x,ioapicaddr=%x,gsib=%x\n", j, t1->ioapicID,t1->ioapicaddr,t1->GlobalSystemInterruptBase);
			if(0 == t1->GlobalSystemInterruptBase)addr_irqioaddr = (void*)(u64)(t1->ioapicaddr);
			break;
		case 2:
			t2 = (void*)(madt->entry+j);
			logtoall("%x-type2: bus=%x,irq=%x,gsi=%x,flag=%x\n", j, t2->bus,t2->irq,t2->GlobalSystemInterrupt,t2->flag);
			if(t2->irq < 16)isa2gsi[t2->irq] = t2->GlobalSystemInterrupt;
			break;
		case 4:
			t4 = (void*)(madt->entry+j);
			logtoall("%x-type4: cpu=%x,flag=%x,LINT=%x\n", j, t4->cpuID,t4->flag,t4->LINT);
			break;
		case 5:
			t5 = (void*)(madt->entry+j);
			logtoall("%x-type5: localapic=%llx\n", j, t5->apicaddr);
			break;
		default:
			logtoall("%x-type%x,len=%x\n", j, madt->entry[j], madt->entry[j+1]);
		}
		if(0 == madt->entry[j+1])break;

		j += madt->entry[j+1];
		if(j >= len)break;
	}
}
void acpi_MCFG(void* p)
{
	int j;
	struct MCFG_CONFSPACE* c = p+0x2c;
	int len = *(u32*)(p+4);
	int sz = (len-0x2c) / 16;

	pcietable_addr = c;
	pcietable_size = sz;

	for(j=0;j<sz;j++){
		logtoall("%02x: base=%llx,group=%x,start=%x,end=%x\n", j,
		c[j].BaseAddr, c[j].GroupNum, c[j].BusNum_start, c[j].BusNum_end);
	}
}
void acpitable(void* p)
{
	logtoall("%.4s@%p\n", p, p);
	switch(*(u32*)p){
	case _APIC_:acpi_MADT(p);break;
	case _DSDT_:acpi_DSDT(p);break;
	case _FACP_:acpi_FACP(p);break;
	case _HPET_:acpi_HPET(p);break;
	case _MCFG_:acpi_MCFG(p);break;
	}
}
void acpirsdt(void* buf)
{
	int j;
	int len = *(u32*)(buf+4);
	logtoall("rsdt,%x@%p\n", len, buf);

	len &= 0xffff;
	printmemory(buf, len);

	u64 addr;
	for(j=0x24;j<len;j+=4){
		addr = *(u32*)(buf+j);
		acpitable((void*)addr);
	}
}
void acpixsdt(void* buf)
{
	int j;
	int len = *(u32*)(buf+4);
	logtoall("xsdt,%x@%p\n", len, buf);

	len &= 0xffff;
	printmemory(buf, len);

	u64 addr;
	for(j=0x24;j<len;j+=8){
		addr = *(u32*)(buf+j);
		acpitable((void*)addr);
	}
}
void parsedevmap_acpi(u8* buf)
{
	if(0 == buf)return;

	logtoall("rsdptr@%p\n", buf);
	printmemory(buf, 0x20);

	int j;
	for(j=0;j<6;j++)oemid[j] = buf[j+9];
	oemid[6] = oemid[7] = 0;

	if(0 == *(u8*)(buf+0xf)){
		void* rsdt = (void*)(u64)(*(u32*)(buf+0x10));
		acpirsdt(rsdt);
	}
	else{
		void* xsdt = (void*)(u64)(*(u32*)(buf+0x18));
		acpixsdt(xsdt);
	}

	logtoall("\n\n");
}