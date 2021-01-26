#include "libuser.h"
int wndmgr_take(void*,void*, void*,int, void*,int, void*,int);
int wndmgr_give(void*,void*, void*,int, void*,int, void*,int);
//
int mbox_call(unsigned char ch);




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

volatile unsigned int mbox[36];




static int width = 0;
static int height = 0;
static int pitch = 0;
static int isrgb = 0;
static unsigned char *lfb = 0;




void window_take(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("wnd=%p,stack=%p\n",wnd,stack);
	wndmgr_take(wnd,foot, stack,sp, arg,key, buf,len);

	int j;
	u32* ibuf = wnd->rgbabuf;
	u32* obuf = (u32*)lfb;
	for(j=0;j<width*height;j++)
	{
		obuf = (void*)obuf + 4;
		*obuf = ibuf[j];
	}
}
void window_give(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//printmemory(buf, 0x20);
	wndmgr_give(wnd,0, stack,sp, 0,0, buf,len);
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct supply* w)
{
}
void windowcreate(struct supply* wnd)
{
	wnd->fmt = _rgba_;
	wnd->vfmt = hex64('r', 'g', 'b', 'a', '8', '8', '8', '8');

	wnd->width = width;
	wnd->height = height;

	wnd->fbwidth = pitch;
	//wnd->fbheight = 0;

	wnd->rgbabuf = (void*)0x4000000;
}




void freewindow()
{
}
void initwindow()
{
	mbox[0] = 35*4;
	mbox[1] = MBOX_REQUEST;

	mbox[2] = 0x48003;	//set phy wh
	mbox[3] = 8;
	mbox[4] = 8;
	mbox[5] = 1920;		 //FrameBufferInfo.width
	mbox[6] = 1080;		 //FrameBufferInfo.height

	mbox[7] = 0x48004;	//set virt wh
	mbox[8] = 8;
	mbox[9] = 8;
	mbox[10] = 1920;	//FrameBufferInfo.virtual_width
	mbox[11] = 1080;	//FrameBufferInfo.virtual_height

	mbox[12] = 0x48009;	//set virt offset
	mbox[13] = 8;
	mbox[14] = 8;
	mbox[15] = 0;		//FrameBufferInfo.x_offset
	mbox[16] = 0;

	mbox[17] = 0x48005;	//set depth
	mbox[18] = 4;
	mbox[19] = 4;
	mbox[20] = 32;		//FrameBufferInfo.depth

	mbox[21] = 0x48006;	//set pixel order
	mbox[22] = 4;
	mbox[23] = 4;
	mbox[24] = 1;		//RGB, not BGR preferably

	mbox[25] = 0x40001;	//get framebuffer, gets alignment on request
	mbox[26] = 8;
	mbox[27] = 8;
	mbox[28] = 0;		//FrameBufferInfo.pointer
	mbox[29] = 0;		//FrameBufferInfo.size

	mbox[30] = 0x40008;	//get pitch
	mbox[31] = 4;
	mbox[32] = 4;
	mbox[33] = 0;		//FrameBufferInfo.pitch

	mbox[34] = MBOX_TAG_LAST;

	//this might not return exactly what we asked for, could be
	//the closest supported resolution instead
	if(mbox_call(MBOX_CH_PROP) && mbox[20]==32 && mbox[28]!=0) {
		mbox[28] &= 0x3FFFFFFF;		//convert GPU address to ARM address
		width = mbox[ 5];		//get actual physical width
		height= mbox[ 6];		//get actual physical height
		pitch = mbox[33];		//get number of bytes per line
		isrgb = mbox[24];		//get the actual channel order
		lfb = (void*)((unsigned long)mbox[28]);

		say("%d,%d,%d,%d,%llx\n",width,height,pitch,isrgb,lfb);
	}
	else {
		say("error@initwindow\n");
	}
}
