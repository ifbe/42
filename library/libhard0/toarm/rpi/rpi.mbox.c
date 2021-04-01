#include "libhard.h"
void* mmiobase();




#define VIDEOCORE_MBOX  (mmio+0x0000B880)
#define MBOX_READ       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x0))
#define MBOX_POLL       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x10))
#define MBOX_SENDER     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x14))
#define MBOX_STATUS     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x18))
#define MBOX_CONFIG     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x1C))
#define MBOX_WRITE      ((volatile unsigned int*)(VIDEOCORE_MBOX+0x20))
//
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000
#define MBOX_REQUEST    0
#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8
#define MBOX_TAG_SETPOWER       0x28001
#define MBOX_TAG_SETCLKRATE     0x38002
#define MBOX_TAG_LAST           0




volatile unsigned int  __attribute__((aligned(16))) mbox[36];
int mbox_call(unsigned char ch)
{
	u8* mmio = mmiobase();

	unsigned int r = (((unsigned int)((unsigned long)&mbox)&~0xF) | (ch&0xF));

	/* wait until we can write to the mailbox */
	do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_FULL);

	/* write the address of our message to the mailbox with channel identifier */
	*MBOX_WRITE = r;

	/* now wait for the response */
	while(1) {
		/* is there a response? */
		do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_EMPTY);

		/* is it a response to our message? */
		if(r == *MBOX_READ)

		/* is it a valid successful response? */
		return mbox[1]==MBOX_RESPONSE;
	}
	return 0;
}
u32 mbox_getbaseclock()
{
    u32 base_clock;

	/* Get the base clock rate */
	// set up the buffer
	mbox[0] = 8 * 4;          // size of this message
	mbox[1] = 0;              // this is a request
	mbox[2] = 0x00030002;     // get clock rate tag
	mbox[3] = 0x8;            // value buffer size
	mbox[4] = 0x4;            // is a request, value length = 4
	mbox[5] = 0x1;            // clock id + space to return clock id
	mbox[6] = 0;              // space to return rate (in Hz)
	mbox[7] = 0;	          // closing tag

	// send the message
	mbox_call(MBOX_CH_PROP);

	if(mbox[1] != MBOX_RESPONSE)
	{
		say("EMMC: property mailbox did not return a valid response.\n");
		return 0;
	}

	if(mbox[5] != 0x1)
	{
		say("EMMC: property mailbox did not return a valid clock id.\n");
		return 0;
	}

	base_clock = mbox[6];
    return base_clock;
}