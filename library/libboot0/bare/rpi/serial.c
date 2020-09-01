#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




#define MMIO_BASE       0x3f000000
//
#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014))
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))
#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE+0x00200034))
#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE+0x00200038))
#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE+0x00200040))
#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE+0x00200044))
#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE+0x00200064))
#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE+0x00200068))
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C))
//
#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))




void miniuart_putc(unsigned int c) {
    do{
		asm volatile("nop");
	}while(!(*AUX_MU_LSR&0x20));

    *AUX_MU_IO = c;
}
int miniuart_getc() {
    if(!(*AUX_MU_LSR&0x01))return 0xffff;

    return *AUX_MU_IO;
}




int lowlevel_input(u8* buf, int len)
{
	int j,ret;
	for(j=0;j<len;j++){
		ret = miniuart_getc();
		if(ret > 0xff)break;

		buf[j] = ret;
	}
	return j;
}
int lowlevel_output(u8* buf, int len)
{
	int j;
	for(j=0;j<len;j++)miniuart_putc(buf[j]);
	return len;
}




void freeserial()
{
}
void initserial()
{
	register unsigned int r;

	/* initialize UART */
	*AUX_ENABLE |= 1;       // enable UART1, AUX mini uart
	*AUX_MU_CNTL = 0;
	*AUX_MU_LCR = 3;       // 8 bits
	*AUX_MU_MCR = 0;
	*AUX_MU_IER = 0;
	*AUX_MU_IIR = 0xc6;    // disable interrupts
	*AUX_MU_BAUD = 270;    // 115200 baud

	/* map UART1 to GPIO pins */
	r = *GPFSEL1;
	r &= ~((7<<12)|(7<<15)); // gpio14, gpio15
	r |= (2<<12)|(2<<15);    // alt5
	*GPFSEL1 = r;
	*GPPUD = 0;            // enable pins 14 and 15
	r=150; while(r--) { asm volatile("nop"); }
	*GPPUDCLK0 = (1<<14)|(1<<15);
	r=150; while(r--) { asm volatile("nop"); }
	*GPPUDCLK0 = 0;        // flush GPIO setup
	*AUX_MU_CNTL = 3;      // enable Tx, Rx
}
