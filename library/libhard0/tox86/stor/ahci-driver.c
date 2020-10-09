#include "libhard.h"
u32 in32(u16 port);
void out32(u16 port, u32 data);




#define commandlist 0x00000
#define commandtable 0x10000
#define receivefis 0x20000
#define receivebuf 0x30000
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
	u32       clb;	// 0x00, command list base address, 1K-byte aligned
	u32      clbu;	// 0x04, command list base address upper 32 bits
	u32        fb;	// 0x08, FIS base address, 256-byte aligned
	u32       fbu;	// 0x0C, FIS base address upper 32 bits
	u32        is;	// 0x10, interrupt status
	u32        ie;	// 0x14, interrupt enable
	u32       cmd;	// 0x18, command and status
	u32      rsv0;	// 0x1C, Reserved
	u32       tfd;	// 0x20, task file data
	u32       sig;	// 0x24, signature
	u32      ssts;	// 0x28, SATA status (SCR0:SStatus)
	u32      sctl;	// 0x2C, SATA control (SCR2:SControl)
	u32      serr;	// 0x30, SATA error (SCR1:SError)
	u32      sact;	// 0x34, SATA active (SCR3:SActive)
	u32        ci;	// 0x38, command issue
	u32      sntf;	// 0x3C, SATA notification (SCR4:SNotification)
	u32       fbs;	// 0x40, FIS-based switch control
	u32  rsv1[11];	// 0x44 ~ 0x6F, Reserved
	u32 vendor[4];	// 0x70 ~ 0x7F, vendor specific
}__attribute__((packed));

struct HBA_MEM{
//[0x00, 0x2B]: Generic Host Control
	u32     cap;	// 0x00, Host capability
	u32     ghc;	// 0x04, Global host control
	u32      is;	// 0x08, Interrupt status
	u32      pi;	// 0x0C, Port implemented
	u32      vs;	// 0x10, Version
	u32 ccc_ctl;	// 0x14, Command completion coalescing control
	u32 ccc_pts;	// 0x18, Command completion coalescing ports
	u32  em_loc;	// 0x1C, Enclosure management location
	u32  em_ctl;	// 0x20, Enclosure management control
	u32    cap2;	// 0x24, Host capabilities extended
	u32    bohc;	// 0x28, BIOS/OS handoff control and status

//[0x2C, 0x9F]: Reserved
	u8 rsv[0xA0-0x2C];

//[0xA0, 0xFF]: Vendor specific registers
	u8 vendor[0x100-0xA0];

//[0x100, x]: Port control registers, 1-32
	struct HBA_PORT ports[0];
}__attribute__((packed));

struct SATA_ident
{
	u16               config;	//[00,01] lots of obsolete bit flags
	u16                 cyls;	//[02,03]obsolete
	u16            reserved2;	//[04,05]special config
	u16                heads;	//[06,07]"physical" heads
	u16          track_bytes;	//[08,09]unformatted bytes per track
	u16         sector_bytes;	//[0a,0b]unformatted bytes per sector
	u16              sectors;	//[0c,0d]"physical" sectors per track
	u16              vendor0;	//[0e,0f]vendor unique
	u16              vendor1;	//[10,11]vendor unique
	u16              vendor2;	//[12,13]vendor unique
	u8         serial_no[20];	//[14,27]0 = not_specified
	u16             buf_type;	//[28,29]
	u16             buf_size;	//[2a,2b]512 byte increments; 0 = not_specified
	u16            ecc_bytes;	//[2c,2d]for r/w long cmds; 0 = not_specified
	u8             fw_rev[8];	//[2e,35]0 = not_specified
	u8             model[40];	//[36,5d]0 = not_specified
	u16          multi_count;	//[5e,5f]Multiple Count
	u16             dword_io;	//[60,61]0=not_implemented; 1=implemented
	u16          capability1;	//[62,63]vendor unique
	u16          capability2;	//[64,65]bits 0:DMA 1:LBA 2:IORDYsw 3:IORDYsup word: 50
	u8               vendor5;	//[66,66]vendor unique
	u8                  tPIO;	//[67,67]0=slow, 1=medium, 2=fast
	u8               vendor6;	//[68,68]vendor unique
	u8                  tDMA;	//[69,69]0=slow, 1=medium, 2=fast
	u16          field_valid;	//[6a,6b]bits 0:cur_ok 1:eide_ok
	u16             cur_cyls;	//[6c,6d]logical cylinders
	u16            cur_heads;	//[6e,6f]logical heads word 55
	u16          cur_sectors;	//[70,71]logical sectors per track
	u16        cur_capacity0;	//[72,73]logical total sectors on drive
	u16        cur_capacity1;	//[74,75] (2 words, misaligned int)    
	u8              multsect;	//[76,76]current multiple sector count
	u8        multsect_valid;	//[77,77]when (bit0==1) multsect is ok
	u32         lba_capacity;	//[78,7b]total number of sectors
	u16            dma_1word;	//[7c,7d]single-word dma info
	u16            dma_mword;	//[7e,7f]multiple-word dma info
	u16       eide_pio_modes;	//[80,81]bits 0:mode3 1:mode4
	u16         eide_dma_min;	//[82,83]min mword dma cycle time (ns)
	u16        eide_dma_time;	//[84,85]recommended mword dma cycle time (ns)
	u16             eide_pio;	//[86,87]min cycle time (ns), no IORDY 
	u16       eide_pio_iordy;	//[88,89]min cycle time (ns), with IORDY
	u16        words69_70[2];	//[8a,8d]reserved words 69-70
	u16        words71_74[4];	//[8e,95]reserved words 71-74
	u16          queue_depth;	//[96,97]
	u16      sata_capability;	//[98,99] SATA Capabilities word 76
	u16      sata_additional;	//[9a,9b] Additional Capabilities
	u16       sata_supported;	//[9c,9d]SATA Features supported 
	u16     features_enabled;	//[9e,9f]SATA features enabled
	u16        major_rev_num;	//[a0,a1] Major rev number word 80
	u16        minor_rev_num;	//[a2,a3]
	u16        command_set_1;	//[a4,a5]bits 0:Smart 1:Security 2:Removable 3:PM
	u16        command_set_2;	//[a6,a7]bits 14:Smart Enabled 13:0 zero
	u16                cfsse;	//[a8,a9]command set-feature supported extensions
	u16         cfs_enable_1;	//[aa,ab]command set-feature enabled
	u16         cfs_enable_2;	//[ac,ad]command set-feature enabled
	u16          csf_default;	//[ae,af]command set-feature default
	u16            dma_ultra;	//[b0,b1]
	u16               word89;	//[b2,b3]reserved (word 89)
	u16               word90;	//[b4,b5]reserved (word 90)
	u16         CurAPMvalues;	//[b6,b7]current APM values
	u16               word92;	//[b8,b9]reserved (word 92)
	u16             comreset;	//[ba,bb]should be cleared to 0
	u16            accoustic;	//[bc,bd]accoustic management
	u16           min_req_sz;	//[be,bf]Stream minimum required size
	u16    transfer_time_dma;	//[c0,c1]Streaming Transfer Time-DMA
	u16       access_latency;	//[c2,c3]Streaming access latency-DMA & PIO WORD 97
	u32     perf_granularity;	//[c4,c7]Streaming performance granularity
	u32 total_usr_sectors[2];	//[c8,cf]Total number of user addressable sectors
	u16    transfer_time_pio;	//[d0,d1]Streaming Transfer time PIO
	u16          reserved105;	//[d2,d3]Word 105
	u16            sector_sz;	//[d4,d5]Puysical Sector size / Logical sector size
	u16     inter_seek_delay;	//[d6,d7]In microseconds
	u16      words108_116[9];	//[d8,e9]
	u32     words_per_sector;	//[ea,ed]words per logical sectors
	u16   supported_settings;	//[ee,ef]continued from words 82-84
	u16        command_set_3;	//[f0,f1]continued from words 85-87
	u16      words121_126[6];	//[f2,fd]reserved words 121-126
	u16              word127;	//[fe,ff]reserved (word 127)
	u16      security_status;	//[100,101]device lock function, 15:9reserved, 8security level set=max/clear=high, 7:6reserved, 5enhanced erase, 4expire, 3frozen, 2locked, 1en/disabled, 0capability*/
	u16                 csfo;	//[102,103]current set features options: 15:4reserved, 3auto-reassign, 2reverting, 1read-look-ahead, 0write-cache*/
	u16     words130_155[26];	//[]reserved vendor words 130-155
	u16              word156;
	u16      words157_159[3];	//[]reserved vendor words 157-159
	u16                  cfa;	//[]CFA Power mode 1
	u16     words161_175[15];	//[]Reserved
	u8      media_serial[60];	//[]words 176-205 Current Media serial number
	u16    sct_cmd_transport;	//[]SCT Command Transport
	u16      words207_208[2];	//[]reserved
	u16          block_align;	//[]Alignement of logical blocks in larger physical blocks
	u32        WRV_sec_count;	//[]Write-Read-Verify sector count mode 3 only
	u32       verf_sec_count;	//[]Verify Sector count mode 2 only
	u16  nv_cache_capability;	//[]NV Cache capabilities
	u16          nv_cache_sz;	//[]NV Cache size in logical blocks
	u16         nv_cache_sz2;	//[]NV Cache size in logical blocks
	u16        rotation_rate;	//[]Nominal media rotation rate
	u16          reserved218;
	u16     nv_cache_options;	//[]NV Cache options
	u16      words220_221[2];	//[]reserved
	u16  transport_major_rev;	//[f2,f3]
	u16  transport_minor_rev;	//[f4,f5]
	u16     words224_233[10];	//[f6,f7]Reserved
	u16   min_dwnload_blocks;	//[f8,f9]Minimum number of 512byte units per DOWNLOAD MICROCODE, command for mode 03h
	u16   max_dwnload_blocks;	//[fa,fb]Maximum number of 512byte units per DOWNLOAD MICROCODE, command for mode 03h
	u16     words236_254[19];	//[fc,fd]Reserved
	u16            integrity;	//[fe,ff]Cheksum, Signature
}__attribute__((packed));





void swap16(u8* buf, int len)
{
	u8 t;
	int j;
	for(j=0;j<len;j+=2){
		t = buf[j];
		buf[j+0] = buf[j+1];
		buf[j+1] = t;
	}
}
int ahci_identify(volatile struct HBA_PORT* port, struct SATA_ident* rdi)
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
		say("err:no cmdslot\n",0);
		return -1;
	}
	cmdheader += cmdslot;
	//say("cmdslot:",(u64)cmdslot);
	//say("cmdheader:",(u64)cmdheader);

	cmdheader->cfl=sizeof(struct FIS_REG_H2D)/4;	//Command FIS size
	cmdheader->w = 0;			// Read from device
	cmdheader->prdtl=1;
	cmdheader->prdbc=0;

	//make the table
	struct HBA_CMD_TBL* cmdtable = (struct HBA_CMD_TBL*)(u64)cmdheader->ctba;
	//say("cmdtable(comheader->ctba):",(u64)cmdtable);
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
	volatile int timeout = 0;
	while(1)
	{
		timeout++;
		if(timeout>0xfffff){
			say("(timeout1)port->tfd:%x\n",(u64)port->tfd);
			return -1;
		}

		//0x88=interface busy|data transfer requested
		temp = port->tfd;	 //0x20
		if( (temp&0x80) == 0x80 )continue;      //bit7,busy
		if( (temp&0x8) == 0x8)continue;	 //bit3,DRQ

		break;
	}
	//say("is:",(u64)port->is);
	//u32* pointer=(u32*)(u64)(port->fb);
	//set issue,wait for completion
	port->ci = 1<<cmdslot;  // Issue command,start reading
	timeout=0;
	while(1)
	{
		timeout++;
		if(timeout>0xfffff){
			say("(timeout2)ci=%x,prdbc=%x\n",temp,cmdheader->prdbc);
			return -2;
		}

		temp=port->is;
		if (temp & 0x40000000)  // Task file error
		{
			say("port error 1\n");
			return -9;
		}

		//in the PxIS port field as well (1<<5)
		temp = port->ci;
		if( (temp & (1<<cmdslot)) != 0) continue;

		break;
	}

	int j,k;
	//printmemory(rdi, 0x200);
	swap16(rdi->serial_no, 20);
	swap16(rdi->model,     40);
	say("	serial=<%.20s>\n"
		"	model=<%.40s>\n", rdi->serial_no, rdi->model);
	return 1;
}
int ahci_list(volatile struct HBA_MEM* abar, int total, u8* buf, int len)
{
	int j,cnt=0;
	volatile struct HBA_PORT* port;
	for(j=0;j<total;j++){
		port = (struct HBA_PORT*)&abar->ports[j];

		u32 ssts = port->ssts;		//0x28
		u8 ipm = (ssts >> 8) & 0x0F;	//0x28.[8,11]
		u8 det = ssts & 0x0F;			//0x28.[0,3]
		if((0 == ipm) | (0 == det)){
			say("%x:empty(%x)\n", j, ssts);
			continue;
		}

		switch(port->sig){
			case 0x00000101:	//sata
			{
				say("%x:sata\n", j);
				ahci_identify(port, (void*)buf);
				cnt += 1;
				break;
			}
			case 0xeb140101:	//atapi
			{
				say("%x:atapi\n", j);
				break;
			}
			case 0xc33c0101:	//enclosure....
			{
				say("%x:enclosure\n", j);
				break;
			}
			case 0x96690101:	//port multiplier
			{
				say("%x:multiplier\n", j);
				break;
			}
			default:{
				say("%x:unknown\n", j);
				break;
			}
		}//switch
	}//for
	return cnt;
}




static void disableport(volatile struct HBA_PORT* port)
{
	//clear status : 0x30,0x10,0x8
	port->serr = 0x07ff0f03;		//0x30
	port->is = 0xfd8000af;			//0x10

	//put port in idle mode
	//say("port->cmd before disable:%x",(u64)(port->cmd));
	port->cmd &= 0xfffffffe;	//0x18,bit0
	port->cmd &= 0xffffffef;	//0x18,bit4,FRE
 
	int timeout = 100000;
	while(timeout){
		timeout--;
		if(0 == timeout){
			say("(timeout)still running:%x\n",(u64)(port->cmd));
			return;
		}

		//0x18,bit14,receive running
		if (port->cmd & (1<<14))continue;

		//0x18,bit15,commandlist running
		if (port->cmd & (1<<15))continue;

		//
		break;
	}
	//say("port->cmd after disable:%x",(u64)(port->cmd));

	//reset port
	port->sctl |= 0x2;
	for(timeout=0;timeout<0xffffff;timeout++)asm("nop");	//wait 1ms(5ms)
	port->sctl &= 0xfffffffd;

	//wait for device detect and communication established
	timeout = 100000;
	while(timeout){
		timeout--;
		if(0 == timeout){
			//say("no device:%x",11111);
			return;
		}

		if( (port->ssts & 0x3) == 0x3)break;
	}

	//clear port serial ata error register
	port->serr = 0x07ff0f03;		//0x30
}
static void enableport(volatile struct HBA_PORT* port)
{
	//say("port->cmd before enable:%x",(u64)(port->cmd));
	while(port->cmd & (1<<15));	//bit15
 
	port->cmd |= 1<<4;	//bit4,receive enable
	port->cmd |= 1; 	//bit0,start
	//say("port->cmd after enable:%x",(u64)(port->cmd));
}




void ahci_mmioinit(struct item* dev, volatile struct HBA_MEM* abar)
{
	say("ahci@mmio:%p{\n", abar);
	//printmmio(abar, 0x20);


	//clear home
	int j,k;
	u8* ptr = memorycreate(0x100000, 0);
	for(j=0;j<0x100000;j++)ptr[j] = 0;


//1: host settings
	abar->ghc |= 0x80000000;
	abar->ghc &= 0xfffffffd;
	abar->is = 0xffffffff;		//clear all

	u32 cnt = 0;
	u32 tmp = abar->pi;		//实际多少个port
	while(tmp & 1){
		tmp >>= 1;
		cnt++;
	}
	say("totalport=%x\n", cnt);


//2: ports settings
	u64 addr;
	volatile struct HBA_PORT* port;
	volatile struct HBA_CMD_HEADER* cmdheader;
	for(j=0;j<cnt;j++)
	{
		//disable
		port = (struct HBA_PORT*)&abar->ports[j];
		disableport(port);	// Stop command engine

		//每个rcvdfis=0x100
		addr = (u64)(ptr + receivefis + j*0x100);
		port->fb = addr & 0xffffffff;
		port->fbu = addr >> 32;

		//每个header=(32*32)*32=0x400*32=0x8000
		addr = (u64)(ptr + commandlist + j*0x400);
		port->clb = addr & 0xffffffff;
		port->clbu = addr >> 32;

		//
		cmdheader = (struct HBA_CMD_HEADER*)(u64)(port->clb);
		for(k=0;k<32;k++)	//0x100*32=0x2000=8k
		{
			//8 prdt entries per command table
			cmdheader[k].prdtl = 8;

			//0x10000/0x20=0x800,(0x800-0x80)/0x10=0x78个
			addr = (u64)(ptr + commandtable + (j*0x10000) + (k<<8));
			cmdheader[k].ctba = addr & 0xffffffff;
			cmdheader[k].ctbau = addr >> 32;
		}

		//enable
		enableport(port);	// Start command engine
	}

	//list
	ahci_list(abar, cnt, ptr+receivebuf, 0x10000);

    say("}\n");
}
void ahci_portinit(struct item* dev, u32 addr)
{
	u32 temp;
	say("ahci@port:%x{\n",addr);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc);
	say("sts,cmd=%x\n", temp);

	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp | (1<<10) | (1<<2));		//bus master=1

	out32(0xcf8, addr+0x10);
	say("bar0=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x14);
	say("bar1=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x18);
	say("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	say("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	say("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	temp = in32(0xcfc);
	say("bar5=%x\n", temp);

	say("}\n");


	//hba addr
	void* mmio = (void*)(u64)(temp&0xfffffff0);
	ahci_mmioinit(dev, mmio);
}
