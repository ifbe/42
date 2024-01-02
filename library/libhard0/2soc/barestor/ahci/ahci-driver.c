#include "libhard.h"
#define ahci_print(fmt, ...) say("<%08lld,ahci>" fmt, timeread_us(), ##__VA_ARGS__)
u32 in32(u16 port);
void out32(u16 port, u32 data);
void filemanager_registerdisk(void*,void*);




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
struct perahci{
	//hc
	struct HBA_MEM* abar;
	struct HBA_PORT* port;	//port0

	//my
	void* onemega;
	void* cmdlist;
	void* fisrecv;
}__attribute__((packed));





static void swap16(u8* buf, int len)
{
	u8 t;
	int j;
	for(j=0;j<len;j+=2){
		t = buf[j];
		buf[j+0] = buf[j+1];
		buf[j+1] = t;
	}
}
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




static int ahci_identify(volatile struct HBA_PORT* port, struct SATA_ident* rdi)
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

	int j,k;
	//printmemory(rdi, 0x200);
	swap16(rdi->serial_no, 20);
	swap16(rdi->model,     40);
	say("type=sata\n"
		"serial=<%.20s>\n"
		"model=<%.40s>\n", rdi->serial_no, rdi->model);
	return 1;
}
static int ahci_satacmd(volatile struct HBA_PORT* port, struct SATA_ident* rdi)
{
	return 0;
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
static int ahci_readinfo(struct item* ahci,void* foot,struct halfrel* stack,int sp, void* buf,int len)
{
	struct perahci* my = (void*)(ahci->priv_256b);
	say("@ahci_readinfo: %p,%p\n",ahci,foot);

	if(0 == buf)buf = my->onemega + receivebuf;
	int ret = ahci_identify(foot, buf);
	return 0;
}




static int ahci_ontake(struct item* ahci,void* foot,struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	say("ahci_ontake:%llx,%x,%p,%x\n",arg,cmd,buf,len);
	if(_info_ == cmd)return ahci_readinfo(ahci,foot, stack,sp, buf,len);

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

	filemanager_registerdisk(dev,port);
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
	dev->ongiving = (void*)ahci_ongive;
	dev->ontaking = (void*)ahci_ontake;


	//
	ahci_list(dev, 0, ptr, 0x100000, ptr+receivebuf, 0x10000);
}
