#include "libhard.h"
int acpi_have8042();
//
int percpu_enableint(int apicid, int intvec, void* isr, int flag);
//
void irqchip_enableirq(int chip, int pin, int apicid, int intvec);
void irqchip_disableirq(int chip, int pin);
void irqchip_endofirq(int irq);
//
u8 in8(u16 port);
void out8(u16 port, u8 data);




static int enablepolling = 1;
//0x01,0x1b,      //esc
//0x47,0x47,      //home
//0x4f,0x4f,      //end
//0x49,0x49,      //page up
//0x51,0x51,      //page down
//0x48,0x48,      //up
//0x50,0x50,      //down
//0x4b,0x4b,      //left
//0x4d,0x4d       //right
static u8 kbdmap[0x80] = {
   0,0x1b,   0,   0,   0,   0,   0,   0,	//[00,07]
   0,   0,   0,   0,   0,   0,   0,   0,	//[08,0f]
   0,   0,   0,   0,   0,   0,   0,   0,	//[10,17]
   0,   0,   0,   0,   0,   0,   0,   0,	//[18,1f]
   0,   0,   0,   0,   0,   0,   0,   0,	//[20,27]
   0,   0,   0,   0,   0,   0,   0,   0,	//[28,2f]
   0,   0,   0,   0,   0,   0,   0,   0,	//[30,37]
   0,   0,   0,   0,   0,   0,   0,   0,	//[38,3f]
   0,   0,   0,   0,   0,   0,   0,0x47,	//[40,47]
0x48,0x49,   0,0x4b,   0,0x4d,   0,0x4f,	//[48,4f]
0x50,0x51,   0,   0,   0,   0,   0,   0,	//[50,57]
};
//0x0e,0x7f,      //backspace: 0x8 or 0x7f
//0x1c,0xd,       //enter: 0xa or 0xd
static u8 chrmap[0x80] = {
0x1b,   0, '1', '2', '3', '4', '5', '6',	//[00,07]
 '7', '8', '9', '0', '-', '=',0x7f, 0x9,	//[08,0f]
 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',	//[10,17]
 'o', 'p', '[', ']', 0xd,   0, 'a', 's',	//[18,1f]
 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	//[20,27]
'\'', '`',   0,'\\', 'z', 'x', 'c', 'v',	//[28,2f]
 'b', 'n', 'm', ',', '.', '/',   0,   0,	//[30,37]
   0, ' '
};
int convert8042(u8* buf, struct event* ev)
{
	int key = buf[0];

	//kbd
	if(kbdmap[key]){
		ev->what = _kbd_;
		ev->why = kbdmap[key];
		return 1;
	}

	//char
	if(chrmap[key]){
		ev->what = _char_;
		ev->why = chrmap[key];
		return 2;
	}

	return 0;
}




int kbdled(u8 on)
{
	//bit0=scrolllock, bit1=numlock, bit2=capslock
	out8(0x60, 0xed);
	out8(0x60, on ? 7 : 0);
	return 0;
}




int readkbd_one(u8* buf)
{
	u8 flag = in8(0x64);
	if( (flag&1) == 0)return 0;

	buf[0] = in8(0x60);
	return 1;
}
int readkbd(u8* buf, int len)
{
	int j,ret;
	for(j=0;j<len;j++)
	{
		ret = readkbd_one(buf+j);
		if(ret == 0)break;
	}
	return j;
}




void* read8042(struct event* ev)
{
	if(0 == enablepolling)return 0;

	u8 buf[1];
	int j = readkbd(buf, 1);
	if(j == 0)return 0;
	if(buf[0] >= 0x80)return 0;

	if(convert8042(buf, ev)){
		return ev;
	}

	return 0;
}
void init8042()
{
	logtoall("@init8042\n");
	if(0 == acpi_have8042()){
		enablepolling = 0;
		logtoall("no 8042 found, enablepoll=%d\n", enablepolling);
	}

	logtoall("\n");
}




__attribute__((interrupt)) static void ps2kbd_isr(void* p)
{
	int j;
	u8 buf[1];

	buf[0] = in8(0x60);
	if(buf[0] >= 0x80)goto byebye;

	struct event ev;
	if(convert8042(buf, &ev)){
		eventwrite(ev.why, ev.what, 0, 0);
	}

byebye:
	irqchip_endofirq(1);
}
void initps2kbd()
{
	logtoall("@initps2kbd\n");
	enablepolling = 0;

	percpu_enableint(0, 0x21, ps2kbd_isr, 0);
	irqchip_enableirq(0,1, 0,0x21);
}




void mouse_write(u8 a_write) //unsigned char
{
	//Wait to be able to send a command
	//sleep_us(1000);
	//Tell the mouse we are sending a command
	out8(0x64, 0xD4);

	//Wait for the final part
	//sleep_us(1000);
	//Finally write
	out8(0x60, a_write);
}
u8 mouse_read()
{
	//Get's response from mouse
	//sleep_us(1000);
	return in8(0x60);
}
__attribute__((interrupt)) static void ps2mouse_isr(void* p)
{
	u8 data = in8(0x60);
	logtoall("data0=%x\n", data);

	irqchip_endofirq(12);
}
void initps2mouse()
{
	logtoall("@initps2mouse\n");

	u8 _status;	//unsigned char

	//Enable the auxiliary mouse device
	sleep_us(1000);
	out8(0x64, 0xA8);

	//Enable the interrupts
	sleep_us(1000);
	out8(0x64, 0x20);
	sleep_us(1000);
	_status=(in8(0x60) | 2);
	sleep_us(1000);
	out8(0x64, 0x60);
	sleep_us(1000);
	out8(0x60, _status);

	//Tell the mouse to use default settings
	mouse_write(0xF6);
	mouse_read();	//Acknowledge

	//Enable the mouse
	mouse_write(0xF4);
	mouse_read();	//Acknowledge

	//Setup the mouse handler
	percpu_enableint(0, 0x20+12, ps2mouse_isr, 0);
	irqchip_enableirq(0,12, 0,0x20+12);
}
