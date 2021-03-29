#warning "this havn't been done"


#define GPIO_OFFS       0x00200000
#define GPFSEL0         (GPIO_OFFS+0x00))
#define GPFSEL1         (GPIO_OFFS+0x04))
#define GPFSEL2         (GPIO_OFFS+0x08))
#define GPFSEL3         (GPIO_OFFS+0x0C))
#define GPFSEL4         (GPIO_OFFS+0x10))
#define GPFSEL5         (GPIO_OFFS+0x14))
#define GPSET0          (GPIO_OFFS+0x1C))
#define GPSET1          (GPIO_OFFS+0x20))
#define GPCLR0          (GPIO_OFFS+0x28))
#define GPLEV0          (GPIO_OFFS+0x34))
#define GPLEV1          (GPIO_OFFS+0x38))
#define GPEDS0          (GPIO_OFFS+0x40))
#define GPEDS1          (GPIO_OFFS+0x44))
#define GPHEN0          (GPIO_OFFS+0x64))
#define GPHEN1          (GPIO_OFFS+0x68))
#define GPPUD           (GPIO_OFFS+0x94))
#define GPPUDCLK0       (GPIO_OFFS+0x98))
#define GPPUDCLK1       (GPIO_OFFS+0x9C))




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
