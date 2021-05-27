#include "libhard.h"
//
u32 in32(u16);
void out32(u16, u32);
void eth_linkup(void*,int, void*,int);
int give_data_into_orel(void* chip,int ftype, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len);
//
#define INTEL_VEND     0x8086    //Vendor ID for Intel
#define E1000_DEV      0x100E    //Device ID for the e1000 Qemu, Bochs, and VirtualBox emmulated NICs
#define E1000_I217     0x153A    //Device ID for Intel I217
#define E1000_82577LM  0x10EA    //Device ID for Intel 82577LM

#define REG_CTRL        0x0000
#define REG_STATUS      0x0008
#define REG_EEPROM      0x0014
#define REG_CTRL_EXT    0x0018
#define REG_IMASK       0x00D0
#define REG_RCTRL       0x0100
#define REG_RXDESCLO    0x2800
#define REG_RXDESCHI    0x2804
#define REG_RXDESCLEN   0x2808
#define REG_RXDESCHEAD  0x2810
#define REG_RXDESCTAIL  0x2818

#define REG_TCTRL       0x0400
#define REG_TXDESCLO    0x3800
#define REG_TXDESCHI    0x3804
#define REG_TXDESCLEN   0x3808
#define REG_TXDESCHEAD  0x3810
#define REG_TXDESCTAIL  0x3818

#define REG_RDTR         0x2820 // RX Delay Timer Register
#define REG_RXDCTL       0x3828 // RX Descriptor Control
#define REG_RADV         0x282C // RX Int. Absolute Delay Timer
#define REG_RSRPD        0x2C00 // RX Small Packet Detect Interrupt

#define REG_TIPG         0x0410      // Transmit Inter Packet Gap
#define ECTRL_SLU        0x40        //set link up

#define RCTL_EN            (1 << 1)    // Receiver Enable
#define RCTL_SBP           (1 << 2)    // Store Bad Packets
#define RCTL_UPE           (1 << 3)    // Unicast Promiscuous Enabled
#define RCTL_MPE           (1 << 4)    // Multicast Promiscuous Enabled
#define RCTL_LPE           (1 << 5)    // Long Packet Reception Enable
#define RCTL_LBM_NONE      (0 << 6)    // No Loopback
#define RCTL_LBM_PHY       (3 << 6)    // PHY or external SerDesc loopback
#define RTCL_RDMTS_HALF    (0 << 8)    // Free Buffer Threshold is 1/2 of RDLEN
#define RTCL_RDMTS_QUARTER (1 << 8)    // Free Buffer Threshold is 1/4 of RDLEN
#define RTCL_RDMTS_EIGHTH  (2 << 8)    // Free Buffer Threshold is 1/8 of RDLEN
#define RCTL_MO_36         (0 << 12)   // Multicast Offset - bits 47:36
#define RCTL_MO_35         (1 << 12)   // Multicast Offset - bits 46:35
#define RCTL_MO_34         (2 << 12)   // Multicast Offset - bits 45:34
#define RCTL_MO_32         (3 << 12)   // Multicast Offset - bits 43:32
#define RCTL_BAM           (1 << 15)   // Broadcast Accept Mode
#define RCTL_VFE           (1 << 18)   // VLAN Filter Enable
#define RCTL_CFIEN         (1 << 19)   // Canonical Form Indicator Enable
#define RCTL_CFI           (1 << 20)   // Canonical Form Indicator Bit Value
#define RCTL_DPF           (1 << 22)   // Discard Pause Frames
#define RCTL_PMCF          (1 << 23)   // Pass MAC Control Frames
#define RCTL_SECRC         (1 << 26)   // Strip Ethernet CRC

// Buffer Sizes
#define RCTL_BSIZE_256     (3 << 16)
#define RCTL_BSIZE_512     (2 << 16)
#define RCTL_BSIZE_1024    (1 << 16)
#define RCTL_BSIZE_2048    (0 << 16)
#define RCTL_BSIZE_4096    ((3 << 16) | (1 << 25))
#define RCTL_BSIZE_8192    ((2 << 16) | (1 << 25))
#define RCTL_BSIZE_16384   ((1 << 16) | (1 << 25))

// Transmit Command
#define CMD_EOP  (1 << 0)    // End of Packet
#define CMD_IFCS (1 << 1)    // Insert FCS
#define CMD_IC   (1 << 2)    // Insert Checksum
#define CMD_RS   (1 << 3)    // Report Status
#define CMD_RPS  (1 << 4)    // Report Packet Sent
#define CMD_VLE  (1 << 6)    // VLAN Packet Enable
#define CMD_IDE  (1 << 7)    // Interrupt Delay Enable

// TCTL Register
#define TCTL_EN         (1 << 1)    // Transmit Enable
#define TCTL_PSP        (1 << 3)    // Pad Short Packets
#define TCTL_CT_SHIFT   4           // Collision Threshold
#define TCTL_COLD_SHIFT 12          // Collision Distance
#define TCTL_SWXOFF     (1 << 22)   // Software XOFF Transmission
#define TCTL_RTLC       (1 << 24)   // Re-transmit on Late Collision

#define TSTA_DD    (1 << 0)    // Descriptor Done
#define TSTA_EC    (1 << 1)    // Excess Collisions
#define TSTA_LC    (1 << 2)    // Late Collision
#define LSTA_TU    (1 << 3)    // Transmit Underrun




struct perdev{
	u8* mmioaddr;
	u8 macaddr[8];
	//
	u8* rxdesc;
	u8* txdesc;
	u8* txbuffer;
	u8* rxbuffer;
};




struct RecvDesc
{
    volatile u64 addr;
    volatile u16 len;
    volatile u16 checksum;
    volatile u8 status;
    volatile u8 errors;
    volatile u16 special;
};
static int e1000_read(struct item* e1000,void* foot, void* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	struct perdev* per = (void*)(e1000->priv_data);
	if(0 == per->mmioaddr)return -1;

	//changed = tail+1
	volatile u32 tail = *(volatile u32*)(per->mmioaddr+0x2818);
	tail = (tail+1)%32;

	//desc = changed
	u8* desc = per->rxdesc + 0x10*tail;
	u8 stat = desc[12];
	if(0 == (stat&0x1))return 0;

//say("@e1000_recv:sts=%x,err=%x\n", desc[12], desc[13]);
//printmemory(desc, 16);
	desc[12] = 0;		//status写0

	//next
	volatile u32 dummy = *(volatile u32*)(per->mmioaddr+0xc0);	//必须读，防止被优化
	*(u32*)(per->mmioaddr+0x2818) = tail;

	//recv
	u8* pkt = *(u8**)(desc);
	int cnt = *(u16*)(desc+8);
	//printmemory(pkt, cnt);

	give_data_into_orel(e1000,0, stack,sp, arg,cmd, pkt,cnt);
	//*(void**)buf = pkt;
	return cnt;
}




struct TransDesc
{
    volatile u64 addr;
    volatile u16 len;
    volatile u8 cso;
    volatile u8 cmd;
    volatile u8 status;
    volatile u8 css;
    volatile u16 special;
};
static int e1000_write(struct item* e1000,void* foot, void* stack,int sp, void* arg,int cmd, u8* buf, int len)
{
	struct perdev* per = (void*)(e1000->priv_data);
	if(0 == per->mmioaddr)return -1;

	//txdesc
	u32 head = *(u32*)(per->mmioaddr+0x3810);
	u8* desc = per->txdesc + 0x10*head;

	//put data
	int j;
	u8* dst = (u8*)( *(u64*)desc );
	for(j=0;j<len;j++)dst[j] = buf[j];
	for(j=0;j<6;j++)dst[j+6] = per->macaddr[j];

	//fill desc: reportstatus, insert fcs/crc, end of packet
	*(u16*)(desc+8) = len;
	*(u8*)(desc+11) = (1<<3) | (1<<1) | 1;

	//new tail
	u32 tail = *(u32*)(per->mmioaddr+0x3818);
	*(u32*)(per->mmioaddr+0x3818) = (tail+1)%8;

	//check send
	u64 timeout=0;
	while(1)
	{
		timeout++;
		if(timeout>0xfffffff)
		{
			say("fail",0);
			return -2;
		}

		volatile u8 status = *(volatile u8*)(desc+12);
		if( (status&0x1) == 0x1 )
		{
			say("sent@%x\n",desc);
			break;
		}
	}
	return len;
}




static int eepromexist(u8* mmio)
{
	int j;
	volatile u32 val = 0;
	*(u32*)(mmio+REG_EEPROM) = 0x1;

	for(j=0;j<1000;j++)
	{
		val = *(volatile u32*)(mmio+REG_EEPROM);
		if(val&0x10)return 1;
	}
	return 0;
}
static u32 eepromread(u8* mmio, u32 addr)
{
	int cnt = 0;
	volatile u32 tmp = 0;

	*(u32*)(mmio+REG_EEPROM) = 1 | (addr << 8);
	while(1){
		tmp = *(volatile u32*)(mmio+REG_EEPROM);
		if(tmp & (1 << 4))break;

		cnt++;
		if(cnt > 0xffffff){
			say("timeout@eepromread\n");
			return 0xffffffff;
		}
	}
/*
	*(u32*)(mmio+REG_EEPROM) = 1 | (addr << 2);
	while( !((tmp = *(u32*)(mmio+REG_EEPROM)) & (1 << 1)) ){
		say("tmp=%x\n",tmp);
	}
*/

	return (tmp >> 16) & 0xFFFF;
}
void e1000_mmioinit(struct item* dev, u8* mmio)
{
	struct perdev* per = (void*)(dev->priv_data);
	per->mmioaddr = mmio;
	say("e1000@mmio:%llx{\n", mmio);


	//
	u8* tmp = memorycreate(0x100000, 0x100000);
	per->rxdesc = tmp;
	per->txdesc = tmp+0x10000;
	per->rxbuffer = tmp+0x40000;
	per->txbuffer = tmp+0x80000;


	//---------------------macaddr-------------------
	if(eepromexist(mmio)){
		say("reading eeprom\n");
		u64 mac01 = eepromread(mmio, 0);
		u64 mac23 = eepromread(mmio, 1);
		u64 mac45 = eepromread(mmio, 2);
		*(u64*)per->macaddr = (mac45<<32) + (mac23<<16) + mac01;
	}
	else{
		u64 lo = *(u32*)(mmio+0x5400);	//receive address low
		u64 hi = *(u32*)(mmio+0x5404);
		say("lo=%x,hi=%x\n", lo, hi);
		*(u64*)per->macaddr = ((hi&0xffff)<<32) + lo;
	}
	say("macaddr=%llx\n", *(u64*)per->macaddr);
	//------------------------------------------


	//-----------------control-----------------
	//set link up
	volatile u32 ctl = *(volatile u32*)(mmio+0);
	*(volatile u32*)(mmio+0) = ctl | (1<<6);

	//clear multicast table array
	int j;
	for(j=0;j<128;j++) *(u32*)(mmio+0x5200+4*j)=0;

	//enable interrupt
	*(u32*)(mmio+0xd0) = 0x1f6dc;

	//read to clear "interrupt cause read"
	say("icr=%x\n", *(u32*)(mmio+0xc0));
	//---------------------------------------


	//--------------------receive descriptor---------------------
	for(j=0;j<32*16;j++){
		per->rxdesc[j] = 0;
	}
	for(j=0;j<32;j++){
		*(u64*)(per->rxdesc+0x10*j) = (u64)per->rxbuffer + 0x800*j;	//每个2048B
	}
	*(u32*)(mmio+0x2800) = ((u64)per->rxdesc) & 0xffffffff;	//addrlow
	*(u32*)(mmio+0x2804) = ((u64)per->rxdesc) >>        32;	//addrhigh
	*(u32*)(mmio+0x2808) =      32*16;	//32个*每个16B
	*(u32*)(mmio+0x2810) =          0;	//head
	*(u32*)(mmio+0x2818) =         31;	//tail
	*(u32*)(mmio+0x0100) = 0x0400801e;	//control
	//   0    4    0    0    8    0    1    e
	//0000 0100 0000 0000 1000 0000 0001 1110
        //RCTL_EN		1<<1
        //RCTL_SBP		1<<2
        //RCTL_UPE		1<<3
        //RCTL_MPE		1<<4
        //RCTL_LBM_NONE		0<<6
       	//RCTL_RDMTS_HALF	0<<8
        //RCTL_BAM		1<<15
        //RCTL_SECRC		1<<26
        //RCTL_BSIZE_2048	0<<16
	//------------------------------------------


	//----------------transmit descriptor-----------------
	for(j=0;j<32*16;j++){
		per->txdesc[j] = 0;
	}
	for(j=0;j<8;j++)
	{
		*(u64*)(per->txdesc+0x10*j) = (u64)per->txbuffer+0x800*j;	//每个2048B
	}
	*(u32*)(mmio+0x3800) = ((u64)per->txdesc) & 0xffffffff;	//addrlow
	*(u32*)(mmio+0x3804) = ((u64)per->txdesc) >>        32;	//addrhigh
	*(u32*)(mmio+0x3808) =       8*16;	//8个*每个16B
	*(u32*)(mmio+0x3810) =          0;	//head
	*(u32*)(mmio+0x3818) =          0;	//tail
	*(u32*)(mmio+0x0400) = 0x010400fa;	//control
	//   0    1    0    4    0    0    f    a
	//0000 0001 0000 0100 0000 0000 1111 1010
	//TCTL_EN			1<<1
	//TCTL_PSP			1<<3
	//(15 << TCTL_CT_SHIFT)		4
	//(64 << TCTL_COLD_SHIFT)	12
	//TCTL_RTLC			1<<24
	//----------------------------------------------


	say("}\n");
	dev->take = (void*)e1000_read;
	dev->give = (void*)e1000_write;

	struct item* eth = devicecreate(_eth_, 0, 0, 0);
	if(eth){
		relationcreate(eth,0, _dev_,0, dev,0, _dri_,0);
		eth_linkup(eth, 0, dev, 0);
	}
}
void e1000_portinit(struct item* dev, u64 addr)
{
	u32 temp;
	say("e1000@port:%x{\n",addr);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc);
	temp |= 1<<2;
	temp &= ~(u32)(1<<10);
	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp);

	out32(0xcf8, addr+0x4);
	say("sts,cmd=%x\n", in32(0xcfc));

	out32(0xcf8, addr+0x3c);
	temp = in32(0xcfc);
	say("int line=%x,pin=%x\n", temp&0xff, (temp>>8)&0xff);

	out32(0xcf8, addr+0x10);
	temp = in32(0xcfc)&0xfffffff0;

	say("}\n");

	e1000_mmioinit(dev, (void*)(u64)temp);
}
