#include "libhard.h"
//
#define COREFREQ (250*1000*1000)
#define SPICLK(speed) (COREFREQ/2/(speed+1))
//
#define SPI0 0x204000
#define SPI3 0x204600
#define SPI4 0x204800
#define SPI5 0x204a00
#define SPI6 0x204c00




int spi_read()
{
	return 0;
}
int spi_write()
{
	return 0;
}


int spi_delete()
{
	return 0;
}
int spi_create()
{
	return 0;
}


int spi_free(_obj* obj)
{
	return 0;
}
_obj* spi_alloc(u64 type, u8* buf)
{
	return 0;
}
