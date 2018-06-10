#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define __kbd__ hex32('k','b','d',0)
#define __char__ hex32('c','h','a','r')
u8 in8(u16 port);
void out8(u16 port, u8 data);
void enableirq(int);
//
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




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




void isr_8042()
{
	u8 ch;
	ch = in8(0x64);
	if((ch&1) != 1)return;
	ch = in8(0x60);
	say("%02x\n",ch);
	if(ch == 0xe0)
	{
		ch = in8(0x60);
		//say("%02x\n",ch);
		if(ch >= 0x80)return;
		eventwrite(ch, __kbd__, 0, 0);
		return;
	}
	if(ch >= 0x80)return;
	//say("%02x,%c\n",ch,keymap[ch]);
	if((ch>=0x3b)&&(ch<=0x44))eventwrite(ch-0x3b+0xf1, __kbd__, 0, 0);
	if((ch>=0x57)&&(ch<=0x58))eventwrite(ch-0x57+0xfb, __kbd__, 0, 0);
	else eventwrite(keymap[ch], __char__, 0, 0);
}
void init8042()
{
	u32* addr;
	say("@init8042\n");
/*
	addr = (u32*)(u64)(0xfec00000);
	addr[0] = 0x10 + (2*1);
	addr[4] = 0x21;
	addr[0] = 0x11 + (2*1);
	addr[4] = 0;
*/
	enableirq(1);
}
