#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define idthome (u64)0x5000
void isr_825x();
void isr_8042();
void isr_rtc();
//
u64 getisr03();
u64 getisr20();
u64 getisr28();
u64 getisr80();
//
u8 in8(u16 port);
void out8(u16 port, u8 data);
void endofirq(int);
void setidt(void* buf, int len);
//
void eventwrite(u64,u64,u64,u64);
void printmemory(void*, int);
void say(void*, ...);



struct idt_entry{
	u16 byte01;
	u16 select;
	u8 zero;
	u8 type;
	u16 byte23;
	u32 byte47;
	u32 what;
}__attribute__((packed));
struct int_frame{
	u64 ip;
	u64 cs;
	u64 flag;
	u64 sp;
	u64 ss;
}__attribute__((packed));




__attribute__((interrupt)) static void isr_00(void* p){
	say("int00!\n");
	while(1);
}//Divide-by-zero
__attribute__((interrupt)) static void isr_01(void* p){
	say("int01!\n");
	while(1);
}//Debug
__attribute__((interrupt)) static void isr_02(void* p){
	say("int02!\n");
	while(1);
}//Non-maskable Interrupt
__attribute__((interrupt)) static void isr_03(struct int_frame* p){
	say("int03: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
}//Breakpoint
__attribute__((interrupt)) static void isr_04(void* p){
	say("int04!\n");
	while(1);
}//Overflow
__attribute__((interrupt)) static void isr_05(void* p){
	say("int05!\n");
	while(1);
}//Bound Range Exceeded
__attribute__((interrupt)) static void isr_06(struct int_frame* p){
	say("int06: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
	while(1);
}//Invalid Opcode
__attribute__((interrupt)) static void isr_07(void* p){
	say("int07!\n");
	while(1);
}//Device Not Available
__attribute__((interrupt)) static void isr_08(void* p, u64 e){
	say("int08:%llx\n", e);
	while(1);
}//Double Fault
__attribute__((interrupt)) static void isr_09(void* p){
	say("int09!\n");
	while(1);
}//Coprocessor Segment Overrun
__attribute__((interrupt)) static void isr_0a(void* p, u64 e){
	say("int0a:%llx\n", e);
	while(1);
}//Invalid TSS
__attribute__((interrupt)) static void isr_0b(void* p, u64 e){
	say("int0b:%llx\n", e);
	while(1);
}//Segment Not Present
__attribute__((interrupt)) static void isr_0c(void* p, u64 e){
	say("int0c:%llx\n", e);
	while(1);
}//Stack-Segment Fault
__attribute__((interrupt)) static void isr_0d(struct int_frame* p, u64 e){
	printmemory((u8*)p-0x18, 0x80);
	say("int0d: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx, err=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp, e);
	asm("cli");
	asm("hlt");
}//General Protection Fault
__attribute__((interrupt)) static void isr_0e(struct int_frame* p, u64 e){
	printmemory((u8*)p-0x18, 0x80);
	say("int0e: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx, err=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp, e);
	asm("cli");
	asm("hlt");
}//Page Fault
__attribute__((interrupt)) static void isr_0f(void* p){
	say("int0f!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_10(void* p){
	say("int10!\n");
	while(1);
}//x87 Floating-Point Exception
__attribute__((interrupt)) static void isr_11(void* p, u64 e){
	say("int11:%llx\n", e);
	while(1);
}//Alignment Check
__attribute__((interrupt)) static void isr_12(void* p){
	say("int12!\n");
	while(1);
}//Machine Check
__attribute__((interrupt)) static void isr_13(void* p){
	say("int13!\n");
	while(1);
}//SIMD Floating-Point Exception
__attribute__((interrupt)) static void isr_14(void* p){
	say("int14!\n");
	while(1);
}//Virtualization Exception
__attribute__((interrupt)) static void isr_15(void* p){
	say("int15!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_16(void* p){
	say("int16!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_17(void* p){
	say("int17!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_18(void* p){
	say("int18!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_19(void* p){
	say("int19!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_1a(void* p){
	say("int1a!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_1b(void* p){
	say("int1b!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_1c(void* p){
	say("int1c!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_1d(void* p){
	say("int1d!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_1e(void* p, u64 e){
	say("int1e:%llx\n", e);
	while(1);
}//Security Exception
__attribute__((interrupt)) static void isr_1f(void* p){
	say("int1f!\n");
	while(1);
}//Reserved
__attribute__((interrupt)) static void isr_20(void* p){
	//say("825x!\n");
	isr_825x();
	endofirq(0);
}
__attribute__((interrupt)) static void isr_21(void* p){
	//say("kbd!\n");
	isr_8042();
	endofirq(1);
}
__attribute__((interrupt)) static void isr_28(void* p){
	//say("rtc!\n");
	isr_rtc();
	endofirq(8);
}
__attribute__((interrupt)) static void isr_80(struct int_frame* p){
	say("int80: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
}
__attribute__((interrupt)) static void isr_ff(struct int_frame* p){
	say("intff: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
}




void interruptinstall(int num, u64 isr)
{
	//0x80: testing syscall: 
	u8 type = (num>=0x80) ? 0xee : 0x8e;
	struct idt_entry* addr = (void*)(idthome + num*16);;

	addr->byte01 = isr&0xffff;
	addr->select = 0x10;		//kernel @ 10
	addr->zero = 0x0;
	addr->type = type;
	addr->byte23 = (isr>>16)&0xffff;
	addr->byte47 = (isr>>32)&0xffffffff;
	addr->what = 0;
}
void initidt()
{
	say("@initidt\n");

	int j;
	u8* buf = (void*)idthome;
	for(j=0;j<0x1000;j++)buf[j] = 0;

	//exception
	interruptinstall(0x00, (u64)isr_00);
	interruptinstall(0x01, (u64)isr_01);
	interruptinstall(0x02, (u64)isr_02);
	interruptinstall(0x03, (u64)isr_03);
	//interruptinstall(0x03, getisr03());
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
	interruptinstall(0x20, getisr20());		//(u64)isr_20
	interruptinstall(0x21, (u64)isr_21);
	interruptinstall(0x28, (u64)isr_28);

	//systemcall
	interruptinstall(0x80, getisr80());		//(u64)isr_80

	//apic spurious
	interruptinstall(0xff, (u64)isr_ff);

	//
	printmemory(buf, 0x40);
	setidt(buf, 0xfff);
}
