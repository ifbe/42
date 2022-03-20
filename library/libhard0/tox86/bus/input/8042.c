#include "libhard.h"
int acpi_have8042();
//
void interruptinstall_bsp(int num, u64 isr);
void enableirq(int);
void endofextirq(int);
//
u8 in8(u16 port);
void out8(u16 port, u8 data);



static int enablepolling = 1;
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




static u8 kbd[9*2] =
{
	0x01,0x1b,      //esc
	0x47,0x47,      //home
	0x4f,0x4f,      //end
	0x49,0x49,      //page up
	0x51,0x51,      //page down
	0x48,0x48,      //up
	0x50,0x50,      //down
	0x4b,0x4b,      //left
	0x4d,0x4d       //right
};
static u8 ch[41*2] =
{
0x02,'1',
0x03,'2',
0x04,'3',
0x05,'4',
0x06,'5',
0x07,'6',
0x08,'7',
0x09,'8',
0x0a,'9',
0x0b,'0',
0x1e,'a',
0x30,'b',
0x2e,'c',
0x20,'d',
0x12,'e',
0x21,'f',
0x22,'g',
0x23,'h',
0x17,'i',
0x24,'j',
0x25,'k',
0x26,'l',
0x32,'m',
0x31,'n',
0x18,'o',
0x19,'p',
0x10,'q',
0x13,'r',
0x1f,'s',
0x14,'t',
0x16,'u',
0x2f,'v',
0x11,'w',
0x2d,'x',
0x15,'y',
0x2c,'z',
0x34,'.',
0x0e,0x7f,      //backspace
0x1c,0xd,       //enter
0x39,0x20,      //space
0x00,0x00
};
int convert8042(u8* buf, struct event* ev)
{
	int j;
	//kbd
	for(j=0;j<9;j++)
	{
		if(buf[0] == kbd[j*2])
		{
			ev->what = _kbd_;
			ev->why = kbd[(j*2) + 1];
			return 1;
		}
	}

	//char
	for(j=0;j<41;j++)
	{
		if(buf[0] == ch[j*2])
		{
			ev->what = _char_;
			ev->why = ch[(j*2) + 1];
			return 2;
		}
	}
	return 0;
}




void* read8042(struct event* ev)
{
	int j;
	u8 buf[1];
	if(0 == enablepolling)return 0;

	j = readkbd(buf, 1);
	if(j == 0)return 0;
	if(buf[0] >= 0x80)return 0;

	convert8042(buf, ev);

	return ev;
}
void init8042()
{
	say("@init8042\n");
	if(0 == acpi_have8042())enablepolling = 0;
	say("enablepoll=%d\n\n", enablepolling);
}




static u8 keymap[0x80] = {
0x1b,   0, '1', '2', '3', '4', '5', '6',	//[00,07]
 '7', '8', '9', '0', '-', '=', 0x8, 0x9,	//[08,0f]
 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',	//[10,17]
 'o', 'p', '[', ']', 0xd,   0, 'a', 's',	//[18,1f]
 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	//[20,27]
'\'', '`',   0,'\\', 'z', 'x', 'c', 'v',	//[28,2f]
 'b', 'n', 'm', ',', '.', '/',   0,   0,	//[30,37]
   0, ' '
};
__attribute__((interrupt)) static void ps2kbd_isr(void* p)
{
	int j;
	u8 buf[1];

	buf[0] = in8(0x60);
	if(buf[0] >= 0x80)goto byebye;

	struct event ev;
	convert8042(buf, &ev);

	eventwrite(ev.why, ev.what, 0, 0);

byebye:
	endofextirq(1);
}
void initps2kbd()
{
	say("@initps2kbd\n");
	enablepolling = 0;

	interruptinstall_bsp(1, (u64)ps2kbd_isr);
	enableirq(1);
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
	//say("data0=%x\n", data);

	endofextirq(12);
}
void initps2mouse()
{
	say("@initps2mouse\n");

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
	interruptinstall_bsp(12, (u64)ps2mouse_isr);
	enableirq(12);
}