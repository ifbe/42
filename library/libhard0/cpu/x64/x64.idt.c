#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define idthome 0x5000
void isr_rtc();
void isr_8042();
//
u8 in8(u16 port);
void out8(u16 port, u8 data);
//
void eventwrite(u64,u64,u64,u64);
void lidt(void* buf, int len);
void say(void*, ...);



struct idt_entry{
	u16 byte01;
	u16 select;
	u8 zero;
	u8 attr;
	u16 byte23;
	u32 byte47;
	u32 what;
};




__attribute__((interrupt)) static void isr_00(void* p){say("int00!\n");}
__attribute__((interrupt)) static void isr_01(void* p){say("int01!\n");}
__attribute__((interrupt)) static void isr_02(void* p){say("int02!\n");}
__attribute__((interrupt)) static void isr_03(void* p){say("int03!\n");}
__attribute__((interrupt)) static void isr_04(void* p){say("int04!\n");}
__attribute__((interrupt)) static void isr_05(void* p){say("int05!\n");}
__attribute__((interrupt)) static void isr_06(void* p){say("int06!\n");}
__attribute__((interrupt)) static void isr_07(void* p){say("int07!\n");}
__attribute__((interrupt)) static void isr_08(void* p){say("int08!\n");}
__attribute__((interrupt)) static void isr_09(void* p){say("int09!\n");}
__attribute__((interrupt)) static void isr_0a(void* p){say("int0a!\n");}
__attribute__((interrupt)) static void isr_0b(void* p){say("int0b!\n");}
__attribute__((interrupt)) static void isr_0c(void* p){say("int0c!\n");}
__attribute__((interrupt)) static void isr_0d(void* p){say("int0d!\n");}
__attribute__((interrupt)) static void isr_0e(void* p){say("int0e!\n");}
__attribute__((interrupt)) static void isr_0f(void* p){say("int0f!\n");}
__attribute__((interrupt)) static void isr_10(void* p){say("int10!\n");}
__attribute__((interrupt)) static void isr_11(void* p){say("int11!\n");}
__attribute__((interrupt)) static void isr_12(void* p){say("int12!\n");}
__attribute__((interrupt)) static void isr_13(void* p){say("int13!\n");}
__attribute__((interrupt)) static void isr_14(void* p){say("int14!\n");}
__attribute__((interrupt)) static void isr_15(void* p){say("int15!\n");}
__attribute__((interrupt)) static void isr_16(void* p){say("int16!\n");}
__attribute__((interrupt)) static void isr_17(void* p){say("int17!\n");}
__attribute__((interrupt)) static void isr_18(void* p){say("int18!\n");}
__attribute__((interrupt)) static void isr_19(void* p){say("int19!\n");}
__attribute__((interrupt)) static void isr_1a(void* p){say("int1a!\n");}
__attribute__((interrupt)) static void isr_1b(void* p){say("int1b!\n");}
__attribute__((interrupt)) static void isr_1c(void* p){say("int1c!\n");}
__attribute__((interrupt)) static void isr_1d(void* p){say("int1d!\n");}
__attribute__((interrupt)) static void isr_1e(void* p){say("int1e!\n");}
__attribute__((interrupt)) static void isr_1f(void* p){say("int1f!\n");}
__attribute__((interrupt)) static void isr_20(void* p)
{
	say("rtc!\n");
	*(u32*)(u64)(0xfee000b0) = 0;
}
__attribute__((interrupt)) static void isr_21(void* p)
{
	//say("kbd!\n");

	isr_8042();

	out8(0x20, 0x20);
	//*(u32*)(u64)(0xfee000b0) = 0;
}




void interruptinstall(int num, u64 isr)
{
	u64 temp = idthome + num*16;
	struct idt_entry* addr = (void*)temp;

	addr->byte01 = isr&0xffff;
	addr->select = 0x8;
	addr->zero = 0x0;
	addr->attr = 0x8e;
	addr->byte23 = (isr>>16)&0xffff;
	addr->byte47 = (isr>>32)&0xffffffff;
	addr->what = 0;
}
void initidt()
{
	int j;
	u64 temp = idthome;
	u8* addr = (void*)temp;
	say("@initidt\n");

	asm("cli");
	for(j=0;j<0x1000;j++)addr[j] = 0;

	//exception
	interruptinstall(0x00, (u64)isr_00);
	interruptinstall(0x01, (u64)isr_01);
	interruptinstall(0x02, (u64)isr_02);
	interruptinstall(0x03, (u64)isr_03);
	interruptinstall(0x04, (u64)isr_04);
	interruptinstall(0x05, (u64)isr_05);
	interruptinstall(0x06, (u64)isr_06);
	interruptinstall(0x07, (u64)isr_07);
	interruptinstall(0x08, (u64)isr_08);
	interruptinstall(0x09, (u64)isr_09);
	interruptinstall(0x0a, (u64)isr_0a);
	interruptinstall(0x0b, (u64)isr_0b);
	interruptinstall(0x0c, (u64)isr_0c);
	interruptinstall(0x0d, (u64)isr_0d);
	interruptinstall(0x0e, (u64)isr_0e);
	interruptinstall(0x0f, (u64)isr_0f);
	interruptinstall(0x10, (u64)isr_10);
	interruptinstall(0x11, (u64)isr_11);
	interruptinstall(0x12, (u64)isr_12);
	interruptinstall(0x13, (u64)isr_13);
	interruptinstall(0x14, (u64)isr_14);
	interruptinstall(0x15, (u64)isr_15);
	interruptinstall(0x16, (u64)isr_16);
	interruptinstall(0x17, (u64)isr_17);
	interruptinstall(0x18, (u64)isr_18);
	interruptinstall(0x19, (u64)isr_19);
	interruptinstall(0x1a, (u64)isr_1a);
	interruptinstall(0x1b, (u64)isr_1b);
	interruptinstall(0x1c, (u64)isr_1c);
	interruptinstall(0x1d, (u64)isr_1d);
	interruptinstall(0x1e, (u64)isr_1e);
	interruptinstall(0x1f, (u64)isr_1f);

	//interrupt
	interruptinstall(0x20, (u64)isr_20);
	interruptinstall(0x21, (u64)isr_21);

	lidt(addr, 0xfff);
	asm("int3");
	asm("sti");
}
