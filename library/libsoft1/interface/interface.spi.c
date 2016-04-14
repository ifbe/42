#define QWORD unsigned long long




static void spi_list()
{
}
static void spi_switch()
{
}
static void spi_read()
{
}
static void spi_write()
{
}
static void spi_start(QWORD type,char* p)
{
}
static void spi_stop()
{
}
void spi_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x697073;		//id
	p[2]=(QWORD)spi_start;
	p[3]=(QWORD)spi_stop;
	p[4]=(QWORD)spi_list;
	p[5]=(QWORD)spi_switch;
	p[6]=(QWORD)spi_read;
	p[7]=(QWORD)spi_write;
}
void spi_kill()
{
	spi_stop();
}
