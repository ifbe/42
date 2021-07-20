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

extern unsigned int maildata[36];




static int width = 0;
static int height = 0;
static int pitch = 0;
static int isrgb = 0;
static unsigned char *lfb = 0;




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
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
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
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
void windowdelete(_obj* w)
{
}
void windowcreate(_obj* wnd)
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
	//must do: add these to config.txt
	//hdmi_mode:1=16
	//hdmi_group:1=1
	//hdmi_mode:0=16
	//hdmi_group:0=1

	maildata[0] = 35*4;
	maildata[1] = MBOX_REQUEST;

	maildata[2] = 0x48003;	//set phy wh
	maildata[3] = 8;
	maildata[4] = 8;
	maildata[5] = 1920;		 //FrameBufferInfo.width
	maildata[6] = 1080;		 //FrameBufferInfo.height

	maildata[7] = 0x48004;	//set virt wh
	maildata[8] = 8;
	maildata[9] = 8;
	maildata[10] = 1920;	//FrameBufferInfo.virtual_width
	maildata[11] = 1080;	//FrameBufferInfo.virtual_height

	maildata[12] = 0x48009;	//set virt offset
	maildata[13] = 8;
	maildata[14] = 8;
	maildata[15] = 0;		//FrameBufferInfo.x_offset
	maildata[16] = 0;

	maildata[17] = 0x48005;	//set depth
	maildata[18] = 4;
	maildata[19] = 4;
	maildata[20] = 32;		//FrameBufferInfo.depth

	maildata[21] = 0x48006;	//set pixel order
	maildata[22] = 4;
	maildata[23] = 4;
	maildata[24] = 1;		//RGB, not BGR preferably

	maildata[25] = 0x40001;	//get framebuffer, gets alignment on request
	maildata[26] = 8;
	maildata[27] = 8;
	maildata[28] = 0;		//FrameBufferInfo.pointer
	maildata[29] = 0;		//FrameBufferInfo.size

	maildata[30] = 0x40008;	//get pitch
	maildata[31] = 4;
	maildata[32] = 4;
	maildata[33] = 0;		//FrameBufferInfo.pitch

	maildata[34] = MBOX_TAG_LAST;

	//this might not return exactly what we asked for, could be
	//the closest supported resolution instead
	if(mbox_call(MBOX_CH_PROP) && maildata[20]==32 && maildata[28]!=0) {
		maildata[28] &= 0x3FFFFFFF;		//convert GPU address to ARM address
		width = maildata[ 5];		//get actual physical width
		height= maildata[ 6];		//get actual physical height
		pitch = maildata[33];		//get number of bytes per line
		isrgb = maildata[24];		//get the actual channel order
		lfb = (void*)((unsigned long)maildata[28]);

		say("%d,%d,%d,%d,%llx\n",width,height,pitch,isrgb,lfb);
	}
	else {
		say("error@initwindow\n");
	}
}
