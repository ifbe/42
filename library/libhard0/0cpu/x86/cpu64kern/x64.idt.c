#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define KERNCODE 0x10
#define KERNDATA 0x18
#define USERCODE 0x20
#define USERDATA 0x28
//
#define APPCPU 0x20000		//[20000,2ffff]
#define APPCPU_GDT (APPCPU+0x000)
#define APPCPU_TSS (APPCPU+0x1000)
#define APPCPU_IDT (APPCPU+0x2000)
#define APPCPU_RSP (APPCPU+0x10000)
//
#define BSPCPU 0x30000		//[30000,3ffff]
#define BSPCPU_GDT (BSPCPU+0x000)
#define BSPCPU_TSS (BSPCPU+0x1000)
#define BSPCPU_IDT (BSPCPU+0x2000)
#define BSPCPU_RSP (BSPCPU+0x10000)
void isr_825x();
void isr_8042();
void isr_rtc();
//
u64 getisr03();
u64 getisr20();
u64 getisr28();
u64 getisr40();
u64 getisr80();
//
void irqchip_endofirq(int);
//
int localapic_coreid();
//
void setidt(void* buf, int len);
//
void eventwrite(u64,u64,u64,u64);
void printmemory(void*, int);
void logtoall(void*, ...);



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




static void (*isr20_later)(void*) = 0;
static void anchorpoint()
{
}




__attribute__((interrupt)) static void allcpu_isr00(void* p){
	logtoall("int00#DE\n");
	asm("cli");
	asm("hlt");
}//Divide-by-zero
__attribute__((interrupt)) static void allcpu_isr01(void* p){
	logtoall("int01#DB\n");
	asm("cli");
	asm("hlt");
}//Debug
__attribute__((interrupt)) static void allcpu_isr02(void* p){
	logtoall("int02!\n");
	asm("cli");
	asm("hlt");
}//Non-maskable Interrupt
__attribute__((interrupt)) static void allcpu_isr03(struct int_frame* p){
	logtoall("int03#BP: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
}//Breakpoint
__attribute__((interrupt)) static void allcpu_isr04(void* p){
	logtoall("int04#OF\n");
	asm("cli");
	asm("hlt");
}//Overflow
__attribute__((interrupt)) static void allcpu_isr05(void* p){
	logtoall("int05#BR\n");
	asm("cli");
	asm("hlt");
}//Bound Range Exceeded
__attribute__((interrupt)) static void allcpu_isr06(struct int_frame* p){
	logtoall("int06#UD: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
	printmemory((void*)(p->ip), 0x10);
	asm("cli");
	asm("hlt");
}//Invalid Opcode
__attribute__((interrupt)) static void allcpu_isr07(struct int_frame* p){
	logtoall("int07#NM@%x\n", localapic_coreid());
	printmemory((void*)(p->ip-0x10), 0x20);
	//asm("cli");
	//asm("hlt");
}//Device Not Available
__attribute__((interrupt)) static void allcpu_isr08(void* p, u64 e){
	logtoall("int08#DF:%llx\n", e);
	asm("cli");
	asm("hlt");
}//Double Fault
__attribute__((interrupt)) static void allcpu_isr09(void* p){
	logtoall("int09!\n");
	asm("cli");
	asm("hlt");
}//Coprocessor Segment Overrun
__attribute__((interrupt)) static void allcpu_isr0a(void* p, u64 e){
	logtoall("int0a#TS:%llx\n", e);
	asm("cli");
	asm("hlt");
}//Invalid TSS
__attribute__((interrupt)) static void allcpu_isr0b(void* p, u64 e){
	logtoall("int0b#NP:%llx\n", e);
	asm("cli");
	asm("hlt");
}//Segment Not Present
__attribute__((interrupt)) static void allcpu_isr0c(void* p, u64 e){
	logtoall("int0c#SS:%llx\n", e);
	asm("cli");
	asm("hlt");
}//Stack-Segment Fault
__attribute__((interrupt)) static void allcpu_isr0d(struct int_frame* p, u64 e){
	printmemory((u8*)p-0x18, 0x80);
	logtoall("int0d#GP@core%x: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx, err=%llx\n", localapic_coreid(), p->flag, p->cs, p->ip, p->ss, p->sp, e);
	logtoall("index=%x,type=%x,internal=%x\n", e>>3, (e>>1)&3, e&1);
	printmemory((void*)(p->ip), 0x10);
	asm("cli");
	asm("hlt");
}//General Protection Fault
__attribute__((interrupt)) static void allcpu_isr0e(struct int_frame* p, u64 e){
	u64 cr2;
	asm volatile("mov %%cr2, %0" : "=r"(cr2) );

	printmemory(p, 0x60);
	logtoall("int0e#PF@core%x: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx, err=%llx, cr2=%llx\n", localapic_coreid(), p->flag, p->cs, p->ip, p->ss, p->sp, e, cr2);
	logtoall("anchor@%p,offset=%llx\n", anchorpoint, p->ip - (u64)anchorpoint);
	printmemory((void*)(p->ip), 0x10);
	asm("cli");
	asm("hlt");
}//Page Fault
__attribute__((interrupt)) static void allcpu_isr0f(void* p){
	logtoall("int0f!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr10(void* p){
	logtoall("int10#MF\n");
	asm("cli");
	asm("hlt");
}//x87 Floating-Point Exception
__attribute__((interrupt)) static void allcpu_isr11(void* p, u64 e){
	logtoall("int11#AC:%llx\n", e);
	asm("cli");
	asm("hlt");
}//Alignment Check
__attribute__((interrupt)) static void allcpu_isr12(void* p){
	logtoall("int12#MC\n");
	asm("cli");
	asm("hlt");
}//Machine Check
__attribute__((interrupt)) static void allcpu_isr13(void* p){
	logtoall("int13#XF\n");
	asm("cli");
	asm("hlt");
}//SIMD Floating-Point Exception
__attribute__((interrupt)) static void allcpu_isr14(void* p){
	logtoall("int14#VE\n");
	asm("cli");
	asm("hlt");
}//Virtualization Exception
__attribute__((interrupt)) static void allcpu_isr15(void* p){
	logtoall("int15!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr16(void* p){
	logtoall("int16!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr17(void* p){
	logtoall("int17!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr18(void* p){
	logtoall("int18!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr19(void* p){
	logtoall("int19!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr1a(void* p){
	logtoall("int1a!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr1b(void* p){
	logtoall("int1b!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr1c(void* p){
	logtoall("int1c!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr1d(void* p){
	logtoall("int1d!\n");
	asm("cli");
	asm("hlt");
}//Reserved
__attribute__((interrupt)) static void allcpu_isr1e(void* p, u64 e){
	logtoall("int1e#SX:%llx\n", e);
	asm("cli");
	asm("hlt");
}//Security Exception
__attribute__((interrupt)) static void allcpu_isr1f(void* p){
	logtoall("int1f!\n");
	asm("cli");
	asm("hlt");
}//Reserved




__attribute__((interrupt)) static void allcpu_isr20(void* p){
	//logtoall("timer!\n");
	if(isr20_later)isr20_later(p);
	else isr_825x();
	irqchip_endofirq(0);
}
__attribute__((interrupt)) static void allcpu_isr21(void* p){
	logtoall("ps2kbd!\n");
	//isr_8042();
	irqchip_endofirq(1);
}
__attribute__((interrupt)) static void allcpu_isr27(struct int_frame* p){
	//logtoall("int27: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
/*
	out8(0x20, 0x0b);
	u8 irr = in8(0x20);
	if(irr&0x80){		//only when this set, real irq
		//isr_parallel();
		irqchip_endofirq(7);
	}*/
}
__attribute__((interrupt)) static void allcpu_isr28(void* p){
	//logtoall("rtc!\n");
	isr_rtc();
	irqchip_endofirq(8);
}
__attribute__((interrupt)) static void allcpu_isr2c(void* p){
	logtoall("ps2mouse!\n");
	irqchip_endofirq(12);
}




__attribute__((interrupt)) static void bspcpu_isr80(struct int_frame* p){
	logtoall("int80: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
}
__attribute__((interrupt)) static void bspcpu_isrff(struct int_frame* p){
	logtoall("intff: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
}




__attribute__((interrupt)) static void appcpu_isrff(struct int_frame* p){
	logtoall("appcpu_isrff: flag=%llx, cs=%llx,ip=%llx, ss=%llx,sp=%llx\n", p->flag, p->cs, p->ip, p->ss, p->sp);
}




void interruptinstall(void* idt, int num, u64 isr, int flag)
{
	//0x80: testing syscall: 
	u8 type = (num>=0x80) ? 0xee : 0x8e;
	struct idt_entry* addr = (void*)(idt + num*16);

	addr->byte01 = isr&0xffff;
	addr->select = KERNCODE;		//kernel @ 10
	addr->zero = 0x0;
	addr->type = type;
	addr->byte23 = (isr>>16)&0xffff;
	addr->byte47 = (isr>>32)&0xffffffff;
	addr->what = 0;
}
int percpu_enableint(int apicid, int intvec, void* isr, int flag)
{
	if(intvec < 0x20)return 0;

	if(0x20 == intvec){
		isr20_later = isr;
	}
	else{
		u8* idt = (void*)BSPCPU_IDT;
		interruptinstall(idt, intvec, (u64)isr, 0);
	}
	return 1;
}
void initidt_bsp()
{
	logtoall("@initidt_bsp\n");

	int j;
	u8* idt = (void*)BSPCPU_IDT;
	for(j=0;j<0x1000;j++)idt[j] = 0;

	//exception
	interruptinstall(idt, 0x00, (u64)allcpu_isr00, 0);
	interruptinstall(idt, 0x01, (u64)allcpu_isr01, 0);
	interruptinstall(idt, 0x02, (u64)allcpu_isr02, 0);
	interruptinstall(idt, 0x03, (u64)allcpu_isr03, 0);
	//interruptinstall(idt, 0x03, getisr03(), 0);
	interruptinstall(idt, 0x04, (u64)allcpu_isr04, 0);
	interruptinstall(idt, 0x05, (u64)allcpu_isr05, 0);
	interruptinstall(idt, 0x06, (u64)allcpu_isr06, 0);
	interruptinstall(idt, 0x07, (u64)allcpu_isr07, 0);
	interruptinstall(idt, 0x08, (u64)allcpu_isr08, 0);
	interruptinstall(idt, 0x09, (u64)allcpu_isr09, 0);
	interruptinstall(idt, 0x0a, (u64)allcpu_isr0a, 0);
	interruptinstall(idt, 0x0b, (u64)allcpu_isr0b, 0);
	interruptinstall(idt, 0x0c, (u64)allcpu_isr0c, 0);
	interruptinstall(idt, 0x0d, (u64)allcpu_isr0d, 0);
	interruptinstall(idt, 0x0e, (u64)allcpu_isr0e, 0);
	interruptinstall(idt, 0x0f, (u64)allcpu_isr0f, 0);
	interruptinstall(idt, 0x10, (u64)allcpu_isr10, 0);
	interruptinstall(idt, 0x11, (u64)allcpu_isr11, 0);
	interruptinstall(idt, 0x12, (u64)allcpu_isr12, 0);
	interruptinstall(idt, 0x13, (u64)allcpu_isr13, 0);
	interruptinstall(idt, 0x14, (u64)allcpu_isr14, 0);
	interruptinstall(idt, 0x15, (u64)allcpu_isr15, 0);
	interruptinstall(idt, 0x16, (u64)allcpu_isr16, 0);
	interruptinstall(idt, 0x17, (u64)allcpu_isr17, 0);
	interruptinstall(idt, 0x18, (u64)allcpu_isr18, 0);
	interruptinstall(idt, 0x19, (u64)allcpu_isr19, 0);
	interruptinstall(idt, 0x1a, (u64)allcpu_isr1a, 0);
	interruptinstall(idt, 0x1b, (u64)allcpu_isr1b, 0);
	interruptinstall(idt, 0x1c, (u64)allcpu_isr1c, 0);
	interruptinstall(idt, 0x1d, (u64)allcpu_isr1d, 0);
	interruptinstall(idt, 0x1e, (u64)allcpu_isr1e, 0);
	interruptinstall(idt, 0x1f, (u64)allcpu_isr1f, 0);

	//interrupt
	interruptinstall(idt, 0x20, (u64)allcpu_isr20, 0);		//(u64)bspcpu_isr20
	interruptinstall(idt, 0x21, (u64)allcpu_isr21, 0);
	interruptinstall(idt, 0x27, (u64)allcpu_isr27, 0);
	interruptinstall(idt, 0x28, (u64)allcpu_isr28, 0);
	interruptinstall(idt, 0x2c, (u64)allcpu_isr2c, 0);

	//apic timer
	interruptinstall(idt, 0x40, getisr40(), 0);

	//systemcall
	interruptinstall(idt, 0x80, getisr80(), 0);		//(u64)bspcpu_isr80

	//apic spurious
	interruptinstall(idt, 0xff, (u64)bspcpu_isrff, 0);

	//
	printmemory(idt, 0x40);
	setidt(idt, 0xfff);
}
void initidt_ap(int coreid)
{
	logtoall("@initidt_ap: %d\n", coreid);

	int j;
	u8* idt = (void*)APPCPU_IDT;
	for(j=0;j<0x1000;j++)idt[j] = 0;

	interruptinstall(idt, 0x00, (u64)allcpu_isr00, 0);
	interruptinstall(idt, 0x01, (u64)allcpu_isr01, 0);
	interruptinstall(idt, 0x02, (u64)allcpu_isr02, 0);
	interruptinstall(idt, 0x03, (u64)allcpu_isr03, 0);
	//interruptinstall(idt, 0x03, getisr03(), 0);
	interruptinstall(idt, 0x04, (u64)allcpu_isr04, 0);
	interruptinstall(idt, 0x05, (u64)allcpu_isr05, 0);
	interruptinstall(idt, 0x06, (u64)allcpu_isr06, 0);
	interruptinstall(idt, 0x07, (u64)allcpu_isr07, 0);
	interruptinstall(idt, 0x08, (u64)allcpu_isr08, 0);
	interruptinstall(idt, 0x09, (u64)allcpu_isr09, 0);
	interruptinstall(idt, 0x0a, (u64)allcpu_isr0a, 0);
	interruptinstall(idt, 0x0b, (u64)allcpu_isr0b, 0);
	interruptinstall(idt, 0x0c, (u64)allcpu_isr0c, 0);
	interruptinstall(idt, 0x0d, (u64)allcpu_isr0d, 0);
	interruptinstall(idt, 0x0e, (u64)allcpu_isr0e, 0);
	interruptinstall(idt, 0x0f, (u64)allcpu_isr0f, 0);
	interruptinstall(idt, 0x10, (u64)allcpu_isr10, 0);
	interruptinstall(idt, 0x11, (u64)allcpu_isr11, 0);
	interruptinstall(idt, 0x12, (u64)allcpu_isr12, 0);
	interruptinstall(idt, 0x13, (u64)allcpu_isr13, 0);
	interruptinstall(idt, 0x14, (u64)allcpu_isr14, 0);
	interruptinstall(idt, 0x15, (u64)allcpu_isr15, 0);
	interruptinstall(idt, 0x16, (u64)allcpu_isr16, 0);
	interruptinstall(idt, 0x17, (u64)allcpu_isr17, 0);
	interruptinstall(idt, 0x18, (u64)allcpu_isr18, 0);
	interruptinstall(idt, 0x19, (u64)allcpu_isr19, 0);
	interruptinstall(idt, 0x1a, (u64)allcpu_isr1a, 0);
	interruptinstall(idt, 0x1b, (u64)allcpu_isr1b, 0);
	interruptinstall(idt, 0x1c, (u64)allcpu_isr1c, 0);
	interruptinstall(idt, 0x1d, (u64)allcpu_isr1d, 0);
	interruptinstall(idt, 0x1e, (u64)allcpu_isr1e, 0);
	interruptinstall(idt, 0x1f, (u64)allcpu_isr1f, 0);

	//what?
	interruptinstall(idt, 0x20, getisr20(), 0);
	interruptinstall(idt, 0x21, (u64)allcpu_isr21, 0);
	interruptinstall(idt, 0x27, (u64)allcpu_isr27, 0);
	interruptinstall(idt, 0x28, (u64)allcpu_isr28, 0);
	interruptinstall(idt, 0x2c, (u64)allcpu_isr2c, 0);

	//apic timer
	interruptinstall(idt, 0x40, getisr40(), 0);

	//systemcall
	interruptinstall(idt, 0x80, getisr80(), 0);		//(u64)bspcpu_isr80

	//apic spurious
	interruptinstall(idt, 0xff, (u64)appcpu_isrff, 0);

	printmemory(idt, 0x40);
	setidt(idt, 0xfff);
}