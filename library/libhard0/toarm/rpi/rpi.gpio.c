#warning "this havn't been done"
void* mmiobase();
//
void wait_msec(int);
void wait_cycles(int);


//
#define GPIO_OFFS       0x00200000
#define GPFSEL0         (*(volatile unsigned int*)(gpio+0x00))
#define GPFSEL1         (*(volatile unsigned int*)(gpio+0x04))
#define GPFSEL2         (*(volatile unsigned int*)(gpio+0x08))
#define GPFSEL3         (*(volatile unsigned int*)(gpio+0x0C))
#define GPFSEL4         (*(volatile unsigned int*)(gpio+0x10))
#define GPFSEL5         (*(volatile unsigned int*)(gpio+0x14))
#define GPSET0          (*(volatile unsigned int*)(gpio+0x1C))
#define GPSET1          (*(volatile unsigned int*)(gpio+0x20))
#define GPCLR0          (*(volatile unsigned int*)(gpio+0x28))
#define GPLEV0          (*(volatile unsigned int*)(gpio+0x34))
#define GPLEV1          (*(volatile unsigned int*)(gpio+0x38))
#define GPEDS0          (*(volatile unsigned int*)(gpio+0x40))
#define GPEDS1          (*(volatile unsigned int*)(gpio+0x44))
#define GPHEN0          (*(volatile unsigned int*)(gpio+0x64))
#define GPHEN1          (*(volatile unsigned int*)(gpio+0x68))
#define GPPUD           (*(volatile unsigned int*)(gpio+0x94))
#define GPPUDCLK0       (*(volatile unsigned int*)(gpio+0x98))
#define GPPUDCLK1       (*(volatile unsigned int*)(gpio+0x9C))
#if RASPPI <= 3
#define ARM_GPIO_GPPUD		(GPIO_OFFS + 0x94)
#define ARM_GPIO_GPPUDCLK0	(GPIO_OFFS + 0x98)
#else
#define ARM_GPIO_GPPINMUXSD	(GPIO_OFFS + 0xD0)
#define ARM_GPIO_GPPUPPDN0	(GPIO_OFFS + 0xE4)
#define ARM_GPIO_GPPUPPDN1	(GPIO_OFFS + 0xE8)
#define ARM_GPIO_GPPUPPDN2	(GPIO_OFFS + 0xEC)
#define ARM_GPIO_GPPUPPDN3	(GPIO_OFFS + 0xF0)
#endif




void gpio_prepsdhci()
{
	long r;
	volatile void* mmio = mmiobase();
	volatile void* gpio = mmio + GPIO_OFFS;

	// GPIO_CD
	r = GPFSEL4;
	r &= ~(7<<(7*3));
	GPFSEL4 = r;
	GPPUD = 2;
	wait_cycles(150);

	GPPUDCLK1 = (1<<15);
	wait_cycles(150);

	GPPUD = 0;
	GPPUDCLK1 = 0;
	r = GPHEN1;
	r |= 1<<15;
	GPHEN1 = r;

	// GPIO_CLK, GPIO_CMD
	r = GPFSEL4;
	r |= (7<<(8*3))|(7<<(9*3));
	GPFSEL4 = r;
	GPPUD = 2;
	wait_cycles(150);

	GPPUDCLK1 = (1<<16)|(1<<17);
	wait_cycles(150);
	GPPUD = 0;
	GPPUDCLK1 = 0;

	// GPIO_DAT0, GPIO_DAT1, GPIO_DAT2, GPIO_DAT3
	r = GPFSEL5;
	r |= (7<<(0*3)) | (7<<(1*3)) | (7<<(2*3)) | (7<<(3*3));
	GPFSEL5 = r;
	GPPUD = 2;
	wait_cycles(150);

	GPPUDCLK1 = (1<<18) | (1<<19) | (1<<20) | (1<<21);
	wait_cycles(150);

	GPPUD = 0;
	GPPUDCLK1 = 0;
}




int gpioread()
{
	return 0;
}
int gpiowrite()
{
	return 0;
}
int gpiostop()
{
	return 0;
}
int gpiostart()
{
	return 0;
}
