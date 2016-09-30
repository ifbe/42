#define u64 unsigned long long
#define u8 unsigned char
//
int systemspi_list(char*);
int systemspi_choose(int,char*);
int systemspi_read(u8 dev,u8 reg,u8* buf,u8 count);
int systemspi_write(u8 dev,u8 reg,u8* buf,u8 count);
int systemspi_start(char* p);
int systemspi_stop();
//
int decstring2data(char*,u64*);
int hexstring2data(char*,u64*);
void say(char*,...);




static int spi_read()
{
}
static int spi_write()
{
}
static int spi_list()
{
	systemspi_list(0);
}
static int spi_choose(char* p)
{
	int ret;
	u64 data;

	//
	if(p==0)
	{
		systemspi_choose(0,0);
		return 0;
	}

	//".."
	if( (p[0]=='.') && (p[1]=='.') )
	{
		systemspi_choose(-1,p);
		return 1;
	}

	//"/dev/spidev0.0"
	if(p[0]=='/')
	{
		ret=decstring2data(p+11,&data);
		if(ret<=0)return -2;

		return systemspi_choose((int)data,p);
	}

	//"0x68"
	if(p[0]=='0'&&p[1]=='x')
	{
		ret=hexstring2data(p+2,&data);
		if(ret<=0)return -2;

		return systemspi_choose((int)data,0);
	}

	//33
	else
	{
		ret=decstring2data(p,&data);
		if(ret<=0)return -3;

		return systemspi_choose((int)data,0);
	}
}
static int spi_start(u64 type,char* p)
{
	systemspi_start(p);
}
static int spi_stop()
{
	systemspi_stop();
}
void spi_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x697073;		//id

	p[10]=(u64)spi_start;
	p[11]=(u64)spi_stop;
	p[12]=(u64)spi_list;
	p[13]=(u64)spi_choose;
	p[14]=(u64)spi_read;
	p[15]=(u64)spi_write;
}
void spi_delete()
{
}
