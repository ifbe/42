#include "libhard.h"
void* mmiobase();
int raspi_version();
//
void pinmgr_gpio3439_sdhciold();
void pinmgr_gpio4653_sdhcinew();
//
int mbox_poweron();
int mbox_poweroff();
u32 mbox_getbaseclock();
//
void wait_msec(int);
void wait_cycles(int);
//
void filemanager_registersupplier(void*,void*);




#define COREFREQ (250*1000*1000)
//
#define SD_CLOCK_400K    400000
#define SD_CLOCK_25M   25000000
#define SD_CLOCK_50M   50000000
#define SD_CLOCK_100M 100000000
#define SD_CLOCK_208M 208000000
//
#define EMMC_ARG2           (*(volatile unsigned int*)(sdhci+0x00))
#define EMMC_BLKSIZECNT     (*(volatile unsigned int*)(sdhci+0x04))
#define EMMC_ARG1           (*(volatile unsigned int*)(sdhci+0x08))
#define EMMC_CMDTM          (*(volatile unsigned int*)(sdhci+0x0C))
#define EMMC_RESP0          (*(volatile unsigned int*)(sdhci+0x10))
#define EMMC_RESP1          (*(volatile unsigned int*)(sdhci+0x14))
#define EMMC_RESP2          (*(volatile unsigned int*)(sdhci+0x18))
#define EMMC_RESP3          (*(volatile unsigned int*)(sdhci+0x1C))
#define EMMC_DATA           (*(volatile unsigned int*)(sdhci+0x20))
#define EMMC_STATUS         (*(volatile unsigned int*)(sdhci+0x24))
#define EMMC_CONTROL0       (*(volatile unsigned int*)(sdhci+0x28))
#define EMMC_CONTROL1       (*(volatile unsigned int*)(sdhci+0x2C))
#define EMMC_INTERRUPT      (*(volatile unsigned int*)(sdhci+0x30))
#define EMMC_INT_MASK       (*(volatile unsigned int*)(sdhci+0x34))
#define EMMC_INT_EN         (*(volatile unsigned int*)(sdhci+0x38))
#define EMMC_CONTROL2       (*(volatile unsigned int*)(sdhci+0x3C))
#define EMMC_SLOTISR_VER    (*(volatile unsigned int*)(sdhci+0xFC))
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
	unsigned long vendor;
	unsigned long hciver;
	unsigned long status;

	unsigned long sd_scr[2];
	unsigned long sd_rca;
	unsigned long sd_err;
	void* sdhci;
};




/**
 * Wait for data or command ready
 */
int sd_status(struct persdhci* per, unsigned int mask)
{
	void* sdhci = per->sdhci;
	int cnt = 500000;
	while((EMMC_STATUS & mask) && !(EMMC_INTERRUPT & INT_ERROR_MASK) && cnt--) wait_msec(1);
	return (cnt <= 0 || (EMMC_INTERRUPT & INT_ERROR_MASK)) ? SD_ERROR : SD_OK;
}

/**
 * Wait for interrupt
 */
int sd_int(struct persdhci* per, unsigned int mask)
{
	void* sdhci = per->sdhci;

	unsigned int r, m=mask | INT_ERROR_MASK;
	int cnt = 1000000;
	while(!(EMMC_INTERRUPT & m) && cnt--) wait_msec(1);

	r = EMMC_INTERRUPT;
	if(cnt<=0 || (r & INT_CMD_TIMEOUT) || (r & INT_DATA_TIMEOUT) ) {
		EMMC_INTERRUPT=r;
		return SD_TIMEOUT;
	}
	else if(r & INT_ERROR_MASK) {
		EMMC_INTERRUPT=r;
		return SD_ERROR;
	}
	EMMC_INTERRUPT = mask;
	return 0;
}


int sd_appcmd(struct persdhci* per)
{
	void* sdhci = per->sdhci;
	int ret = 0;
	per->sd_err = SD_OK;

	if(sd_status(per, SR_CMD_INHIBIT)) {
		say("ERROR: EMMC busy 1\n");
		per->sd_err = SD_TIMEOUT;
		return 0;
	}

	u32 code = CMD_APP_CMD | (per->sd_rca?CMD_RSPNS_48:0);
	u32 arg = per->sd_rca;
	say("EMMC: Sending appcmd %x,%x\n",code, arg);

	EMMC_INTERRUPT = EMMC_INTERRUPT;
	EMMC_ARG1 = arg;
	EMMC_CMDTM = code;

	if((ret = sd_int(per, INT_CMD_DONE))) {
		say("error@send app cmd\n");
		per->sd_err = ret;
		return 0;
	}

	if(0 == per->sd_rca)return 0;
	return EMMC_RESP0 & SR_APP_CMD;
}
int sd_cmd(struct persdhci* per, unsigned int code, unsigned int arg)
{
	void* sdhci = per->sdhci;
	int ret = 0;
	per->sd_err = SD_OK;

	if(code&CMD_NEED_APP) {
		ret = sd_appcmd(per);
		if(per->sd_rca && !ret) {
			say("ERROR: failed to send SD APP command\n");
			per->sd_err=SD_ERROR;
			return 0;
		}
		code &= ~CMD_NEED_APP;
	}
	if(sd_status(per, SR_CMD_INHIBIT)) {
		say("ERROR: EMMC busy 2\n");
		per->sd_err = SD_TIMEOUT;
		return 0;
	}

	wait_msec(1000);	//must delay, but why?
	//say("EMMC: Sending mmccmd %x,%x\n",code, arg);

	EMMC_INTERRUPT = EMMC_INTERRUPT;
	EMMC_ARG1 = arg;
	EMMC_CMDTM = code;

	if(code==CMD_SEND_OP_COND) wait_msec(1000);
	else if(code==CMD_SEND_IF_COND) wait_msec(300);

	if((ret = sd_int(per, INT_CMD_DONE))) {
		say("error@send mmc cmd: code=%x,arg=%x\n",code,arg);
		per->sd_err = ret;
		return 0;
	}

	ret = EMMC_RESP0;
	switch(code){
	case CMD_GO_IDLE:
		return 0;
	case CMD_SEND_OP_COND:
		return ret;
	case CMD_SEND_IF_COND:
		return ret==arg? SD_OK : SD_ERROR;
	case CMD_ALL_SEND_CID:
		return ret | EMMC_RESP3| EMMC_RESP2| EMMC_RESP1;
	case CMD_SEND_REL_ADDR:
		per->sd_err=(((ret&0x1fff))|((ret&0x2000)<<6)|((ret&0x4000)<<8)|((ret&0x8000)<<8))&CMD_ERRORS_MASK;
		return ret&CMD_RCA_MASK;
	}

	return ret&CMD_ERRORS_MASK;
}




/**
 * set SD clock to frequency in Hz
 */
int sd_clk(struct persdhci* per, unsigned int f)
{
	void* sdhci = per->sdhci;

	unsigned int d,c=41666666/f,x,s=32,h=0;
	int cnt = 100000;
	while((EMMC_STATUS & (SR_CMD_INHIBIT|SR_DAT_INHIBIT)) && cnt--) wait_msec(1);
	if(cnt <= 0) {
		say("ERROR: timeout waiting for inhibit flag\n");
		return SD_ERROR;
	}

	EMMC_CONTROL1 &= ~C1_CLK_EN;
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

	if(per->hciver > HOST_SPEC_V2) d=c;
	else d=(1<<s);

	if(d<=2) {d=2;s=0;}
	say("EMMC: sd_clk divisor %x, shift %x\n", d, s);

	if(per->hciver > HOST_SPEC_V2) h=(d&0x300)>>2;
	d=(((d&0x0ff) << 8) | h);
	EMMC_CONTROL1 = (EMMC_CONTROL1&0xffff003f)|d;
	wait_msec(10);
	EMMC_CONTROL1 |= C1_CLK_EN;
	wait_msec(10);

	cnt=10000;
	while(!(EMMC_CONTROL1 & C1_CLK_STABLE) && cnt--) wait_msec(10);
	if(cnt <= 0) {
		say("ERROR: failed to get stable clock\n");
		return SD_ERROR;
	}
	return SD_OK;
}
static u32 sd_get_clock_divider(u32 base_clock, u32 target_rate)
{
    // TODO: implement use of preset value registers
    u32 targetted_divisor = 0;
    if(target_rate > base_clock)targetted_divisor = 1;
    else
    {
        targetted_divisor = base_clock / target_rate;
        u32 mod = base_clock % target_rate;
        if(mod)targetted_divisor--;
    }

	// Find the first bit set
	int divisor = -1;
	for(int first_bit = 31; first_bit >= 0; first_bit--)
	{
		u32 bit_test = (1 << first_bit);
		if(targetted_divisor & bit_test)
		{
			divisor = first_bit;
			targetted_divisor &= ~bit_test;
			if(targetted_divisor)
			{
				// The divisor is not a power-of-two, increase it
				divisor++;
			}
			break;
		}
	}

	if(divisor == -1)divisor = 31;
	if(divisor >= 32)divisor = 31;
	if(divisor != 0)divisor = (1 << (divisor - 1));
	if(divisor >= 0x400)divisor = 0x3ff;

	u32 freq_select = divisor & 0xff;
	u32 upper_bits = (divisor >> 8) & 0x3;
	u32 ret = (freq_select << 8) | (upper_bits << 6) | (0 << 5);

	int denominator = 1;
	if(divisor != 0)denominator = divisor * 2;
	int actual_clock = base_clock / denominator;
	say("EMMC: base_clock: %d, target_rate: %d, divisor: %08x, "
			"actual_clock: %d, ret: %08x\n", base_clock, target_rate,
			divisor, actual_clock, ret);

	return ret;
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

	//say("EMMC: sd_readblock lba %x,%x\n", lba, num);
	if(sd_status(per, SR_DAT_INHIBIT)) {per->sd_err=SD_TIMEOUT; return 0;}

	unsigned int* tmp = (unsigned int*)buf;
	if(per->sd_scr[0] & SCR_SUPP_CCS) {
		if(num > 1 && (per->sd_scr[0] & SCR_SUPP_SET_BLKCNT)) {
			sd_cmd(per, CMD_SET_BLOCKCNT,num);
			if(per->sd_err) return 0;
		}
		EMMC_BLKSIZECNT = (num << 16) | 512;
		sd_cmd(per, num == 1 ? CMD_READ_SINGLE : CMD_READ_MULTI, lba);
		if(per->sd_err) return 0;
	} else {
		EMMC_BLKSIZECNT = (1 << 16) | 512;
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
		for(d=0;d<128;d++)tmp[d] = EMMC_DATA;
		c++;
		tmp += 128;
	}
	if( num > 1 && !(per->sd_scr[0] & SCR_SUPP_SET_BLKCNT) && (per->sd_scr[0] & SCR_SUPP_CCS)) sd_cmd(per, CMD_STOP_TRANS, 0);
	return per->sd_err!=SD_OK || c!=num? 0 : num*512;
}




static int sdhci_ontake(struct item* dev, void* foot, void* stack, int sp, void* arg, int idx, void* buf, int len)
{
	struct persdhci* per = (void*)dev->priv_256b;
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
int initsdhci(struct item* dev, int offs)
{
	long r,cnt,ccs=0;
	say("@initsdhci@%x\n", offs);

	//which
	void* sdhci = mmiobase() + offs;
	printmmio(sdhci, 0x40);

	//save addr in node
	struct persdhci* per = (void*)dev->priv_256b;
	per->sdhci = sdhci;


	//read information
	u32 tmp = EMMC_SLOTISR_VER;
	per->vendor = tmp >> 24;
	per->hciver =(tmp >> 16) & 0xff;
	per->status = tmp & 0xff;
	say("EMMC: vendor=%x, hciver=%x, status %x\n", per->vendor, per->hciver, per->status);


	// Reset the controller
	say("EMMC: reset hc\n");
	u32 control1 = EMMC_CONTROL1;
	control1 |= (1 << 24);
	control1 &= ~(1 << 2);
	control1 &= ~(1 << 0);
	EMMC_CONTROL1 = control1;

	cnt = 1000000;
	while(cnt--){
		if(0 == (EMMC_CONTROL1 & (0x7 << 24)))break;
		wait_msec(1);
	}
	if(cnt <= 0) {
		say("ERROR: failed to reset EMMC\n");
		return SD_ERROR;
	}
	say("ctl0: %08x, ctl1: %08x, ctl2: %08x\n", EMMC_CONTROL0, EMMC_CONTROL1, EMMC_CONTROL2);
	say("EMMC: reset ok\n");


	// Enable SD Bus Power VDD1 at 3.3V
	if(raspi_version() >= 4){
		say("EMMC: vdd = 3.3v\n");
		u32 control0 = EMMC_CONTROL0;
		control0 |= (0x0F << 8);
		EMMC_CONTROL0 = control0;
		wait_msec(2);
		say("EMMC: vdd ok\n");
	}

/*
	// Read the capabilities registers
	u32 capabilities_0 = *EMMC_CAPABILITIES_0;
	u32 capabilities_1 = *EMMC_CAPABILITIES_1;
	say("EMMC: capabilities: %08x%08x\n", capabilities_1, capabilities_0);
*/

	// Check for a valid card
	say("EMMC: checking for an inserted card\n");
	cnt = 1000000;
	while(cnt--){
		if(0 != (EMMC_STATUS & (1 << 16)))break;
		wait_msec(1);
	}
	if(cnt <= 0){
		say("no card inserted\n");
	}
	say("EMMC: status: %08x\n", EMMC_STATUS);


	// Clear control2
	EMMC_CONTROL2 = 0;

	// Get the base clock rate
	u32 baseclock = mbox_getbaseclock();
	say("baseclock_frommbox=%d\n",baseclock);

	// Set clock rate to something slow
	control1 = EMMC_CONTROL1;
	control1 |= 1;			// enable clock

	// Set to identification frequency (400 kHz)
	u32 f_id = sd_get_clock_divider(baseclock, SD_CLOCK_400K);
	if(f_id == 0xffffffff){
		say("EMMC: unable to get a valid clock divider for ID frequency\n");
		return -1;
	}
	control1 |= f_id;

	control1 &= ~(0xF << 16);
	control1 |= (11 << 16);         // data timeout = TMCLK * 2^24
	EMMC_CONTROL1 = control1;

	cnt = 1000000;
	while(cnt--){
		if(0 != (EMMC_CONTROL1 & 0x2))break;
		wait_msec(1);
	}
	if(cnt <= 0){
		say("EMMC: controller's clock did not stabilise within 1 second\n");
		//return -1;
	}
	say("EMMC: ctl0: %08x, ctl1: %08x\n", EMMC_CONTROL0, EMMC_CONTROL1);


	// Enable the SD clock
	say("EMMC: SD clock en\n");
	wait_msec(2);
	control1 = EMMC_CONTROL1;
	control1 |= 4;
	EMMC_CONTROL1 = control1;
	wait_msec(2);
	say("EMMC: SD clock ok\n");

/*
	// Mask off sending interrupts to the ARM
	*EMMC_INT_EN = 0;
	// Reset interrupts
	mmio_write(emmc_base + EMMC_INTERRUPT, 0xffffffff);
	// Have all interrupts sent to the INTERRUPT register
	u32 irpt_mask = 0xffffffff & (~SD_CARD_INTERRUPT);
    irpt_mask |= SD_CARD_INTERRUPT;
	mmio_write(emmc_base + EMMC_IRPT_MASK, irpt_mask);

	say("EMMC: interrupts disabled\n");
*/
	EMMC_INT_EN   = 0xffffffff;
	EMMC_INT_MASK = 0xffffffff;
	wait_msec(2);


	per->sd_scr[0] = 0;
	per->sd_scr[1] = 0;
	per->sd_rca = 0;
	per->sd_err = 0;

	//CMD0: reset to idle state
	say("CMD0 ??\n");
	sd_cmd(per, CMD_GO_IDLE, 0);
	if(per->sd_err) return per->sd_err;
	say("CMD0 ok\n");

	//CMD8: check if it is SD V2
	say("CMD8 ??\n");
	sd_cmd(per, CMD_SEND_IF_COND, 0x000001AA);
	if(per->sd_err) return per->sd_err;
	say("CMD8 ok: rsp0=%x\n", EMMC_RESP0);

	//CMD5: check if it is sdio device

	//ACMD41
	say("APPCMD41 inquery ??\n");
	r = sd_cmd(per, 0x29000000|CMD_NEED_APP, 0);
	say("APPCMD41 inquery ok\n");

	cnt = 6;
	while(cnt--){
		r = sd_cmd(per, CMD_SEND_OP_COND, ACMD41_ARG_HC);
		say("EMMC: CMD_SEND_OP_COND ret=%x,err=%x\n", r, per->sd_err);

		if(r&ACMD41_CMD_CCS) ccs = SCR_SUPP_CCS;
		if(r&ACMD41_CMD_COMPLETE)break;
		if(cnt <= 0)break;
	}
/*
	cnt = 6;
	r=0;
	while(!(r&ACMD41_CMD_COMPLETE) && cnt--) {
		wait_cycles(400);

		r = sd_cmd(per, CMD_SEND_OP_COND, ACMD41_ARG_HC);
		say("EMMC: CMD_SEND_OP_COND ret=%x,err=%x\n", r, per->sd_err);

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
*/

	say("CMD_ALL_SEND_CID ??\n");
	sd_cmd(per, CMD_ALL_SEND_CID,0);
	say("CMD_ALL_SEND_CID ok\n");


	per->sd_rca = sd_cmd(per, CMD_SEND_REL_ADDR,0);
	say("EMMC: CMD_SEND_REL_ADDR returned %08x,%08x\n", per->sd_rca>>32, per->sd_rca);
	if(per->sd_err) return per->sd_err;


	if((r=sd_clk(per, 25000000))) return r;

	sd_cmd(per, CMD_CARD_SELECT, per->sd_rca);
	if(per->sd_err) return per->sd_err;

	if(sd_status(per, SR_DAT_INHIBIT)) return SD_TIMEOUT;
	EMMC_BLKSIZECNT = (1<<16) | 8;
	sd_cmd(per, CMD_SEND_SCR, 0);
	if(per->sd_err) return per->sd_err;
	if(sd_int(per, INT_READ_RDY)) return SD_TIMEOUT;

	r = 0;
	cnt = 100000;
	while(r<2 && cnt) {
		if(EMMC_STATUS & SR_READ_AVAILABLE )per->sd_scr[r++] = EMMC_DATA;
		else wait_msec(1);
	}
	if(r!=2) return SD_TIMEOUT;
	if(per->sd_scr[0] & SCR_SD_BUS_WIDTH_4) {
		sd_cmd(per, CMD_SET_BUS_WIDTH, per->sd_rca|2);
		if(per->sd_err) return per->sd_err;
		EMMC_CONTROL0 |= C0_HCTL_DWITDH;
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




int initsdhci_wifi(struct item* dev, int offs)
{
	say("@initsdhci_wifi@%x\n", offs);
	pinmgr_gpio3439_sdhciold();
	return 0;
}
int initsdhci_sdcard(struct item* dev, int offs)
{
	say("@initsdhci_sdcard@%x\n", offs);

	//gpio
	say("EMMC: gpio[46,53]\n");
	pinmgr_gpio4653_sdhcinew();
	say("EMMC: gpio ok\n");

	//power
	say("EMMC: power ??\n");
	mbox_poweroff();
	wait_msec(2);
	mbox_poweron();
	wait_msec(200);
	say("EMMC: power ok\n");

	//sdhci
	initsdhci(dev, offs);
	return 0;
}