#include "libhard.h"
void* mmiobase();
//
void gpio_prepsdhci();
//
u32 mbox_getbaseclock();
//
void wait_msec(int);
void wait_cycles(int);
//
void printmmio(void*, int);
void filemanager_registersupplier(void*,void*);




#define COREFREQ (250*1000*1000)
//
#define SD_CLOCK_ID         400000
#define SD_CLOCK_NORMAL     25000000
#define SD_CLOCK_HIGH       50000000
#define SD_CLOCK_100        100000000
#define SD_CLOCK_208        208000000
//300000=Arasan sdhost, 340000=rpi4 sdmmc2
#define EMMC_OFFS           0x00300000
#define EMMC_ARG2           ((volatile unsigned int*)(sdhci+0x00))
#define EMMC_BLKSIZECNT     ((volatile unsigned int*)(sdhci+0x04))
#define EMMC_ARG1           ((volatile unsigned int*)(sdhci+0x08))
#define EMMC_CMDTM          ((volatile unsigned int*)(sdhci+0x0C))
#define EMMC_RESP0          ((volatile unsigned int*)(sdhci+0x10))
#define EMMC_RESP1          ((volatile unsigned int*)(sdhci+0x14))
#define EMMC_RESP2          ((volatile unsigned int*)(sdhci+0x18))
#define EMMC_RESP3          ((volatile unsigned int*)(sdhci+0x1C))
#define EMMC_DATA           ((volatile unsigned int*)(sdhci+0x20))
#define EMMC_STATUS         ((volatile unsigned int*)(sdhci+0x24))
#define EMMC_CONTROL0       ((volatile unsigned int*)(sdhci+0x28))
#define EMMC_CONTROL1       ((volatile unsigned int*)(sdhci+0x2C))
#define EMMC_INTERRUPT      ((volatile unsigned int*)(sdhci+0x30))
#define EMMC_INT_MASK       ((volatile unsigned int*)(sdhci+0x34))
#define EMMC_INT_EN         ((volatile unsigned int*)(sdhci+0x38))
#define EMMC_CONTROL2       ((volatile unsigned int*)(sdhci+0x3C))
#define EMMC_SLOTISR_VER    ((volatile unsigned int*)(sdhci+0xFC))
//
#define GO_IDLE_STATE           0
#define ALL_SEND_CID            2
#define SEND_RELATIVE_ADDR      3
#define SET_DSR                 4
#define IO_SET_OP_COND          5
#define SWITCH_FUNC             6
#define SELECT_CARD             7
#define DESELECT_CARD           7
#define SELECT_DESELECT_CARD    7
#define SEND_IF_COND            8
#define SEND_CSD                9
#define SEND_CID                10
#define VOLTAGE_SWITCH          11
#define STOP_TRANSMISSION       12
#define SEND_STATUS             13
#define GO_INACTIVE_STATE       15
#define SET_BLOCKLEN            16
#define READ_SINGLE_BLOCK       17
#define READ_MULTIPLE_BLOCK     18
#define SEND_TUNING_BLOCK       19
#define SPEED_CLASS_CONTROL     20
#define SET_BLOCK_COUNT         23
#define WRITE_BLOCK             24
#define WRITE_MULTIPLE_BLOCK    25
#define PROGRAM_CSD             27
#define SET_WRITE_PROT          28
#define CLR_WRITE_PROT          29
#define SEND_WRITE_PROT         30
#define ERASE_WR_BLK_START      32
#define ERASE_WR_BLK_END        33
#define ERASE                   38
#define LOCK_UNLOCK             42
#define APP_CMD                 55
#define GEN_CMD                 56
// command flags
#define CMD_NEED_APP        0x80000000
#define CMD_RSPNS_48        0x00020000
#define CMD_ERRORS_MASK     0xfff9c004
#define CMD_RCA_MASK        0xffff0000
// COMMANDs
#define CMD_GO_IDLE         0x00000000
#define CMD_ALL_SEND_CID    0x02010000
#define CMD_SEND_REL_ADDR   0x03020000
#define CMD_CARD_SELECT     0x07030000
#define CMD_SEND_IF_COND    0x08020000
#define CMD_STOP_TRANS      0x0C030000
#define CMD_READ_SINGLE     0x11220010
#define CMD_READ_MULTI      0x12220032
#define CMD_SET_BLOCKCNT    0x17020000
#define CMD_APP_CMD         0x37000000
#define CMD_SET_BUS_WIDTH   (0x06020000|CMD_NEED_APP)
#define CMD_SEND_OP_COND    (0x29020000|CMD_NEED_APP)
#define CMD_SEND_SCR        (0x33220010|CMD_NEED_APP)
// STATUS register settings
#define SR_READ_AVAILABLE   0x00000800
#define SR_DAT_INHIBIT      0x00000002
#define SR_CMD_INHIBIT      0x00000001
#define SR_APP_CMD          0x00000020
// INTERRUPT register settings
#define INT_DATA_TIMEOUT    0x00100000
#define INT_CMD_TIMEOUT     0x00010000
#define INT_READ_RDY        0x00000020
#define INT_CMD_DONE        0x00000001
#define INT_ERROR_MASK      0x017E8000
// CONTROL register settings
#define C0_SPI_MODE_EN      0x00100000
#define C0_HCTL_HS_EN       0x00000004
#define C0_HCTL_DWITDH      0x00000002
#define C1_SRST_DATA        0x04000000
#define C1_SRST_CMD         0x02000000
#define C1_SRST_HC          0x01000000
#define C1_TOUNIT_DIS       0x000f0000
#define C1_TOUNIT_MAX       0x000e0000
#define C1_CLK_GENSEL       0x00000020
#define C1_CLK_EN           0x00000004
#define C1_CLK_STABLE       0x00000002
#define C1_CLK_INTLEN       0x00000001
// SLOTISR_VER values
#define HOST_SPEC_NUM       0x00ff0000
#define HOST_SPEC_NUM_SHIFT 16
#define HOST_SPEC_V3        2
#define HOST_SPEC_V2        1
#define HOST_SPEC_V1        0
// SCR flags
#define SCR_SD_BUS_WIDTH_4  0x00000400
#define SCR_SUPP_SET_BLKCNT 0x02000000
#define SCR_SUPP_CCS        0x00000001
//
#define ACMD41_VOLTAGE      0x00ff8000
#define ACMD41_CMD_COMPLETE 0x80000000
#define ACMD41_CMD_CCS      0x40000000
#define ACMD41_ARG_HC       0x51ff8000
//
#define SD_OK                0
#define SD_TIMEOUT          -1
#define SD_ERROR            -2




struct persdhci{
	unsigned long sd_scr[2];
	unsigned long sd_rca;
	unsigned long sd_err;
	unsigned long sd_hv;
	void* sdhci;
};




/**
 * Wait for data or command ready
 */
int sd_status(struct persdhci* per, unsigned int mask)
{
	void* sdhci = per->sdhci;
	int cnt = 500000;
	while((*EMMC_STATUS & mask) && !(*EMMC_INTERRUPT & INT_ERROR_MASK) && cnt--) wait_msec(1);
	return (cnt <= 0 || (*EMMC_INTERRUPT & INT_ERROR_MASK)) ? SD_ERROR : SD_OK;
}

/**
 * Wait for interrupt
 */
int sd_int(struct persdhci* per, unsigned int mask)
{
	void* sdhci = per->sdhci;

	unsigned int r, m=mask | INT_ERROR_MASK;
	int cnt = 1000000; while(!(*EMMC_INTERRUPT & m) && cnt--) wait_msec(1);
	r=*EMMC_INTERRUPT;
	if(cnt<=0 || (r & INT_CMD_TIMEOUT) || (r & INT_DATA_TIMEOUT) ) {
		*EMMC_INTERRUPT=r;
		return SD_TIMEOUT;
	}
	else if(r & INT_ERROR_MASK) {
		*EMMC_INTERRUPT=r;
		return SD_ERROR;
	}
	*EMMC_INTERRUPT = mask;
	return 0;
}

/**
 * Send a command
 */
int sd_cmd(struct persdhci* per, unsigned int code, unsigned int arg)
{
	void* sdhci = per->sdhci;
	int ret = 0;
	per->sd_err = SD_OK;
	if(code&CMD_NEED_APP) {
		ret = sd_cmd(per, CMD_APP_CMD|(per->sd_rca?CMD_RSPNS_48:0), per->sd_rca);
		if(per->sd_rca && !ret) {
			say("ERROR: failed to send SD APP command\n");
			per->sd_err=SD_ERROR;
			return 0;
		}
		code &= ~CMD_NEED_APP;
	}
	if(sd_status(per, SR_CMD_INHIBIT)) {
		say("ERROR: EMMC busy\n");
		per->sd_err = SD_TIMEOUT;
		return 0;
	}
	//say("EMMC: Sending command %x,%x\n",code, arg);

	*EMMC_INTERRUPT = *EMMC_INTERRUPT;
	*EMMC_ARG1 = arg;
	*EMMC_CMDTM = code;
	if(code==CMD_SEND_OP_COND) wait_msec(1000);
	else if(code==CMD_SEND_IF_COND || code==CMD_APP_CMD) wait_msec(100);

	if((ret = sd_int(per, INT_CMD_DONE))) {
		say("error@send EMMC command: code=%x,arg=%x\n",code,arg);
		per->sd_err = ret;
		return 0;
	}
	ret = *EMMC_RESP0;

	if(code==CMD_GO_IDLE || code==CMD_APP_CMD) return 0;
	else if(code==(CMD_APP_CMD|CMD_RSPNS_48)) return ret&SR_APP_CMD;
	else if(code==CMD_SEND_OP_COND) return ret;
	else if(code==CMD_SEND_IF_COND) return ret==arg? SD_OK : SD_ERROR;
	else if(code==CMD_ALL_SEND_CID) {
		ret |= *EMMC_RESP3;
		ret |= *EMMC_RESP2;
		ret |= *EMMC_RESP1;
		return ret;
	}
	else if(code==CMD_SEND_REL_ADDR) {
		per->sd_err=(((ret&0x1fff))|((ret&0x2000)<<6)|((ret&0x4000)<<8)|((ret&0x8000)<<8))&CMD_ERRORS_MASK;
		return ret&CMD_RCA_MASK;
	}
	return ret&CMD_ERRORS_MASK;
	// make gcc happy
	return 0;
}


/**
 * set SD clock to frequency in Hz
 */
int sd_clk(struct persdhci* per, unsigned int f)
{
	void* sdhci = per->sdhci;

	unsigned int d,c=41666666/f,x,s=32,h=0;
	int cnt = 100000;
	while((*EMMC_STATUS & (SR_CMD_INHIBIT|SR_DAT_INHIBIT)) && cnt--) wait_msec(1);
	if(cnt <= 0) {
		say("ERROR: timeout waiting for inhibit flag\n");
		return SD_ERROR;
	}

	*EMMC_CONTROL1 &= ~C1_CLK_EN;
	wait_msec(10);

	x=c-1;
	if(!x) s=0;
	else {
		if(!(x & 0xffff0000u)) { x <<= 16; s -= 16; }
		if(!(x & 0xff000000u)) { x <<= 8;  s -= 8; }
		if(!(x & 0xf0000000u)) { x <<= 4;  s -= 4; }
		if(!(x & 0xc0000000u)) { x <<= 2;  s -= 2; }
		if(!(x & 0x80000000u)) { x <<= 1;  s -= 1; }
		if(s>0) s--;
		if(s>7) s=7;
	}

	if(per->sd_hv>HOST_SPEC_V2) d=c;
	else d=(1<<s);

	if(d<=2) {d=2;s=0;}
	say("EMMC: sd_clk divisor %x, shift %x\n", d, s);

	if(per->sd_hv>HOST_SPEC_V2) h=(d&0x300)>>2;
	d=(((d&0x0ff)<<8)|h);
	*EMMC_CONTROL1=(*EMMC_CONTROL1&0xffff003f)|d;
	wait_msec(10);
	*EMMC_CONTROL1 |= C1_CLK_EN;
	wait_msec(10);

	cnt=10000;
	while(!(*EMMC_CONTROL1 & C1_CLK_STABLE) && cnt--) wait_msec(10);
	if(cnt<=0) {
		say("ERROR: failed to get stable clock\n");
		return SD_ERROR;
	}
	return SD_OK;
}




/**
 * read a block from sd card and return the number of bytes read
 * returns 0 on error.
 */
int sd_readblock(struct persdhci* per, unsigned int lba, unsigned char *buf, int num)
{
	void* sdhci = per->sdhci;

	int r,c=0,d;
	if(num<1) num=1;

	say("EMMC: sd_readblock lba %x,%x\n", lba, num);
	if(sd_status(per, SR_DAT_INHIBIT)) {per->sd_err=SD_TIMEOUT; return 0;}

	unsigned int* tmp = (unsigned int*)buf;
	if(per->sd_scr[0] & SCR_SUPP_CCS) {
		if(num > 1 && (per->sd_scr[0] & SCR_SUPP_SET_BLKCNT)) {
			sd_cmd(per, CMD_SET_BLOCKCNT,num);
			if(per->sd_err) return 0;
		}
		*EMMC_BLKSIZECNT = (num << 16) | 512;
		sd_cmd(per, num == 1 ? CMD_READ_SINGLE : CMD_READ_MULTI, lba);
		if(per->sd_err) return 0;
	} else {
		*EMMC_BLKSIZECNT = (1 << 16) | 512;
	}

	while( c < num ) {
		if(!(per->sd_scr[0] & SCR_SUPP_CCS)) {
			sd_cmd(per, CMD_READ_SINGLE,(lba+c)*512);
			if(per->sd_err) return 0;
		}
		if((r=sd_int(per, INT_READ_RDY))){
			say("ERROR: Timeout waiting for ready to read\n");
			per->sd_err = r;
			return 0;
		}
		for(d=0;d<128;d++)tmp[d] = *EMMC_DATA;
		c++;
		tmp += 128;
	}
	if( num > 1 && !(per->sd_scr[0] & SCR_SUPP_SET_BLKCNT) && (per->sd_scr[0] & SCR_SUPP_CCS)) sd_cmd(per, CMD_STOP_TRANS, 0);
	return per->sd_err!=SD_OK || c!=num? 0 : num*512;
}


static int sdhci_ontake(struct item* dev, void* foot, void* stack, int sp, void* arg, int idx, void* buf, int len)
{
	struct persdhci* per = (void*)dev->priv_data;
	return sd_readblock(per, idx>>9, buf, len>>9);
}
static int sdhci_ongive(struct item* dev, void* foot, void* stack, int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}




int freesdhci()
{
	return 0;
}
int initsdhci(struct item* dev)
{
	long r,cnt,ccs=0;
	say("@initsdhci\n");

	//get addr
	struct persdhci* per = (void*)dev->priv_data;
	void* sdhci = mmiobase() + EMMC_OFFS;
	per->sdhci = sdhci;
	printmmio(sdhci, 0x40);

	//prep gpio
	gpio_prepsdhci();
	say("EMMC: GPIO set up\n");

	//get clock
	u32 baseclock = mbox_getbaseclock();
	say("baseclock_frommbox=%d\n",baseclock);

	per->sd_hv = (*EMMC_SLOTISR_VER & HOST_SPEC_NUM) >> HOST_SPEC_NUM_SHIFT;
	say("sd_hv=%x\n", per->sd_hv);

	// Reset the card.
	*EMMC_CONTROL0 = 0;
	*EMMC_CONTROL1 |= C1_SRST_HC;
	cnt = 10000;
	do{wait_msec(10);} while( (*EMMC_CONTROL1 & C1_SRST_HC) && cnt-- );
	if(cnt <= 0) {
		say("ERROR: failed to reset EMMC\n");
		return SD_ERROR;
	}
	say("EMMC: reset OK\n");
	*EMMC_CONTROL1 |= C1_CLK_INTLEN | C1_TOUNIT_MAX;
	wait_msec(10);

	// Set clock to setup frequency.
	if((r=sd_clk(per, 400000))) return r;

	*EMMC_INT_EN   = 0xffffffff;
	*EMMC_INT_MASK = 0xffffffff;
	per->sd_scr[0] = per->sd_scr[1] = per->sd_rca = per->sd_err = 0;

	sd_cmd(per, CMD_GO_IDLE, 0);
	if(per->sd_err) return per->sd_err;

	sd_cmd(per, CMD_SEND_IF_COND,0x000001AA);
	if(per->sd_err) return per->sd_err;

	cnt = 6;
	r=0;
	while(!(r&ACMD41_CMD_COMPLETE) && cnt--) {
		wait_cycles(400);
		r = sd_cmd(per, CMD_SEND_OP_COND, ACMD41_ARG_HC);
		say("EMMC: CMD_SEND_OP_COND returned ");
		if(r&ACMD41_CMD_COMPLETE)say("COMPLETE ");
		if(r&ACMD41_VOLTAGE)say("VOLTAGE ");
		if(r&ACMD41_CMD_CCS)say("CCS %08x,%08x", r>>32, r);
		say("\n");

		if(per->sd_err!=SD_TIMEOUT && per->sd_err!=SD_OK ) {
			say("ERROR: EMMC ACMD41 returned error\n");
			return per->sd_err;
		}
	}
	if(!(r&ACMD41_CMD_COMPLETE) || !cnt ) return SD_TIMEOUT;
	if(!(r&ACMD41_VOLTAGE)) return SD_ERROR;
	if(r&ACMD41_CMD_CCS) ccs = SCR_SUPP_CCS;

	sd_cmd(per, CMD_ALL_SEND_CID,0);

	per->sd_rca = sd_cmd(per, CMD_SEND_REL_ADDR,0);
	say("EMMC: CMD_SEND_REL_ADDR returned %08x,%08x\n", per->sd_rca>>32, per->sd_rca);
	if(per->sd_err) return per->sd_err;

	if((r=sd_clk(per, 25000000))) return r;

	sd_cmd(per, CMD_CARD_SELECT, per->sd_rca);
	if(per->sd_err) return per->sd_err;

	if(sd_status(per, SR_DAT_INHIBIT)) return SD_TIMEOUT;
	*EMMC_BLKSIZECNT = (1<<16) | 8;
	sd_cmd(per, CMD_SEND_SCR, 0);
	if(per->sd_err) return per->sd_err;
	if(sd_int(per, INT_READ_RDY)) return SD_TIMEOUT;

	r = 0;
	cnt = 100000;
	while(r<2 && cnt) {
		if( *EMMC_STATUS & SR_READ_AVAILABLE )per->sd_scr[r++] = *EMMC_DATA;
		else wait_msec(1);
	}
	if(r!=2) return SD_TIMEOUT;
	if(per->sd_scr[0] & SCR_SD_BUS_WIDTH_4) {
		sd_cmd(per, CMD_SET_BUS_WIDTH, per->sd_rca|2);
		if(per->sd_err) return per->sd_err;
		*EMMC_CONTROL0 |= C0_HCTL_DWITDH;
	}
	// add software flag
	say("EMMC: supports ");
	if(per->sd_scr[0] & SCR_SUPP_SET_BLKCNT)say("SET_BLKCNT ");
	if(ccs)say("CCS ");
	say("\n");

	per->sd_scr[0] &= ~SCR_SUPP_CCS;
	per->sd_scr[0] |= ccs;

	dev->ontaking = (void*)sdhci_ontake;
	dev->ongiving = (void*)sdhci_ongive;
	filemanager_registersupplier(dev, 0);
	return SD_OK;
}
