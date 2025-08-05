#include "libhard.h"
void filemanager_registerdisk(void*,void*);


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


struct privdata{
    union{
        void* satarel;
        u8 padd[0x1000];
    };
    u8 buffer[0];
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
void sata_printidentify(struct SATA_ident* rdi)
{
	printmemory(rdi, 0x200);
    swap16(rdi->fw_rev, 8);
	swap16(rdi->serial_no, 20);
	swap16(rdi->model,     40);
	logtoall("type=sata\n"
        "fw_ver=<%.8s>\n"
		"serial=<%.20s>\n"
		"model=<%.40s>\n"
        "lba_capacity=<%x>\n"
        "sector_sz=<%x>\n"
        "total_usr_sectors=<%x,%x>\n",
        rdi->fw_rev, rdi->serial_no, rdi->model,
        rdi->lba_capacity, rdi->sector_sz, rdi->total_usr_sectors[0], rdi->total_usr_sectors[1]);
}


static int sata_takeby(struct item* sata,void* selfrel, void* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
    struct privdata* priv = sata->priv_ptr;
    struct halfrel* peer = priv->satarel;
    struct item* ahci = peer ? peer->pchip : 0;
    struct item* port = peer ? peer->pfoot : 0;
    logtoall("%s: cmd=%x, peer=%p, ahci=%p\n", __FUNCTION__, cmd, peer, ahci);

    if(0 == ahci)return -1;
    if(0 == port)return -2;
    return ahci->ontaking(ahci, port, stack, sp, arg, cmd, buf, len);
}
static int sata_giveby(struct item* sata,void* selfrel, void* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
    return 0;
}
static int sata_detach(struct halfrel* self, struct halfrel* peer)
{
    return 0;
}
static int sata_attach(struct halfrel* self, struct halfrel* peer)
{
    struct item* sata = self->pchip;
    struct item* that = peer->pchip;
    logtoall("%s: cmd=%x\n", __FUNCTION__);

    if(that->type == _ahci_){
        struct privdata* priv = sata->priv_ptr;
        priv->satarel = peer;
	    filemanager_registerdisk(sata,0);
    }
    return 0;
}


static int sata_reader(struct item* sata,void* selfrel, p64 arg,int cmd, void* buf,int len)
{
    //logtoall("%s: cmd=%x\n", __FUNCTION__, cmd);
    struct privdata* priv = sata->priv_ptr;
    struct halfrel* peer = priv->satarel;
    struct item* ahci = peer ? peer->pchip : 0;
    struct item* port = peer ? peer->pfoot : 0;
    logtoall("%s: cmd=%x, peer=%p, ahci=%p, port=%p\n", __FUNCTION__, cmd, peer, ahci, port);

    if(0 == ahci)return -1;
    if(0 == port)return -2;
    if(0 == ahci->onreader)return -3;

    int ret = 0;
    if( (_info_ == cmd) && (0 == buf) ){
        ret = ahci->onreader(ahci, port, arg, cmd, priv->buffer, len);
        sata_printidentify((struct SATA_ident*)priv->buffer);
    }
    else{
        ret = ahci->onreader(ahci, port, arg, cmd, buf, len);
    }
    return ret;
}
static int sata_writer(struct item* sata,void* selfrel, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
static int sata_delete(struct item* sata)
{
    return 0;
}
static int sata_create(struct item* sata)
{
	struct privdata* priv = memoryalloc(0x100000, 0);
    sata->priv_ptr = priv;

    logtoall("%s: item=%p priv=%p\n", __FUNCTION__, sata, priv);
    return 0;
}


void* sata_alloc()
{
	struct item* sata = device_alloc_fromtype(_sata_);
	sata->tier = _dev_;
	sata->kind = _stor_;
    //sata->type = _sata_;

    sata->oncreate = (void*)sata_create;
    sata->ondelete = (void*)sata_delete;
    sata->onattach = (void*)sata_attach;
    sata->ondetach = (void*)sata_detach;
    sata->onreader = (void*)sata_reader;
    sata->onwriter = (void*)sata_writer;
    sata->ontaking = (void*)sata_takeby;
    sata->ongiving = (void*)sata_giveby;
    return sata;
}