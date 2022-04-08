#include "libhard.h"
void* mmiobase();
//
void wait_msec(int);
void wait_cycles(int);


//
#define GPIOACCESS(x) (*(volatile unsigned int*)(gpio+x))
#define GPIO_OFFS 0x00200000
//altfunc
#define GPFSEL0   0x00
#define GPFSEL1   0x04
#define GPFSEL2   0x08
#define GPFSEL3   0x0C
#define GPFSEL4   0x10
#define GPFSEL5   0x14
//output
#define GPSET0    0x1C
#define GPSET1    0x20
#define GPCLR0    0x28
#define GPCLR1    0x2c
//input.value
#define GPLEV0    0x34
#define GPLEV1    0x38
#define GPEDS0    0x40
#define GPEDS1    0x44
//input.enable
#define GPREN0    0x4c
#define GPREN1    0x50
#define GPFEN0    0x58
#define GPFEN1    0x5c
#define GPHEN0    0x64
#define GPHEN1    0x68
#define GPLEN0    0x70
#define GPLEN1    0x74
#define GPAREN0   0x7c
#define GPAREN1   0x80
#define GPAFEN0   0x88
#define GPAFEN1   0x8c
//pull
#define GPPUD     0x94
#define GPPUDCLK0 0x98
#define GPPUDCLK1 0x9C
#if RASPPI <= 3
#define ARM_GPIO_GPPUD		0x94
#define ARM_GPIO_GPPUDCLK0	0x98
#else
#define ARM_GPIO_GPPINMUXSD	0xD0
#define ARM_GPIO_GPPUPPDN0	0xE4
#define ARM_GPIO_GPPUPPDN1	0xE8
#define ARM_GPIO_GPPUPPDN2	0xEC
#define ARM_GPIO_GPPUPPDN3	0xF0
#endif




/*
--------bcm2835 gpio--------
gpio    pull    alt0        alt1        alt2        alt3          alt4            alt5
0       High    SDA0        SA5         -           -             -               -
1       High    SCL0        SA4         -           -             -               -
2       High    SDA1        SA3         -           -             -               -
3       High    SCL1        SA2         -           -             -               -
4       High    GPCLK0      SA1         -           -             -               ARM_TDI
5       High    GPCLK1      SA0         -           -             -               ARM_TDO
6       High    GPCLK2      SOE_N/SE    -           -             -               ARM_RTCK
7       High    SPI0_CE1_N  SWE_N/SRW_N -           -             -               -
8       High    SPI0_CE0_N  SD0         -           -             -               -
9       Low     SPI0_MISO   SD1         -           -             -               -
10      Low     SPI0_MOSI   SD2         -           -             -               -
11      Low     SPI0_SCLK   SD3         -           -             -               -
12      Low     PWM0_0      SD4         -           -             -               ARM_TMS
13      Low     PWM0_1      SD5         -           -             -               ARM_TCK
14      Low     TXD0        SD6         -           -             -               TXD1
15      Low     RXD0        SD7         -           -             -               RXD1
16      Low     -           SD8         -           CTS0          SPI1_CE2_N      CTS1
17      Low     -           SD9         -           RTS0          SPI1_CE1_N      RTS1
18      Low     PCM_CLK     SD10        -           SPI6_CE0_N    SPI1_CE0_N      PWM0_0
19      Low     PCM_FS      SD11        -           SPI6_MISO     SPI1_MISO       PWM0_1
20      Low     PCM_DIN     SD12        -           SPI6_MOSI     SPI1_MOSI       GPCLK0
21      Low     PCM_DOUT    SD13        -           SPI6_SCLK     SPI1_SCLK       GPCLK1
22      Low     SD0_CLK     SD14        -           SD1_CLK       ARM_TRST        -
23      Low     SD0_CMD     SD15        -           SD1_CMD       ARM_RTCK        -
24      Low     SD0_DAT0    SD16        -           SD1_DAT0      ARM_TDO         -
25      Low     SD0_DAT1    SD17        -           SD1_DAT1      ARM_TCK         -
26      Low     SD0_DAT2    -           -           SD1_DAT2      ARM_TDI         -
27      Low     SD0_DAT3    -           -           SD1_DAT3      ARM_TMS         -
28      -       SDA0        SA5         PCM_CLK     -             -               -
29      -       SCL0        SA4         PCM_FS      -             -               -
30      Low     -           SA3         PCM_DIN     CTS0          -               CTS1
31      Low     -           SA2         PCM_DOUT    RTS0          -               RTS1
32      Low     GPCLK0      SA1         -           TXD0          -               TXD1
33      Low     -           SA0         -           RXD0          -               RXD1
34      High    GPCLK0      SOE_N/SE    -           -             -               -
35      High    SPI0_CE1_N  SWE_N/SRW_N -           -             -               -
36      High    SPI0_CE0_N  SD0         TXD0        -             -               -
37      Low     SPI0_MISO   SD1         RXD0        -             -               -
38      Low     SPI0_MOSI   SD2         RTS0        -             -               -
39      Low     SPI0_SCLK   SD3         CTS0        -             -               -
40      Low     PWM1_0      SD4         -           -             SPI0_MISO       TXD1
41      Low     PWM1_1      SD5         -           -             SPI0_MOSI       RXD1
42      Low     GPCLK1      SD6         -           -             SPI0_SCLK       RTS1
43      Low     GPCLK2      SD7         -           -             SPI0_CE0_N      CTS1
44      -       GPCLK1      SDA0        SDA1        -             SPI0_CE1_N      -
45      -       PWM0_1      SCL0        SCL1        -             SPI0_CE2_N      -
*/
/*
--------bcm2711 gpio--------
gpio    pull    alt0        alt1        alt2        alt3          alt4            alt5
0       High    SDA0        SA5         PCLK        SPI3_CE0_N    TXD2            SDA6
1       High    SCL0        SA4         DE          SPI3_MISO     RXD2            SCL6
2       High    SDA1        SA3         LCD_VSYNC   SPI3_MOSI     CTS2            SDA3
3       High    SCL1        SA2         LCD_HSYNC   SPI3_SCLK     RTS2            SCL3
4       High    GPCLK0      SA1         DPI_D0      SPI4_CE0_N    TXD3            SDA3
5       High    GPCLK1      SA0         DPI_D1      SPI4_MISO     RXD3            SCL3
6       High    GPCLK2      SOE_N/SE    DPI_D2      SPI4_MOSI     CTS3            SDA4
7       High    SPI0_CE1_N  SWE_N/SRW_N DPI_D3      SPI4_SCLK     RTS3            SCL4
8       High    SPI0_CE0_N  SD0         DPI_D4      BSCSL/CE_N    TXD4            SDA4
9       Low     SPI0_MISO   SD1         DPI_D5      BSCSL/MISO    RXD4            SCL4
10      Low     SPI0_MOSI   SD2         DPI_D6      BSCSLSDA/MOSI CTS4            SDA5
11      Low     SPI0_SCLK   SD3         DPI_D7      BSCSLSCL/SCLK RTS4            SCL5
12      Low     PWM0_0      SD4         DPI_D8      SPI5_CE0_N    TXD5            SDA5
13      Low     PWM0_1      SD5         DPI_D9      SPI5_MISO     RXD5            SCL5
14      Low     TXD0        SD6         DPI_D10     SPI5_MOSI     CTS5            TXD1
15      Low     RXD0        SD7         DPI_D11     SPI5_SCLK     RTS5            RXD1
16      Low     -           SD8         DPI_D12     CTS0          SPI1_CE2_N      CTS1
17      Low     -           SD9         DPI_D13     RTS0          SPI1_CE1_N      RTS1
18      Low     PCM_CLK     SD10        DPI_D14     SPI6_CE0_N    SPI1_CE0_N      PWM0_0
19      Low     PCM_FS      SD11        DPI_D15     SPI6_MISO     SPI1_MISO       PWM0_1
20      Low     PCM_DIN     SD12        DPI_D16     SPI6_MOSI     SPI1_MOSI       GPCLK0
21      Low     PCM_DOUT    SD13        DPI_D17     SPI6_SCLK     SPI1_SCLK       GPCLK1
22      Low     SD0_CLK     SD14        DPI_D18     SD1_CLK       ARM_TRST        SDA6
23      Low     SD0_CMD     SD15        DPI_D19     SD1_CMD       ARM_RTCK        SCL6
24      Low     SD0_DAT0    SD16        DPI_D20     SD1_DAT0      ARM_TDO         SPI3_CE1_N
25      Low     SD0_DAT1    SD17        DPI_D21     SD1_DAT1      ARM_TCK         SPI4_CE1_N
26      Low     SD0_DAT2    -           DPI_D22     SD1_DAT2      ARM_TDI         SPI5_CE1_N
27      Low     SD0_DAT3    -           DPI_D23     SD1_DAT3      ARM_TMS         SPI6_CE1_N
28      -       SDA0        SA5         PCM_CLK     -             MII_A_RX_ERR    RGMII_MDIO
29      -       SCL0        SA4         PCM_FS      -             MII_A_TX_ERR    RGMII_MDC
30      Low     -           SA3         PCM_DIN     CTS0          MII_A_CRS       CTS1
31      Low     -           SA2         PCM_DOUT    RTS0          MII_A_COL       RTS1
32      Low     GPCLK0      SA1         -           TXD0          SD_CARD_PRES    TXD1
33      Low     -           SA0         -           RXD0          SD_CARD_WRPROT  RXD1
34      High    GPCLK0      SOE_N/SE    -           SD1_CLK       SD_CARD_LED     RGMII_IRQ
35      High    SPI0_CE1_N  SWE_N/SRW_N -           SD1_CMD       RGMII_START_STOP -
36      High    SPI0_CE0_N  SD0         TXD0        SD1_DAT0      RGMII_RX_OK     MII_A_RX_ERR
37      Low     SPI0_MISO   SD1         RXD0        SD1_DAT1      RGMII_MDIO      MII_A_TX_ERR
38      Low     SPI0_MOSI   SD2         RTS0        SD1_DAT2      RGMII_MDC       MII_A_CRS
39      Low     SPI0_SCLK   SD3         CTS0        SD1_DAT3      RGMII_IRQ       MII_A_COL
40      Low     PWM1_0      SD4         -           SD1_DAT4      SPI0_MISO       TXD1
41      Low     PWM1_1      SD5         -           SD1_DAT5      SPI0_MOSI       RXD1
42      Low     GPCLK1      SD6         -           SD1_DAT6      SPI0_SCLK       RTS1
43      Low     GPCLK2      SD7         -           SD1_DAT7      SPI0_CE0_N      CTS1
44      -       GPCLK1      SDA0        SDA1        -             SPI0_CE1_N      SD_CARD_VOLT
45      -       PWM0_1      SCL0        SCL1        -             SPI0_CE2_N      SD_CARD_PWR0
46      -       -           -           -           -             -               ??
47      -       -           -           -           -             -               cd
48      -       -           -           -           -             -               clk
49      -       -           -           -           -             -               cmd
50      -       -           -           -           -             -               dat0
51      -       -           -           -           -             -               dat1
52      -       -           -           -           -             -               dat2
53      -       -           -           -           -             -               dat3
*/




char* val2str[8] = {
	"input","output","alt5","alt4",
	"alt0", "alt1",  "alt2","alt3"
};
int pinmgr_setfunc(int pin, int func)
{
	if(pin < 0)return 0;
	if(pin > 45)return 0;

	volatile void* mmio = mmiobase();
	volatile void* gpio = mmio + GPIO_OFFS;

	int hh = pin/10;
	int ll = pin%10;

	u32 val = GPIOACCESS(GPFSEL0 + hh*4);
	val &= ~(3 << (ll*3));
	val |= func << (ll*3);
	GPIOACCESS(GPFSEL0 + hh*4) = val;
	return 1;
}
int pinmgr_setfunc_many(int pin, int func)
{
	return 1;
}




int pinmgr_setpull(int pin, int pull)
{
	return 1;
}
int pinmgr_setpull_many(int pin, int func)
{
	return 1;
}




//input detect: rise, fall, high, low, asyncrise, asyncfall
int pinmgr_setdetect(int pin, int detect)
{
	return 1;
}
int pinmgr_setdetect_many(int pin, int func)
{
	return 1;
}




void pinmgr_gpio2227_sdhost()
{
}
void pinmgr_gpio2227_sdhciold()
{
}
void pinmgr_gpio3439_sdhciold()
{
}
void pinmgr_gpio4653_sdhcinew()
{
	long r;
	volatile void* mmio = mmiobase();
	volatile void* gpio = mmio + GPIO_OFFS;
/*
	gpio_setfunc(47, 0);
	gpio_setpull(47, up);
	wait_cycles(150);
	gpio_setpull(47, none);
	gpio_setdetect(47, high);

	gpio_setfunc(48, 7);
	gpio_setfunc(49, 7);
	gpio_setpull_many((1<<48)|(1<<49), up);
	wait_cycles(150);
	gpio_setpull_many((1<<48)|(1<<49), none);

	gpio_setfunc(50, 7);
	gpio_setfunc(51, 7);
	gpio_setfunc(52, 7);
	gpio_setfunc(53, 7);
	gpio_setpull_many((1<<50)|(1<<51)|(1<<52)|(1<<53), up);
	wait_cycles(150);
	gpio_setpull_many((1<<50)|(1<<51)|(1<<52)|(1<<53), none);
*/
	// GPIO_CD
	r = GPIOACCESS(GPFSEL4);
	r &= ~(7<<(7*3));
	GPIOACCESS(GPFSEL4) = r;

	GPIOACCESS(GPPUD) = 2;
	wait_cycles(150);
	GPIOACCESS(GPPUDCLK1) = (1<<15);
	wait_cycles(150);
	GPIOACCESS(GPPUD) = 0;
	GPIOACCESS(GPPUDCLK1) = 0;

	r = GPIOACCESS(GPHEN1);
	r |= 1<<15;
	GPIOACCESS(GPHEN1) = r;

	// GPIO_CLK, GPIO_CMD
	r = GPIOACCESS(GPFSEL4);
	r |= (7<<(8*3))|(7<<(9*3));
	GPIOACCESS(GPFSEL4) = r;

	GPIOACCESS(GPPUD) = 2;
	wait_cycles(150);
	GPIOACCESS(GPPUDCLK1) = (1<<16)|(1<<17);
	wait_cycles(150);
	GPIOACCESS(GPPUD) = 0;
	GPIOACCESS(GPPUDCLK1) = 0;

	// GPIO_DAT0, GPIO_DAT1, GPIO_DAT2, GPIO_DAT3
	r = GPIOACCESS(GPFSEL5);
	r |= (7<<(0*3)) | (7<<(1*3)) | (7<<(2*3)) | (7<<(3*3));
	GPIOACCESS(GPFSEL5) = r;

	GPIOACCESS(GPPUD) = 2;
	wait_cycles(150);
	GPIOACCESS(GPPUDCLK1) = (1<<18) | (1<<19) | (1<<20) | (1<<21);
	wait_cycles(150);
	GPIOACCESS(GPPUD) = 0;
	GPIOACCESS(GPPUDCLK1) = 0;
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




void initpinmgr()
{
	volatile void* mmio = mmiobase();
	volatile void* gpio = mmio + GPIO_OFFS;
	say("@initpinmgr: mmio=%p,gpio=%p\n", mmio, gpio);

	int j,k;
	u32 val;
	for(k=0;k<5;k++){
		val = GPIOACCESS(GPFSEL0 + k*4);
		say("GPFSEL%d=%x\n", k, val);
		for(j=0;j<10;j++){
			say("pin%d: func=%d(%s)\n", k*10+j, val&7, val2str[val&7]);
			val >>= 3;
		}
	}
}