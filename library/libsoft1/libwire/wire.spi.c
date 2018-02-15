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
int decstr2data(char*,u64*);
int hexstr2data(char*,u64*);
void say(char*,...);




static int spi_read()
{
	return 0;
}
static int spi_write()
{
	return 0;
}
static int spi_list()
{
	systemspi_list(0);
	return 0;
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
		ret=decstr2data(p+11,&data);
		if(ret<=0)return -2;

		return systemspi_choose((int)data,p);
	}

	//"0x68"
	if(p[0]=='0'&&p[1]=='x')
	{
		ret=hexstr2data(p+2,&data);
		if(ret<=0)return -2;

		return systemspi_choose((int)data,0);
	}

	//33
	else
	{
		ret=decstr2data(p,&data);
		if(ret<=0)return -3;

		return systemspi_choose((int)data,0);
	}
}
static int spi_start(u64 type,char* p)
{
	return systemspi_start(p);
}
static int spi_stop()
{
	return systemspi_stop();
}
void spi_create(void* w)
{
}
void spi_delete()
{
}
