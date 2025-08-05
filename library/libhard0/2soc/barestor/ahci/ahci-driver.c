#include "libhard.h"
#define ahci_print(fmt, ...) logtoall("<%08lld,ahci>" fmt, timeread_us(), ##__VA_ARGS__)
u32 in32(u16 port);
void out32(u16 port, u32 data);
//void filemanager_registerdisk(void*,void*);
void* sata_alloc();



//my
#define commandlist 0x00000
#define commandtable 0x10000
#define receivefis 0x20000
#define receivebuf 0x30000
//host
//port
#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3




enum FIS_TYPE
{
	FIS_TYPE_REG_H2D   = 0x27,		// Register FIS - host to device
	FIS_TYPE_REG_D2H   = 0x34,		// Register FIS - device to host
	FIS_TYPE_DMA_ACT   = 0x39,		// DMA activate FIS - device to host
	FIS_TYPE_DMA_SETUP = 0x41,		// DMA setup FIS - bidirectional
	FIS_TYPE_DATA	   = 0x46,		// Data FIS - bidirectional
	FIS_TYPE_BIST	   = 0x58,		// BIST activate FIS - bidirectional
	FIS_TYPE_PIO_SETUP = 0x5F,		// PIO setup FIS - device to host
	FIS_TYPE_DEV_BITS  = 0xA1,		// Set device bits FIS - device to host
};
struct FIS_REG_H2D
{
	//[0, 3]
	u8 fis_type;	//0: FIS_TYPE_REG_H2D

	u8 pmport:4;	//1.[7,4]: Port multiplier
	u8   rsv0:3;	//1.[3,1]: Reserved
	u8      c:1;	//1.[0,0]: 1=Command, 0=Control

	u8  command;	//2: Command register
	u8 featurel;	//3: Feature register, 7:0

	//[4, 7]
	u8     lba0;	//4: LBA low register, 7:0
	u8     lba1;	//5: LBA mid register, 15:8
	u8     lba2;	//6: LBA high register, 23:16
	u8   device;	//7: Device register

	//[8, b]
	u8     lba3;	//8: LBA register, 31:24
	u8     lba4;	//9: LBA register, 39:32
	u8     lba5;	//a: LBA register, 47:40
	u8 featureh;	//b: Feature register, 15:8

	//[c, f]
	u8   countl;	//c: Count register, 7:0
	u8   counth;	//d: Count register, 15:8
	u8      icc;	//e: Isochronous command completion
	u8  control;	//f: Control register

	//[10, 13]
	u8  rsv1[4];
}__attribute__((packed));

struct HBA_PRDT_ENTRY
{
	//[0,3]
	u32     dba;	// Data base address
	//[4,7]
	u32    dbau;	// Data base address upper 32 bits
	//[8,b]
	u32    rsv0;	// Reserved
	//[c,f]
	u32  dbc:22;	// Byte count, 4M max
	u32 rsv1: 9;	// Reserved
	u32    i: 1;	// Interrupt on completion
}__attribute__((packed));

struct HBA_CMD_TBL
{
	//[0x00, 0x3f]
	u8 cfis[64];	// Command FIS

	//[0x40, 0x4f]
	u8 acmd[16];	// ATAPI command, 12 or 16 bytes

	//[0x50,0x7f]
	u8 rsvd[48];	// Reserved

	//[0x80, x]
	struct HBA_PRDT_ENTRY prdt_entry[1];	// Physical region descriptor table entries, 0xffff个，每个0x18,总共0x180000=1.5M
}__attribute__((packed));

struct HBA_CMD_HEADER{
	//0
	u8 cfl:5;	// Command FIS length in u32S, 2 ~ 16
	u8   a:1;	// ATAPI
	u8   w:1;	// Write, 1: H2D, 0: D2H
	u8   p:1;	// Prefetchable

	//1
	u8    r:1;	// Reset
	u8    b:1;	// BIST
	u8    c:1;	// Clear busy upon R_OK
	u8 rsv0:1;	// Reserved
	u8  pmp:4;	// Port multiplier port

	//[2, 3]
	u16 prdtl;	// Physical region descriptor table length in entries

	//[4, 7]
	volatile u32 prdbc;	// Physical region descriptor byte count transferred

	//[8, f]
	u32  ctba;	// Command table descriptor base address
	u32 ctbau;	// Command table descriptor base address upper 32 bits

	//[10, 1f]
	u32 rsv1[4];	// Reserved
}__attribute__((packed));

struct HBA_PORT{
	volatile u32       clb;	// 0x00, command list base address, 1K-byte aligned
	volatile u32      clbu;	// 0x04, command list base address upper 32 bits
	volatile u32        fb;	// 0x08, FIS base address, 256-byte aligned
	volatile u32       fbu;	// 0x0C, FIS base address upper 32 bits
	volatile u32        is;	// 0x10, interrupt status
	volatile u32        ie;	// 0x14, interrupt enable
	volatile u32       cmd;	// 0x18, command and status
	volatile u32      rsv0;	// 0x1C, Reserved
	volatile u32       tfd;	// 0x20, task file data
	volatile u32       sig;	// 0x24, signature
	volatile u32      ssts;	// 0x28, SATA status (SCR0:SStatus)
	volatile u32      sctl;	// 0x2C, SATA control (SCR2:SControl)
	volatile u32      serr;	// 0x30, SATA error (SCR1:SError)
	volatile u32      sact;	// 0x34, SATA active (SCR3:SActive)
	volatile u32        ci;	// 0x38, command issue
	volatile u32      sntf;	// 0x3C, SATA notification (SCR4:SNotification)
	volatile u32       fbs;	// 0x40, FIS-based switch control
	volatile u32  rsv1[11];	// 0x44 ~ 0x6F, Reserved
	volatile u32 vendor[4];	// 0x70 ~ 0x7F, vendor specific
}__attribute__((packed));

struct HBA_MEM{
//[0x00, 0x2B]: Generic Host Control
	volatile u32     cap;	// 0x00, Host capability
	volatile u32     ghc;	// 0x04, Global host control
	volatile u32      is;	// 0x08, Interrupt status
	volatile u32      pi;	// 0x0C, Port implemented
	volatile u32      vs;	// 0x10, Version
	volatile u32 ccc_ctl;	// 0x14, Command completion coalescing control
	volatile u32 ccc_pts;	// 0x18, Command completion coalescing ports
	volatile u32  em_loc;	// 0x1C, Enclosure management location
	volatile u32  em_ctl;	// 0x20, Enclosure management control
	volatile u32    cap2;	// 0x24, Host capabilities extended
	volatile u32    bohc;	// 0x28, BIOS/OS handoff control and status

//[0x2C, 0x9F]: Reserved
	u8 rsv[0xA0-0x2C];

//[0xA0, 0xFF]: Vendor specific registers
	u8 vendor[0x100-0xA0];

//[0x100, x]: Port control registers, 1-32
	struct HBA_PORT ports[0];
}__attribute__((packed));

struct perahci{
	//hc
	struct HBA_MEM* abar;
	struct HBA_PORT* port;	//port0

	//my
	void* onemega;
	void* cmdlist;
	void* fisrecv;
}__attribute__((packed));





static int find_cmdslot(struct HBA_PORT* port)
{
	// If not set in SACT and CI, the slot is free
	int i;
	u32 cmdslot = (port->sact | port->ci);
	for (i=0; i<32; i++){    //cmdslots=32
		if(0 == (cmdslot&1))return i;
		cmdslot >>= 1;
	}
	return -1;
}
void maketable(struct HBA_CMD_HEADER* cmdheader, u64 from, u8* buf, u64 count)
{
	//Command FIS size
	cmdheader->cfl=sizeof(struct FIS_REG_H2D)/4;

	// Read from device
	cmdheader->w = 0;

	//PRDT entries count,example:
	cmdheader->prdtl = (u16)((count-1)>>7) + 1;
	//13sectors    >>>>    prdt=1个
	//0x80sectors    >>>>    prdt=1个
	//0x93sectors    >>>>    prdt=2个
	//0x100sectors    >>>>    prdt=2个
	//0x173sectors    >>>>    prdt=3个
	//0x181sectors    >>>>    prdt=4个
	//ahci_print("ptdtl",(u64)cmdheader->prdtl);

	struct HBA_CMD_TBL* cmdtable = (struct HBA_CMD_TBL*)(u64)cmdheader->ctba;
	//ahci_print("cmdtable=%p\n", cmdtable);
	char* p=(char*)cmdtable;
	int i=sizeof(struct HBA_CMD_TBL)+(cmdheader->prdtl-1)*sizeof(struct HBA_PRDT_ENTRY);
	for(;i>0;i--){p[i]=0;}

	// 8K bytes (16 sectors) per PRDT
	u64 addr;
	for (i=0; i<cmdheader->prdtl-1; i++){
		addr = (u64)buf;
		cmdtable->prdt_entry[i].dba = addr&0xffffffff;
		cmdtable->prdt_entry[i].dbau = addr>>32;
		cmdtable->prdt_entry[i].dbc = 0x80*0x200-1;	//bit0=1, 0x1ffff
		//cmdtable->prdt_entry[i].i = 1;

		buf += 0x10000;  // 64KB words?
		count -= 0x80;    // 16 sectors
	}
	addr = (u64)buf;
	cmdtable->prdt_entry[i].dba = addr&0xffffffff;	//Last entry
	cmdtable->prdt_entry[i].dbau = addr>>32;		//Last entry
	cmdtable->prdt_entry[i].dbc = count*0x200-1;	// 512 bytes per sector
	//cmdtable->prdt_entry[i].i = 1;

	// Setup command
	struct FIS_REG_H2D* fis = (struct FIS_REG_H2D*)(&cmdtable->cfis);
	fis->fis_type = FIS_TYPE_REG_H2D;       //0
	fis->c = 1;             //Command       //[1].[0,0]
	//fis->command = 0x20;
	//fis->command = ATA_CMD_READ_DMA_EX;
	fis->command = 0x25;            //2

	fis->lba0 = (u8)from;           //4
	fis->lba1 = (u8)(from>>8);      //5
	fis->lba2 = (u8)(from>>16);     //6
	fis->device = 1<<6;             //7     1<<6=LBA mode

	fis->lba3 = (u8)(from>>24);     //8
	fis->lba4 = (u8)(from>>32);     //9
	fis->lba5 = (u8)(from>>40);     //a

	fis->countl = count&0xff;       //c
	fis->counth = (count>>8)&0xff;  //d
}
int ahci_readblock(struct HBA_PORT* port, u64 from, u8* buf, u64 count)
{
	struct HBA_CMD_HEADER* cmdheader = (void*)(u64)(port->clb);

	//Clear pending interrupt
	port->is = 0xffffffff;

	//find cmdslot
	int cmdslot = find_cmdslot(port);
	if(-1 == cmdslot){
		ahci_print("err:no cmdslot\n");
		return -1;
	}

	//ahci_print("cmdslot=%x\n", cmdslot);
	//ahci_print("cmdheader=%p\n", cmdheader);
	cmdheader += cmdslot;
	maketable(cmdheader, from, buf, count);

	//make the table
	u32 temp=0;
	int endcycle = 0xffffff;
	u64 endtime = timeread_us() + 10*1000*1000;
	while(1){
		if(timeread_us() > endtime){
			ahci_print("ahci_readblock: err=endtime, tfd=%x\n",(u64)port->tfd);
			return -11;
		}

		endcycle--;
		if(0 == endcycle){
			ahci_print("ahci_readblock: err=endcycle, tfd=%x\n",(u64)port->tfd);
			return -11;
		}

		temp=port->tfd;                  //0x20
		if(0x80 == (temp&0x80))continue;      //bit7,busy
		if(0x8 == (temp&0x8))continue;  //bit3,DRQ
		//0x88=interface busy|data transfer requested

		break;
	}
	//ahci_print("is=%x\n", port->is);
	//unsigned int* pointer=(unsigned int*)(u64)(port->fb);

	//issue
	port->ci = 1<<cmdslot;    //Issue command
	endcycle = 0xffffff;
	endtime = timeread_us() + 10*1000*1000;
	while (1){
		if(timeread_us() > endtime){
			ahci_print("ahci_readblock: err=endtime, is=%x,ci=%x\n", port->is, port->ci);
			return -11;
		}

		endcycle--;
		if(0 == endcycle){
			ahci_print("ahci_readblock: err=endcycle, is=%x,ci=%x\n", port->is, port->ci);
			return -22;
		}

		//
		if (port->is & 0x40000000){  //Task file error
			ahci_print("port err 1\n",0);
			return -33;
		}

		// in the PxIS port field as well (1 << 5)
		if((port->ci & (1<<cmdslot)) != 0)continue;

		break;
	}
	return 0;
}




static int ahci_identify(volatile struct HBA_PORT* port, void* rdi)
{
	volatile struct HBA_CMD_HEADER* cmdheader = (void*)(u64)(port->clb);
	u32 cmdslot;
	u32 temp;
	port->is = 0xffffffff;	//Clear pending interrupt bits

	//find a cmd slot
	temp=(port->sact | port->ci);
	for(cmdslot=0; cmdslot<32; cmdslot++)    //cmdslots=32
	{
		if ((temp&1) == 0)break;
		temp>>= 1;
	}
	if(cmdslot == 32)
	{
		ahci_print("err:no cmdslot\n",0);
		return -1;
	}
	cmdheader += cmdslot;
	//ahci_print("cmdslot:",(u64)cmdslot);
	//ahci_print("cmdheader:",(u64)cmdheader);

	cmdheader->cfl=sizeof(struct FIS_REG_H2D)/4;	//Command FIS size
	cmdheader->w = 0;			// Read from device
	cmdheader->prdtl=1;
	cmdheader->prdbc=0;

	//make the table
	struct HBA_CMD_TBL* cmdtable = (struct HBA_CMD_TBL*)(u64)cmdheader->ctba;
	//ahci_print("cmdtable(comheader->ctba):",(u64)cmdtable);
	char* p = (char*)cmdtable;
	int i = sizeof(struct HBA_CMD_TBL);
	for(;i>0;i--){p[i]=0;}
	cmdtable->prdt_entry[0].dba = ((u64)rdi) & 0xffffffff;
	cmdtable->prdt_entry[0].dbau = ((u64)rdi) >> 32;
	cmdtable->prdt_entry[0].dbc = 0x200-1;	//bit0=1
	//cmdtable->prdt_entry[0].i = 1;
	struct FIS_REG_H2D* fis = (struct FIS_REG_H2D*)(&cmdtable->cfis);

	fis->fis_type = FIS_TYPE_REG_H2D;	//0
	fis->c = 1;				//Command
	fis->command = 0xec;	//identify
	fis->device = 0;		//LBA mode

	//wait until the port is no longer busy
	int endcycle = 0xffffff;
	u64 endtime = timeread_us() + 10*1000*1000;
	while(1){
		if(timeread_us() > endtime){
			ahci_print("ahci_identify: err=endtime, port->tfd=%x\n",(u64)port->tfd);
			return -1;
		}

		endcycle--;
		if(0 == endcycle){
			ahci_print("ahci_identify: err=endcycle, port->tfd=%x\n",(u64)port->tfd);
			return -1;
		}

		//0x88=interface busy|data transfer requested
		temp = port->tfd;	 //0x20
		if( (temp&0x80) == 0x80 )continue;      //bit7,busy
		if( (temp&0x8) == 0x8)continue;	 //bit3,DRQ

		break;
	}
	//ahci_print("is:",(u64)port->is);
	//u32* pointer=(u32*)(u64)(port->fb);
	//set issue,wait for completion
	port->ci = 1<<cmdslot;  // Issue command,start reading
	endcycle = 0xffffff;
	endtime = timeread_us() + 3000*1000;
	while(1){
		if(timeread_us() > endtime){
			ahci_print("ahci_identify: err=endtime, ci=%x,prdbc=%x\n",temp,cmdheader->prdbc);
			return -2;
		}

		endcycle--;
		if(0 == endcycle){
			ahci_print("ahci_identify: err=endtime, ci=%x,prdbc=%x\n",temp,cmdheader->prdbc);
			return -2;
		}

		temp=port->is;
		if (temp & 0x40000000)  // Task file error
		{
			ahci_print("port error 1\n");
			return -9;
		}

		//in the PxIS port field as well (1<<5)
		temp = port->ci;
		if( (temp & (1<<cmdslot)) != 0) continue;

		break;
	}

	return 1;
}
static int ahci_readdata(struct item* ahci,void* foot,struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	struct perahci* my = (void*)(ahci->priv_256b);
	struct HBA_MEM* abar = my->abar;
	struct HBA_PORT* port = foot;
	//ahci_print("@ahci_ontake: node=%p,abar=%p,port=%p, off=%llx,len=%x\n", my,abar,port, arg,len);

	int ret = ahci_readblock(port, arg>>9, buf, len>>9);
	if(ret < 0)return 0;

	//ahci_print("ret=%d\n",ret);
	return len;
}
static int ahci_readinfo(struct item* ahci,void* foot, void* buf,int len)
{
	struct perahci* my = (void*)(ahci->priv_256b);
	logtoall("@ahci_readinfo: %p,%p\n",ahci,foot);

	if(0 == buf)buf = my->onemega + receivebuf;
	int ret = ahci_identify(foot, buf);
	return ret;
}




static int ahci_reader(struct item* ahci,void* foot, p64 arg,int cmd, void* buf,int len)
{
	logtoall("%s: cmd=%x\n", __FUNCTION__, cmd);
	if(_info_ == cmd){
		return ahci_readinfo(ahci,foot, buf,len);
	}
	return 0;
}
static int ahci_writer(struct item* ahci,void* foot,struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
static int ahci_ontake(struct item* ahci,void* foot,struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("ahci_ontake:%llx,%x,%p,%x\n",arg,cmd,buf,len);
	if(_info_ == cmd)return ahci_readinfo(ahci,foot, buf,len);

	if(0 == buf){
		ahci_print("error: buf=0\n");
		return 0;
	}
	if(512 > len){
		ahci_print("error: len=%d\n", len);
		return 0;
	}
	return ahci_readdata(ahci,foot, stack,sp, arg,cmd, buf,len);
}
static int ahci_ongive(struct item* ahci,void* foot,struct halfrel* stack,int sp, u8* arg,int off, void* buf,int len)
{
	//ahci_print("@ahci_ongive: %p,%p\n",ahci,foot);
	return 0;
}




static void ahci_disableport(volatile struct HBA_PORT* port)
{
	//clear status : 0x30,0x10,0x8
	port->serr = 0x07ff0f03;		//0x30
	port->is = 0xfd8000af;			//0x10

	//put port in idle mode
	//ahci_print("port->cmd before disable:%x",(u64)(port->cmd));
	port->cmd &= 0xfffffffe;	//0x18,bit0
	port->cmd &= 0xffffffef;	//0x18,bit4,FRE
 
	int endcycle = 0xffffff;
	u64 endtime = timeread_us() + 1000*1000;
	while(1){
		if(timeread_us() > endtime){
			ahci_print("ahci_disableport: err=endtime, cmd=%x\n", (u64)(port->cmd));
			return;
		}

		endcycle--;
		if(0 == endcycle){
			ahci_print("ahci_disableport: err=endcycle, cmd=%x\n", (u64)(port->cmd));
			return;
		}

		//0x18,bit14,receive running
		if (port->cmd & (1<<14))continue;

		//0x18,bit15,commandlist running
		if (port->cmd & (1<<15))continue;

		//
		break;
	}
	//ahci_print("port->cmd after disable:%x",(u64)(port->cmd));

	//reset port
	port->sctl |= 0x2;
	ahci_print("sleep 5ms+++\n");
	sleep_ms(5);	//wait 1ms(5ms)
	ahci_print("sleep 5ms---\n");
	//for(endcycle=0;endcycle<0xffffff;endcycle++)asm("nop");
	port->sctl &= 0xfffffffd;

	//wait for device detect and communication established
	endcycle = 0xffffff;
	endtime = timeread_us() + 1000*1000;
	while(1){
		if( (port->ssts & 0x3) == 0x3)break;

		if(timeread_us() > endtime){
			ahci_print("ahci_disableport: err=endtime, ssts=%x\n",port->ssts);
			return;
		}

		endcycle--;
		if(0 == endcycle){
			ahci_print("ahci_disableport: err=endcycle, ssts=%x\n",port->ssts);
			return;
		}

	}

	//clear port serial ata error register
	port->serr = 0x07ff0f03;		//0x30
}
static void ahci_enableport(volatile struct HBA_PORT* port)
{
	//ahci_print("port->cmd before enable:%x",(u64)(port->cmd));
	while(port->cmd & (1<<15));	//bit15
 
	port->cmd |= 1<<4;	//bit4,receive enable
	port->cmd |= 1; 	//bit0,start
	//ahci_print("port->cmd after enable:%x",(u64)(port->cmd));
}
static void ahci_configport(volatile struct HBA_PORT* port, int id, u8* ptr, int max)
{
	volatile struct HBA_CMD_HEADER* cmdheader;

	//每个rcvdfis=0x100
	u64 fis = (u64)(ptr + receivefis + id*0x100);
	port->fb = fis & 0xffffffff;
	port->fbu = fis >> 32;

	//每个header=(32*32)*32=0x400*32=0x8000
	u64 clb = (u64)(ptr + commandlist + id*0x400);
	port->clb = clb & 0xffffffff;
	port->clbu = clb >> 32;

	//
	int k;
	u64 addr;
	cmdheader = (struct HBA_CMD_HEADER*)clb;
	for(k=0;k<32;k++)	//0x100*32=0x2000=8k
	{
		//8 prdt entries per command table
		cmdheader[k].prdtl = 8;

		//0x10000/0x20=0x800,(0x800-0x80)/0x10=0x78个
		addr = (u64)(ptr + commandtable + (id*0x10000) + (k<<8));
		cmdheader[k].ctba = addr & 0xffffffff;
		cmdheader[k].ctbau = addr >> 32;
	}
}




int ahci_contractor(struct item* dev, int who, u8* buf, int len)
{
	struct perahci* my = (void*)(dev->priv_256b);
	struct HBA_MEM* abar = my->abar;
	struct HBA_PORT* port = &abar->ports[who];

	int ret = ahci_identify(port, (void*)buf);
	if(ret < 0)return -1;

	struct item* sata = sata_alloc();
	if(0 == sata)return -2;

	if(0 == sata->oncreate)return -3;
	sata->oncreate(sata, 0, 0, 0);

	if(0 == sata->onattach)return -4;
	struct relation* rel2 = relationcreate(sata,0, _dev_,_src_, dev,port, _dev_,0);
	if(0 == rel2)return -5;

	relationattach((struct halfrel*)&rel2->src, (struct halfrel*)&rel2->dst);
	
	return 0;
}
int ahci_list(struct item* dev, int total, u8* ptr, int max, u8* buf, int len)
{
	int j,k,cnt=0;
	struct perahci* my = (void*)(dev->priv_256b);
	struct HBA_MEM* abar = my->abar;
	struct HBA_PORT* port;
	for(j=0;j<32;j++){
		port = &abar->ports[j];

		//host support
		if(0 == (abar->pi & (1<<j)))continue;

		//device present
		u32 ssts = port->ssts;		//0x28
		u8 ipm = (ssts >> 8) & 0x0F;	//0x28.[8,11]
		u8 det = ssts & 0x0F;			//0x28.[0,3]
		if(HBA_PORT_DET_PRESENT != det){
			ahci_print("%x:empty(err@det)\n", j);
			continue;
		}
		if(HBA_PORT_IPM_ACTIVE != ipm){
			ahci_print("%x:empty(err@ipm)\n", j);
			continue;
		}

		ahci_disableport(port);	// Stop command engine

		ahci_configport(port, j, ptr, max);

		ahci_enableport(port);	// Start command engine

		//type
		switch(port->sig){
		case 0x00000101:	//sata
			ahci_print("%x:sata\n", j);
			ahci_contractor(dev, j, buf, len);
			cnt += 1;
			break;
		case 0xeb140101:	//atapi
			ahci_print("%x:atapi\n", j);
			break;
		case 0xc33c0101:	//enclosure....
			ahci_print("%x:enclosure\n", j);
			break;
		case 0x96690101:	//port multiplier
			ahci_print("%x:multiplier\n", j);
			break;
		default:
			ahci_print("%x:unknown\n", j);
			break;
		}//switch
	}//for
	return cnt;
}




void ahci_mmioinit(struct item* dev, struct HBA_MEM* abar)
{
	ahci_print("mmio@%p{\n", abar);
	//printmmio(abar, 0x20);


	//clear home
	int j,k;
	u8* ptr = memoryalloc(0x100000, 0);
	for(j=0;j<0x100000;j++)ptr[j] = 0;

	ahci_print("ghc=%x\n", abar->ghc);
	ahci_print("pi=%x\n", abar->pi);
	abar->ghc |= 0x80000000;
	abar->ghc &= 0xfffffffd;
	abar->is = 0xffffffff;		//clear all

    ahci_print("}\n");


	//data
	struct perahci* my = (void*)(dev->priv_256b);
	my->abar = abar;
	my->port = &abar->ports[0];
	my->onemega = ptr;
	my->cmdlist = ptr+commandlist;
	my->fisrecv = ptr+receivefis;

	dev->kind = _stor_;
	dev->type = _ahci_;
	dev->onreader = (void*)ahci_reader;
	dev->onwriter = (void*)ahci_writer;
	dev->ongiving = (void*)ahci_ongive;
	dev->ontaking = (void*)ahci_ontake;

	//
	ahci_list(dev, 0, ptr, 0x100000, ptr+receivebuf, 0x10000);
}
